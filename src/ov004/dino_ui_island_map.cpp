// DinoUiIslandMap, DrawFont, BitmapMove, SpotMove, DinoUiIslandMapMenuItem, IslandMapKeyCheckTask, GuidMan.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_field.h"
#include "m/input.h"
#include "m/lcd.h"
#include "m/bg.h"
#include "m/pal.h"
#include "m/window.h"
#include "game/dino_ui_island_map_menu.h"
#include "game/dino_ui_island_map.h"
#include "game/draw_font.h"
#include "m/console.h"
#include "game/bitmap_move.h"
#include "game/spot_move.h"
#include "game/dino_ui_island_map_menu_item.h"
#include "game/island_map_key_check_task.h"
#include "game/guid_man.h"
#include "game/dino_map_back_button.h"
#include "game/dino_sound.h"

extern const int data_02089adc[10]; // the place each map item stands for

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

#pragma auto_inline off
void DinoUiIslandMapMenu::SetSilentFocus(BOOL silent) {
    mSilentFocus = silent;
}

BOOL DinoUiIslandMapMenu::IsSilentFocus() {
    return mSilentFocus;
}

DinoUiIslandMapMenu::DinoUiIslandMapMenu(int *result, u32 arg)
    : MMenu(NULL, 2), mUnkE8(0), mMap("image/all_map", 0) {
    mUnk144 = arg;
    mResult = result;
    mBackButton = NULL;
}
#pragma auto_inline on

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

const char *DinoMapBackButton::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiIslandMapMenuItem::OnSelectChanged(BOOL selected) {
    if (selected) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DrawFont::Idle() {
}

// Centres the text on pos.
void DrawFont::SetCenter(const MPoint &pos) {
    int width = mConsole->GetWidth();
    int height = mConsole->GetHeight();
    mConsole->SetPosition(pos.x - width / 2, pos.y - height / 2);
}

void DrawFont::Show() {
    mConsole->Show();
}

void DinoMapBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        if (!mOwner->IsSilentFocus()) {
            DinoSoundPlaySe(0x96, 0);
        }
        mOwner->SetSilentFocus(FALSE);
        mMotion->Play(3, 1);
    } else {
        mMotion->Play(2, 1);
    }
}

void DinoUiIslandMapMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        if (!mOwner->IsSilentFocus()) {
            DinoSoundPlaySe(0x96, 0);
        }
        mOwner->SetSilentFocus(FALSE);
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoMapBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiIslandMapMenuItem::OnDecide() {
    mSerial.AddTail(mMotion->CreatePlayTransfer(3, 0));
    mOwner->OnSpotDecided();
    DinoSoundPlaySe(0x98, 0);
}

void DinoUiIslandMapMenu::OnDecide(MMenuItem *item) {
    if (item == NULL) {
        return;
    }
    for (int i = 0; i < 10; i++) {
        if (item == mItems[i]) {
            *mResult = data_02089adc[i];
            return;
        }
    }
}

void BitmapMove::Apply(int step) {
    MPointAnime::Apply(step);
    MPoint pos = mValue;
    mScreen->SetOffset(pos);
}

void DinoUiIslandMapMenu::OnBackDecided() {
    *mResult = -1;
    OnSpotDecided();
}

void IslandMapKeyCheckTask::Execute() {
    mMenu->CheckKeys();
}

void GuidMan::Execute() {
    if (mTasks->IsEmpty()) {
        mHelp->Enable();
    } else {
        mHelp->Disable();
    }
}

void DinoUiIslandMapMenu::CheckKeys() {
    MSerialTaskSet *serial = &mSerial;
    if (serial->IsEmpty()) {
        if (GetInput()->GetPressed() & 2) {
            if (mBackButton) {
                if (mBackButton->IsFocused()) {
                    mBackButton->Decide();
                } else {
                    mBackButton->Focus();
                }
            }
        }
    }
}

DinoMapSelectMenu::DinoMapSelectMenu(s32 *result, u32 unlocked) : MProcess(NULL) {
    mUnlocked = unlocked;
    mResult = result;
}
