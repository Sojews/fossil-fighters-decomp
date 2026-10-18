// DinoEventKasekiRockGet: the fossil rock get screen, and its container select.
// Also the string vector instances the game emits here.

#pragma ipa file

#include "game/dino_event_exec.h"

DinoEventKasekiRockGet::~DinoEventKasekiRockGet() {
}

DinoEventKasekiRockGetContainerSelect::~DinoEventKasekiRockGetContainerSelect() {
}

void DinoEventKasekiRockGet::Idle() {
    (this->*mState)();
}

template class std::__vec_deleter<std::string, std::allocator<std::string> >;
