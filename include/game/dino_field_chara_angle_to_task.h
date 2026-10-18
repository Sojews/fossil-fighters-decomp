#ifndef GAME_DINO_FIELD_CHARA_ANGLE_TO_TASK_H
#define GAME_DINO_FIELD_CHARA_ANGLE_TO_TASK_H

#include "m/task.h"

class DinoFieldChara;

class DinoFieldCharaAngleToTask : public MTransfer {
public:
    DinoFieldCharaAngleToTask(DinoFieldChara *chara, u32 angle, u32 a, u32 b) {
        mChara = chara;
        mAngle = angle;
        mA = a;
        mB = b;
    }
    virtual ~DinoFieldCharaAngleToTask();
    virtual void Transfer();

    DinoFieldChara *mChara; // 0x10
    u32 mAngle;             // 0x14
    u32 mA;                 // 0x18
    u32 mB;                 // 0x1C
};

#endif
