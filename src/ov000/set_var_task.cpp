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
#include <string.h>
#include "m/text_codec.h"
#include "m/seed_generator.h"
#include "game/dino_static_init.h"

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

// Reads a decimal number from the console (pushing back the character after it).
static inline int DinoBattleReadNumber(MConsole *console) {
    int n = 0;
    while (!console->IsEmpty()) {
        u32 c = console->PopChar();
        if (c >= '0' && c <= '9') {
            n *= 10;
            n += c - '0';
        } else {
            console->PushChar(c);
            break;
        }
    }
    return n;
}


// Which creature id a "$d"/"$o" number names: {number, creature slot}.
struct DinoBattleLiveCreatureTag {
    int number;
    int slot;
};
extern "C" const DinoBattleLiveCreatureTag data_ov000_02190460[4];
extern "C" const char data_ov000_02197228[];   // a rate's characters
extern "C" double atof(const char *s);

// Reads a typing rate (a decimal fraction) from the console.
static inline MFx32 DinoBattleReadRate(MConsole *console) {
    std::string text;
    while (!console->IsEmpty()) {
        u32 ch = console->PopChar();
        if (ch > 0x100 || strchr(data_ov000_02197228, ch) == NULL) {
            console->PushChar(ch);
            break;
        }
        text.push_back((char)ch);
    }
    double value = atof(text.c_str());
    return MFx32::FromRaw((fx32)((float)value > 0 ? (float)value * 4096.0f + 0.5f : (float)value * 4096.0f - 0.5f));
}

