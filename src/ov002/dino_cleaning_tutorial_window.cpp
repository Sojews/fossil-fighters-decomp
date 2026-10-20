// DinoCleaningTutorialWindow.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_tutorial_window.h"
#include "types.h"
#include "game/dino_cleaning.h"
#include "game/dino_static_init.h"

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

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov002_02164258[];
#pragma always_inline on
static struct DinoCleaningTutorialWindowStaticInit {
    DinoCleaningTutorialWindowStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_02164258, 0xc, 0x78, 0x14, 0x1c, 0x68, 0x48, 0x50, 0x58);
    }
} sDinoCleaningTutorialWindowStaticInit;
