#ifndef GAME_DINO_FIELD_CHARA_MOVE_TO_REGION_TASK_H
#define GAME_DINO_FIELD_CHARA_MOVE_TO_REGION_TASK_H

#include "m/task.h"

class DinoField;

class DinoFieldCharaMoveToRegionTask : public MTransfer {
public:
    DinoFieldCharaMoveToRegionTask(DinoField *field, u32 chara, u32 region, u32 a, u32 b, u8 c) {
        mField = field;
        mChara = chara;
        mRegion = region;
        mA = a;
        mB = b;
        mC = c;
    }
    virtual ~DinoFieldCharaMoveToRegionTask();
    virtual void Transfer();

    DinoField *mField;  // 0x10
    u32 mChara;         // 0x14
    u32 mRegion;        // 0x18
    u32 mA;             // 0x1C
    u32 mB;             // 0x20
    u8 mC;              // 0x24
};

#endif
