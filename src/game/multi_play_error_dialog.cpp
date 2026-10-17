// MultiPlayErrorDialog.
// Only the destructors are decompiled so far.

#include "m/ulti_play_error_dialog.h"

MultiPlayErrorDialog::~MultiPlayErrorDialog() {
}

void MultiPlayErrorDialog::Idle() {
    (this->*mState)();
}
