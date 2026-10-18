#ifndef GAME_DINO_BATTLE_VIEW_3D_CAMERA_H
#define GAME_DINO_BATTLE_VIEW_3D_CAMERA_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class Dino3DLoadCsv;
class M3DScene;

class DinoBattleView3DCamera : public MProcess {
public:
    virtual ~DinoBattleView3DCamera();
    virtual void Idle();

    void Reset();
    void SetLight(int light);
    void Load(const char *path, int light);

    void (DinoBattleView3DCamera::*mState)(); // 0x74
    Dino3DLoadCsv *mCamera;                     // 0x7C
    u32 unk80;
    M3DScene *mScene;                           // 0x84
    u32 unk88;
    u32 mUnk8C;                                 // 0x8C
    u32 mUnk90;                                 // 0x90
    int mUnk94;                                 // 0x94
    u32 mUnk98;                                 // 0x98
    u32 unk9C;
    u8 mLoaded;                                 // 0xA0
    u8 mUnkA1;                                  // 0xA1
    u8 mUnkA2;                                  // 0xA2
};

#endif
