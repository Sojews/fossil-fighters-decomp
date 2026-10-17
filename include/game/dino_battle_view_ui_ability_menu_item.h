#ifndef GAME_DINO_BATTLE_VIEW_UI_ABILITY_MENU_ITEM_H
#define GAME_DINO_BATTLE_VIEW_UI_ABILITY_MENU_ITEM_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "game/dino_battle_help.h"

class DinoBattleViewUiAbilityMenuItem : public MMenuItem {
public:
    virtual ~DinoBattleViewUiAbilityMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    u32 unk88[4];
    MMenuItem *mUnk98;  // 0x98
};

class DinoBattleViewUiAbilityMenu : public MMenu, public DinoHelpBase {
public:
    virtual ~DinoBattleViewUiAbilityMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u8 unkB4[0x50];
    u32 mUnk104;    // 0x104
};

class AttackMenuBarMotion : public MProcess {
public:
    virtual ~AttackMenuBarMotion();
    virtual void Idle();

    class ShowTask : public MTask {
    public:
        virtual ~ShowTask();
        virtual void Execute();
    };

    class SetPriorityTask : public MTask {
    public:
        virtual ~SetPriorityTask();
        virtual void Execute();
    };
};

class MenuEnableTask : public MTask {
public:
    virtual ~MenuEnableTask();
    virtual void Execute();

    MMenu *mMenu;   // 0x10
    u8 mEnable;     // 0x14
};

class DinAttackItemHelp : public DinoBattleHelpBase {
public:
    virtual ~DinAttackItemHelp();
    virtual MRect GetRect();
};

#endif
