// DinoTalkConsoleFilter, DinoTalkWindowWaitStartTask.
// Only the destructors are decompiled so far.

#define MSL_STRING_DEFINITIONS
#include "game/dino_talk_console_filter.h"
#include "game/dino_talk_window_wait_start_task.h"
#include "game/dino_talk_window.h"
#include "m/console.h"
#include "game/dino_resource.h"
#include "game/dino_sound.h"

// The text settings in the resource table.
struct DinoTalkSettings {
    u8 unk00[0x10];
    MFx32 mRate;    // 0x10: characters per frame
};

DinoTalkConsoleFilter::~DinoTalkConsoleFilter() {
}

DinoTalkWindowWaitStartTask::~DinoTalkWindowWaitStartTask() {
}

void DinoTalkWindow::Idle() {
    (this->*mState)();
}

DinoTalkWindow::~DinoTalkWindow() {
}

DinoTalkWindowNamePanel::~DinoTalkWindowNamePanel() {
}

void DinoTalkWindowNamePanel::Idle() {
}

void DinoTalkWindowWaitStartTask::Execute() {
    if (*mStarted) {
        Exit();
    }
}

#pragma auto_inline off

void DinoTalkWindow::SetUnkF4() {
    mUnkF4 = TRUE;
}

void DinoTalkWindow::ClearUnkF4() {
    mUnkF4 = 0;
}

void DinoTalkWindow::SetRate(MFx32 rate) {
    mRate = rate;
    mConsole->SetRate(rate);
}

void DinoTalkWindow::Print(const std::string &text) {
    mConsole->Print(std::string(text));
}

void DinoTalkWindow::ResetRate() {
    MFx32 rate = ((const DinoTalkSettings *) gDinoResource->GetExcavate7C())->mRate;
    mConsole->SetRate(rate);
}

BOOL DinoTalkWindow::IsEmpty() {
    return mConsole->IsEmpty();
}

void DinoTalkWindow::SetUnkAC(u32 value) {
    mUnkAC = value;
}

void DinoTalkWindow::SetUnkB0(u8 value) {
    mUnkB0 = value;
}

u8 DinoTalkWindow::GetUnkB0() {
    return mUnkB0;
}

// The page is all out (the console waits) and the window is still.
BOOL DinoTalkWindow::IsPageDone() {
    BOOL done = mConsole->mState == MTask::STATE_SUSPEND;
    if (done) {
        done = mAnims->IsEmpty();
    }
    return done;
}

// Shows the rest of the page at once.
void DinoTalkWindow::Skip() {
    DinoSoundPlaySe(0xAF, 0);
    mConsole->SetSpeed(MFx32(0));
    if (!mAnims->IsEmpty()) {
        mConsole->Run();
        mAnims->Clear();
    }
}

BOOL DinoTalkWindow::IsAnimating() {
    return !mAnims->IsEmpty();
}

// String members the game emits in this file (made explicitly until the filter code using them is decompiled).
template std::wstring::const_iterator std::wstring::erase(std::wstring::const_iterator p);
template void std::wstring::push_back(wchar_t c);
template void std::string::push_back(char c);
