// MultiPlayErrorDialog.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/multi_play_error_dialog_menu.h"
#include "m/ulti_play_error_dialog.h"

MultiPlayErrorDialog::~MultiPlayErrorDialog() {
}

void MultiPlayErrorDialog::Idle() {
    (this->*mState)();
}

DinoMpGame::~DinoMpGame() {
}
