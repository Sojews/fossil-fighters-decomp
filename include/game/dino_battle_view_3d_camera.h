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

#include "game/dino_3d_load_csv.h"
class M3DScene;

class DinoBattleView3DCamera : public MProcess {
public:
    DinoBattleView3DCamera(M3DScene *scene, Dino3DLoadCsv *camera, u32 unk88);
    virtual ~DinoBattleView3DCamera();
    virtual void Idle();

    void Start(u32 unk88, const void *unk9C, int light);
    void Stop();
    void Resume();
    void Reset();
    void SetLight(int light);
    void Load(const char *path, int light);
    // Has the camera follow the model (none for NULL).
    void Follow(Dino3DLoadCsv *parent) {
        mParent = parent;
        mCamera->SetParent(mParent);
    }

    void (DinoBattleView3DCamera::*mState)(); // 0x74
    Dino3DLoadCsv *mCamera;                     // 0x7C
    Dino3DLoadCsv *mParent;                     // 0x80: what the camera follows
    M3DScene *mScene;                           // 0x84
    u32 mUnk88;                                 // 0x88
    u32 mUnk8C;                                 // 0x8C
    u32 mUnk90;                                 // 0x90
    int mUnk94;                                 // 0x94
    int mLight;                                 // 0x98: or -1
    const void *mUnk9C;                         // 0x9C
    u8 mLoaded;                                 // 0xA0
    u8 mUnkA1;                                  // 0xA1
    u8 mUnkA2;                                  // 0xA2
};

#endif
