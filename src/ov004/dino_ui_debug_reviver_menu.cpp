// DinoUiDebugReviverMenu.

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
#include "nitro/mi.h"

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

// R and L pick the next or previous creature; up and down raise or lower the focused part's level (it jumps between 0
// and 50); B leaves.
// NONMATCHING: stack slots and registers only (the game puts each branch's state below the redraw's, and the item in r6).
void DinoUiDebugReviverMenu::OnIdle() {
    if (GetInput()->GetPressed() & 0x100) {
        mUnkBC++;
        if (mUnkBC >= gDinoResource->GetCreatureCount()) {
            mUnkBC = 1;
        }
        Refresh();
        return;
    }
    if (GetInput()->GetPressed() & 0x200) {
        mUnkBC--;
        if ((int)mUnkBC < 1) {
            mUnkBC = gDinoResource->GetCreatureCount() - 1;
        }
        Refresh();
        return;
    }
    if (GetInput()->GetPressed() & 0x40) {
        DinoUiDebugReviverMenuItem *item = (DinoUiDebugReviverMenuItem *)GetFocusedItem();
        DinoCreatureState state;
        gDinoBackup->GetCreature(item->mCreature, &state);
        if (state.parts[item->mPart].level < 150) {
            state.parts[item->mPart].level++;
            if (state.parts[item->mPart].level < 50) {
                state.parts[item->mPart].level = 50;
            }
            gDinoBackup->SetCreature(item->mCreature, &state);
            item->Update();
        }
        return;
    }
    if (GetInput()->GetPressed() & 0x80) {
        DinoUiDebugReviverMenuItem *item = (DinoUiDebugReviverMenuItem *)GetFocusedItem();
        DinoCreatureState state;
        gDinoBackup->GetCreature(item->mCreature, &state);
        if (state.parts[item->mPart].level) {
            state.parts[item->mPart].level--;
            if (state.parts[item->mPart].level < 50) {
                state.parts[item->mPart].level = 0;
            }
            gDinoBackup->SetCreature(item->mCreature, &state);
            item->Update();
        }
        return;
    }
    if (GetInput()->GetPressed() & 2) {
        Exit();
    }
}

// Draws the creature's name, and an item for each of its parts.
// NONMATCHING: stack slots of the items' inlined constructor only (the state, the rock and the strings).
void DinoUiDebugReviverMenu::Refresh() {
    ClearItems();
    mCanvas->Clear();
    char text[256];
    sprintf(text, "din%03d: %s", mUnkBC, gDinoResource->GetCreatureName(mUnkBC));
    mCanvas->DrawText(mFont, MPoint(0x32, 0x28), 0, std::string(text));
    for (u32 i = 0; i < 4; i++) {
        MPoint pos(0x40 + i * 0x20, 0x60);
        AddItem(new DinoUiDebugReviverMenuItem(mCanvas, mFont, pos, i, mUnkBC));
    }
    FocusNearestItem(GetCursor());
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
