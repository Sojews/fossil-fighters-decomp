#ifndef GAME_DINO_BATTLE_VIEW_UI_SELECT_ALL_H
#define GAME_DINO_BATTLE_VIEW_UI_SELECT_ALL_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewUiSelectAllMenu : public MMenu, public DinoHelpBase {
public:
    virtual ~DinoBattleViewUiSelectAllMenu();
    virtual void OnStart();
    virtual void OnDecide(MMenuItem *item);
};

#endif
