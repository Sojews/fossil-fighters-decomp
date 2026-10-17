// DinoBattleView2.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view2.h"
#include "types.h"

AnyInputTimerWait::~AnyInputTimerWait() {
}

DinoBattleViewUiRootSelect::~DinoBattleViewUiRootSelect() {
}

DinoBattleViewTurnEndDialog::~DinoBattleViewTurnEndDialog() {
}

DinoBattleViewGivingUpDialog::~DinoBattleViewGivingUpDialog() {
}

DinoBattleViewGivingUpMenu::~DinoBattleViewGivingUpMenu() {
}

MotionDisableHelp::~MotionDisableHelp() {
}

DinoBattleViewCreatureAttackIcon::~DinoBattleViewCreatureAttackIcon() {
}

DinoBattleViewUiRootSelectItem::~DinoBattleViewUiRootSelectItem() {
}

MenuExitTask::~MenuExitTask() {
}

WaitEnableRootMenu::~WaitEnableRootMenu() {
}

DinoBattleViewAbilityIcon::~DinoBattleViewAbilityIcon() {
}

void DinoBattleViewUiRootSelectItem::OnDecide() {
}

void DinoBattleViewCreatureAttackIcon::OnFocusChanged(BOOL value) {
}

void DinoBattleViewCreatureAttackIcon::OnDecide() {
}

MRect DinoBattleViewUiRootSelectItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoBattleViewUiRootSelectItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoBattleViewUiRootSelectItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoBattleViewUiRootSelectItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoBattleViewGivingUpMenu::Idle() {
    (this->*mState)();
}

extern "C" int func_ov000_0215bbd0(void *menu);

void WaitEnableRootMenu::Execute() {
    if (func_ov000_0215bbd0(mMenu) <= 0) {
        Exit();
    }
}

void DinoBattleViewUiRootSelectItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(focused ? mFocusAnime : mBlurAnime, 0);
}
