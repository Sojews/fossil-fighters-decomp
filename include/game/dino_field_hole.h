#ifndef GAME_DINO_FIELD_HOLE_H
#define GAME_DINO_FIELD_HOLE_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoFieldHole : public MProcess {
public:
    virtual ~DinoFieldHole();
    virtual void Idle();
};

#endif
