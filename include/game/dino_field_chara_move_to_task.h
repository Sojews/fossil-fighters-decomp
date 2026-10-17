#ifndef GAME_DINO_FIELD_CHARA_MOVE_TO_TASK_H
#define GAME_DINO_FIELD_CHARA_MOVE_TO_TASK_H

#include "m/fx32.h"
#include "m/task.h"

class DinoFieldChara;

class DinoFieldCharaMoveToTask : public MTransfer {
public:
    virtual ~DinoFieldCharaMoveToTask();
    virtual void Transfer();

    DinoFieldChara *mChara; // 0x10
    MFx32Point mCoord;      // 0x14
    u32 mA;                 // 0x1C
    u32 mB;                 // 0x20
    u8 mC;                  // 0x24
};

#endif
