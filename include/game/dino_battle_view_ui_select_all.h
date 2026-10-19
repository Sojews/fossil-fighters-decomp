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

class DinoBattleView;

class DinoBattleViewUiSelectAllMenu : public MMenu, public DinoHelpBase {
public:
    DinoBattleViewUiSelectAllMenu(DinoBattleView *view, int player, MMessageObserver *target, const u32 *ids,
                                  int count);
    virtual ~DinoBattleViewUiSelectAllMenu();
    virtual void OnStart();
    virtual void OnDecide(MMenuItem *item);

    u32 mView;          // 0xB4: DinoBattleView
    u32 mPlayer;        // 0xB8
    MMessageObserver *mTarget; // 0xBC: told the choice
    u32 mIds[3];        // 0xC0: creature ids
    int mCount;         // 0xCC
    u32 unkD0[3];
    MMenuItem *mOkItem; // 0xDC
};

#endif
