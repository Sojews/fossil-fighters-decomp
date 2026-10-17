#ifndef GAME_DINO_FIELD_CHARA_WAIT_MOVE_TASK_H
#define GAME_DINO_FIELD_CHARA_WAIT_MOVE_TASK_H

#include "m/task.h"

class DinoField;

class DinoFieldCharaWaitMoveTask : public MProcess {
public:
    DinoFieldCharaWaitMoveTask(DinoField *field, u32 chara) : MProcess(NULL) {
        mField = field;
        mChara = chara;
    }
    virtual ~DinoFieldCharaWaitMoveTask();
    virtual void Idle();

    DinoField *mField;  // 0x74
    u32 mChara;         // 0x78
};

#endif
