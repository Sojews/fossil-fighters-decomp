// DinoGuideMessage.
// The constructor (with the "  " + text operator+ instance), the destructor and the hooks are decompiled so far.

#define MSL_STRING_PLUS_DEFINITIONS
#define MSL_STRING_BASICS
#include "game/dino_guide_message.h"
#include "m/lcd.h"

// The TU's pooled constants (see menu.cpp).
extern "C" struct {
    u32 unk00;
    const char *helpFrame_04;                       // 0x04
    u8 unk08[0x18 - 0x08];
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
