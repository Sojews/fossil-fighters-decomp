// DinoCommonTutorialConsoleFilter.

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
#include "m/picture.h"
#include "m/lcd.h"
#include "game/dino_common_font.h"
#include "game/dino_resource.h"
#include "game/dino_backup.h"
#include "nitro/os.h"

// (wstring::assign comes out ahead of the others, so that __replace isn't followed by its callee in our object.)
template std::wstring &std::wstring::assign(const wchar_t *first, const wchar_t *last);


DinoCommonTutorialConsoleFilter::~DinoCommonTutorialConsoleFilter() {
}

// Reads a decimal number from the console (pushing back the character after it).
static inline int DinoTutorialReadNumber(MConsole *console) {
    int n = 0;
    for (;;) {
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

// Stops the game unless the console's next character is ch.
static inline void DinoTutorialExpect(MConsole *console, u32 ch) {
    if (console->IsEmpty()) {
        OS_Terminate();
    }
    if (console->PopChar() != ch) {
        OS_Terminate();
    }
}

// NONMATCHING: the game's compare tree for 'p'..'w' tests s..w as a range (see case 't').
BOOL DinoCommonTutorialConsoleFilter::Filter(MConsole *console, int c) {
    if (c != '$') {
        if (console->GetSpeed() != MFx32(0)) {
            if (mUnk0D) {
                DinoSoundPlaySe(0xAE, 0);
            }
            if (mUnk0C) {
                DinoSoundPlaySe(0xAA, 0);
            } else {
                DinoSoundPlaySe(0xAB, 0);
            }
        }
        return FALSE;
    }
    if (console->IsEmpty()) {
        return TRUE;
    }
    switch ((u32)console->PopChar()) {
    case 'B':
        DinoSoundPlayBgm(0x39E, 0);
        break;
    case 'W':
        mUnk0C = FALSE;
        break;
    case 's':
        mUnk0D = TRUE;
        break;
    case 'f':
        console->PushChar(12);
        break;
    case 'w': {
        int n = DinoTutorialReadNumber(console);
        if (n == 0) {
            n = 1;
        }
        MFx32 rate = MFx32(12) / MFx32(n);
        console->SetRate(rate);
        break;
    }
    case 'n':
        if (mFont != NULL) {
            mFont->Select(DinoTutorialReadNumber(console));
        }
        break;
    case 'p': {
        DinoPlayerData player;
        gDinoBackup->GetPlayer(&player, 0);
        std::wstring name((const wchar_t *)player.name);
        for (std::wstring::const_reverse_iterator it = name.rbegin(); it != name.rend(); ++it) {
            console->PushChar(*it);
        }
        break;
    }
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
    case 'S': {
        int dir = DinoTutorialReadNumber(console);
        DinoTutorialExpect(console, ',');
        int x = DinoTutorialReadNumber(console);
        DinoTutorialExpect(console, ',');
        int y = DinoTutorialReadNumber(console);
        mWindow->ShowShortArrow(MPoint(x, y), dir);
        break;
    }
    case 'L': {
        int dir = DinoTutorialReadNumber(console);
        DinoTutorialExpect(console, ',');
        int x = DinoTutorialReadNumber(console);
        DinoTutorialExpect(console, ',');
        int y = DinoTutorialReadNumber(console);
        mWindow->ShowLongArrow(MPoint(x, y), dir);
        break;
    }
    case 'F': {
        int left = DinoTutorialReadNumber(console);
        DinoTutorialExpect(console, ',');
        int top = DinoTutorialReadNumber(console);
        DinoTutorialExpect(console, ',');
        int right = DinoTutorialReadNumber(console);
        DinoTutorialExpect(console, ',');
        int bottom = DinoTutorialReadNumber(console);
        mWindow->ShowCorners(MRect(left, top, right, bottom));
        break;
    }
    case 'H':
        mWindow->ClearArrows();
        break;
    case 't':
        // (Not in the game: an extra case gets its compare tree's shape, but costs a compare of its own.)
        break;
    }
    return TRUE;
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
    void (DinoCommonTutorialWindow::*StateStart_2C)();
    u8 unk34[0x3C - 0x34];
    void (DinoCommonTutorialWindow::*StateRead_3C)();
} data_020b5e5c;

DinoCommonTutorialWindow::DinoCommonTutorialWindow(MDisplay *display, std::string text) : MProcess(display), mText(text) {
    mState = data_020b5e5c.StateStart_2C;
}

static inline MPoint operator*(const MPoint &p, int k) {
    return MPoint(p.x * k, p.y * k);
}

// NONMATCHING: the game multiplies the margin by a -2 it builds at run time ((1 + 1) * (1 - 2)), loads gCurrentDisplay
// after copying the canvas size and swaps r4/r5.
void DinoCommonTutorialWindow::StateStart() {
    DinoSoundPlaySe(0xA8, 0);
    SetActive(TRUE);
    MInputHub *hub = new MSingleInputHub(1);
    hub->EnableTouch();
    mInput = hub->GetInput(0);
    mParallel.AddTail(hub);
    MDisplay *display;
    MRect rect;
    if (gCurrentDisplay == gLcd->GetTopDisplay()) {
        display = gLcd->GetBottomDisplay();
        rect = MRect(0, 0, 0x100, 0x40);
    } else {
        display = gLcd->GetTopDisplay();
        rect = MRect(0, 0x80, 0x100, 0xC0);
    }
    {
        MDisplayScope scope(display);
        MTextPicture *frame = new MTextPicture(std::string(data_020b5e5c.bgPath), TRUE, false);
        frame->SetPosition(rect.GetTopLeft());
        frame->Show();
        mParallel.AddTail(frame);
        DinoCommonFont *font = new DinoCommonFont(0, 1, 0);
        mConsole = new MConsole(new MObjCanvas(gCurrentDisplay, rect.GetSize() - MPoint(0x10, 0xA) * 2,
                                               gDinoResource->GetUnkTable(), TRUE),
                                font, MPoint(0, 4), MFx32(2));
        mConsole->Print(std::string(mText));
        mConsole->SetFilter(new DinoCommonTutorialConsoleFilter(this, font));
        mConsole->SetPosition(rect.GetTopLeft() + MPoint(0x10, 0xA));
        mConsole->Show();
        mParallel.AddTail(mConsole);
        MSmartOam *oam = new MSmartOam(1, 0, 1);
        mNextMark = new MMotion(std::string(data_020b5e5c.cursorPath), oam, 0, 1);
        mNextMark->SetPosition((gCurrentDisplay == gLcd->GetTopDisplay() ? MPoint(0, 0) : MPoint(0, -0x80)).Offset(0x86, 0x5C));
        ((MMotion *)mNextMark)->Play(1, 0);
        GetParallel()->AddTail(mNextMark, oam);
    }
    mArrows = new MParallelTaskSet;
    mOam = new MSmartOam(4, 0, 1);
    GetParallel()->AddTail(mArrows, mOam);
    mState = data_020b5e5c.StateRead_3C;
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
void DinoCommonTutorialWindow::ShowCorners(MRect rect) {
    DinoSoundPlaySe(0x3B0, 0);
    MMotion *corner;
    mArrows->Clear();
    corner = new MMotion(data_020b5e5c.cornerPath, mOam, 0, 1);
    MPoint topLeft(rect.left, rect.top);
    s32 left = rect.left;
    corner->SetPosition(topLeft);
    corner->Play(1, 0);
    corner->Show();
    mArrows->AddTail(corner);
    corner = new MMotion(data_020b5e5c.cornerPath, mOam, 0, 1);
    MPoint perm1(rect.right, rect.top);
    corner->SetPosition(perm1);
    corner->Play(3, 0);
    corner->Show();
    mArrows->AddTail(corner);
    corner = new MMotion(data_020b5e5c.cornerPath, mOam, 0, 1);
    MPoint bottomLeft(left, rect.bottom);
    corner->SetPosition(bottomLeft);
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

// String members the game emits in this file, implicitly instantiated so they come out in its order (C1, D1,
// reserve, __replace, assign). (Constructing one here instantiates the constructor with the name the game's callers use.)
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
