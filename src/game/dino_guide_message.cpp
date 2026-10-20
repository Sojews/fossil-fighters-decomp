// DinoGuideMessage.
// The constructor (with the "  " + text operator+ instance), the destructor and the hooks are decompiled so far.

#define MSL_STRING_PLUS_DEFINITIONS
#define MSL_STRING_BASICS
#include "m/oam_canvas.h"
#include "game/dino_guide_message.h"
#include "m/lcd.h"
#include "m/font.h"
#include "m/smart_oam.h"
#include "game/dino_resource.h"

// The TU's pooled constants (see menu.cpp).
extern "C" struct {
    u32 unk00;
    const char *helpFrame_04;                       // 0x04
    void (DinoGuideMessage::*Scroll_08)();          // 0x08
    u8 unk10[0x18 - 0x10];
    void (DinoGuideMessage::*Restart_18)();         // 0x18
} data_020b5c40;

DinoGuideMessage::DinoGuideMessage(std::string text)
    : MProcess(gLcd->GetTopDisplay()), mText("  " + text), mMotion(data_020b5c40.helpFrame_04, 2) {
    OS_InitMutex(&mMutex);
    mState = data_020b5c40.Restart_18;
}

DinoGuideMessage::~DinoGuideMessage() {
    Deactivate();
    mFont->Release();
}

void DinoGuideMessage::Idle() {
    (this->*mState)();
}

extern "C" const u32 data_020899ec;     // the text color
extern "C" const u32 data_02089a14;     // its shadow (and the strip's edges)
extern "C" const u32 data_02089a80;     // the font's background
extern "C" const u32 data_020899f0;     // frames to wait before scrolling

// Draws the text on a strip at the bottom of the top screen (twice over when it's wider than the screen, so it
// can scroll round), then waits and scrolls it.
void DinoGuideMessage::Restart() {
    MScopedLock lock(&mMutex, TRUE);
    MDisplayScope scope(GetDisplay());
    Activate();
    int color = data_020899ec;
    int edge = data_02089a14;
    mFont = gDinoResource->CreateFont12(color, edge, data_02089a80);
    mFont->AddRef();
    mOam = new MSmartOam(0x3C, 9, 1);
    MPoint size = mFont->MeasureText(mText, 0, 0);
    size.y = 16;
    if (size.x <= 256) {
        mCanvas = new MSmartObjCanvas(mOam, MPoint(256, 16), (u8 *) mMotion.GetPalette(0, NULL) + 4, TRUE);
        mCanvas->FillRect(MRect(MPoint(0, 0), mCanvas->GetSize()), color);
        mCanvas->DrawText(mFont, MPoint(0, 2), 0, std::string(mText));
    } else {
        mCanvas = new MSmartObjCanvas(mOam, MPoint(size.x + 256, size.y), (u8 *) mMotion.GetPalette(0, NULL) + 4,
                                      TRUE);
        mCanvas->FillRect(MRect(MPoint(0, 0), mCanvas->GetSize()), color);
        mCanvas->DrawText(mFont, MPoint(0, 2), 0, std::string(mText));
        mCanvas->DrawText(mFont, MPoint(size.x, 2), 0, std::string(mText));
    }
    mCanvas->FillRect(MRect(0, 0, mCanvas->GetWidth(), 1), edge);
    mCanvas->FillRect(MRect(0, 15, mCanvas->GetWidth(), 16), edge);
    mCanvas->SetPosition(0, 0xB0);
    mCanvas->Show();
    GetParallel()->AddTail(mCanvas, mOam);
    mSerial.AddTail(new MTimer(data_020899f0));
    mState = data_020b5c40.Scroll_08;
}

void DinoGuideMessage::Unk08() {
    MScopedLock lock(&mMutex, TRUE);
    mFont->Release();
    mParallel.Clear();
    mSerial.Clear();
}

void DinoGuideMessage::Unk0C() {
    MScopedLock lock(&mMutex, TRUE);
    Restart();
}

// String members the game emits in this file.
template std::string::basic_string(const std::string &other);

// (Making one here emits the canvas's destructors, which the game emits in this file.)
#pragma dont_inline on
MSmartObjCanvas *DinoGuideMessageCanvasAnchor(MDisplay *display, MSmartOam *oam, MPoint size, const void *palette) {
    return new MSmartObjCanvas(display, oam, size, palette, TRUE);
}
#pragma dont_inline reset
