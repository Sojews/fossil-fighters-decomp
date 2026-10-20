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
#include "m/console.h"
#include "game/dino_common_font.h"
#include "m/fx32.h"

DinoBattleViewTutorialWindow::~DinoBattleViewTutorialWindow() {
}

DinoBattleTutorialConsoleFilter::~DinoBattleTutorialConsoleFilter() {
}

// Reads a decimal number from the console (pushing back the character after it).
static inline int DinoReadNumber(MConsole *console) {
    int n = 0;
    u32 c;
    for (;;) {
        c = console->PopChar();
        if (c < '0' || c > '9') {
            break;
        }
        n *= 10;
        n += c - '0';
    }
    console->PushChar(c);
    return n;
}

BOOL DinoBattleTutorialConsoleFilter::Filter(MConsole *console, int c) {
    if (c != '$') {
        if (console->GetSpeed() != MFx32(0)) {
            if (mUnk11) {
                DinoSoundPlaySe(0xAE, 0);
            }
        }
        if (mUnk10) {
            DinoSoundPlaySe(0xAA, 0);
        } else {
            DinoSoundPlaySe(0xAB, 0);
        }
        return FALSE;
    }
    if (console->IsEmpty()) {
        return TRUE;
    }
    switch ((u32)console->PopChar()) {
    case 'W':
        mUnk10 = FALSE;
        break;
    case 's':
        mUnk11 = TRUE;
        break;
    case 'f':
        console->Clear();
        break;
    case 'w': {
        int n = DinoReadNumber(console);
        if (n == 0) {
            n = 1;
        }
        MFx32 rate = MFx32(12) / MFx32(n);
        console->SetRate(rate);
        break;
    }
    case 'n':
        if (mFont != NULL) {
            int n = DinoReadNumber(console);
            mFont->Select(n);
        }
        break;
    case 'c':
        if (!console->IsEmpty()) {
            u32 color = console->PopChar();
            if (color >= '0' && color <= '9') {
                color -= '0';
            } else if (color >= 'a' && color <= 'f') {
                color -= 'a' - 10;
            } else if (color >= 'A' && color <= 'F') {
                color -= 'A' - 5;
            } else {
                color = 0;
            }
            if (mFont != NULL) {
                mFont->SetShadowColor(color + 1);
            }
        }
        break;
    }
    return TRUE;
}

KeyWaitTask::~KeyWaitTask() {
}

DinoBattleTutorial::~DinoBattleTutorial() {
    delete mDisabler;
}

extern "C" const char data_ov000_021977bc[];  // the tutorial data's path format

DinoBattleTutorial::DinoBattleTutorial(u32 id) : mId(id), mUnk08(0), mDisabler(NULL) {
    if (id != 0) {
        char path[32];
        OS_SPrintf(path, data_ov000_021977bc, id);
        mData = MResource<DinoBattleTutorialData>(std::string(path));
        mUnk0C = FALSE;
        mUnk0D = TRUE;
        mDisabler = new DinoHelpDisabler();
    } else {
        mUnk0C = TRUE;
        mUnk0D = FALSE;
    }
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
