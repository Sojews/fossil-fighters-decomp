#ifndef GAME_DINO_FIELD_POINT_H
#define GAME_DINO_FIELD_POINT_H

#include "m/geometry.h"
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
    DinoFieldPoint(const std::string &path, int digits, u32 a, u32 b, u32 c);
    virtual ~DinoFieldPoint();
    virtual void Idle();
    virtual void Format(char *buf) = 0;  // the shown value
    MTask *CreateShowTask();
    MTask *CreateHideTask();

    std::string mText;     // 0x74
    int mDigits;           // 0x80
    u32 mA;                // 0x84
    u32 mB;                // 0x88
    u32 mC;                // 0x8C
    u32 mUnk90[14];        // 0x90
};

class DinoFieldMoney : public DinoFieldPoint {
public:
    DinoFieldMoney();
    virtual ~DinoFieldMoney();
    virtual void Format(char *buf);
};

class DinoFieldDonate : public DinoFieldPoint {
public:
    DinoFieldDonate();
    virtual ~DinoFieldDonate();
    virtual void Format(char *buf);
};

#endif
