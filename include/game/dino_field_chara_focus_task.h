#ifndef GAME_DINO_FIELD_CHARA_FOCUS_TASK_H
#define GAME_DINO_FIELD_CHARA_FOCUS_TASK_H

#include "m/task.h"

class DinoField;

class DinoFieldCharaFocusTask : public MTransfer {
public:
    DinoFieldCharaFocusTask(DinoField *field, u32 chara) {
        mField = field;
        mChara = chara;
    }
    virtual ~DinoFieldCharaFocusTask();
    virtual void Transfer();

    DinoField *mField;  // 0x10
    u32 mChara;         // 0x14
};

#endif
