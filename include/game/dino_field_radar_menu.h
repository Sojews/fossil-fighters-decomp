#ifndef GAME_DINO_FIELD_RADAR_MENU_H
#define GAME_DINO_FIELD_RADAR_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoFieldRadarManager : public MProcess, public DinoFieldCameraHook {
public:
    virtual ~DinoFieldRadarManager();
    virtual void Idle();
    virtual void CameraUnk0();
    virtual void CameraUnk1();
};

#endif
