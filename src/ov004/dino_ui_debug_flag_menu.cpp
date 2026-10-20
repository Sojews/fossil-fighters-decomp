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

// NONMATCHING: the game computes the right and bottom edges before storing any edge (and keeps 16 in r1).
MRect DinoUiDebugFlagMenuItem::GetFrame() {
    MRect frame(0, 0, 16, 16);
    frame.MoveTo(mPos);
    frame.Shift(-5, -2);
    return frame.Inflate(2);
}

// Toggles the flag and redraws it.
void DinoUiDebugFlagMenuItem::OnDecide() {
    BOOL value = !GetValue();
    SetValue(value);
    if (!GetValue()) {
        mCanvas->DrawText(mFont, mPos, 0, std::string("0"));
    } else {
        std::string one("1");
        mCanvas->DrawText(mFont, mPos, 0, one);
    }
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

// Draws the page's row and column labels and makes an item for each of its flags.
void DinoUiDebugFlagMenu::ShowPage() {
    ClearItems();
    mCanvas->Clear();
    u32 count = mPage == mPageCount - 1 ? mLastPageCount : 100;
    for (u32 i = 0; i < 10; i++) {
        MPoint pos(0x1E, 0x1C + i * 16);
        char text[256];
        OS_SPrintf(text, "%06d:", mPage * 100 + i * 10);
        mCanvas->DrawText(mFont, pos, 0, std::string(text));
    }
    for (u32 i = 0; i < 10; i++) {
        MPoint pos(0x4D + i * 16, 0xA);
        char text[256];
        OS_SPrintf(text, "+%d", i);
        mCanvas->DrawText(mFont, pos, 0, std::string(text));
    }
    for (u32 i = 0; i < count; i++) {
        u32 id = mPage * 100 + i;
        MPoint pos(i % 10 * 16 + 0x50, i / 10 * 16 + 0x1C);
        AddItem(new DinoUiDebugFlagMenuItem(mMode, id, mCanvas, mFont, pos));
    }
    FocusNearestItem(GetCursor());
}

// (Off at the end of the file, where template members are instantiated: the game calls them out of line.)
#pragma auto_inline off
