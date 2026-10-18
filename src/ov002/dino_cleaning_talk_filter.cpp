// DinoCleaningTalkFilter.
// Only the destructors are decompiled so far.

#include "game/dino_cleaning_talk_filter.h"
#include "game/dino_cleaning_talk_window.h"
#include "m/blend.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/console.h"

DinoCleaningTalkFilter::~DinoCleaningTalkFilter() {
}

DinoCleaningTalkWindow::~DinoCleaningTalkWindow() {
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
