// DinoBattleViewWinnerScrollTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/dino_battle_view_winner_scroll_task.h"
#include "types.h"
#include "game/motion_advantage_arrow.h"
#include "game/dino_battle_view_field.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view2.h"

DinoBattleViewWinnerScrollTask::~DinoBattleViewWinnerScrollTask() {
    if (!mUnk78) {
        DinoSoundPlaySe(0x74, 0);
    }
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

void SetTutorialMotionRectTask::Execute() {
    mMotion->SetRect(MRect(mTopLeft, mBottomRight));
    Exit();
}

void SetTutorialMotionArrorTask::Execute() {
    if (mDir == 0) {
        mMotion->Hide(mIndex);
    } else {
        mMotion->ShowArrow(mIndex, mDir, mPos);
    }
    Exit();
}

void WaitUpdateSupportEffectAnimeTask::Execute() {
    for (int i = 0; i < 2; i++) {
        if (mView->GetUnkE4(i)->GetUnk108()->IsAnimating()) {
            return;
        }
    }
    Exit();
}

void TutorialWindowCloseTask::Execute() {
    func_ov000_0218e680((void *)mUnk10);
    Exit();
}
