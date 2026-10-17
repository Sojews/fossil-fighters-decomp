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

class CameraLoadTask : public MTask {
public:
    virtual ~CameraLoadTask();
    virtual void Execute();
};

class NextCameraTask : public MTask {
public:
    virtual ~NextCameraTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
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
};

class CameraSetDefParentTask : public MTask {
public:
    virtual ~CameraSetDefParentTask();
    virtual void Execute();
};

#endif
