#ifndef GAME_DINO_EVENT_SAVE_OPTION_H
#define GAME_DINO_EVENT_SAVE_OPTION_H

#include "m/task.h"

class DinoEventLoop;
class DinoUiSaveOption;

class DinoEventSaveOption : public MProcess {
public:
    virtual ~DinoEventSaveOption();
    virtual void Idle();

    void OpenUi();

    DinoEventLoop *mEvent;  // 0x74
    u32 mStep;              // 0x78
    u32 mResult;            // 0x7C
    DinoUiSaveOption *mUi;  // 0x80
};

#endif
