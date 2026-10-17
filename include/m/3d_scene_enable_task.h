#ifndef M_3D_SCENE_ENABLE_TASK_H
#define M_3D_SCENE_ENABLE_TASK_H

#include "m/task.h"

class M3DSceneEnableTask : public MTask {
public:
    virtual ~M3DSceneEnableTask();
    virtual void Execute();
};

#endif
