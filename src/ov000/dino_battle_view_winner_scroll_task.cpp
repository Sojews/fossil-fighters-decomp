// DinoBattleViewWinnerScrollTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_winner_scroll_task.h"
#include "types.h"
#include "game/motion_advantage_arrow.h"
#include "game/dino_battle_view_field.h"

DinoBattleViewWinnerScrollTask::~DinoBattleViewWinnerScrollTask() {
}

void DinoBattleViewLCDFader::Idle() {
}

void DinoBattleViewTutorialRect::Idle() {
}

void DinoBattleViewField::Idle() {
    Dispatch();
}

extern "C" void func_ov000_0218e680(void *);

MotionAdvantageArrow::~MotionAdvantageArrow() {
}

DinoBattleViewAdvantageArrow::~DinoBattleViewAdvantageArrow() {
}

DinoBattleViewTutorialMotion::~DinoBattleViewTutorialMotion() {
}

DinoBattleViewLCDFader::~DinoBattleViewLCDFader() {
}

TutorialWindowCloseTask::~TutorialWindowCloseTask() {
}

WaitUpdateSupportEffectAnimeTask::~WaitUpdateSupportEffectAnimeTask() {
}

DinoBattleViewTutorialRect::~DinoBattleViewTutorialRect() {
}

SetTutorialMotionArrorTask::~SetTutorialMotionArrorTask() {
}

SetTutorialMotionRectTask::~SetTutorialMotionRectTask() {
}

void DinoBattleViewTutorialMotion::Idle() {
}

void TutorialWindowCloseTask::Execute() {
    func_ov000_0218e680((void *)mUnk10);
    Exit();
}
