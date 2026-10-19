// DinoCommonTutorialConsoleFilter.
// Only the destructors are decompiled so far.

#define DINO_COMMON_TUTORIAL_WINDOW_OWNER
#include "game/dino_common_tutorial_console_filter.h"
#include "game/dino_common_tutorial_window.h"
#include "game/dino_sound.h"
#include "m/motion.h"

DinoCommonTutorialConsoleFilter::~DinoCommonTutorialConsoleFilter() {
}

void DinoCommonTutorialWindow::Idle() {
    (this->*mState)();
}

DinoCommonTutorialWindow::~DinoCommonTutorialWindow() {
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x8];
    const char *longArrowPath;      // 0x08
    const char *cornerPath;         // 0x0C
    const char *bgPath;             // 0x10
    const char *cursorPath;         // 0x14
    const char *shortArrowPath;     // 0x18
    u8 unk1C[0x2C - 0x1C];
    void (DinoCommonTutorialWindow::*Start_2C)();
} data_020b5e5c;

DinoCommonTutorialWindow::DinoCommonTutorialWindow(MDisplay *display, std::string text) : MProcess(display), mText(text) {
    mState = data_020b5e5c.Start_2C;
}

// Points at pos with an arrow (dir picks the anime).
void DinoCommonTutorialWindow::ShowLongArrow(MPoint pos, int dir) {
    DinoSoundPlaySe(0x397, 0);
    mArrows->Clear();
    MMotion *arrow = new MMotion(data_020b5e5c.longArrowPath, mOam, 0, 1);
    arrow->SetPosition(pos);
    arrow->Play(dir + 1, 0);
    arrow->Show();
    mArrows->AddTail(arrow);
}

void DinoCommonTutorialWindow::ShowShortArrow(MPoint pos, int dir) {
    DinoSoundPlaySe(0x397, 0);
    mArrows->Clear();
    MMotion *arrow = new MMotion(data_020b5e5c.shortArrowPath, mOam, 0, 1);
    arrow->SetPosition(pos);
    arrow->Play(dir + 1, 0);
    arrow->Show();
    mArrows->AddTail(arrow);
}
