// DinoUiDebugFlagMenu.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/font.h"
#include "game/dino_ui_debug_flag_menu_menu.h"
#include "types.h"
#include "m/input.h"

DinoUiDebugFlagMenu::DinoUiDebugFlagMenu(int mode) : MMenu(0), mMode(mode) {
}

DinoUiDebugFlagMenu::~DinoUiDebugFlagMenu() {
    mFont->Release();
}

DinoUiDebugFlagMenuItem::~DinoUiDebugFlagMenuItem() {
    mFont->Release();
}

// NONMATCHING: the game folds the offsets differently.
MRect DinoUiDebugFlagMenuItem::GetFrame() {
    MRect frame(0, 0, 16, 16);
    frame.Offset(mPos);
    frame.Offset(MPoint(-5, -2));
    return frame.Inflate(2);
}

void DinoUiDebugFlagMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    if (mCursor->IsHidden()) {
        mCursor->Show();
        mCursor->SetFrame(item->GetFrame());
    } else {
        mTasks->Clear();
        mTasks->AddTail(mCursor->CreateFrameAnime(mCursor->GetFrame(), item->GetFrame(), 5, -2));
    }
}

// R and L turn the page; B leaves.
void DinoUiDebugFlagMenu::OnIdle() {
    if (GetInput()->GetPressed() & 0x100) {
        mPage = (mPage + 1) % mPageCount;
        ShowPage();
        return;
    }
    if (GetInput()->GetPressed() & 0x200) {
        mPage = (mPage + mPageCount - 1) % mPageCount;
        ShowPage();
        return;
    }
    if (GetInput()->GetPressed() & 2) {
        Exit();
    }
}
