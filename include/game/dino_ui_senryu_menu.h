#ifndef GAME_DINO_UI_SENRYU_MENU_H
#define GAME_DINO_UI_SENRYU_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiSenryuMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiSenryuMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u8 unkB4[0x38];
    MView *mCursor;     // 0xEC
    u8 unkF0[0x40];
    u32 mPage;          // 0x130
    u8 unk134[9];
    u8 mSilentFocus;    // 0x13D
};

#endif
