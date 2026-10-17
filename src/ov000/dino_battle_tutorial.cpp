// DinoBattleTutorial.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/dino_battle_tutorial.h"
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
