// SetToturialFlagTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/set_toturial_flag_task.h"
#include "types.h"
#include "game/dino_battle_tutorial.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_message.h"

SetToturialFlagTask::~SetToturialFlagTask() {
}

void SetToturialFlagTask::Execute() {
    mTutorial->mFlags |= mFlag;
    Exit();
}

#pragma auto_inline off

// Instances the game emits here (for the tutorial's requests to the battle view); until the code that posts them
// is decompiled they are made explicitly.
template MTask *DinoCreatePostAndWaitTask(DinoBattleView *target, DinoBattleViewTutorialSetRectRequest *msg);
template MTask *DinoCreatePostAndWaitTask(DinoBattleView *target, DinoBattleViewTutorialSetArroeRequest *msg);
template MTask *DinoCreatePostAndWaitTask(DinoBattleView *target, DinoBattleViewTutorialWindowRequest *msg);

// Gives the tutorial its view, and the view its tutorial.
void DinoBattleTutorial::SetView(DinoBattleView *view) {
    mView = view;
    view->SetUnk12C((u32)this);
}

void DinoBattleTutorial::SetUnk24(u32 value) {
    mUnk24 = value;
}
