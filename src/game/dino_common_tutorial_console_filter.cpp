// DinoCommonTutorialConsoleFilter.
// Only the destructors are decompiled so far.

#pragma ipa file

#define DINO_COMMON_TUTORIAL_WINDOW_OWNER
#define MSL_STRING_BASICS
#define MSL_STRING_REPLACE_DEFINITION
#include "m/oam_canvas.h"
#include "game/dino_common_tutorial_console_filter.h"
#include "game/dino_common_tutorial_window.h"
#include "game/dino_sound.h"
#include "m/motion.h"
#include "m/view.h"
#include "m/input.h"
#include "m/console.h"


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
    u8 unk1C[0x24 - 0x1C];
    void (DinoCommonTutorialWindow::*StateClose_24)();
    void (DinoCommonTutorialWindow::*Start_2C)();
} data_020b5e5c;

DinoCommonTutorialWindow::DinoCommonTutorialWindow(MDisplay *display, std::string text) : MProcess(display), mText(text) {
    mState = data_020b5e5c.Start_2C;
}

void DinoCommonTutorialWindow::StateRead() {
    if (mConsole->mState == STATE_SUSPEND) {
        mNextMark->Show();
        if (mInput->GetPressed() & 0x1003) {
            DinoSoundPlaySe(0x93, 0);
            if (mConsole->IsEmpty()) {
                mState = data_020b5e5c.StateClose_24;
                return;
            }
            mNextMark->Hide();
            mConsole->Clear();
            mConsole->SetSpeed(MFx32(2));
            mConsole->Run();
        }
    } else if (mInput->GetPressed() & 0x1003) {
        DinoSoundPlaySe(0xAF, 0);
        mConsole->SetSpeed(MFx32(0));
    }
}

void DinoCommonTutorialWindow::StateClose() {
    SetActive(FALSE);
    DinoSoundPlaySe(0xA9, 0);
    Exit();
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

// Marks a rectangle with a corner at each of its corners.
// NONMATCHING: the game keeps the left edge in a stack slot of its own.
void DinoCommonTutorialWindow::ShowCorners(MRect rect) {
    DinoSoundPlaySe(0x3B0, 0);
    mArrows->Clear();
    MMotion *corner = new MMotion(data_020b5e5c.cornerPath, mOam, 0, 1);
    corner->SetPosition(MPoint(rect.left, rect.top));
    corner->Play(1, 0);
    corner->Show();
    mArrows->AddTail(corner);
    corner = new MMotion(data_020b5e5c.cornerPath, mOam, 0, 1);
    corner->SetPosition(MPoint(rect.right, rect.top));
    corner->Play(3, 0);
    corner->Show();
    mArrows->AddTail(corner);
    corner = new MMotion(data_020b5e5c.cornerPath, mOam, 0, 1);
    corner->SetPosition(MPoint(rect.left, rect.bottom));
    corner->Play(2, 0);
    corner->Show();
    mArrows->AddTail(corner);
    corner = new MMotion(data_020b5e5c.cornerPath, mOam, 0, 1);
    corner->SetPosition(MPoint(rect.right, rect.bottom));
    corner->Play(4, 0);
    corner->Show();
    mArrows->AddTail(corner);
}

void DinoCommonTutorialWindow::SetActive(BOOL active) {
    OnSetActive(active);
}

void DinoCommonTutorialWindow::OnSetActive(BOOL active) {
}

void DinoCommonTutorialWindow::ClearArrows() {
    mArrows->Clear();
}

// String members the game emits in this file.
template std::wstring::~basic_string();
template std::wstring &std::wstring::assign(const wchar_t *first, const wchar_t *last);
// (Constructing one here instantiates the constructor with the name the game's callers use.)
#pragma dont_inline on
void DinoTutorialFilterWStringAnchor(const wchar_t *text) {
    std::wstring s(text);
}

// (Likewise reserve and __replace, which explicit instances here would name differently.)
void DinoTutorialFilterReserveAnchor(std::wstring *s) {
    s->reserve(0);
}

void DinoTutorialFilterReplaceAnchor(std::wstring *s, const wchar_t *first, const wchar_t *last) {
    s->__replace(0, 0, first, last, std::random_access_iterator_tag());
}
#pragma dont_inline reset

// (Making one here emits the canvas's destructors, which the game emits in this file.)
#pragma dont_inline on
MObjCanvas *DinoTutorialFilterCanvasAnchor(MDisplay *display, MOam *oam, MPoint size, const void *palette) {
    return new MObjCanvas(display, oam, size, palette, TRUE);
}
#pragma dont_inline reset
