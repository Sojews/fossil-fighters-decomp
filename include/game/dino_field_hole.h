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
#include "m/3d_resource.h"

class MAnmModelBase;

class DinoFieldHole : public MProcess {
public:
    virtual ~DinoFieldHole();
    virtual void Idle();

    u8 GetUnk110();
    void SetUnk114();
    MPoint GetPos();
    void ChangeToHole();
    void StartChangeToHole();
    void StateWait();
    void StateCountdown();

    void (DinoFieldHole::*mState)();    // 0x74
    u32 mUnk7C;                         // 0x7C
    u32 mUnk80;                         // 0x80
    MAnmModelBase *mUnk84;              // 0x84
    MPoint mPos;                        // 0x88
    u32 unk90;
    M3DModelResource mModel;            // 0x94
    u8 unkF8[0x110 - 0xF8];
    u8 mUnk110;                         // 0x110
    u32 mUnk114;                        // 0x114
    u8 mUnk118;                         // 0x118
};

#endif
