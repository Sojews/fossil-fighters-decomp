// DinoUiDebugFlagMenu.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/font.h"
#include "game/dino_resource.h"
#include "m/canvas.h"
#include "m/smart_oam.h"
#include "game/dino_common_cursor.h"
#include "game/dino_ui_debug_flag_menu_menu.h"
#include "types.h"
#include "m/input.h"

DinoUiDebugFlagMenu::DinoUiDebugFlagMenu(int mode) : MMenu(0), mMode(mode) {
}

DinoUiDebugFlagMenu::~DinoUiDebugFlagMenu() {
    mFont->Release();
}

// A page of 100 flags (the game's, or 48 for the other mode).
void DinoUiDebugFlagMenu::OnStart() {
    GetInput()->SetRepeat(0x3F0, 20, 3);
    mFont = gDinoResource->CreateFont12(0, 10, 0);
    mFont->AddRef();
    mCanvas = new MTextCanvas(MPoint(256, 192), gDinoResource->GetUnkTable(), 0, TRUE);
    mCanvas->Show();
    mOam = new MSmartOam(0x28, 0, TRUE);
    mTasks = new MParallelTaskSet;
    mCursor = new DinoCommonCursor(mOam);
    u32 count;
    if (mMode == 0) {
        count = gDinoResource->GetFlagCount();
    } else {
        count = 0x30;
    }
    mPage = 0;
    mPageCount = (count - 1) / 100 + 1;
    mLastPageCount = (count - 1) % 100 + 1;
    GetParallel()->AddTail(mCanvas, mTasks, mCursor, mOam);
    ShowPage();
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
