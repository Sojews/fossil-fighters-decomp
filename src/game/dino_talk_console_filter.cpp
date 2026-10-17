// DinoTalkConsoleFilter, DinoTalkWindowWaitStartTask.
// Only the destructors are decompiled so far.

#include "game/dino_talk_console_filter.h"
#include "game/dino_talk_window_wait_start_task.h"
#include "game/dino_talk_window.h"

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

void DinoTalkWindow::ClearUnkF4() {
    mUnkF4 = 0;
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
