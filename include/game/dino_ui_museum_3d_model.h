#ifndef GAME_DINO_UI_MUSEUM_3D_MODEL_H
#define GAME_DINO_UI_MUSEUM_3D_MODEL_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "m/fx32.h"

class M3DScene;
class MAnmModelBase;

class MExtAffinePicture;

class DinoUiMuseum3DModel : public MProcess {
public:
    virtual ~DinoUiMuseum3DModel();
    virtual void Idle();

    void SetModel(u32 id);
    void LoadModel();

    MTaskSet *mTasks;   // 0x74
    M3DScene *mScene;   // 0x78
    MAnmModelBase *mModel;  // 0x7C
    u32 mCurrent;       // 0x80: the model shown
    u32 mRequested;     // 0x84: the model to show next
    u32 mAngle;         // 0x88: of the camera around the model
    MFx32 mDistance;    // 0x8C: of the camera from the model
    MFx32 mHeight;      // 0x90: of the camera's target
    MExtAffinePicture *mBackground; // 0x94: slid into place as the model appears
    int mTimer;         // 0x98: frames since the model changed (up to 60)
};

#endif
