// Raster effects.

#include <algorithm>
#include "m/raster.h"
#include "m/display.h"
#include "m/dma.h"
#include "m/irq.h"
#include "m/transfer.h"
#include "m/window.h"

MRasterTrigger::MRasterTrigger(MRasterIO *io) {
    mIO = io;
}

void MRasterTrigger::Execute() {
    mIO->OnVBlankIntr();
}

MRasterIO::MRasterIO(u32 dma, volatile void *dest, u32 size) : MLooperVSyncTask(FALSE, FALSE) {
    mDma = dma;
    mCurrent = NULL;
    mDest = dest;
    mSize = size;
    mFlipOnVBlank = FALSE;
    if (mDma != (u32) -1) {
        MReserveDmaChannel(mDma);
    } else {
        mDma = MAllocDmaChannel();
    }
    mFront = new u8[size * (M_RASTER_LINES + 1)];
    mBack = new u8[size * (M_RASTER_LINES + 1)];
    mTrigger = new MRasterTrigger(this);
    MAddIntrTask(0, mTrigger);
    Enable();
}

MRasterIO::~MRasterIO() {
    Disable();
    Stop();
    MFreeDmaChannel(mDma);
    delete[] mFront;
    delete[] mBack;
}

void MRasterIO::Flip() {
    MIrqLock lock;
    u8 *front = mFront;
    mFront = mBack;
    mBack = front;
    DC_FlushRange(mFront, mSize * (M_RASTER_LINES + 1));
    mCurrent = mFront;
}

void MRasterIO::SetFlipOnVBlank(u8 flip) {
    mFlipOnVBlank = flip;
}

void MRasterIO::Stop() {
    if (mTrigger != NULL) {
        MRemoveIntrTask(0, mTrigger);
        MI_StopDma(mDma);
        delete mTrigger;
        mTrigger = NULL;
    }
}

void MRasterIO::OnVBlankIntr() {
    if (mFlipOnVBlank) {
        Flip();
    }
    Start();
}

void MRasterIO::Start() {
    MIrqLock lock;
    MI_StopDma(mDma);
    if (mCurrent != NULL) {
        // Line 0 is written now; the DMA copies the rest, one line per HBlank, ending with line 0 again.
        MIi_CpuCopy16(mCurrent, mCurrent + mSize * M_RASTER_LINES, mSize);
        MIi_CpuCopy16(mCurrent, (void *) mDest, mSize);
        if ((mSize & 3) == 0) {
            MI_HBlankDmaCopy32(mDma, mCurrent + mSize, (void *) mDest, mSize);
        } else {
            MI_HBlankDmaCopy16(mDma, mCurrent + mSize, (void *) mDest, mSize);
        }
    }
}

void MRasterIO::OnVSync() {
    Start();
}

MRasterPal::MRasterPal(u32 dma, MDisplay *display)
    : MRasterIO(dma, display != NULL ? display->GetBackdropReg() : gCurrentDisplay->GetBackdropReg(), 2) {
}

MRasterWindow::MRasterWindow(u32 dma, MDisplay *display, u8 autoFlip)
    : MRasterIO(dma,
                display != NULL ? display->mWin->GetWindow0()->GetShapeRegs()
                                : gCurrentDisplay->mWin->GetWindow0()->GetShapeRegs(),
                4) {
    mWin = display != NULL ? display->mWin : gCurrentDisplay->mWin;
    mAutoFlip = autoFlip;
    mChanged = FALSE;
    mSpans = new MRasterSpan[M_RASTER_LINES * 2];
    Hide();
    SetTarget(0);
    SetObjTarget(0);
    SetBlendEnabled(FALSE);
    SetObjBlendEnabled(FALSE);
    mWin->GetWindow0()->SetShape(0, 0, 0, 255);
    mWin->GetWindow1()->SetShape(0, 0, 0, 255);
}

MRasterWindow::~MRasterWindow() {
    delete[] mSpans;
}

void MRasterWindow::Show() {
    mWin->GetWindow0()->Show();
    mWin->GetWindow1()->Show();
}

void MRasterWindow::Hide() {
    mWin->GetWindow0()->Hide();
    mWin->GetWindow1()->Hide();
}

void MRasterWindow::SetTarget(int planes) {
    mWin->GetWindow0()->SetTarget(planes);
    mWin->GetWindow1()->SetTarget(planes);
}

void MRasterWindow::SetObjTarget(int planes) {
    mWin->GetObjWindow()->SetTarget(planes);
}

void MRasterWindow::SetBlendEnabled(BOOL enabled) {
    mWin->GetWindow0()->SetBlendEnabled(enabled);
    mWin->GetWindow1()->SetBlendEnabled(enabled);
}

void MRasterWindow::SetObjBlendEnabled(BOOL enabled) {
    mWin->GetObjWindow()->SetBlendEnabled(enabled);
}

void MRasterWindow::Clear() {
    mChanged = TRUE;
    MFill(0, mBack, M_RASTER_LINES * 4);
    MFill(0, mSpans, M_RASTER_LINES * 2 * sizeof(MRasterSpan));
}

void MRasterWindow::OnVSync() {
    if (mAutoFlip && mChanged) {
        Flip();
        mChanged = FALSE;
    }
    MRasterIO::OnVSync();
}

// Sets a line's window registers from its spans. A window can't cover a whole line (256 doesn't fit in a byte), so
// that's done with the two windows covering half each.
static inline void WriteLine(MRasterSpan *spans, u8 *regs) {
    if (spans[0].right == 256 && spans[0].left == 0) {
        regs[1] = 0;
        regs[0] = 128;
        regs[3] = 128;
        regs[2] = 0;
    } else {
        regs[0] = spans[0].right;
        regs[1] = spans[0].left;
        regs[2] = spans[1].right;
        regs[3] = spans[1].left;
    }
}

