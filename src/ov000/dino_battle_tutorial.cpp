// DinoBattleTutorial.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "m/resource.h"
#include "game/dino_battle_tutorial.h"
#include "game/dino_message.h"
#include "game/dino_battle_view_class.h"
#include "types.h"
#include "game/dino_static_init.h"

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

#pragma auto_inline off

extern "C" void func_ov000_0218e65c(DinoBattleViewTutorialWindow *window);

// Opens the window with the text.
extern "C" void func_ov000_0218e600(DinoBattleViewTutorialWindow *window, const char *text) {
    func_ov000_0218e65c(window);
    window->mConsole->Clear();
    window->mConsole->ClearText();
    window->mConsole->SetSpeed(MFx32(2));
    window->mConsole->Print(std::string(text));
    window->mConsole->Run();
}

extern "C" void func_ov000_0218e65c(DinoBattleViewTutorialWindow *window) {
    DinoSoundPlaySe(0xA8, 0);
    window->mConsole->Show();
    window->mFrame->Show();
}

// Closes it.
extern "C" void func_ov000_0218e680(DinoBattleViewTutorialWindow *window) {
    DinoSoundPlaySe(0xA9, 0);
    window->mConsole->Clear();
    window->mConsole->Hide();
    window->mFrame->Hide();
}

// Waits for a button or a touch (speeding the text up first).
extern "C" MTask *func_ov000_0218e6b0(DinoBattleViewTutorialWindow *window) {
    return new KeyWaitTask(window);
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_02197560[];
static struct DinoBattleTutorialStaticInit {
    DinoBattleTutorialStaticInit() {
        DINO_POOL_WORD(data_ov000_02197560, 0x34) = 216;
        DINO_POOL_WORD(data_ov000_02197560, 0x38) = 48;
        DINO_POOL_WORD(data_ov000_02197560, 0x24) = 128;
        DINO_POOL_WORD(data_ov000_02197560, 0x28) = 176;
        DINO_POOL_WORD(data_ov000_02197560, 0x14) = 8;
        DINO_POOL_WORD(data_ov000_02197560, 0x18) = 150;
        DINO_POOL_WORD(data_ov000_02197560, 0xc) = 64;
        DINO_POOL_WORD(data_ov000_02197560, 0x10) = 112;
    }
} sDinoBattleTutorialStaticInit;
