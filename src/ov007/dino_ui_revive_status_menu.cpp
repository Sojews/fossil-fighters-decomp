// DinoUiReviveStatusMenu.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_revive_status_menu.h"
#include "types.h"
#include "game/dino_sound.h"

DinoUiReviveStatusMenu::~DinoUiReviveStatusMenu() {
}

DinoUiReviveStatusLoop::~DinoUiReviveStatusLoop() {
}

DinoUiReviveStatusLoopCleaning::~DinoUiReviveStatusLoopCleaning() {
}

DinoUiReviveStatusLoopBattle::~DinoUiReviveStatusLoopBattle() {
}

DinoUiReviveStatusMenuItem::~DinoUiReviveStatusMenuItem() {
}

DinoUiReviveStatusBackButton::~DinoUiReviveStatusBackButton() {
}

MRect DinoUiReviveStatusMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiReviveStatusMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiReviveStatusMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiReviveStatusBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

const char *DinoUiReviveStatusMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiReviveStatusBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

extern "C" void func_ov007_02188e00(DinoUiReviveStatusMenu *menu, u8 index);

void DinoUiReviveStatusMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 4; i++) {
        if (item == mItems[i]) {
            func_ov007_02188e00(this, i);
        }
    }
}

// Where the cursor sits on each status item.
extern MPoint data_020b5890[4];

void DinoUiReviveStatusMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (int i = 0; i < 4; i++) {
        if (item == mItems[i]) {
            frame.MoveTo(data_020b5890[i]);
            mCursor->SetMode(2);
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    mCursor->Show();
}
