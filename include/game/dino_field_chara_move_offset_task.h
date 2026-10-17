#ifndef GAME_DINO_FIELD_CHARA_MOVE_OFFSET_TASK_H
#define GAME_DINO_FIELD_CHARA_MOVE_OFFSET_TASK_H

#include "m/fx32.h"
#include "m/task.h"

class DinoField;

class DinoFieldCharaMoveOffsetTask : public MTransfer {
public:
    DinoFieldCharaMoveOffsetTask(DinoField *field, u32 chara, MFx32Point offset, u32 a, u32 b, u8 c) {
        mField = field;
        mChara = chara;
        mOffset = offset;
        mA = a;
        mB = b;
        mC = c;
    }
    virtual ~DinoFieldCharaMoveOffsetTask();
    virtual void Transfer();

    DinoField *mField;  // 0x10
    u32 mChara;         // 0x14
    MFx32Point mOffset;  // 0x18
    u32 mA;             // 0x20
    u32 mB;             // 0x24
    u8 mC;              // 0x28
};

#endif