// $x closes, $d/$o names a creature (or the player) of the side talking or the other, $l/$r a face, $e/$f clears,
// $t waits, $w sets the rate, $n the font, $c the shadow color and $p a player's name.
// NONMATCHING: for $d/$o the game loads the field's side before testing the command (ours after).
BOOL DinoBattleLiveConsoleFilter::Filter(MConsole *console, int c) {
    if (c != '$') {
        return FALSE;
    }
    if (console->IsEmpty()) {
        return TRUE;
    }
    switch ((u32)(c = console->PopChar())) {
    case 'x':
        mWindow->SetUnk84(DinoBattleReadNumber(console) + 20);
        func_ov000_0218c960(mWindow);
        break;
    case 'd':
    case 'o': {
        int side = ((DinoBattleView *)mView)->mUnkE0->unkA8 ^ (c == 'd');
        DinoBattleViewPlayer *player = ((DinoBattleView *)mView)->GetUnkE4(side);
        int number = DinoBattleReadNumber(console);
        std::string name;
        if (number == 0) {
            name = player->GetUnkC0();
        } else {
            const DinoBattleLiveCreatureTag *tag = data_ov000_02190460;
            for (u32 i = 0; i < 4; i++) {
                u32 id;
                if (number == tag[i].number && (id = mCreatures[side][tag[i].slot]) != 0) {
                    name = gDinoResource->GetCreatureText08(id);
                    break;
                }
            }
        }
        if (name.size() != 0) {
            std::wstring text = MDecodeText(name, 0);
            for (int i = (int)text.size() - 1; i >= 0; i--) {
                console->PushChar(text.c_str()[i]);
            }
        }
        break;
    }
    case 'l':
    case 'r': {
        BOOL side = c == 'r';
        u32 face;
        u32 kind = console->PopChar();
        switch (kind) {
        case 'a':
            face = gDinoResource->GetRandom()->Next(5) + 1;
            break;
        case 'c':
            face = gDinoResource->GetRandom()->Next(5) + 6;
            break;
        default:
            console->PushChar(kind);
            face = DinoBattleReadNumber(console);
            break;
        }
        func_ov000_0218c150(mWindow, face, side);
        break;
    }
    case 'e':
    case 'f':
        func_ov000_0218cb80(mWindow);
        break;
    case 't':
        mWindow->mTimer = DinoBattleReadNumber(console);
        console->PushChar(12);
        break;
    case 'w':
        func_ov000_0218cb5c(mWindow, DinoBattleReadRate(console));
        break;
    case 'n':
        if (mFont != NULL) {
            mFont->Select(DinoBattleReadNumber(console));
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
    case 'p': {
        std::string name;
        if (DinoBattleReadNumber(console) != 0) {
            name = ((DinoBattleView *)mView)->GetPlayerByAC(1)->GetUnkC0();
        } else {
            name = ((DinoBattleView *)mView)->GetPlayerByAC(0)->GetUnkC0();
        }
        if (name.size() != 0) {
            std::wstring text = MDecodeText(name, 0);
            for (int i = (int)text.size() - 1; i >= 0; i--) {
                console->PushChar(text.c_str()[i]);
            }
        }
        break;
    }
    }
    return TRUE;
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

// The file's static initialization (see dino_static_init.h).
// NONMATCHING: 89.7%, same instructions but other registers (the game's are those of real static MPoint objects;
// the last read-backs are plain loads, kept in registers).
extern "C" u32 data_ov000_021971e0[];
extern "C" u32 data_ov000_021971f0[];
extern "C" u32 data_ov000_02197200[];
static struct SetVarTaskStaticInit {
    SetVarTaskStaticInit() {
        DINO_POOL_WORD(data_ov000_02197138, 0x1c) = 128;
        DINO_POOL_WORD(data_ov000_02197138, 0x20) = 176;
        DINO_POOL_WORD(data_ov000_02197138, 0x4c) = 8;
        DINO_POOL_WORD(data_ov000_02197138, 0x50) = 150;
        DINO_POOL_WORD(data_ov000_02197138, 0x2c) = 64;
        DINO_POOL_WORD(data_ov000_02197138, 0x30) = 112;
        DINO_POOL_WORD(data_ov000_02197138, 0x3c) = 192;
        DINO_POOL_WORD(data_ov000_02197138, 0x40) = 28;
        DINO_POOL_WORD(data_ov000_02197138, 0x24) = 8;
        DINO_POOL_WORD(data_ov000_02197138, 0x28) = 158;
        DINO_POOL_WORD(data_ov000_02197138, 0xc) = -32;
        DINO_POOL_WORD(data_ov000_02197138, 0x10) = 160;
        DINO_POOL_WORD(data_ov000_02197138, 0x54) = 32;
        DINO_POOL_WORD(data_ov000_02197138, 0x58) = 160;
        DINO_POOL_WORD(data_ov000_02197138, 0x14) = 288;
        DINO_POOL_WORD(data_ov000_02197138, 0x18) = 160;
        DINO_POOL_WORD(data_ov000_02197138, 0x44) = 224;
        DINO_POOL_WORD(data_ov000_02197138, 0x48) = 160;
        DINO_POOL_WORD(data_ov000_02197138, 0xa0) = 32;
        DINO_POOL_WORD(data_ov000_02197138, 0xa4) = 160;
        DINO_POOL_WORD(data_ov000_021971e0, 0x0) = 224;
        DINO_POOL_WORD(data_ov000_021971e0, 0x4) = 160;
        DINO_POOL_WORD(data_ov000_02197138, 0xb0) = -32;
        DINO_POOL_WORD(data_ov000_02197138, 0xb4) = 160;
        DINO_POOL_WORD(data_ov000_021971f0, 0x0) = 288;
        DINO_POOL_WORD(data_ov000_021971f0, 0x4) = 160;
        DINO_POOL_WORD(data_ov000_02197138, 0xc0) = 0;
        DINO_POOL_WORD(data_ov000_02197138, 0xc4) = 0;
        DINO_POOL_WORD(data_ov000_02197200, 0x0) = -64;
        DINO_POOL_WORD(data_ov000_02197200, 0x4) = 0;
        const s32 *pos = (const s32 *)&data_ov000_02197138;
        s32 x = pos[0x24 / 4];
        DINO_POOL_WORD(data_ov000_02197138, 0xd0) = x + 0x30;
        s32 y = pos[0x28 / 4];
        DINO_POOL_WORD(data_ov000_02197138, 0xd4) = y;
        DINO_POOL_WORD(data_ov000_02197138, 0xd8) = x;
        DINO_POOL_WORD(data_ov000_02197138, 0xdc) = y;
    }
} sSetVarTaskStaticInit;
