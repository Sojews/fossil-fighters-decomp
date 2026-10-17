// DinoEventKasekiRockGet: the fossil rock get screen, and its container select.
// Only the destructors are decompiled so far.

#include "game/dino_event_exec.h"

DinoEventKasekiRockGet::~DinoEventKasekiRockGet() {
}

DinoEventKasekiRockGetContainerSelect::~DinoEventKasekiRockGetContainerSelect() {
}

void DinoEventKasekiRockGet::Idle() {
    (this->*mState)();
}
