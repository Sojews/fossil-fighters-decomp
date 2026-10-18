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

class DinoUiMuseum3DModel : public MProcess {
public:
    virtual ~DinoUiMuseum3DModel();
    virtual void Idle();

    void SetModel(u32 id);

    MTaskSet *mTasks;   // 0x74
    u32 unk78[2];
    u32 mCurrent;       // 0x80: the model shown
    u32 mRequested;     // 0x84: the model to show next
};

#endif
