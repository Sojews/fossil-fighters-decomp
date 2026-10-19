#ifndef GAME_CAMERA_LOAD_TASK_H
#define GAME_CAMERA_LOAD_TASK_H

#include <string.h>

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class Dino3DLoadCsv;
class DinoBattleView3DCamera;

class CameraLoadTask : public MTask {
public:
    CameraLoadTask(DinoBattleView3DCamera *camera, const char *name, int light) {
        mCamera = camera;
        mLight = light;
        strcpy(mName, name);
    }
    virtual ~CameraLoadTask();
    virtual void Execute();

    char mName[0x40];   // 0x10
    DinoBattleView3DCamera *mCamera;    // 0x50
    int mLight;         // 0x54
};

class NextCameraTask : public MTask {
public:
    NextCameraTask(DinoBattleView3DCamera *camera, int light) {
        mCamera = camera;
        mLight = light;
    }
    virtual ~NextCameraTask();
    virtual void Execute();

    DinoBattleView3DCamera *mCamera;    // 0x10
    int mLight;         // 0x14
};

class ChangeSetTask : public MTask {
public:
    ChangeSetTask(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
        mUnk10 = arg0;
        mUnk14 = arg1;
        mUnk18 = arg3;
        mUnk1C = arg2;
    }
    virtual ~ChangeSetTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
    u32 mUnk18;         // 0x18
    u32 mUnk1C;         // 0x1C
};

class CameraPauseTask : public MTask {
public:
    CameraPauseTask(void *camera, u8 pause) {
        mCamera = camera;
        mPause = pause;
    }
    virtual ~CameraPauseTask();
    virtual void Execute();

    void *mCamera;  // 0x10
    u8 mPause;      // 0x14
};

class CameraSetDefParentTask : public MTask {
public:
    struct Camera;
    CameraSetDefParentTask(Camera *camera, Dino3DLoadCsv *parent) {
        mCamera = camera;
        mParent = parent;
    }
    virtual ~CameraSetDefParentTask();
    virtual void Execute();

    struct Camera {
        u8 unk00[0x7C];
        Dino3DLoadCsv *mUnk7C;  // 0x7C
        Dino3DLoadCsv *mParent; // 0x80
    } *mCamera;         // 0x10
    Dino3DLoadCsv *mParent; // 0x14
};

#endif
