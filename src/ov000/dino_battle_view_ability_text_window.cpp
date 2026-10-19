// DinoBattleViewAbilityTextWindow.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_ability_text_window.h"
#include "types.h"
#include "m/text.h"
#include "game/dino_battle_view_ability_menu_bg.h"

DinoBattleViewAbilityTextWindow::~DinoBattleViewAbilityTextWindow() {
    delete mTask;
}

#pragma auto_inline off

void DinoBattleViewAbilityMenuBG::SetPriority(int priority) {
    if (mUnk10D4) {
        mText->mBgCntBits.priority = priority;
    }
}

#pragma auto_inline on

void DinoBattleViewAbilityMenuBG::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x38];
    void (DinoBattleViewAbilityMenuBG::*StateUpdate_38)();
} data_ov000_021947ac;

void DinoBattleViewAbilityMenuBG::StateShow() {
    mText->Show();
    mState = data_ov000_021947ac.StateUpdate_38;
}

#pragma auto_inline off

// The tiles of each row style of the window (top, middle, bottom) in the picture's screen, by window kind.
extern "C" const u32 data_ov000_0218ffa4[][3];
extern "C" void func_ov000_0217c038(DinoBattleViewAbilityMenuBG *bg, MRect rect);
extern "C" void func_ov000_0217c074(DinoBattleViewAbilityMenuBG *bg, int kind);

void DinoBattleViewAbilityMenuBG::StateUpdate() {
    MI_CpuFill8(mScreen, 0, sizeof(mScreen));
    for (std::vector<void *>::iterator it = mUnk10C8.begin(); it != mUnk10C8.end(); ++it) {
        if (!((DinoBattleViewAbilityTextWindow *)*it)->IsHidden()) {
            if (((DinoBattleViewAbilityTextWindow *)*it)->GetUnk18()) {
                func_ov000_0217c074(this, ((DinoBattleViewAbilityTextWindow *)*it)->GetUnk1C());
            } else {
                func_ov000_0217c038(this, ((DinoBattleViewAbilityTextWindow *)*it)->GetRect());
            }
        }
    }
    mText->Transfer(&mScreen[0][0], MPoint(32, 64), MPoint(0, 0), mChrBlock->index, mPalBlock->index, false, false);
}

// Draws a window frame: corners and edges, the rest filled (tiles 1 to 9 of the frame).
extern "C" void func_ov000_0217bfa0(DinoBattleViewAbilityMenuBG *bg, MPoint pos, MPoint size) {
    for (int row = 0; row < size.y; row++) {
        u16 *dst = bg->mScreen[0] + pos.x + (pos.y + row) * 32;
        int style = (row > 0) + (row == size.y - 1);
        int tile = style * 3;
        int x = pos.x;
        if (x >= 0 && x <= 31) {
            *dst++ = tile + 1;
        } else {
            dst++;
        }
        x++;
        for (int col = 2; col < size.x; col++) {
            if (x >= 0 && x <= 31) {
                *dst++ = tile + 2;
            } else {
                dst++;
            }
            x++;
        }
        if (x >= 0 && x <= 31) {
            *dst = tile + 3;
        }
    }
}

extern "C" void func_ov000_0217c038(DinoBattleViewAbilityMenuBG *bg, MRect rect) {
    MPoint pos(rect.left, rect.top);
    MPoint size(rect.right + 1 - rect.left, rect.bottom + 1 - rect.top);
    func_ov000_0217bfa0(bg, pos, size);
}

// Fills rows 18 to 20 across with the window kind's tiles.
extern "C" void func_ov000_0217c074(DinoBattleViewAbilityMenuBG *bg, int kind) {
    const u32 *rows = data_ov000_0218ffa4[kind];
    for (int i = 0; i < 3; i++) {
        u16 tile = ((const u16 *)bg->mPicture.GetScreen())[rows[i]];
        u16 *dst = bg->mScreen[i + 18];
        for (int x = 0; x < 32; x++) {
            *dst++ = tile;
        }
    }
}

// A new text window (shown over the whole rows for kind != 0).
extern "C" DinoBattleViewAbilityTextWindow *func_ov000_0217c0b8(DinoBattleViewAbilityMenuBG *bg, BOOL rows) {
    DinoBattleViewAbilityTextWindow *window = new DinoBattleViewAbilityTextWindow(rows, 6);
    bg->mUnk10C8.push_back(window);
    bg->mParallel.AddTail(window);
    return window;
}

DinoBattleViewAbilityTextWindow::DinoBattleViewAbilityTextWindow(BOOL rows, u32 arg)
    : mShown(FALSE), mTask(NULL), mUnk18(rows), mUnk1C(arg) {
}
