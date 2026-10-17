// DinoCleaningLive (the cleaning screen's live commentary).

// The base destructor is inline in the game.
#pragma auto_inline on

#include "game/dino_cleaning_live_task.h"
#include "game/dino_cleaning_talk_window.h"
#include "game/dino_cleaning_tutorial_window.h"

extern "C" void func_ov002_021573e8(DinoCleaningTalkWindow *window);
extern "C" void func_ov002_0215785c(DinoCleaningTalkWindow *window);
extern "C" void func_ov002_021579ec(DinoCleaningTalkWindow *window);

#pragma auto_inline off

DinoCleaningLive::DinoCleaningLive(u32 arg, const std::string &name) : DinoCommonLive(name, 0, 0) {
    mUnkD4 = arg;
    mTalk = new DinoCleaningTalkWindow(mUnkD4);
    mParallel.AddTail((MTask *)mTalk);
}

void DinoCleaningLive::ShowTutorial(MDisplay *display, const char *text) {
    func_ov002_021579ec(mTalk);
    mSerial.AddTail(new DinoCleaningTutorialWindow(display, mUnkD4, std::string(text)));
}

void DinoCleaningLive::Unk021580cc() {
    func_ov002_021573e8(mTalk);
}

void DinoCleaningLive::Unk021580d8() {
    func_ov002_0215785c(mTalk);
}

#pragma auto_inline on

DinoCleaningLive::~DinoCleaningLive() {
}
