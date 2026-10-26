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
#include "game/dino_static_init.h"

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

extern "C" const int data_020899c0; // frames before the keys are taken
extern "C" const int data_020899cc; // the fade in's frames
extern "C" const int data_020899e4;
extern "C" const u32 data_0208abc8; // "back"
extern "C" const u32 data_0208ac48; // the bottom screen's help
extern "C" const u32 data_0208ac4c; // the top screen's help
extern "C" MPoint data_ov004_02175ba4[];      // the items' places with up to five places open
extern "C" MPoint data_ov004_02175c1c[];      // with more
extern "C" const char *data_ov004_02175bcc[]; // per place: its item's motion
extern "C" const char *data_ov004_021759d8[];
extern "C" const MPoint data_ov004_02175a4c;  // the back button's place
extern "C" const char data_ov004_0217620c[];  // the title picture

inline DinoMapBackButton::DinoMapBackButton(DinoUiIslandMapMenu *owner, u32 textId, MSmartOam *oam) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(data_ov004_021759d8[5]), oam, 2, 1);
    mMotion->Play(2, 0);
    mMotion->SetPosition(data_ov004_02175a4c);
    mMotion->SetPriority(3);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    DinoHelpItem::Enable();
    SetMode(0);
}

static inline void DinoCenterOffset(MScreen *screen) {
    screen->SetOffset(MPoint(0x80, 0xA0));
}
// Reads all of an archive file.
static inline void DinoReadAll(const MArchiveFile &file, void *dest) {
    const volatile MArchiveFile &f = file;
    u32 size = f.archive->mEntries[f.index].size;
    f.archive->Read(f.index, 0, dest, size, NULL);
}

