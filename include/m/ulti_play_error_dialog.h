#ifndef M_ULTI_PLAY_ERROR_DIALOG_H
#define M_ULTI_PLAY_ERROR_DIALOG_H

#include "m/task.h"

class MultiPlayErrorDialog : public MProcess {
public:
    virtual ~MultiPlayErrorDialog();
    virtual void Idle();
};

#endif
