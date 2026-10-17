#ifndef GAME_CAMERA_WIPE_H
#define GAME_CAMERA_WIPE_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class CameraWipe : public MProcess {
public:
    virtual ~CameraWipe();
    virtual void Idle();
};

class M3DFadeTask : public MTask {
public:
    virtual ~M3DFadeTask();
    virtual void Execute();
};

class DinoBattleViewActionInfoBar : public MProcess {
public:
    virtual ~DinoBattleViewActionInfoBar();
    virtual void Idle();
};

#endif
