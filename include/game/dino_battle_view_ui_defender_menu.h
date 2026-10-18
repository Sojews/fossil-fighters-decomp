#ifndef GAME_DINO_BATTLE_VIEW_UI_DEFENDER_MENU_H
#define GAME_DINO_BATTLE_VIEW_UI_DEFENDER_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "game/dino_battle_view.h"

class DinoBattleViewUiDefenderMenu : public DinoBattleViewUiSelectAreaCommon, public DinoHelpBase {
public:
    virtual ~DinoBattleViewUiDefenderMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    BOOL CanDefend(u32 id);
    void ResetRows();   // (animates the two chosen rows' items back)

    u32 unk188;
    u32 mIds[3];    // 0x18C: the creatures that can defend
    int mCount;     // 0x198
};

#endif
