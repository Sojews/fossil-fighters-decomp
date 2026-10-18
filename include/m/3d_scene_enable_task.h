#ifndef M_3D_SCENE_ENABLE_TASK_H
#define M_3D_SCENE_ENABLE_TASK_H

#include "m/task.h"

class M3DScene;

class M3DSceneEnableTask : public MTask {
public:
    M3DSceneEnableTask(M3DScene *scene, u8 enable) {
        mScene = scene;
        mEnable = enable;
    }
    virtual ~M3DSceneEnableTask();
    virtual void Execute();

    M3DScene *mScene;   // 0x10
    u8 mEnable;         // 0x14
};

#endif
