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
// NONMATCHING: the offset's -y goes through a register copy (opt_common_subs off fixes the rest).
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
            int range = mUnkCC;
            MPoint d(gDinoResource->GetRandom()->Next(range * 2 + 1) - mUnkCC,
                     gDinoResource->GetRandom()->Next(range * 2 + 1) - range);
            mConsole->SetPosition(data_ov002_0216431c.mPos + d);
            mFace->SetPosition(d);
            mOam->SetOffset(MPoint(-d.x, -d.y));
        }
    }
}
#pragma opt_common_subs reset

// NONMATCHING: 87%, the points' temporaries take other stack slots.
void DinoCleaningTalkWindow::Close() {
    mUnkCC = 0;
    mUnkD0 = 0;
    mConsole->Run();
    MTask *clear = gLcd->GetTopDisplay()->mBlend->CreateClearTransfer();
    MTask *empty = MTaskSetMakeEmptyTask::Create(mTasksB8);
    MPoint frameFrom(256, 192);
    MTask *frame = mFrame->CreateCoordAnime(frameFrom, MPoint(frameFrom.x + 64, frameFrom.y), 20, 2);
    MTask *window = mConsole->CreateCoordAnime(data_ov002_0216431c.mPos,
                                               MPoint(data_ov002_0216431c.mPos.x, data_ov002_0216431c.mPos.y + 40), 20, 2);
    MTask *face = mFace->CreateCoordAnime(MPoint(0, 0), MPoint(0, 40), 20, 2);
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(face);
    set->AddTail(window);
    set->AddTail(frame);
    MSerialTaskSet *serial = mTasksB4;
    serial->AddTail(set);
    serial->AddTail(empty);
    serial->AddTail(clear);
}

#pragma auto_inline off

void DinoCleaningTalkWindow::SetUnkD0(u32 value) {
    mUnkD0 = value;
}

void DinoCleaningTalkWindow::SetUnkCC(u32 value) {
    mUnkCC = value;
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