// Calls op for each line of rect on the screen, with the rect's left and right edges.
template <class Op> inline void MRasterWindow::ForEachLine(MRect rect, Op op) {
    if (mAutoFlip && !mChanged) {
        Clear();
    }
    if (MRect(0, 0, 256, M_RASTER_LINES).Intersects(rect)) {
        rect = rect.Intersection(MRect(0, 0, 256, M_RASTER_LINES));
        for (int y = rect.top; y < rect.bottom; y++) {
            op(rect.left, rect.right, y);
        }
    }
}

// Adds [left, right) to a line, merged into a span it touches.
struct AddSpanOp {
    AddSpanOp(MRasterWindow *window) : mWindow(window) {}
    inline void operator()(int left, int right, int line) {
        MRasterSpan *spans = &mWindow->mSpans[line * 2];
        u8 *regs = mWindow->mBack + line * 4;
        if (spans[0].IsEmpty()) {
            spans[0].Set(left, right);
        } else if (spans[0].Touches(left, right)) {
            spans[0].Merge(left, right);
            if (!spans[1].IsEmpty() && spans[0].Touches(spans[1].left, spans[1].right)) {
                spans[0].Merge(spans[1].left, spans[1].right);
                spans[1].Clear();
            }
        } else if (spans[1].IsEmpty()) {
            spans[1].Set(left, right);
        } else if (spans[1].Touches(left, right)) {
            spans[1].Merge(left, right);
        }
        WriteLine(spans, regs);
    }

    MRasterWindow *mWindow;
};

struct SubtractSpanOp {
    SubtractSpanOp(MRasterWindow *window) : mWindow(window) {}
    void operator()(int left, int right, int line) { mWindow->SubtractSpan(left, right, line); }

    MRasterWindow *mWindow;
};

// (The span adder is big; the game inlines it anyway.)
#pragma inline_max_size(2000)
void MRasterWindow::AddRect(MRect rect) {
    ForEachLine(rect, AddSpanOp(this));
}

void MRasterWindow::SubtractSpan(int left, int right, int line) {
    if (left >= right) {
        return;
    }
    MRasterSpan *spans = &mSpans[line * 2];
    u8 *regs = mBack + line * 4;
    for (;;) {
        if (spans[0].IsEmpty()) {
            break;
        }
        if (left <= spans[0].left && right >= spans[0].right) {
            spans[0].bits = spans[1].bits;
            spans[1].Clear();
            continue;
        }
        if (left > spans[0].left && right < spans[0].right) {
            spans[1].right = spans[0].right;
            spans[0].right = left;
            spans[1].left = right;
            break;
        }
        if (left <= spans[0].left && right > spans[0].left) {
            spans[0].left = right;
        } else if (left < spans[0].right && right >= spans[0].right) {
            spans[0].right = left;
        }
        if (!spans[0].IsEmpty()) {
            if (left <= spans[1].left && right >= spans[1].right) {
                spans[1].right = 0;
                spans[1].left = 0;
            } else if (left <= spans[1].left && right > spans[1].left) {
                spans[1].left = right;
            } else if (left < spans[1].right && right >= spans[1].right) {
                spans[1].right = left;
            }
        }
        break;
    }
    WriteLine(spans, regs);
}

void MRasterWindow::SubtractRect(MRect rect) {
    ForEachLine(rect, SubtractSpanOp(this));
}

// The screen's width, which spans are clipped to.
static const int sRasterWidth = 256;

// Calls op for the lines of a disc, four at a time (midpoint circle), with each line's clipped left and right edges.
template <class Op> inline void MRasterWindow::ForEachCircleLine(MPoint center, int radius, Op op) {
    if (mAutoFlip && !mChanged) {
        Clear();
    }
    if (radius == 0) {
        return;
    }
    MRect rect(center.x - radius, center.y - radius, center.x + radius, center.y + radius);
    int bottom = center.y + radius;
    if (!MRect(0, 0, 256, M_RASTER_LINES).Intersects(rect)) {
        return;
    }
    int err = radius;
    int y = radius;
    int below = center.y;
    for (int x = 0; y >= x; x++) {
        int left = std::min(sRasterWidth, std::max(0, center.x - y));
        int right = std::min(sRasterWidth, std::max(0, center.x + y));
        int line = center.y - x;
        if (line >= 0 && line < M_RASTER_LINES) {
            op(left, right, line);
        }
        line = below - 1;
        if (line >= 0 && line < M_RASTER_LINES) {
            op(left, right, line);
        }
        left = std::min(sRasterWidth, std::max(0, center.x - x));
        right = std::min(sRasterWidth, std::max(0, center.x + x));
        line = center.y - y;
        if (line >= 0 && line < M_RASTER_LINES) {
            op(left, right, line);
        }
        line = bottom - 1;
        if (line >= 0 && line < M_RASTER_LINES) {
            op(left, right, line);
        }
        err -= 2 * x - 1;
        if (err < 0) {
            err += (y - 1) * 2;
            y--;
            bottom--;
        }
        below++;
    }
}

// NONMATCHING: stack slots, and the inlined span adds differ (as in AddRect).
void MRasterWindow::SetCircle(MPoint center, int radius) {
    ForEachCircleLine(center, radius, AddSpanOp(this));
}

// NONMATCHING: the game swaps the stack slots of the rect and its copy for Intersects, and keeps the window in a
// register from later.
void MRasterWindow::SubtractCircle(MPoint center, int radius) {
    ForEachCircleLine(center, radius, SubtractSpanOp(this));
}
