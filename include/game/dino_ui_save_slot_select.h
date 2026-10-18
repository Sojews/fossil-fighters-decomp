#ifndef GAME_DINO_UI_SAVE_SLOT_SELECT_H
#define GAME_DINO_UI_SAVE_SLOT_SELECT_H

#include "m/task.h"

class MInput;

class DinoUiSaveSlotSelect : public MProcess {
public:
    DinoUiSaveSlotSelect(MInput *input, u32 *result);
    virtual ~DinoUiSaveSlotSelect();
    virtual void Idle();

    MInput *mInput;     // 0x74
    u32 *mResult;       // 0x78
};

#endif
