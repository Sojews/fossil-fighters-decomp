// DinoCleaningLive (the cleaning screen's live commentary).

// The base destructor is inline in the game.
#pragma auto_inline on

#include "game/dino_cleaning_live_task.h"
#include "game/dino_cleaning_talk_window.h"
#include "game/dino_cleaning_tutorial_window.h"
#include "game/dino_static_init.h"

extern "C" void func_ov002_021573e8(DinoCleaningTalkWindow *window);
extern "C" void func_ov002_0215785c(DinoCleaningTalkWindow *window);

#pragma auto_inline off

DinoCleaningLive::DinoCleaningLive(u32 arg, const std::string &name) : DinoCommonLive(name, 0, 0) {
    mUnkD4 = arg;
    mTalk = new DinoCleaningTalkWindow(mUnkD4);
    mParallel.AddTail((MTask *)mTalk);
}

void DinoCleaningLive::ShowTutorial(MDisplay *display, const char *text) {
    mTalk->Reset();
    mSerial.AddTail(new DinoCleaningTutorialWindow(display, mUnkD4, std::string(text)));
}

void DinoCleaningLive::Open(const char *text) {
    func_ov002_021573e8(mTalk);
}

void DinoCleaningLive::Close() {
    func_ov002_0215785c(mTalk);
}

#pragma auto_inline on

DinoCleaningLive::~DinoCleaningLive() {
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov002_021644b4[];
#pragma always_inline on
static struct DinoCleaningLiveTaskStaticInit {
    DinoCleaningLiveTaskStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_021644b4, 0x14, 0x5c, 0x1c, 0x4, 0x6c, 0x7c, 0x84, 0x8c);
    }
} sDinoCleaningLiveTaskStaticInit;
