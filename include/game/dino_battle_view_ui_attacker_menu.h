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
    DinoBattleViewUiAttackerMenu(DinoBattleView *view, int player, MMessageObserver *target, const u32 *ids, int count,
                                 u32 attacker, u32 arg6);
    virtual ~DinoBattleViewUiAttackerMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnBlur(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    BOOL HasId(u32 id);

    int mSide;          // 0x188
    u32 mIds[3];        // 0x18C
    int mCount;         // 0x198
    u32 mUnk19C;        // 0x19C
};

#endif
