// DinoTalkConsoleFilter, DinoTalkWindowWaitStartTask.
// Only the destructors are decompiled so far.

#include "game/dino_talk_console_filter.h"
#include "game/dino_talk_window_wait_start_task.h"
#include "game/dino_talk_window.h"
#include "m/console.h"
#include "game/dino_resource.h"

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
