// DinoEventTalkWindow: the window event text is shown in.
// Only the background-call template it instantiates is here so far.

#pragma ipa file

#include "m/input.h"
#include "game/dino_talk_window.h"
#include "game/dino_event_talk_window.h"
#include "m/bg_new_task.h"
#include "game/dino_window_choice.h"

DinoEventTalkWindow::~DinoEventTalkWindow() {
}

void DinoEventTalkWindow::SetYesNo(u32 arg, MPoint pos, u32 unkB4, const std::string &a, const std::string &b,
                                   const std::string &c, const std::string &d) {
    mUnk98 = arg;
    mPos = pos;
    mUnkB4 = unkB4;
    mUnkBC = a;
    mUnkC8 = b;
    mUnkD4 = c;
    mUnkE0 = d;
}

void DinoEventTalkWindow::SetChoices(u32 arg, MPoint pos, const std::vector<DinoWindowChoice> &choices, u32 unkB4) {
    mUnk9C = arg;
    mPos = pos;
    mChoices = choices;
    mUnkB4 = unkB4;
}

void DinoEventTalkWindow::AddEntry(u32 id, MRect rect, const std::string &text) {
    DinoCommonWindowMenuEntry entry;
    entry.id = id;
    entry.rect = rect;
    entry.text = text;
    mEntries.push_back(entry);
}

void DinoEventTalkWindow::Idle() {
    (this->*mState)();
}

// Emitted in this file in the game (the window starts background loads with MCreateBgCallTask); until the code
// that makes them is decompiled, the instances are made explicitly.
template class MBgCallTask0<DinoEventTalkWindow>;
template MTask *MCreateBgCallTask(DinoEventTalkWindow *obj, void (DinoEventTalkWindow::*func)(), int thread);
template class std::__vec_deleter<DinoWindowChoice, std::allocator<DinoWindowChoice> >;
template class std::__vec_deleter<std::string, std::allocator<std::string> >;
template class std::__vec_deleter<DinoCommonWindowMenuEntry, std::allocator<DinoCommonWindowMenuEntry> >;

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoEventTalkWindow::*Load_04)();
    void (DinoEventTalkWindow::*Unk02007ab4_0C)();
    void (DinoEventTalkWindow::*Unk02007648_14)();
    void (DinoEventTalkWindow::*StateStart_1C)();
    u8 unk24[0x2C - 0x24];
    void (DinoEventTalkWindow::*Unk0200770c_2C)();
    void (DinoEventTalkWindow::*Unk02007a00_34)();
} data_020b6144;

// Waits for the window to show the page, then asks, chooses or ends.
void DinoEventTalkWindow::StatePage() {
    if (mWindow->IsEmpty()) {
        if (mUnk98 != 0) {
            mState = data_020b6144.Unk0200770c_2C;
        } else if (mUnk9C != 0) {
            mState = data_020b6144.Unk02007a00_34;
        } else {
            mState = data_020b6144.Unk02007ab4_0C;
        }
    } else {
        mWindow->NextPage();
        mUnk90 = 0;
    }
}

DinoEventTalkWindow::DinoEventTalkWindow(MDisplay *display, std::string text, int arg, u8 flag)
    : MProcess(display), mText(text), mArg(arg), mUnk98(0), mUnk9C(0), mFlag(flag) {
    mState = data_020b6144.StateStart_1C;
}

#pragma dont_inline on
void DinoEventTalkWindow::StateStart() {
    mInputHub = new MSingleInputHub(1);
    mInputHub->EnableTouch();
    mParallel.AddTail(mInputHub);
    MTaskSet *parallel = &mParallel;
    parallel->mUseMutex = TRUE;
    MTaskSet *serial = &mSerial;
    serial->mUseMutex = TRUE;
    mSerial.AddTail(MCreateBgCallTask(this, data_020b6144.Load_04, 1));
    mState = data_020b6144.Unk02007648_14;
}
#pragma dont_inline reset
