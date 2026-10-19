// DinoCleaningTutorialWindow.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_tutorial_window.h"
#include "types.h"
#include "game/dino_cleaning.h"

#pragma auto_inline off
DinoCleaningTutorialWindow::DinoCleaningTutorialWindow(MDisplay *display, u32 cleaning, const std::string &text)
    : DinoCommonTutorialWindow(display, text), mCleaning((DinoCleaning *)cleaning) {
}

void DinoCleaningTutorialWindow::OnSetActive(BOOL active) {
    mCleaning->SetUnk128(active);
}
#pragma auto_inline on

DinoCleaningTutorialWindow::~DinoCleaningTutorialWindow() {
}
