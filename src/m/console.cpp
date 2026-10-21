// MConsole, and MCanvasCopyTransfer (which scrolls it). Not decompiled yet: the line vector's template functions.

#define MSL_STRING_BASICS
#pragma ipa file
#include "m/console.h"
#include "m/text_codec.h"
#include "m/canvas_copy_transfer.h"
#include "m/canvas.h"
#include "m/looper.h"
#include "nitro/mi.h"
#include "m/oam_canvas.h"

void MCanvasCopyTransfer::Create(MCanvas *dst, MCanvas *src, MRect rect, MPoint pos) {
    gLooper->AddTransferTask(new MCanvasCopyTransfer(dst, src, rect, pos));
}

void MCanvasCopyTransfer::Transfer() {
    int width;
    u8 srcLine[0x100];
    u8 dstLine[0x100];
    int height = mRect.bottom - mRect.top;
    width = mRect.right - mRect.left;
    for (int i = 0; i < height; i++) {
        mSrc->GetLine(mRect.top + i, srcLine);
        mDst->GetLine(mPos.y + i, dstLine);
        MI_CpuCopy8(srcLine + mRect.left, dstLine + mPos.x, width);
        mDst->SetLine(mPos.y + i, dstLine);
    }
}

MCanvasCopyTransfer::~MCanvasCopyTransfer() {
}

MConsole::MConsole(MCanvas *canvas, MFont *font, MPoint spacing, MFx32 speed)
    : MFixedSizeView(canvas->GetSize()), mCanvas(canvas), mFont(font), mSpacing(spacing.x), mLineSpacing(spacing.y),
      mPen(0, 0), mUnk30(0),
      mRate(speed == MFx32(0) ? MFx32(0) : MFx32(font->GetMaxSize().x) / speed), mProgress(0), mUnk3C(0), mUnk40(0),
      mUnk44(0), mFilter(NULL), mCursor(0, 0), mUnk68(FALSE), mUnk69(FALSE) {
    mFont->AddRef();
    mHyphenWidth = mFont->GetGlyphSize('-').x;
}

MConsole::~MConsole() {
    mFont->Release();
    delete mCanvas;
    delete mFilter;
}

// The filters' hook (each filter class declares it).
class MConsoleFilterHook : public MConsoleFilter {
public:
    // Whether it took the character (it isn't typed).
    virtual BOOL Filter(MConsole *console, int c) = 0;
};

// Each frame: scrolls up while lines are owed; else types the frame's characters (a line break before a word that
// doesn't fit, or a hyphen inside one), suspending when the text runs out, at a form feed, or when the page is full
// (scrolling the rest in, with a scroll speed).
void MConsole::Execute() {
    if (mCanvas->mState == STATE_RUN) {
        mCanvas->Execute();
    }
    if (mUnk40 > MFx32(0)) {
        MFx32 old = mUnk40;
        mUnk40 = std::max(MFx32(0), mUnk40 - mUnk3C);
        int lines = MFx32ToInt(old) - MFx32ToInt(mUnk40);
        if (lines != 0) {
            int bottom = std::min(mUnk30, GetHeight());
            MCanvasCopyTransfer::Create(mCanvas, mCanvas, MRect(0, lines, GetWidth(), bottom), MPoint(0, 0));
            mCanvas->ClearRect(MRect(0, bottom - lines, GetWidth(), bottom));
            mUnk30 -= lines;
            mPen.y -= lines;
        }
        return;
    }
    mProgress -= mRate;
    while (mRate == MFx32(0) || mProgress < MFx32(0)) {
        u32 c;
        for (;;) {
            if (mText.size() == 0) {
                Suspend();
                return;
            }
            c = PopChar();
            if (mFilter != NULL && ((MConsoleFilterHook *)mFilter)->Filter(this, c)) {
                continue;
            }
            if (c == '\n' && mPen.x != 0) {
                mUnk60 = 0;
                mPen = MPoint(0, mUnk30 + mLineSpacing);
                continue;
            }
            if (c == '\f') {
                mUnk60 = 0;
                Suspend();
                return;
            }
            if (c >= ' ') {
                break;
            }
        }
        int w, h;
        MPoint glyph;
        glyph = mFont->GetGlyphSize(c);
        w = glyph.x;
        h = glyph.y;
        if ((mUnk60 == 0 || mUnk60 == ' ') && MIsUnkChar(c)) {
            int width = w;
            for (const wchar_t *p = mText.begin(); p != mText.end(); p++) {
                if (!MIsUnkChar(*p)) {
                    break;
                }
                width += mFont->GetGlyphSize(*p).x;
            }
            if (mPen.x + width + mUnk44 > GetSize().x && width + mUnk44 <= GetSize().x) {
                int room = GetSize().x - mUnk44 - mPen.x;
                int over = width - room;
                if (room < mFont->GetMaxSize().y * 2 || over < mFont->GetMaxSize().y * 2) {
                    mPen = MPoint(0, mUnk30 + mLineSpacing);
                    mUnk60 = 0;
                }
            }
        }
        if (mPen.x + w + mUnk44 > GetSize().x &&
            (mPen.x + w > GetSize().x || !MIsUnkTextChar(c))) {
            if (mPen.x + mHyphenWidth < GetSize().x && MIsUnkChar(mUnk60) && MIsUnkChar(c)) {
                PushChar(c);
                c = '-';
            } else {
                mPen = MPoint(0, mUnk30 + mLineSpacing);
                mUnk60 = 0;
            }
        }
        if (mPen.y + h > mUnk30) {
            mUnk30 = mPen.y + h;
            if (mUnk30 > GetHeight()) {
                PushChar(c);
                if (mUnk3C != MFx32(0)) {
                    mUnk40 = MFx32(mUnk30 - GetHeight());
                    mProgress = 0;
                    return;
                }
                Suspend();
                if (mUnk69) {
                    IsEmpty();
                }
                return;
            }
        }
        if (mUnk68 && c == ' ' && mPen.x == 0) {
            mUnk60 = c;
        } else {
            mCanvas->DrawChar(mFont, mPen, c);
            mUnk60 = c;
            mCursor.x = std::max(mCursor.x, mPen.x + w);
            mCursor.y = std::max(mCursor.y, mPen.y + h);
            mPen.x += w + mSpacing;
            mProgress += MFx32(w);
        }
    }
}

