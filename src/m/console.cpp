// MConsole, and MCanvasCopyTransfer (which scrolls it). Not decompiled yet: Execute (the typing itself) and the line vector's template functions.

#define MSL_STRING_BASICS
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
      mUnk28(0), mUnk2C(0), mUnk30(0),
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
    mUnk28 = 0;
    mUnk2C = 0;
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
#pragma dont_inline reset
