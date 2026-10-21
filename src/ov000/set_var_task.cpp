// SetVarTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/set_var_task.h"
#include "types.h"
#include "game/dino_battle2.h"
#include "m/console.h"
#include "m/motion.h"
#include "m/blend.h"
#include "m/lcd.h"
#include "m/picture.h"
#include "m/canvas.h"
#include "game/dino_resource.h"
#include "game/dino_common_font.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view_creature.h"
#include <string>

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

extern "C" MPoint data_ov000_021971d8[2];   // the face, in place
extern "C" MPoint data_ov000_021971e8[2];   // the face, gone

// The three tasks at once (the arguments are evaluated last to first).
static inline MTask *Together(MTask *a, MTask *b, MTask *c) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(a, b, c);
    return set;
}

// Closes the window: slides the face, the console and the frame away and clears the screen.
extern "C" void func_ov000_0218c960(DinoBattleTalkWindow *window) {
    window->mFace = 0;
    if (!window->mTasksC4->IsEmpty()) {
        if (window->mState == 2) {
            return;
        }
        window->mState = 2;
        window->mTasksC0->Clear();
        window->mConsole->Suspend();
        window->mTasksC0->AddTail(
            Together(window->mFrame->CreateCoordAnime(data_ov000_021971f8[window->mSide],
                                                      data_ov000_021971f8[window->mSide].Offset(0, 0x28), 20, 2),
                     window->mConsole->CreateCoordAnime(data_ov000_02197208[window->mSide],
                                                        data_ov000_02197208[window->mSide].Offset(0, 0x28), 20, 2),
                     window->mFaceMotion->CreateCoordAnime(data_ov000_021971d8[window->mSide],
                                                           data_ov000_021971e8[window->mSide], 20, 2)),
            MTaskSetMakeEmptyTask::Create(window->mTasksC4), gLcd->GetTopDisplay()->mBlend->CreateClearTransfer(),
            new SetVarTask(&window->mState, 0));
    }
}

inline void DinoBattleLiveConsoleFilter::Refresh() {
    for (u32 side = 0; side < 2; side++) {
        DinoBattleViewPlayer *player = ((DinoBattleView *)mView)->GetUnkE4(side);
        for (u32 i = 0; i < 4; i++) {
            DinoBattleViewCreature *creature = player->FindCreatureByB4(i);
            mCreatures[side][i] = creature != NULL ? creature->GetUnkA8() : 0;
        }
    }
}

extern "C" MPoint data_ov000_02197174;   // the console's size
// The TU's pooled face motion path (word 1).
extern "C" const char *const data_ov000_02197138[];

// Says text: opens the window (the frame, the console and the face, sliding up into place) or, when it's open,
// puts it back in place and replaces the text. "$l"/"$r" at the start picks the side.
extern "C" void func_ov000_0218c3d4(DinoBattleTalkWindow *window, const char *text) {
    window->mRate = MFx32::FromRaw(((const s32 *)gDinoResource->GetExcavate7C())[6]);
    window->mUnk84 = 0;
    if (!window->mTasksC4->IsEmpty()) {
        window->mTasksC0->Clear();
        window->mFrame->SetPosition(data_ov000_021971f8[window->mSide]);
        window->mFaceMotion->SetPosition(data_ov000_021971d8[window->mSide]);
        window->mConsole->SetRate(window->mRate);
        window->mConsole->SetPosition(data_ov000_02197208[window->mSide]);
        window->mConsole->Clear();
        window->mConsole->ClearText();
        window->mFilter->Refresh();
        window->mConsole->Print(std::string(text));
        MBlend *blend = gLcd->GetTopDisplay()->mBlend;
        blend->SetMode(1);
        blend->SetTarget1(window->mFrame->GetPlaneMask());
        blend->SetTarget2(0x3F);
        blend->SetAlpha(12, 4);
        return;
    }
    DinoCommonFont *font = new DinoCommonFont(0, 10, 11);
    window->mConsole = new MConsole(new MTextCanvas(gLcd->GetTopDisplay(), data_ov000_02197174,
                                                    gDinoResource->GetUnkTable(), 0, 1),
                                    font, MPoint(0, 4), MFx32(2));
    window->mConsole->SetRate(window->mRate);
    window->mFrame = new MTextPicture(gLcd->GetTopDisplay(), window->mPicture, TRUE, false);
    window->mFrame->Show();
    window->mFilter = new DinoBattleLiveConsoleFilter(font, window, window->mUnkD8);
    window->mConsole->SetFilter(window->mFilter);
    window->mConsole->Show();
    window->mFilter->Refresh();
    window->mConsole->Print(std::string(text));
    window->mConsole->Suspend();
    window->mFaceMotion = new MMotion(std::string(data_ov000_02197138[1]), window->mOam, 3, 1);
    window->mFaceMotion->Show();
    if (window->mFace != 0) {
        func_ov000_0218c150(window, window->mFace, window->mSide);
    }
    MBlend *blend = gLcd->GetTopDisplay()->mBlend;
    blend->SetMode(1);
    blend->SetTarget1(window->mFrame->GetPlaneMask());
    blend->SetTarget2(0x3F);
    blend->SetAlpha(12, 4);
    if (text[0] == '$') {
        switch (text[1]) {
        case 'l':
            window->mSide = 0;
            break;
        case 'r':
            window->mSide = 1;
            break;
        }
    }
    window->mTasksC4->AddTail(window->mFrame, window->mConsole, window->mFaceMotion);
    window->mTasksC0->AddTail(
        Together(window->mFrame->CreateCoordAnime(data_ov000_021971f8[window->mSide].Offset(0, 0x28),
                                                  data_ov000_021971f8[window->mSide], 10, 12),
                 window->mConsole->CreateCoordAnime(data_ov000_02197208[window->mSide].Offset(0, 0x28),
                                                    data_ov000_02197208[window->mSide], 10, 12),
                 window->mFaceMotion->CreateCoordAnime(data_ov000_021971e8[window->mSide],
                                                       data_ov000_021971d8[window->mSide], 10, 12)),
        MTaskRunTransfer::Create(window->mConsole), new SetVarTask(&window->mState, 0));
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
