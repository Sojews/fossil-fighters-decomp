// DinoUiIslandMap, DrawFont, BitmapMove, SpotMove, DinoUiIslandMapMenuItem, IslandMapKeyCheckTask, GuidMan.

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
#include "m/bitmap.h"
#include "m/anime.h"
#include "m/raster.h"
#include "m/blend.h"
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
    : MMenu(NULL, 2), mNameMotion(0), mMap("image/all_map", 0) {
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

#pragma auto_inline off
// Centres the text on pos.
void DrawFont::SetCenter(const MPoint &pos) {
    int width = mConsole->GetWidth();
    int height = mConsole->GetHeight();
    mConsole->SetPosition(pos.x - width / 2, pos.y - height / 2);
}

void DrawFont::Show() {
    mConsole->Show();
}
#pragma auto_inline reset

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

extern "C" const int data_ov004_02175c6c[]; // per place: the x of its spot on the map (pairs with the y)
extern "C" const int data_ov004_02175c70[]; // and the y
extern "C" const char *data_ov004_02175bf4[]; // per place: its name plate's motion
extern "C" const char *data_ov004_021759d8[];
extern "C" const MPoint data_ov004_02175a54, data_ov004_02175a6c;

// Scrolls the map to the focused place and shines the spotlight on it.
// NONMATCHING: the game loads both spot tables' addresses first (the x and y swap registers).
void DinoUiIslandMapMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        return;
    }
    int place = -1;
    for (int i = 0; i < 10; i++) {
        if (item == mItems[i]) {
            place = data_02089adc[i];
            break;
        }
    }
    if (place == -1) {
        return;
    }
    mSpotTasks->Clear();
    mMoveTasks->Clear();
    int x = data_ov004_02175c6c[place * 2];
    int y = data_ov004_02175c70[place * 2];
    MPoint scroll(-x, -y);
    if (scroll.x < 0) {
        scroll.x = 0;
    } else if (scroll.x > 256) {
        scroll.x = 256;
    }
    if (scroll.y < 0) {
        scroll.y = 0;
    } else if (scroll.y > 320) {
        scroll.y = 320;
    }
    int dy = scroll.y - mScroll.y;
    int dx = scroll.x - mScroll.x;
    MPoint center(128 - (scroll.x + x), 64 - (scroll.y + y));
    int sy = scroll.y;
    *(vu16 *) 0x040002B0 = 0;
    *(vu32 *) 0x040002B8 = dx * dx + dy * dy;
    while (*(vu16 *) 0x040002B0 & 0x8000) {
    }
    int frames = (*(vu32 *) 0x040002B4 << 10) >> 12;
    if (frames < 20) {
        frames = 20;
    }
    mNameMotion = new MMotion(std::string(data_ov004_02175bf4[place]), mOam, 0, 1);
    mNameMotion->Play(1, 0);
    mNameMotion->SetPosition(data_ov004_02175a54);
    mNameMotion->SetPriority(5);
    mNameMotion->SetAnchor(2);
    mNameMotion->Hide();
    mSpotTasks->AddTail(mNameMotion);
    mMarkMotion = new MMotion(std::string(data_ov004_021759d8[3]), mOam, 0, 1);
    mMarkMotion->Play(1, 0);
    mMarkMotion->SetPosition(data_ov004_02175a6c);
    mMarkMotion->SetPriority(3);
    mMarkMotion->SetAnchor(2);
    mMarkMotion->Hide();
    mSpotTasks->AddTail(mMarkMotion);
    DrawFont *font = new DrawFont(place);
    mSpotTasks->AddTail(font);
    BitmapMove *move = new BitmapMove(mBitmap, mScroll, scroll, frames);
    SpotMove *spot = new SpotMove(mWindow, center, frames, mNameMotion, mMarkMotion, font, place == 8 ? TRUE : FALSE);
    MTaskSet *tasks = mMoveTasks;
    tasks->AddTail(spot);
    tasks->AddTail(move);
    mScroll.x = scroll.x;
    mScroll.y = sy;
}

// The name plates' and markers' places (the TU's pooled data, from 0x02175a58).
extern "C" const struct {
    u32 unk0;
    MPoint mNameIn;     // 0x04: the name plate's place (coming in from the left)
    MPoint mNameInFlip; // 0x0C: (from the right)
    MPoint mMarkOut;    // 0x14
    MPoint mMarkIn;     // 0x1C
} data_ov004_02175a58;
extern "C" const MPoint data_ov004_02175a44, data_ov004_02175a5c, data_ov004_02175a64, data_ov004_02175a74;

// Fades the top screen in, follows the scroll, then narrows the spotlight while the name plate and marker come in.
// NONMATCHING: the last centre's x and y load in the other order.
void SpotMove::Idle() {
    if (mFrame <= 3) {
        gLcd->mMain->mBlend->SetBrightness((3 - mFrame) * 2);
    } else if (mFrames - 15 == mFrame) {
        gLcd->mMain->mBlend->SetBrightness(8);
        mRadius = new MValueAnime(256, 32);
        mParallel.AddTail(mRadius);
        mRadius->SetDuration(15);
        mRadius->mEase = -6;
        if (!mFlip) {
            mMarkMove = new MPointAnime(data_ov004_02175a6c, data_ov004_02175a58.mMarkIn);
            MPointAnime *mark = mMarkMove;
            mNameMove = new MPointAnime(data_ov004_02175a54, data_ov004_02175a58.mNameIn);
            MParallelTaskSet *tasks = &mParallel;
            tasks->AddTail(mNameMove);
            tasks->AddTail(mark);
        } else {
            mMarkMove = new MPointAnime(data_ov004_02175a6c, data_ov004_02175a58.mMarkIn);
            MPointAnime *mark = mMarkMove;
            mNameMove = new MPointAnime(data_ov004_02175a44, data_ov004_02175a58.mNameInFlip);
            MParallelTaskSet *tasks = &mParallel;
            tasks->AddTail(mNameMove);
            tasks->AddTail(mark);
        }
        mMarkMove->SetDuration(15);
        mMarkMove->mEase = -6;
        mMark->SetPosition(data_ov004_02175a6c);
        mMark->Show();
        mFont->SetCenter(data_ov004_02175a6c);
        mFont->Show();
        mNameMove->SetDuration(15);
        mNameMove->mEase = -6;
        mName->SetPosition(data_ov004_02175a54);
        mName->Show();
    } else if (mFrames <= mFrame) {
        mWindow->SetCircle(mCenter, 32);
        if (!mFlip) {
            mName->SetPosition(data_ov004_02175a5c);
        } else {
            mName->SetPosition(data_ov004_02175a64);
        }
        mMark->SetPosition(data_ov004_02175a74);
        mFont->SetCenter(data_ov004_02175a74);
        Exit();
    } else if (mFrames - 15 < mFrame) {
        mWindow->SetCircle(mCenter, mRadius->mValue);
        MPoint name(mNameMove->mValue.x, mNameMove->mValue.y);
        mName->SetPosition(name);
        MPoint mark(mMarkMove->mValue.x, mMarkMove->mValue.y);
        mMark->SetPosition(mark);
        MPoint font(mMarkMove->mValue.x, mMarkMove->mValue.y);
        mFont->SetCenter(font);
    }
    mFrame++;
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

// (Off at the end of the file, where template members are instantiated: the game calls them out of line.)
#pragma auto_inline off
