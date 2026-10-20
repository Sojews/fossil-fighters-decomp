#ifndef GAME_DINO_BATTLE_VIEW2_H
#define GAME_DINO_BATTLE_VIEW2_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "nitro/g2.h"
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/input.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_common_yes_no_menu_item_menu.h"
#include "game/dino_field_camera_hook.h"
#include "game/dino_common_yes_no_menu_item.h"

class AnyInputTimerWait : public MProcess {
public:
    AnyInputTimerWait(int frames);
    virtual ~AnyInputTimerWait();
    virtual void Idle();

    MInput *mInput; // 0x74
    int mTimer;     // 0x78
};

class DinoBattleView;
class MIndicator;
struct MSmartOamUnit;

class DinoBattleViewUiRootSelect : public MMenu, public DinoHelpBase {
public:
    DinoBattleViewUiRootSelect(DinoBattleView *view, MMessageObserver *observer, u32 unkC0, u32 unkC4, u32 unkBC,
                               u32 unkC8, u32 unkCC);
    virtual ~DinoBattleViewUiRootSelect();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    void StateBegin();
    void StateIdle();

    void (DinoBattleViewUiRootSelect::*mState)();   // 0xB4
    u32 mUnkBC;         // 0xBC
    u32 mUnkC0;         // 0xC0
    u32 mUnkC4;         // 0xC4
    u32 mUnkC8;         // 0xC8
    u32 mUnkCC;         // 0xCC
    u32 mUnkD0;         // 0xD0
    u8 mGivingUp;       // 0xD4: set by the giving-up menu
    u8 mSilentFocus;    // 0xD5
    u8 unkD6[2];
    DinoBattleView *mView;          // 0xD8
    u8 unkDC[0xFC - 0xDC];
    MMessageObserver *mObserver;    // 0xFC
    std::vector<void *> mUnk100;    // 0x100
};

class DinoBattleViewTurnEndDialog : public DinoCommonYesNoMenu {
public:
    DinoBattleViewTurnEndDialog(u32 view, MMessageObserver *target, BOOL focusNo);
    virtual ~DinoBattleViewTurnEndDialog();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    MMessageObserver *mTarget; // 0xDC: told the answer
    u32 mUnkE0;     // 0xE0
};

class DinoBattleViewGivingUpDialog : public DinoCommonYesNoMenu {
public:
    DinoBattleViewGivingUpDialog(const char *text, u8 *givingUp);
    virtual ~DinoBattleViewGivingUpDialog();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u8 *mGivingUp;  // 0xDC
};

class DinoBattleViewGivingUpMenu : public MProcess {
public:
    DinoBattleViewGivingUpMenu(MMessageObserver *view, u8 *result);
    virtual ~DinoBattleViewGivingUpMenu();
    virtual void Idle();

    void StateAsk();
    void StateAnswered();
    void StateDone();

    MMessageObserver *mView;    // 0x74
    void (DinoBattleViewGivingUpMenu::*mState)(); // 0x78
    u8 mGaveUp;                 // 0x80
    u8 *mResult;                // 0x84
};

class MotionDisableHelp : public MMotion, public DinoHelpBase {
public:
    virtual ~MotionDisableHelp();
    // slot unk3 0x2060710 _ZN7MMotion11SetPositionE6MPoint
    // slot unk4 0x2060770 _ZN7MMotion11GetPositionEv
    // slot unk5 0x20619a4 _ZN7MMotion7SetSizeE6MPoint
    // slot unk6 0x20619ac _ZN7MMotion7GetSizeEv
    // slot unk7 0x2060c7c _ZN7MMotion8GetFrameEv
    // slot unk8 0x206077c _ZN7MMotion11SetPriorityEi
    // slot unk9 0x20607ac _ZN7MMotion11GetPriorityEv
    // slot unk10 0x2060804 _ZN7MMotion4ShowEv
    // slot unk11 0x206089c _ZN7MMotion4HideEv
    // slot unk12 0x20608dc _ZN7MMotion8IsHiddenEv
    // slot unk13 0x20619d0 _ZN7MMotion12GetPlaneMaskEv
    // slot unk14 0x2060ec8 _ZN7MMotion4PlayEii
    // slot unk15 0x20607b8 _ZN7MMotion9SetAnchorEi
    // slot unk16 0x20607e8 _ZN7MMotion9GetAnchorEv
    // slot unk17 0x206199c _ZN7MMotion5Unk44Ei
    // slot unk18 0x20619a0 _ZN7MMotion5Unk48Ei
};

class DinoBattleViewCreature;

class DinoBattleViewCreatureAttackIcon : public MMenuItem {
public:
    virtual ~DinoBattleViewCreatureAttackIcon();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    DinoBattleViewCreature *mCreature;  // 0x88
};

class DinoBattleViewUiRootSelectItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoBattleViewUiRootSelectItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x4];
    MMotion *mMotion;   // 0x98
    u8 unk9C[0x4];
    u32 mTextId;        // 0xA0
    int mFocusAnime;    // 0xA4
    int mBlurAnime;     // 0xA8
};

class MenuExitTask : public MTask {
public:
    MenuExitTask(MTask *menu, u8 *flag) {
        mMenu = menu;
        mFlag = flag;
    }
    virtual ~MenuExitTask();
    virtual void Execute();

    MTask *mMenu;   // 0x10
    u8 *mFlag;      // 0x14
};

class WaitEnableRootMenu : public MTask {
public:
    WaitEnableRootMenu(void *menu) { mMenu = menu; }
    virtual ~WaitEnableRootMenu();
    virtual void Execute();

    void *mMenu;    // 0x10
};

class DinoBattleView;

class DinoBattleViewAbilityIcon : public MProcess {
public:
    DinoBattleViewAbilityIcon(DinoBattleView *view, u32 unk, MPoint pos);
    virtual ~DinoBattleViewAbilityIcon();
    virtual void Idle();

    void Update(u32 mask, int *sums);
    BOOL IsAnimating();
    MRect GetRect();
    BOOL IsHidden();

    MMotion *mMotion;       // 0x74
    MSmartOam *mOam;        // 0x78
    MAfnBlock *mAffine;     // 0x7C
    MIndicator *mTexts[4];  // 0x80: by ability
    MSmartOamUnit *mUnits[4]; // 0x90: by ability
    GXOamAffine *mOamAffine; // 0xA0: the counting abilities' scale
    u32 mMask;              // 0xA4
    int mTarget[4];         // 0xA8
    int mValue[4];          // 0xB8: counts toward mTarget
    u8 mWasCounting;        // 0xC8
    u8 mCounting;           // 0xC9
};

#endif
