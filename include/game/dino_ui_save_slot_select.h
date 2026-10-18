#ifndef GAME_DINO_UI_SAVE_SLOT_SELECT_H
#define GAME_DINO_UI_SAVE_SLOT_SELECT_H

#include "m/task.h"

class DinoUiSaveSlotSelect : public MProcess {
public:
    DinoUiSaveSlotSelect(u32 a, u32 b);
    virtual ~DinoUiSaveSlotSelect();
    virtual void Idle();

    u32 mUnk74;     // 0x74
    u32 mUnk78;     // 0x78
};

#endif
