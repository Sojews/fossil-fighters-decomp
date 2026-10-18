#ifndef GAME_CAMERA_LOAD_TASK_H
#define GAME_CAMERA_LOAD_TASK_H

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
    virtual ~CameraLoadTask();
    virtual void Execute();

    char mName[0x40];   // 0x10
    DinoBattleView3DCamera *mCamera;    // 0x50
    int mLight;         // 0x54
};

class NextCameraTask : public MTask {
public:
    virtual ~NextCameraTask();
    virtual void Execute();

    DinoBattleView3DCamera *mCamera;    // 0x10
    int mLight;         // 0x14
};

class ChangeSetTask : public MTask {
public:
    virtual ~ChangeSetTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
    u32 mUnk18;         // 0x18
    u32 mUnk1C;         // 0x1C
};

class CameraPauseTask : public MTask {
public:
    virtual ~CameraPauseTask();
    virtual void Execute();

    void *mCamera;  // 0x10
    u8 mPause;      // 0x14
};

class CameraSetDefParentTask : public MTask {
public:
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
