// DinoCleaningTalkFilter.
// Only the destructors are decompiled so far.

#pragma ipa file

#include "game/dino_cleaning_talk_filter.h"
#include "game/dino_cleaning_talk_window.h"
#include "m/blend.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/console.h"
#include "m/seed_generator.h"
#include "m/smart_oam.h"
#include "game/dino_resource.h"
#include "game/dino_static_init.h"
#include "game/dino_backup.h"
#include "game/dino_common_font.h"
#include "game/dino_cleaning.h"
#include "m/motion.h"
#include "m/picture.h"
#include "m/oam_canvas.h"
#include "m/task.h"
#include <string>
#include <string.h>
#include <stdlib.h>

DinoCleaningTalkFilter::~DinoCleaningTalkFilter() {
}

DinoCleaningTalkWindow::~DinoCleaningTalkWindow() {
}

// Reads a decimal number from the console (pushing back the character after it).
static inline int DinoCleaningReadNumber(MConsole *console) {
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

extern "C" const char data_ov002_02164404[];   // "-+0123456789."
extern "C" double atof(const char *s);

BOOL DinoCleaningTalkFilter::Filter(MConsole *console, int c) {
    if (c != '$') {
        return FALSE;
    }
    if (console->IsEmpty()) {
        return TRUE;
    }
    switch ((u32)console->PopChar()) {
    case 'p': {
        DinoPlayerData player;
        gDinoBackup->GetPlayer(&player, FALSE);
        std::wstring name((const wchar_t *)player.name);
        for (std::wstring::const_reverse_iterator it = name.rbegin(); it != name.rend(); ++it) {
            console->PushChar(*it);
        }
        break;
    }
    case 't':
        mWindow->SetUnkD0(DinoCleaningReadNumber(console));
        console->PushChar(12);
        break;
    case 'f':
        console->Clear();
        break;
    case 'w': {
        MFx32 rate;
        {
            std::string text;
            while (!console->IsEmpty()) {
                u32 ch = console->PopChar();
                if (ch > 0x100 || strchr(data_ov002_02164404, ch) == NULL) {
                    console->PushChar(ch);
                    break;
                }
                text.push_back((char)ch);
            }
            double value = atof(text.c_str());
            rate = MFx32::FromRaw((fx32)((float)value > 0 ? (float)value * 4096.0f + 0.5f
                                                            : (float)value * 4096.0f - 0.5f));
        }
        mWindow->SetRate(rate);
        break;
    }
    case 'n':
        if (mFont != NULL) {
            mFont->Select(DinoCleaningReadNumber(console));
        }
        break;
    case 'q':
        mWindow->SetUnkCC(DinoCleaningReadNumber(console));
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
            color++;
            if (color == 1) {
                color = 10;
            }
            if (mFont != NULL) {
                mFont->SetShadowColor(color);
            }
        }
        break;
    }
    return TRUE;
}

// The pooled constants the code reads.
extern "C" struct {
    u8 unk00[0x8];
    const char *mMotionPath;    // 0x08
    const char *mPicturePath;   // 0x0C
    u8 unk10[0x20 - 0x10];
    MPoint mPos;                // 0x20: the window's place
} data_ov002_0216431c;

DinoCleaningTalkWindow::DinoCleaningTalkWindow(u32 arg)
    : MProcess(NULL), mUnk74(arg), mPicture(std::string(data_ov002_0216431c.mPicturePath), 7),
      mMotion(std::string(data_ov002_0216431c.mMotionPath), 7) {
    mUnkCC = 0;
    mUnkD0 = 0;
    mRate.v = ((const u32 *)gDinoResource->GetExcavate7C())[5];
    mTasksB4 = new MSerialTaskSet(FALSE);
    mTasksB8 = new MParallelTaskSet(FALSE);
    MParallelTaskSet *tasks = mTasksB8;
    MParallelTaskSet *parallel = GetParallel();
    parallel->AddTail(mTasksB4);
    parallel->AddTail(tasks);
}

// Resumes the text once the wait is over (while B8 runs and B4 doesn't); shakes the window while a shake is left.
// (Without common subexpression elimination.)
#pragma opt_common_subs off
void DinoCleaningTalkWindow::Idle() {
    if (mUnkD0 != 0) {
        mUnkD0--;
        if (mUnkD0 == 0 && !mTasksB8->IsEmpty() && mTasksB4->IsEmpty()) {
            mConsole->Run();
        }
    }
    if (mUnkCC != 0) {
        mUnkCC--;
        if (!mTasksB8->IsEmpty() && mTasksB4->IsEmpty()) {
            int dy;
            int range = mUnkCC;
            dy = gDinoResource->GetRandom()->Next(range * 2 + 1) - range;
            int dx = gDinoResource->GetRandom()->Next(range * 2 + 1) - mUnkCC;
            MPoint d(dx, dy);
            mConsole->SetPosition(data_ov002_0216431c.mPos + d);
            mFace->SetPosition(d);
            mOam->SetOffset(MPoint(-d.x, -dy));
        }
    }
}
#pragma opt_common_subs reset


static inline MTask *DinoTalkParallel(MTask *a, MTask *b, MTask *c) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(a);
    set->AddTail(b);
    set->AddTail(c);
    return set;
}

