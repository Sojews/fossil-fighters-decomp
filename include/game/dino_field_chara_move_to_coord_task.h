#ifndef GAME_DINO_FIELD_CHARA_MOVE_TO_COORD_TASK_H
#define GAME_DINO_FIELD_CHARA_MOVE_TO_COORD_TASK_H

#include "m/fx32.h"
#include "m/task.h"

class DinoField;

class DinoFieldCharaMoveToCoordTask : public MTransfer {
public:
    DinoFieldCharaMoveToCoordTask(DinoField *field, u32 chara, MFx32Point coord, u32 a, u32 b, u8 c) {
        mField = field;
        mChara = chara;
        mCoord = coord;
        mA = a;
        mB = b;
        mC = c;
    }
    virtual ~DinoFieldCharaMoveToCoordTask();
    virtual void Transfer();

    DinoField *mField;  // 0x10
    u32 mChara;         // 0x14
    MFx32Point mCoord;  // 0x18
    u32 mA;             // 0x20
    u32 mB;             // 0x24
    u8 mC;              // 0x28
};

#endif
