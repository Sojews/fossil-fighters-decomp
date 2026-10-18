// DinoCommonTutorialConsoleFilter.
// Only the destructors are decompiled so far.

#define DINO_COMMON_TUTORIAL_WINDOW_OWNER
#include "game/dino_common_tutorial_console_filter.h"
#include "game/dino_common_tutorial_window.h"

DinoCommonTutorialConsoleFilter::~DinoCommonTutorialConsoleFilter() {
}

void DinoCommonTutorialWindow::Idle() {
    (this->*mState)();
}

DinoCommonTutorialWindow::~DinoCommonTutorialWindow() {
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x2C];
    void (DinoCommonTutorialWindow::*Start_2C)();
} data_020b5e5c;

DinoCommonTutorialWindow::DinoCommonTutorialWindow(MDisplay *display, std::string text) : MProcess(display), mText(text) {
    mState = data_020b5e5c.Start_2C;
}
