// SetVarTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/set_var_task.h"
#include "types.h"
#include "game/dino_battle2.h"
#include "m/console.h"

SetVarTask::~SetVarTask() {
}

DinoBattleLiveConsoleFilter::~DinoBattleLiveConsoleFilter() {
}

void SetVarTask::Execute() {
    *mDst = mValue;
    Exit();
}

#pragma auto_inline off

void DinoBattleTalkWindow::SetUnk84(u32 value) {
    mUnk84 = value;
}

// Whether the window is done talking (counting down its extra waits first).
extern "C" BOOL func_ov000_0218c354(DinoBattleTalkWindow *window) {
    if (window->mTasksC4->IsEmpty()) {
        if (window->mUnk84 != 0) {
            window->mUnk84--;
            return FALSE;
        }
        return TRUE;
    }
    BOOL done = window->mConsole->IsEmpty();
    if (done && window->mUnk84 != 0) {
        window->mUnk84--;
        done = FALSE;
    }
    return done;
}

extern "C" void func_ov000_0218cb5c(DinoBattleTalkWindow *window, MFx32 rate) {
    window->mRate = rate;
    window->mConsole->SetRate(window->mRate);
}

extern "C" void func_ov000_0218cb80(DinoBattleTalkWindow *window) {
    if (!window->mTasksC4->IsEmpty()) {
        window->mConsole->Clear();
    }
}

// Starts the console once the timer runs out, when the window has tasks and the others are done.
void DinoBattleTalkWindow::Idle() {
    if (mTimer != 0) {
        mTimer--;
        if (mTimer == 0 && !mTasksC4->IsEmpty() && mTasksC0->IsEmpty()) {
            mConsole->Run();
        }
    }
}