void MConsole::SetSpeed(MFx32 speed) {
    SetRate(speed == MFx32(0) ? MFx32(0) : MFx32(mFont->GetMaxSize().x) / speed);
}

MFx32 MConsole::GetSpeed() {
    return mRate == MFx32(0) ? MFx32(0) : MFx32(mFont->GetMaxSize().x) / mRate;
}

void MConsole::SetRate(MFx32 rate) {
    if (rate != mRate) {
        mProgress.v = 0;
        mRate = rate;
    }
}

void MConsole::SetUnk3C(MFx32 value) {
    mUnk3C = value;
}

MPoint MConsole::GetCursor() {
    return mCursor;
}

void MConsole::SetPosition(int x, int y) {
    SetPosition(MPoint(x, y));
}

void MConsole::SetPosition(MPoint pos) {
    mCanvas->SetPosition(pos);
    if (mState == STATE_SUSPEND && mCanvas->mState == STATE_RUN) {
        mCanvas->Execute();
    }
}

MPoint MConsole::GetPosition() {
    return mCanvas->GetPosition();
}

void MConsole::SetPriority(int priority) {
    mCanvas->SetPriority(priority);
}

int MConsole::GetPriority() {
    return mCanvas->GetPriority();
}

void MConsole::Hide() {
    mCanvas->Hide();
}

void MConsole::Show() {
    mCanvas->Show();
}

BOOL MConsole::IsHidden() {
    return mCanvas->IsHidden();
}

u32 MConsole::GetPlaneMask() {
    return mCanvas->GetPlaneMask();
}

void MConsole::Clear() {
    mProgress.v = 0;
    mPen.x = 0;
    mPen.y = 0;
    mCursor.x = 0;
    mCursor.y = 0;
    mUnk30 = 0;
    mUnk40 = 0;
    mCanvas->Clear();
    mUnk60 = 0;
    Run();
}

BOOL MConsole::IsEmpty() {
    return mText.size() == 0;
}

void MConsole::ClearText() {
    mUnk60 = 0;
    mText.clear();
}

void MConsole::SetUnk44(int value) {
    mUnk44 = value;
}

void MConsole::SetFilter(MConsoleFilter *filter) {
    delete mFilter;
    mFilter = filter;
}

void MConsole::Print(const std::string &text) {
    mText += MDecodeText(text, mFont->GetEncoding());
    Run();
}

void MConsole::Print(const std::wstring &text) {
    mText += text;
    Run();
}

// Template instances the game emits in this file.
template class std::__vec_deleter<MOamCanvas<MOam, MOamUnit>::Piece, std::allocator<MOamCanvas<MOam, MOamUnit>::Piece> >;
template class std::__vec_deleter<MOamCanvas<MSmartOam, MSmartOamUnit>::Piece,
                                  std::allocator<MOamCanvas<MSmartOam, MSmartOamUnit>::Piece> >;
template std::__vec_deleter<MRect, std::allocator<MRect> >::iterator
std::__vec_deleter<MRect, std::allocator<MRect> >::erase(iterator pos, UnkMSLStruct1);

// The next character to type, taken off the text (for the filters).
wchar_t MConsole::PopChar() {
    wchar_t c = *mText.begin();
    mText.erase(mText.begin());
    return c;
}

// Puts a character back in front of the text.
void MConsole::PushChar(int c) {
    mText.insert(mText.begin(), c);
}

// String members the game emits in this file.
template std::wstring &std::wstring::operator+=(const std::wstring &str);

// (Destroying one here emits the window's destructors, which the game emits in this file.)
#pragma dont_inline on
void MConsoleWindowAnchor(MCanvasWindow *window) {
    window->MCanvasWindow::~MCanvasWindow();
}

void MConsoleWindowDeleteAnchor(MCanvasWindow *window) {
    delete window;
}
#pragma dont_inline reset
