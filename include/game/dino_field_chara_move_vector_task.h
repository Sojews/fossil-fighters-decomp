#ifndef GAME_DINO_FIELD_CHARA_MOVE_VECTOR_TASK_H
#define GAME_DINO_FIELD_CHARA_MOVE_VECTOR_TASK_H

#include "m/task.h"

class DinoField;

class DinoFieldCharaMoveVectorTask : public MTransfer {
public:
    DinoFieldCharaMoveVectorTask(DinoField *field, u32 chara, u16 direction, u32 a, u32 b, u32 c, u8 d) {
        mField = field;
        mChara = chara;
        mDirection = direction;
        mA = a;
        mB = b;
        mC = c;
        mD = d;
    }
    virtual ~DinoFieldCharaMoveVectorTask();
    virtual void Transfer();

    DinoField *mField;  // 0x10
    u32 mChara;         // 0x14
    u16 mDirection;     // 0x18
    u32 mA;             // 0x1C
    u32 mB;             // 0x20
    u32 mC;             // 0x24
    u8 mD;              // 0x28
};

#endif