// The open places' items (laid out in one row, or two with more than five) and the back button, their links, the
// title, the map and its spotlight, and the help areas.
// NONMATCHING: the map offset's two dead MPoint temporaries trade stack slots (and the item ctor that follows it
// in the game makes objdiff's report read its call as a branch inside this function).
void DinoUiIslandMapMenu::OnStart() {
    mUnk150 = new MParallelTaskSet;
    mParallel.AddTail(mUnk150);
    mSpotTasks = new MParallelTaskSet;
    mParallel.AddTail(mSpotTasks);
    mMoveTasks = new MParallelTaskSet;
    mParallel.AddTail(mMoveTasks);
    mItemOam = new MSmartOam(0x55, 0, TRUE);
    mParallel.AddTail(mItemOam);
    mUnk150->AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(data_020899e4, data_020899cc),
                     gLcd->mSub->mBright->CreateFadeInFromBlack(data_020899e4, data_020899cc));
    mParallel.AddTail(new GuidMan(mMoveTasks, this));
    u32 open = 0;
    for (int i = 0; i < 10; i++) {
        if ((1 << i) & mUnk144) {
            open++;
        }
    }
    MPoint *place;
    if (open <= 5) {
        place = data_ov004_02175ba4;
    } else {
        place = data_ov004_02175c1c;
    }
    int count, i;
    int order[10];
    i = count = 0;
    int *next = order;
    for (; i < 10; i++) {
        if (mUnk144 & (1 << data_02089adc[i])) {
            AddItem(mItems[i] = new DinoUiIslandMapMenuItem(this, std::string(data_ov004_02175bcc[data_02089adc[i]]),
                                                            *place, 0, mItemOam));
            place++;
            *next++ = i;
            count++;
        } else {
            mItems[i] = NULL;
        }
    }
    DinoMapBackButton *back = new DinoMapBackButton(this, data_0208abc8, mItemOam);
    AddItem(back);
    mBackButton = back;
    SetLinkFallback(0);
    for (int i = 0; i < count; i++) {
        if (i != 0 && i != 5) {
            AddLink(mItems[order[i]], 4, mItems[order[(i - 1 + count) % count]]);
        }
        if (i != 4 && i != count - 1) {
            AddLink(mItems[order[i]], 2, mItems[order[(i + 1) % count]]);
        }
    }
    if (count > 5) {
        AddLink(mItems[order[0]], 4, mItems[order[4]]);
        AddLink(mItems[order[4]], 2, mItems[order[0]]);
        if (count > 5) {
            AddLink(mItems[order[5]], 4, back);
            AddLink(mItems[order[count - 1]], 2, back);
        }
        AddLink(back, 4, mItems[order[count - 1]]);
        AddLink(back, 2, mItems[order[5]]);
        for (int i = 0; i < 5; i++) {
            if (i + 5 < count) {
                AddLink(mItems[order[i]], 3, mItems[order[i + 5]]);
                AddLink(mItems[order[i]], 1, mItems[order[i + 5]]);
                AddLink(mItems[order[i + 5]], 3, mItems[order[i]]);
                AddLink(mItems[order[i + 5]], 1, mItems[order[i]]);
            } else {
                AddLink(mItems[order[i]], 3, back);
                AddLink(mItems[order[i]], 1, back);
            }
        }
        AddLink(back, 3, mItems[order[4]]);
        AddLink(back, 1, mItems[order[4]]);
    } else {
        AddLink(mItems[order[0]], 4, back);
        AddLink(mItems[order[count - 1]], 2, back);
        AddLink(back, 4, mItems[order[count - 1]]);
        AddLink(back, 2, mItems[order[0]]);
    }
    MTextPicture *title = new MTextPicture(std::string(data_ov004_0217620c), TRUE, false);
    title->Show();
    title->SetPriority(3);
    mParallel.AddTail(title);
    gCurrentDisplay = gLcd->mMain;
    mOam = new MSmartOam(0x40, 0, TRUE);
    mParallel.AddTail(mOam);
    mBitmap = gLcd->GetTopDisplay()->mBg->CreateIndexBitmap(3, 1);
    MPal *pal = gLcd->GetTopDisplay()->mBg->mPal;
    mPalette = pal->Alloc(0x10, mMap.GetPalette(), 1);
    DinoReadAll(mMap.GetChrFile(), (void *)mBitmap->mBase);
    mBitmap->SetCenter(0x100, 0x100);
    DinoCenterOffset(mBitmap);
    mBitmap->SetScale(MFx32(1), MFx32(1));
    mBitmap->mBgCntBits.priority = 3;
    mSerial.AddTail(mBitmap->CreateShowTransfer());
    mScroll = MPoint(0x80, 0xA0);
    SetSilentFocus(TRUE);
    gLcd->mMain->mBlend->SetMode(3);
    gLcd->mMain->mBlend->SetBrightness(8);
    gLcd->mMain->mBlend->SetTarget1(0xF);
    mWindow = new MRasterWindow(-1, NULL, TRUE);
    mWindow->SetTarget(0x3F);
    mWindow->SetObjTarget(0x3F);
    mWindow->SetBlendEnabled(FALSE);
    mWindow->SetObjBlendEnabled(TRUE);
    mWindow->Clear();
    mWindow->SetCircle(MPoint(0x80, 0x60), 0x20);
    mWindow->Show();
    mParallel.AddTail(mWindow);
    gCurrentDisplay = gLcd->mSub;
    FocusNearestItem(MPoint(0, 0));
    GetInput()->SetRepeat(0xF0, 0x1E, 10);
    DinoHelpDummyItem *help = new DinoHelpDummyItem(gLcd->GetBottomDisplay(), MRect(0, 8, 0x100, 0xB7),
                                                    std::string(gDinoResource->GetText(data_0208ac48)));
    help->Enable();
    mParallel.AddTail(help);
    help = new DinoHelpDummyItem(gLcd->GetTopDisplay(), MRect(0, 8, 0x100, 0xB7),
                                 std::string(gDinoResource->GetText(data_0208ac4c)));
    help->Enable();
    mParallel.AddTail(help);
    mSerial.AddTail(new MTimer(data_020899c0));
    IslandMapKeyCheckTask *keys = new IslandMapKeyCheckTask(this);
    keys->Suspend();
    mParallel.AddTail(keys);
    mSerial.AddTail(MTaskRunTransfer::Create(keys));
    mKeyCheck = keys;
}

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
    mKeyCheck->Exit();
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

