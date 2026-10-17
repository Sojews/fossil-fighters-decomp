#ifndef GAME_DINO_UI_SAVE_OPTION_H
#define GAME_DINO_UI_SAVE_OPTION_H

#include "m/task.h"

class DinoUiSaveOption : public MProcess {
public:
    virtual ~DinoUiSaveOption();
    virtual void Idle();

    u32 mUnk74;     // 0x74: passed on to the menu
};

#endif
