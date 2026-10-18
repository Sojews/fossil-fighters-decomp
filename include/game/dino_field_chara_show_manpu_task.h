#ifndef GAME_DINO_FIELD_CHARA_SHOW_MANPU_TASK_H
#define GAME_DINO_FIELD_CHARA_SHOW_MANPU_TASK_H

#include "m/geometry.h"
#include "m/task.h"

class DinoFieldChara;

class DinoFieldCharaShowManpuTask : public MTransfer {
public:
    DinoFieldCharaShowManpuTask(DinoFieldChara *chara, u32 manpu, MPoint offset) {
        mChara = chara;
        mManpu = manpu;
        mOffset = offset;
    }
    virtual ~DinoFieldCharaShowManpuTask();
    virtual void Transfer();

    DinoFieldChara *mChara; // 0x10
    u32 mManpu;             // 0x14
    MPoint mOffset;         // 0x18
};

#endif
