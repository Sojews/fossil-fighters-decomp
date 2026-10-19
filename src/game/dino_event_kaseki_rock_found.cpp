// DinoEventKasekiRockFound.
// Only the destructors are decompiled so far.

#include "game/dino_event_kaseki_rock_found.h"
#include "game/dino_field.h"
#include "m/lcd.h"

DinoEventKasekiRockFound::~DinoEventKasekiRockFound() {
    mEvent->GetField()->SetUnk258(FALSE);
}

void DinoEventKasekiRockFound::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    u8 unk00[0x68];
    void (DinoEventKasekiRockFound::*Start_68)();
} data_020bd380;

DinoEventKasekiRockFound::DinoEventKasekiRockFound(DinoEventLoop *event, u32 arg, MPoint pos)
    : MProcess(gLcd->GetBottomDisplay()), mEvent(event), mPos(pos), mArg(arg) {
    mMessages = MResource<DinoMessageData>("/msg/msg_dig");
    mState = data_020bd380.Start_68;
}

#pragma auto_inline off
// Vector instances the game emits in this file (made explicitly until the code that uses them is decompiled).
template class std::__vec_deleter<std::string, std::allocator<std::string> >;
