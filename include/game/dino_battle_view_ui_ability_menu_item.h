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

class AttackMenuBarMotion;

class DinoBattleViewUiAbilityMenuItem : public MMenuItem {
public:
    virtual ~DinoBattleViewUiAbilityMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MRect GetRect();

    MPoint GetBarPosition();
    void SetBarPosition(MPoint pos);
    u32 GetUnk8C();
    u8 GetUnk94();
    u32 GetUnk90();
    AttackMenuBarMotion *GetBar();

    u32 unk88;
    u32 mUnk8C;                 // 0x8C
    u32 mUnk90;                 // 0x90
    u8 mUnk94;                  // 0x94
    AttackMenuBarMotion *mUnk98; // 0x98
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

class MMotion;

class MSmartOam;

class AttackMenuBarMotion : public MProcess {
public:
    AttackMenuBarMotion(void *owner, MSmartOam *oam, u32 flags);
    AttackMenuBarMotion(void *owner, MSmartOam *oam, u32 attack, bool flag1, bool flag2, bool flag3, bool flag4);
    virtual ~AttackMenuBarMotion();
    virtual void Idle();
    virtual MRect GetFrame();

    void Init(MSmartOam *oam, u32 attack, bool flag1, bool flag2, bool flag3, bool flag4);
    void SetPriority(int priority);
    void SetPosition(MPoint pos);
    MPoint GetPosition();
    void Hide();
    void Show();
    MTask *CreateShowTask();
    void Play(int anime);

    class ShowTask : public MTask {
    public:
        ShowTask(AttackMenuBarMotion *bar) { mBar = bar; }
        virtual ~ShowTask();
        virtual void Execute();

        AttackMenuBarMotion *mBar;  // 0x10
    };

    class SetPriorityTask : public MTask {
    public:
        virtual ~SetPriorityTask();
        virtual void Execute();

        AttackMenuBarMotion *mBar;  // 0x10
        int mPriority;              // 0x14
    };

    MMotion *mMotion;   // 0x74
    MView *mUnk78;      // 0x78: optional
    MView *mUnk7C;      // 0x7C
    MView *mUnk80;      // 0x80
    void *mOwner;       // 0x84
    u32 mFlags;         // 0x88: attack id, flag1 << 16, flag2 << 17
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
