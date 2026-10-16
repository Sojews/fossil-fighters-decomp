// Raster effects.

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
    mSpans = new int[M_RASTER_LINES * 4];
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
    MFill(0, mSpans, M_RASTER_LINES * 4 * sizeof(int));
}

void MRasterWindow::OnVSync() {
    if (mAutoFlip && mChanged) {
        Flip();
        mChanged = FALSE;
    }
    MRasterIO::OnVSync();
}
