#ifndef GAME_DINO_FIELD_LOWER_SPOT_H
#define GAME_DINO_FIELD_LOWER_SPOT_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoFieldLowerSpotIn : public MProcess {
public:
    virtual ~DinoFieldLowerSpotIn();
    virtual void Idle();
};

class DinoFieldLowerSpotOut : public MProcess {
public:
    virtual ~DinoFieldLowerSpotOut();
    virtual void Idle();
};

#endif
