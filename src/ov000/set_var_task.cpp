// SetVarTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/set_var_task.h"
#include "types.h"
#include "game/dino_battle2.h"
#include "m/console.h"
#include "m/motion.h"

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

// Where the face and the window go for each side (set up at startup).
extern "C" MPoint data_ov000_02197144, data_ov000_0219714c;     // the face, coming in
extern "C" MPoint data_ov000_0219717c, data_ov000_0219718c;     // the face, in place
extern "C" MPoint data_ov000_021971f8[2];   // the frame
extern "C" MPoint data_ov000_02197208[2];   // the console

// Shows the face (moving everything over when the other side talks now). -1 keeps the face.
extern "C" void func_ov000_0218c150(DinoBattleTalkWindow *window, u32 face, int side) {
    if (!window->mTasksC4->IsEmpty()) {
        MParallelTaskSet *set = new MParallelTaskSet(TRUE);
        window->mFaceMotion->Play(face, 0);
        MPoint from;
        MPoint to;
        int old = window->mSide;
        if (old == side) {
            from = window->mFaceMotion->GetPosition();
            to.x = (side == 0 ? data_ov000_0219718c : data_ov000_0219717c).x;
            to.y = (side == 0 ? data_ov000_0219718c : data_ov000_0219717c).y;
        } else {
            from.x = (side == 0 ? data_ov000_02197144 : data_ov000_0219714c).x;
            from.y = (side == 0 ? data_ov000_02197144 : data_ov000_0219714c).y;
            to.x = (side == 0 ? data_ov000_0219718c : data_ov000_0219717c).x;
            to.y = (side == 0 ? data_ov000_0219718c : data_ov000_0219717c).y;
            set->AddTail(window->mFrame->CreateCoordAnime(data_ov000_021971f8[window->mSide], data_ov000_021971f8[side],
                                                          10, 12),
                         window->mConsole->CreateCoordAnime(data_ov000_02197208[old], data_ov000_02197208[side], 10,
                                                            12));
        }
        window->mFaceMotion->SetPosition(from);
        set->AddTail(window->mFaceMotion->CreateCoordAnime(from, to, 10, 12));
        window->mTasksC0->AddTail(set);
    }
    if (face != (u32)-1) {
        window->mFace = face;
    }
    window->mSide = side;
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
