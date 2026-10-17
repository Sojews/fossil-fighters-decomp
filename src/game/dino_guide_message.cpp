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
