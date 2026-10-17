// DinoUiIslandMap, DrawFont, BitmapMove, SpotMove, DinoUiIslandMapMenuItem, IslandMapKeyCheckTask, GuidMan.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/lcd.h"
#include "m/bg.h"
#include "m/pal.h"
#include "m/window.h"
#include "game/dino_ui_island_map_menu.h"
#include "game/dino_ui_island_map.h"
#include "game/draw_font.h"
#include "game/bitmap_move.h"
#include "game/spot_move.h"
#include "game/dino_ui_island_map_menu_item.h"
#include "game/island_map_key_check_task.h"
#include "game/guid_man.h"
#include "game/dino_map_back_button.h"

DinoUiIslandMap::~DinoUiIslandMap() {
}

DrawFont::~DrawFont() {
}

BitmapMove::~BitmapMove() {
}

SpotMove::~SpotMove() {
}

DinoUiIslandMapMenuItem::~DinoUiIslandMapMenuItem() {
}

IslandMapKeyCheckTask::~IslandMapKeyCheckTask() {
}

GuidMan::~GuidMan() {
}

DinoMapBackButton::~DinoMapBackButton() {
}

DinoUiIslandMapMenu::~DinoUiIslandMapMenu() {
    gLcd->GetTopDisplay()->mBg->DeleteIndexBitmap(mBitmap);
    gLcd->GetTopDisplay()->mBg->mPal->Free(mPalette);
    gLcd->mMain->mWin->Reset();
    gLcd->mSub->mWin->Reset();
}

void DinoUiIslandMapMenu::OnIdle() {
}

void DinoMapBackButton::OnSelectChanged(BOOL value) {
}

MRect DinoMapBackButton::GetRect() {
    return GetFrame();
}

MDisplay *DinoMapBackButton::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoMapBackButton::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiIslandMapMenuItem::GetFrame() {
    return mMotion->GetFrame();
}
