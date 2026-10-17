#ifndef GAME_DINO_FIELD_POINT_H
#define GAME_DINO_FIELD_POINT_H

#include <string>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoFieldPoint : public MProcess {
public:
    virtual ~DinoFieldPoint();
    virtual void Idle();

    std::string mText;     // 0x74
};

class DinoFieldMoney : public DinoFieldPoint {
public:
    virtual ~DinoFieldMoney();
};

class DinoFieldDonate : public DinoFieldPoint {
public:
    virtual ~DinoFieldDonate();
};

#endif
