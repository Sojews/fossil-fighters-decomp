// DinoBattleTutorial.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "m/resource.h"
#include "game/dino_battle_tutorial.h"
#include "game/dino_message.h"
#include "game/dino_battle_view_class.h"
#include "types.h"

DinoBattleViewTutorialWindow::~DinoBattleViewTutorialWindow() {
}

DinoBattleTutorialConsoleFilter::~DinoBattleTutorialConsoleFilter() {
}

KeyWaitTask::~KeyWaitTask() {
}

DinoBattleTutorial::~DinoBattleTutorial() {
    delete mFilter;
}

void KeyWaitTask::Idle() {
    if (mHub->GetInput(0)->GetPressed() & 0x1003) {
        if (mWindow->mConsole->IsEmpty()) {
            DinoSoundPlaySe(0x93, 0);
            Exit();
            return;
        }
        DinoSoundPlaySe(0xAF, 0);
        mWindow->mConsole->SetSpeed(MFx32(0));
    }
}

void DinoBattleViewTutorialWindow::Idle() {
    if (!mConsole->IsHidden() && mConsole->mState == MTask::STATE_SUSPEND) {
        mCursor->Show();
    } else {
        mCursor->Hide();
    }
}

// Resources the file loads (their data types are unknown); until the code that constructs them is decompiled,
// these keep out-of-line copies of the constructors.
struct DinoBattleTutorialData;
#pragma dont_inline on
MResource<DinoBattleTutorialData> DinoBattleTutorialDataLoad(std::string path) {
    return MResource<DinoBattleTutorialData>(path);
}
#pragma dont_inline reset

#pragma auto_inline off

// Instances the game emits here (for the tutorial's requests to the battle view); until the code that posts them
// is decompiled they are made explicitly.
template MTask *DinoCreatePostAndWaitTask(DinoBattleView *target, DinoBattleViewTutorialSetRectRequest *msg);
template MTask *DinoCreatePostAndWaitTask(DinoBattleView *target, DinoBattleViewTutorialSetArroeRequest *msg);
template MTask *DinoCreatePostAndWaitTask(DinoBattleView *target, DinoBattleViewTutorialWindowRequest *msg);
