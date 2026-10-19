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
#include "m/oam_canvas.h"
#include "m/text_codec.h"
#include "m/obj.h"
#include "game/dino_resource.h"
#include "game/bitmap_move.h"
#include "game/spot_move.h"
#include "game/dino_ui_island_map_menu_item.h"
#include "game/island_map_key_check_task.h"
#include "game/guid_man.h"
#include "game/dino_map_back_button.h"
#include "game/dino_sound.h"
#include "m/bright.h"
#include "m/display.h"

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

DinoUiIslandMapMenuItem::DinoUiIslandMapMenuItem(DinoUiIslandMapMenu *owner, const std::string &path, MPoint pos, u32 spot,
                                                 MSmartOam *oam) {
    mOwner = owner;
    mSpot = spot;
    mMotion = new MMotion(std::string(path), oam, 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(3);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}
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

extern "C" const u16 data_ov004_02175b50[]; // the texts DrawFont shows

#pragma opt_common_subs off
DrawFont::DrawFont(int text) : MProcess(NULL) {
    gCurrentDisplay = gLcd->GetTopDisplay();
    MFont *font = gDinoResource->CreateFont12(0, 1, 0);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, gCurrentDisplay->mObj->mOam, MPoint(0xE0, 0x30),
                                        gDinoResource->GetUnkTable(), TRUE);
    canvas->SetPriority(4);
    mConsole = new MConsole(canvas, font, MPoint(0, 3), MFx32(1));
    mConsole->SetUnk44(4);
    int width = mConsole->GetWidth();
    int height = mConsole->GetHeight();
    mConsole->SetPosition((256 - width) / 2, (192 - height) / 2);
    mConsole->Hide();
    mConsole->Print(MDecodeUtf8(std::string(gDinoResource->GetText(data_ov004_02175b50[text]))));
    mConsole->SetSpeed(MFx32(0));
    mParallel.AddTail(mConsole);
    gCurrentDisplay = gLcd->GetBottomDisplay();
}
#pragma opt_common_subs reset

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

extern const int data_020899c4, data_020899d8, data_02089a00;

// Fades both screens out, then exits.
void DinoUiIslandMapMenu::OnSpotDecided() {
    mUnk150->Clear();
    mUnkF8->Exit();
    MTask *exit = CreateExitTransfer();
    MTask *fadeSub = gLcd->mSub->mBright->CreateFadeOutToBlack(data_020899d8, data_020899c4);
    MTask *fadeMain = gLcd->mMain->mBright->CreateFadeOutToBlack(data_020899d8, data_020899c4);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(fadeMain);
    fades->AddTail(fadeSub);
    MTask *timer = new MTimer(data_02089a00);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(fades);
    serial->AddTail(exit);
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

DinoUiIslandMap::DinoUiIslandMap(s32 *result, u32 unlocked) : MProcess(NULL) {
    mUnlocked = unlocked;
    mResult = result;
}

#pragma dont_inline on
void DinoUiIslandMap::Idle() {
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mSub->Setup(5, 0, 4, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mSub->mObj->Show();
    gLcd->mMain->Setup(6, 3, 0, 0);
    gLcd->mMain->mBg->SetMode(5);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->mMain->mBright->SetMode(2);
    gLcd->mMain->mBright->SetFactor(16);
    gLcd->mSub->mBright->SetMode(2);
    gLcd->mSub->mBright->SetFactor(16);
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiIslandMapMenu((int *)mResult, mUnlocked);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}
#pragma dont_inline reset
