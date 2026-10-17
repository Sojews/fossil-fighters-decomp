// DinoEventKasekiRockFound.
// Only the destructors are decompiled so far.

#include "game/dino_event_kaseki_rock_found.h"
#include "game/dino_field.h"

DinoEventKasekiRockFound::~DinoEventKasekiRockFound() {
    mEvent->GetField()->SetUnk258(FALSE);
}

void DinoEventKasekiRockFound::Idle() {
    (this->*mState)();
}
