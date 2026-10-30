// DinoBattleViewUiAbilityMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_ability_text_window.h"
#include "game/dino_battle_view_ability_menu_bg.h"
#include "game/dino_battle_view_ui_ability_menu_item.h"
#include "types.h"
#include "m/anime.h"

DinoBattleViewUiAbilityMenuItem::~DinoBattleViewUiAbilityMenuItem() {
}

DinoBattleViewUiAbilityMenu::~DinoBattleViewUiAbilityMenu() {
}

AttackMenuBarMotion::~AttackMenuBarMotion() {
}

MenuEnableTask::~MenuEnableTask() {
}

DinAttackItemHelp::~DinAttackItemHelp() {
}

// The help sits on the item (two pixels higher for some).
MRect DinAttackItemHelp::GetRect() {
    MRect r;
    if (mItem != NULL) {
        MRect frame = mItem->GetFrame();
        if (mFlag) {
            int y = frame.top - 2;
            return DinoBattleHelpBase::GetRect().Moved(frame.left, y);
        }
        int y = frame.top;
        return DinoBattleHelpBase::GetRect().Moved(frame.left, y);
    }
    r = MRect(0, 0, 0, 0);
    return r;
}

void AttackMenuBarMotion::Idle() {
}

AttackMenuBarMotion::ShowTask::~ShowTask() {
}

AttackMenuBarMotion::SetPriorityTask::~SetPriorityTask() {
}

DinoBattleViewAbilityMenuBG::SetPriorityTask::~SetPriorityTask() {
}

DinoBattleViewAbilityTextWindow::ShowTask::~ShowTask() {
}

void DinoBattleViewAbilityTextWindow::Execute() {
    MTask *task = (MTask *)mTask;
    if (task != NULL && task->mState == MTask::STATE_RUN) {
        task->Execute();
    }
}

#pragma auto_inline off

void DinoBattleViewAbilityTextWindow::StartAnime(MRect from, MRect to, u32 duration, int ease) {
    if (mTask != NULL) {
        delete mTask;
    }
    mTask = new MRectAnime(from, to);
    mTask->SetDuration(duration);
    mTask->mEase = ease;
    if (mTask->mState == MTask::STATE_RUN) {
        mTask->Execute();
    }
}

void DinoBattleViewAbilityTextWindow::SetRect(MRect rect) {
    StartAnime(rect, rect, 0, 0);
}

// NONMATCHING: the game copies the four fields straight off the anime pointer (offsets 0x30-0x3c); ours forms &mValue
MRect DinoBattleViewAbilityTextWindow::GetRect() {
    if (mTask != NULL) {
        return mTask->GetValue();
    }
    return MRect(0, 0, 0, 0);
}

void DinoBattleViewAbilityTextWindow::Show() {
    mShown = TRUE;
}

void DinoBattleViewAbilityTextWindow::Hide() {
    mShown = FALSE;
}

BOOL DinoBattleViewAbilityTextWindow::IsHidden() {
    if (mShown == 0) {
        return TRUE;
    }
    return FALSE;
}

u8 DinoBattleViewAbilityTextWindow::GetUnk18() {
    return mUnk18;
}

u32 DinoBattleViewAbilityTextWindow::GetUnk1C() {
    return mUnk1C;
}

void DinoBattleViewAbilityTextWindow::SetUnk1C(u32 value) {
    mUnk1C = value;
}

MTask *DinoBattleViewAbilityTextWindow::CreateShowTask() {
    return new ShowTask(this, TRUE);
}

void DinoBattleViewAbilityTextWindow::ShowTask::Execute() {
    if (mShow) {
        mWindow->Show();
    } else {
        mWindow->Hide();
    }
    Exit();
}

void DinoBattleViewAbilityMenuBG::SetPriorityTask::Execute() {
    mBG->SetPriority(mPriority);
    Exit();
}

void AttackMenuBarMotion::SetPriorityTask::Execute() {
    mBar->SetPriority(mPriority);
    Exit();
}

void AttackMenuBarMotion::ShowTask::Execute() {
    mBar->Show();
    Exit();
}

#pragma auto_inline on

void MenuEnableTask::Execute() {
    if (mEnable) {
        mMenu->Activate();
    } else {
        mMenu->Deactivate();
    }
    Exit();
}