// The file's static initialization (see dino_static_init.h): layout points, path tables copied from the pooled paths,
// the map spots, and text ids copied from main's constants.
// NONMATCHING: 88%, the same stores in the same order; the game keeps the anchors and constants in other registers.
extern "C" u32 data_0208abfc[];
extern "C" u32 data_0208ac00[];
extern "C" u32 data_0208ac04[];
extern "C" u32 data_0208ac08[];
extern "C" u32 data_0208ac0c[];
extern "C" u32 data_0208ac10[];
extern "C" u32 data_0208ac14[];
extern "C" u32 data_0208ac18[];
extern "C" u32 data_0208ac1c[];
extern "C" u32 data_0208ac20[];
extern "C" u32 data_ov004_02175b18[];
extern "C" u32 data_ov004_02175b58[];
extern "C" u32 data_ov004_02175bac[];
extern "C" u32 data_ov004_02175bb4[];
extern "C" u32 data_ov004_02175bbc[];
extern "C" u32 data_ov004_02175bc4[];
extern "C" u32 data_ov004_02175bd8[];
extern "C" u32 data_ov004_02175c24[];
extern "C" u32 data_ov004_02175c2c[];
extern "C" u32 data_ov004_02175c34[];
extern "C" u32 data_ov004_02175c3c[];
extern "C" u32 data_ov004_02175c44[];
extern "C" u32 data_ov004_02175c4c[];
extern "C" u32 data_ov004_02175c54[];
extern "C" u32 data_ov004_02175c58[];
extern "C" u32 data_ov004_02175c5c[];
extern "C" u32 data_ov004_02175c64[];
static struct DinoUiIslandMapStaticInit {
    DinoUiIslandMapStaticInit() {
        DINO_POOL_WORD(data_ov004_021759d8, 0x74) = 0x80;
        DINO_POOL_WORD(data_ov004_021759d8, 0x78) = 0x60;
        DINO_POOL_WORD(data_ov004_02175b58, 0x74) = DINO_POOL_WORD(data_ov004_021759d8, 0x40);
        DINO_POOL_WORD(data_ov004_02175b58, 0x78) = DINO_POOL_WORD(data_ov004_021759d8, 0x2C);
        DINO_POOL_WORD(data_ov004_02175b58, 0x7C) = DINO_POOL_WORD(data_ov004_021759d8, 0x34);
        volatile s32 *p = (volatile s32 *) &data_ov004_02175bd8;
        p[0] = DINO_POOL_WORD(data_ov004_021759d8, 0x1C);
        p[1] = DINO_POOL_WORD(data_ov004_021759d8, 0x50);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x8) = DINO_POOL_WORD(data_ov004_021759d8, 0x28);
        DINO_POOL_WORD(data_ov004_02175bd8, 0xC) = DINO_POOL_WORD(data_ov004_021759d8, 0x20);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x10) = DINO_POOL_WORD(data_ov004_021759d8, 0x4);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x14) = DINO_POOL_WORD(data_ov004_021759d8, 0x58);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x18) = DINO_POOL_WORD(data_ov004_021759d8, 0x54);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x1C) = DINO_POOL_WORD(data_ov004_021759d8, 0x48);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x20) = DINO_POOL_WORD(data_ov004_021759d8, 0x18);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x24) = DINO_POOL_WORD(data_ov004_021759d8, 0x38);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x28) = DINO_POOL_WORD(data_ov004_021759d8, 0x44);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x2C) = DINO_POOL_WORD(data_ov004_021759d8, 0x8);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x30) = DINO_POOL_WORD(data_ov004_021759d8, 0x4C);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x34) = DINO_POOL_WORD(data_ov004_021759d8, 0x10);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x38) = DINO_POOL_WORD(data_ov004_021759d8, 0x24);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x3C) = DINO_POOL_WORD(data_ov004_021759d8, 0x30);
        DINO_POOL_WORD(data_ov004_02175bd8, 0x40) = DINO_POOL_WORD(data_ov004_021759d8, 0x3C);
        DINO_POOL_WORD(data_ov004_021759d8, 0x7C) = -0x40;
        p = (volatile s32 *) &data_ov004_02175a58;
        p[0] = 0x70;
        p[1] = 0x3C;
        DINO_POOL_WORD(data_ov004_02175a58, 0x8) = 0x70;
        DINO_POOL_WORD(data_ov004_021759d8, 0x6C) = 0x140;
        DINO_POOL_WORD(data_ov004_021759d8, 0x70) = 0x70;
        DINO_POOL_WORD(data_ov004_02175a58, 0xC) = 0xCC;
        DINO_POOL_WORD(data_ov004_02175a58, 0x10) = 0x70;
        DINO_POOL_WORD(data_ov004_02175a58, 0x14) = 0x80;
        DINO_POOL_WORD(data_ov004_02175a58, 0x18) = 0xB0;
        DINO_POOL_WORD(data_ov004_02175a58, 0x1C) = 0x80;
        DINO_POOL_WORD(data_ov004_02175a58, 0x20) = 0x9C;
        DINO_POOL_WORD(data_ov004_02175bd8, 0x44) = 0x44;
        DINO_POOL_WORD(data_ov004_02175bd8, 0x48) = 0x1B;
        DINO_POOL_WORD(data_ov004_02175c24, 0x0) = 0x44;
        DINO_POOL_WORD(data_ov004_02175c24, 0x4) = 0x40;
        DINO_POOL_WORD(data_ov004_02175c2c, 0x0) = 0x44;
        DINO_POOL_WORD(data_ov004_02175c2c, 0x4) = 0x65;
        DINO_POOL_WORD(data_ov004_02175c34, 0x0) = 0x44;
        DINO_POOL_WORD(data_ov004_02175c34, 0x4) = 0x8A;
        DINO_POOL_WORD(data_ov004_02175c3c, 0x0) = 0x44;
        DINO_POOL_WORD(data_ov004_02175c3c, 0x4) = 0xAF;
        DINO_POOL_WORD(data_ov004_02175c44, 0x0) = 0xBC;
        DINO_POOL_WORD(data_ov004_02175c44, 0x4) = 0x2D;
        DINO_POOL_WORD(data_ov004_02175c4c, 0x0) = 0xBC;
        DINO_POOL_WORD(data_ov004_02175c4c, 0x4) = 0x52;
        DINO_POOL_WORD(data_ov004_02175c54, 0x0) = 0xBC;
        DINO_POOL_WORD(data_ov004_02175c54, 0x4) = 0x77;
        DINO_POOL_WORD(data_ov004_02175c5c, 0x0) = 0xBC;
        DINO_POOL_WORD(data_ov004_02175c5c, 0x4) = 0x9C;
        DINO_POOL_WORD(data_ov004_02175c64, 0x0) = 0xBC;
        DINO_POOL_WORD(data_ov004_02175c64, 0x4) = 0xC1;
        DINO_POOL_WORD(data_ov004_02175b58, 0x4C) = 0x80;
        DINO_POOL_WORD(data_ov004_02175b58, 0x50) = 0x1B;
        DINO_POOL_WORD(data_ov004_02175bac, 0x0) = 0x80;
        DINO_POOL_WORD(data_ov004_02175bac, 0x4) = 0x40;
        DINO_POOL_WORD(data_ov004_02175bb4, 0x0) = 0x80;
        DINO_POOL_WORD(data_ov004_02175bb4, 0x4) = 0x65;
        DINO_POOL_WORD(data_ov004_02175bbc, 0x0) = 0x80;
        DINO_POOL_WORD(data_ov004_02175bbc, 0x4) = 0x8A;
        DINO_POOL_WORD(data_ov004_02175bc4, 0x0) = 0x80;
        DINO_POOL_WORD(data_ov004_02175bc4, 0x4) = 0xAF;
        DINO_POOL_WORD(data_ov004_02175c58, 0x14) = -0xD5;
        DINO_POOL_WORD(data_ov004_02175c58, 0x18) = -0xB7;
        DINO_POOL_WORD(data_ov004_02175c58, 0x1C) = -0x69;
        DINO_POOL_WORD(data_ov004_02175c58, 0x20) = -0xCE;
        DINO_POOL_WORD(data_ov004_02175c58, 0x24) = -0xB3;
        DINO_POOL_WORD(data_ov004_02175c58, 0x28) = -0x6C;
        DINO_POOL_WORD(data_ov004_02175c58, 0x2C) = -0x9F;
        DINO_POOL_WORD(data_ov004_02175c58, 0x30) = -0xAE;
        DINO_POOL_WORD(data_ov004_02175c58, 0x34) = 0xD;
        DINO_POOL_WORD(data_ov004_02175c58, 0x38) = -0x48;
        DINO_POOL_WORD(data_ov004_02175c58, 0x3C) = -0x79;
        DINO_POOL_WORD(data_ov004_02175c58, 0x40) = -0x61;
        DINO_POOL_WORD(data_ov004_02175c58, 0x44) = 0x80;
        DINO_POOL_WORD(data_ov004_02175c58, 0x48) = 0x40;
        DINO_POOL_WORD(data_ov004_02175c58, 0x4C) = -0xD4;
        DINO_POOL_WORD(data_ov004_02175c58, 0x50) = -0x83;
        DINO_POOL_WORD(data_ov004_02175c58, 0x54) = 0x46;
        DINO_POOL_WORD(data_ov004_02175c58, 0x58) = -0x170;
        DINO_POOL_WORD(data_ov004_02175c58, 0x5C) = -0x10F;
        DINO_POOL_WORD(data_ov004_02175c58, 0x60) = 0xA;
        DINO_POOL_WORD_16(data_ov004_02175b18, 0x38) = DINO_POOL_WORD(data_0208ac20, 0x0);
        DINO_POOL_WORD_16(data_ov004_02175b18, 0x3A) = DINO_POOL_WORD(data_0208ac1c, 0x0);
        DINO_POOL_WORD_16(data_ov004_02175b18, 0x3C) = DINO_POOL_WORD(data_0208ac18, 0x0);
        DINO_POOL_WORD_16(data_ov004_02175b18, 0x3E) = DINO_POOL_WORD(data_0208ac14, 0x0);
        DINO_POOL_WORD_16(data_ov004_02175b58, 0x0) = DINO_POOL_WORD(data_0208ac10, 0x0);
        DINO_POOL_WORD_16(data_ov004_02175b58, 0x2) = DINO_POOL_WORD(data_0208ac0c, 0x0);
        DINO_POOL_WORD_16(data_ov004_02175b58, 0x4) = DINO_POOL_WORD(data_0208ac08, 0x0);
        DINO_POOL_WORD_16(data_ov004_02175b58, 0x6) = DINO_POOL_WORD(data_0208ac04, 0x0);
        DINO_POOL_WORD_16(data_ov004_02175b58, 0x8) = DINO_POOL_WORD(data_0208ac00, 0x0);
        DINO_POOL_WORD_16(data_ov004_02175b58, 0xA) = DINO_POOL_WORD(data_0208abfc, 0x0);
    }
} sDinoUiIslandMapStaticInit;
