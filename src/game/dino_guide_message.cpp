// DinoGuideMessage.
// Only the destructor is decompiled so far.

#include "game/dino_guide_message.h"

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
