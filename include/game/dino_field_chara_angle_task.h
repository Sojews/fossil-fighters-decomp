#ifndef GAME_DINO_FIELD_CHARA_ANGLE_TASK_H
#define GAME_DINO_FIELD_CHARA_ANGLE_TASK_H

#include "m/task.h"

class DinoField;

class DinoFieldCharaAngleTask : public MTransfer {
public:
    DinoFieldCharaAngleTask(DinoField *field, u32 chara, u32 angle, u32 a, u32 b) {
        mField = field;
        mChara = chara;
        mAngle = angle;
        mA = a;
        mB = b;
    }
    virtual ~DinoFieldCharaAngleTask();
    virtual void Transfer();

    DinoField *mField;  // 0x10
    u32 mChara;         // 0x14
    u32 mAngle;         // 0x18
    u32 mA;             // 0x1C
    u32 mB;             // 0x20
};

#endif
