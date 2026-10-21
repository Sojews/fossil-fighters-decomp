#ifndef GAME_DISP_BLOWER_H
#define GAME_DISP_BLOWER_H

#include "m/task.h"

class MicTestButton;

// Samples the mic for the option menu's mic test and shows the strength on its meter.
class DispBlower : public MTask {
public:
    DispBlower(MicTestButton *button) { mButton = button; }
    virtual ~DispBlower();
    virtual void Execute();

    MicTestButton *mButton;     // 0x10
};

#endif
