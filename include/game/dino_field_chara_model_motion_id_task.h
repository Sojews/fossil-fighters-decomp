#ifndef GAME_DINO_FIELD_CHARA_MODEL_MOTION_ID_TASK_H
#define GAME_DINO_FIELD_CHARA_MODEL_MOTION_ID_TASK_H

#include "m/fx32.h"
#include "m/task.h"

class DinoFieldChara;

class DinoFieldCharaModelMotionIdTask : public MTransfer {
public:
    DinoFieldCharaModelMotionIdTask(DinoFieldChara *chara, u32 motion, u8 loop, const MFx32 &speed, u8 force) {
        mChara = chara;
        mMotion = motion;
        mLoop = loop;
        mSpeed = speed;
        mForce = force;
    }
    virtual ~DinoFieldCharaModelMotionIdTask();
    virtual void Transfer();

    DinoFieldChara *mChara; // 0x10
    u32 mMotion;            // 0x14
    u8 mLoop;               // 0x18
    MFx32 mSpeed;           // 0x1C
    u8 mForce;              // 0x20: restart the motion even if it is playing
};

#endif
