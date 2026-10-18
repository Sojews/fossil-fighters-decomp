#ifndef GAME_DINO_BATTLE_VIEW_UI_CHANGE_MENU_H
#define GAME_DINO_BATTLE_VIEW_UI_CHANGE_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "game/dino_battle_view.h"

class DinoBattleViewUiChangeMenu : public DinoBattleViewUiSelectAreaCommon, public DinoHelpBase {
public:
    virtual ~DinoBattleViewUiChangeMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    BOOL HasId(u32 id);

    u8 unk188[0x14];
    u32 mIds[3];        // 0x19C
    int mCount;         // 0x1A8
};

#endif
