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

    u8 GetUnk110();
    void SetUnk114();
    MPoint GetPos();

    void (DinoFieldHole::*mState)();    // 0x74
    u32 mUnk7C[3];                      // 0x7C
    MPoint mPos;                        // 0x88
    u8 unk90[0x110 - 0x90];
    u8 mUnk110;                         // 0x110
    u32 mUnk114;                        // 0x114
};

#endif
