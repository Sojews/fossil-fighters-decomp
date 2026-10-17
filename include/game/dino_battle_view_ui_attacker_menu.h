#ifndef GAME_DINO_BATTLE_VIEW_UI_ATTACKER_MENU_H
#define GAME_DINO_BATTLE_VIEW_UI_ATTACKER_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "game/dino_battle_view.h"

class DinoBattleViewUiAttackerMenu : public DinoBattleViewUiSelectAreaCommon, public DinoHelpBase {
public:
    virtual ~DinoBattleViewUiAttackerMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnBlur(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

#endif
