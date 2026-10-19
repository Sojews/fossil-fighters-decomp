#ifndef M_ULTI_PLAY_ERROR_DIALOG_H
#define M_ULTI_PLAY_ERROR_DIALOG_H

#include "m/task.h"

class MultiPlayErrorDialog : public MProcess {
public:
    MultiPlayErrorDialog();
    virtual ~MultiPlayErrorDialog();
    virtual void Idle();

    // Shows the error, then waits and ends (the game's wait state is DinoMpGame's, folded with it).
    void ErrorStart();

    void (MultiPlayErrorDialog::*mState)(); // 0x74
};

#endif
