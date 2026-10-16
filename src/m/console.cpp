// MConsole. Not decompiled yet: Execute (the typing itself) and the line vector's template functions.

#include "m/console.h"
#include "m/text_codec.h"

// NONMATCHING (also SetSpeed, GetSpeed): the game doesn't copy the divisor when dividing by speed.
MConsole::MConsole(MCanvas *canvas, MFont *font, int spacing, int lineSpacing, MFx32 speed)
    : MFixedSizeView(canvas->GetSize()) {
    mCanvas = canvas;
    mFont = font;
    mSpacing = spacing;
    mLineSpacing = lineSpacing;
    mUnk28 = 0;
    mUnk2C = 0;
    mUnk30 = 0;
    mRate = speed == MFx32(0) ? MFx32(0) : MFx32(font->GetMaxSize().x) / speed;
    mProgress.v = 0;
    mUnk3C.v = 0;
    mUnk40 = 0;
    mUnk44 = 0;
    mFilter = NULL;
    mCursor.x = 0;
    mCursor.y = 0;
    mUnk68 = FALSE;
    mUnk69 = FALSE;
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
    MFx32 rate = mRate;
    return rate == MFx32(0) ? MFx32(0) : MFx32(mFont->GetMaxSize().x) / rate;
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