// One expression, so the points' temporaries get the game's stack slots (in source order).
// NONMATCHING: the clear and window tasks' spill slots are swapped, and 320 is made differently.
void DinoCleaningTalkWindow::Close() {
    mUnkCC = 0;
    mUnkD0 = 0;
    mConsole->Run();
    MTask *clear = gLcd->GetTopDisplay()->mBlend->CreateClearTransfer();
    MTask *empty = MTaskSetMakeEmptyTask::Create(mTasksB8);
    mTasksB4->AddTail(
        DinoTalkParallel(mFace->CreateCoordAnime(MPoint(0, 0), MPoint(0, 40), 20, 2),
                         mConsole->CreateCoordAnime(data_ov002_0216431c.mPos,
                                                    MPoint(data_ov002_0216431c.mPos.x, data_ov002_0216431c.mPos.y + 40),
                                                    20, 2),
                         mFrame->CreateCoordAnime(MPoint(256, 192), MPoint(320, 192), 20, 2)),
        empty, clear);
}

#pragma auto_inline off

void DinoCleaningTalkWindow::SetUnkD0(u32 value) {
    mUnkD0 = value;
}

void DinoCleaningTalkWindow::SetUnkCC(u32 value) {
    mUnkCC = value;
}

// Says text: opens the window (the face, the frame with a random motion and the console, sliding in) or, when it's
// open, puts it back in place and replaces the text.
// NONMATCHING: two task spills take each other's stack slots, and 256 and 320 are made the other way round.
extern "C" void func_ov002_021573e8(DinoCleaningTalkWindow *window, const char *text) {
    window->mRate = MFx32::FromRaw(((const s32 *)gDinoResource->GetExcavate7C())[5]);
    if (!window->mTasksB8->IsEmpty()) {
        u32 motion;
        do {
            motion = ((DinoCleaning *)window->mUnk74)->GetRandom()->Next(window->mMotion.GetUnk18() - 1) + 1;
        } while (motion == window->unkB0);
        window->mTasksB4->Clear();
        window->mFace->SetPosition(0, 0);
        ((MMotion *)window->mFrame)->SetPosition(256, 192);
        window->mConsole->SetRate(window->mRate);
        window->mConsole->SetPosition(data_ov002_0216431c.mPos);
        window->mConsole->Clear();
        window->mConsole->ClearText();
        window->mConsole->Print(std::string(text));
        return;
    }
    window->mOam = new MSmartOam(gLcd->GetTopDisplay(), 0x14, 0, 1);
    window->mFace = new MTextPicture(gLcd->GetTopDisplay(), window->mPicture, TRUE, false);
    window->mFace->Show();
    DinoCommonFont *font = new DinoCommonFont(0, 10, 11);
    window->mConsole = new MConsole(new MSmartObjCanvas(window->mOam, DINO_POOL(MPoint, data_ov002_0216431c, 0x30),
                                                        gDinoResource->GetUnkTable(), TRUE),
                                    font, MPoint(0, 4), MFx32(2));
    window->mConsole->SetRate(window->mRate);
    window->mConsole->SetFilter(new DinoCleaningTalkFilter(window, font));
    window->mConsole->Show();
    window->mConsole->Print(std::string(text));
    window->mConsole->Suspend();
    window->unkB0 = ((DinoCleaning *)window->mUnk74)->GetRandom()->Next(window->mMotion.GetUnk18() - 1) + 1;
    window->mFrame = new MMotion(MMotionFile(window->mMotion), window->mOam, 0, 1);
    window->mFrame->Show();
    ((MMotion *)window->mFrame)->Play(window->unkB0, 0);
    MBlend *blend = gLcd->GetTopDisplay()->mBlend;
    blend->SetMode(1);
    blend->SetTarget1(window->mFace->GetPlaneMask());
    blend->SetTarget2(0x3F);
    blend->SetAlpha(12, 4);
    window->mTasksB8->AddTail(window->mFace, window->mConsole, window->mFrame, window->mOam);
    MTask *run = MTaskRunTransfer::Create(window->mConsole);
    window->mTasksB4->AddTail(
        DinoTalkParallel(window->mFace->CreateCoordAnime(MPoint(0, 40), MPoint(0, 0), 10, 12),
                         window->mConsole->CreateCoordAnime(MPoint(data_ov002_0216431c.mPos.x,
                                                                   data_ov002_0216431c.mPos.y + 40),
                                                            data_ov002_0216431c.mPos, 10, 12),
                         window->mFrame->CreateCoordAnime(MPoint(320, 192), MPoint(256, 192), 10, 12)),
        run);
}

void DinoCleaningTalkWindow::SetRate(MFx32 rate) {
    mRate = rate;
    mConsole->SetRate(rate);
}

void DinoCleaningTalkWindow::Reset() {
    mUnkCC = 0;
    mUnkD0 = 0;
    mTasksB4->Clear();
    mTasksB8->Clear();
    gLcd->GetTopDisplay()->mBlend->Reset();
}

// The file's static initialization (see dino_static_init.h).
// NONMATCHING: scheduling, as in dino_cleaning_stop_confirm_menu_item.cpp (the screen rect lies past the 0x80 anchor).
#pragma always_inline on
static struct DinoCleaningTalkFilterStaticInit {
    DinoCleaningTalkFilterStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_0216431c, 0x28, 0xb8, 0x18, 0x10, 0xa8, 0xc8, 0xd0, 0xd8);
        DINO_POOL(MPoint, data_ov002_0216431c, 0x30) = MPoint(216, 28);
        DINO_POOL(MPoint, data_ov002_0216431c, 0x20) = MPoint(8, 158);
    }
} sDinoCleaningTalkFilterStaticInit;
