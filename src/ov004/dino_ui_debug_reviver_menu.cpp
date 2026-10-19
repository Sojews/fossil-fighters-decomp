// DinoUiDebugReviverMenu.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/font.h"
#include "m/canvas.h"
#include "m/input.h"
#include "m/smart_oam.h"
#include "game/dino_common_cursor.h"
#include "game/dino_resource.h"
#include "game/dino_ui_debug_reviver_menu_menu.h"
#include "types.h"

DinoUiDebugReviverMenu::DinoUiDebugReviverMenu() : MMenu(0) {
}

DinoUiDebugReviverMenu::~DinoUiDebugReviverMenu() {
    mFont->Release();
}

void DinoUiDebugReviverMenu::OnStart() {
    GetInput()->SetRepeat(0x3C0, 20, 3);
    mFont = gDinoResource->CreateFont12(0, 10, 0);
    mFont->AddRef();
    mCanvas = new MTextCanvas(MPoint(256, 192), gDinoResource->GetUnkTable(), 0, TRUE);
    mCanvas->Show();
    mOam = new MSmartOam(0x28, 0, TRUE);
    mTasks = new MParallelTaskSet;
    mCursor = new DinoCommonCursor(mOam);
    mUnkBC = 1;
    GetParallel()->AddTail(mCanvas, mTasks, mCursor, mOam);
    Refresh();
}

DinoUiDebugReviverMenuItem::~DinoUiDebugReviverMenuItem() {
    mFont->Release();
}

void DinoUiDebugReviverMenuItem::OnDecide() {
}

// NONMATCHING: register allocation only (the game builds the rect without moving the return slot out of r0).
MRect DinoUiDebugReviverMenuItem::GetFrame() {
    MRect frame(0, 0, 24, 12);
    frame.Offset(mPos);
    return frame.Inflate(6);
}

void DinoUiDebugReviverMenu::OnFocus(MMenuItem *item) {
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
