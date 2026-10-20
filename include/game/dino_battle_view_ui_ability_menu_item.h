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
class DinoBattleViewUiAbilityMenu;
class DinoBattleView;
class MIndicator;
class DinAttackItemHelp;
class MSmartOam;

class DinoBattleViewUiAbilityMenuItem : public MMenuItem {
public:
    DinoBattleViewUiAbilityMenuItem(DinoBattleViewUiAbilityMenu *menu, bool flag, bool flag3, u32 attack, u32 kind, u32 unk90);
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

    u32 mUnk88;                 // 0x88
    u32 mUnk8C;                 // 0x8C: attack
    u32 mUnk90;                 // 0x90
    bool mUnk94;                // 0x94
    AttackMenuBarMotion *mUnk98; // 0x98
    DinoBattleViewUiAbilityMenu *mMenu; // 0x9C
    MParallelTaskSet *mTasks;   // 0xA0
};

// What the ability menu is opened with (passed on as is).
struct DinoBattleViewUiAbilityMenuArgs {
    u32 unk[6];
};

class DinoBattleViewUiAbilityMenu : public MMenu, public DinoHelpBase {
public:
    DinoBattleViewUiAbilityMenu(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4, DinoBattleViewUiAbilityMenuArgs args,
                                u8 arg6, u32 arg7, DinoBattleView *view);
    virtual ~DinoBattleViewUiAbilityMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    MSmartOam *GetUnkB4();
    // Slides the item bars and the BG off (up a screen), then reports the menu's result and exits.
    void Close();

    MSmartOam *mUnkB4;  // 0xB4
    u32 mUnkB8;     // 0xB8
    u32 mUnkBC;     // 0xBC
    u32 mUnkC0;     // 0xC0
    u32 mUnkC4;     // 0xC4
    u32 mUnkC8;     // 0xC8
    u32 mUnkCC;     // 0xCC
    u32 unkD0;
    u8 mUnkD4;      // 0xD4
    u32 mUnkD8;     // 0xD8
    DinoBattleViewUiAbilityMenuArgs mArgs;  // 0xDC
    class DinoBattleViewAbilityMenuBG *mBG;         // 0xF4
    u32 unkF8;
    class DinoBattleViewAbilityTextWindow *mTextWindow; // 0xFC
    MIndicator *mCostText;  // 0x100
    void *mUnk104;  // 0x104: the DinoBattleView
    DinoBattleViewUiAbilityMenuItem *mItems[5];     // 0x108
    u32 mUnk11C;    // 0x11C
    u32 unk120;
    DinAttackItemHelp *mHelps[3];   // 0x124
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

    void SetItem(MMenuItem *item, u8 flag) {
        mItem = item;
        mFlag = flag;
    }

    MMenuItem *mItem;   // 0x94: the attack item it explains
    u8 mFlag;           // 0x98: the item's mUnk94
};

#endif
