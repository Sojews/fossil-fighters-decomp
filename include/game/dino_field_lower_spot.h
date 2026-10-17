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
    DinoFieldLowerSpotIn(int duration);
    virtual ~DinoFieldLowerSpotIn();
    virtual void Idle();

    u32 mUnk74;     // 0x74
    int mDuration;  // 0x78
    u32 mUnk7C;     // 0x7C
    u32 mUnk80;     // 0x80
    u32 mUnk84;     // 0x84
};

class DinoFieldLowerSpotOut : public MProcess {
public:
    DinoFieldLowerSpotOut(int duration);
    virtual ~DinoFieldLowerSpotOut();
    virtual void Idle();

    u32 mUnk74;     // 0x74
    int mDuration;  // 0x78
    u32 mUnk7C;     // 0x7C
    u32 mUnk80;     // 0x80
    u32 mUnk84;     // 0x84
};

#endif
