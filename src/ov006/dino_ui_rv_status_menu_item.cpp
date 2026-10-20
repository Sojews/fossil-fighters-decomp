// DinoUiRvStatusMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_backup.h"
#include "game/dino_ui_rv_status_menu_item_menu.h"
#include "game/dino_ui_rv_status_menu_item.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_ui_field_menu_fraction4.h"
#include "game/dino_ui_rv_information.h"
#include "game/dino_static_init.h"
#include "game/dino_ui_field_menu_cursor.h"
#include "game/dino_ui_parts_tab_icon_menu_item.h"
#include "m/input.h"
#include "m/smart_oam.h"
#include "m/blend.h"

#pragma auto_inline off

// The TU's pooled statics (paths and positions; the positions are set by the static initialization below).
extern "C" struct {
    u8 unk00[0x4];
    const char *mPageLPath;     // 0x04
    const char *mPageRPath;     // 0x08
    u8 unk0C[0x10 - 0xC];
    const char *mBackPath;      // 0x10
    u8 unk14[0x24 - 0x14];
    MPoint mBackPos;            // 0x24
    MPoint mFraction3Pos;       // 0x2C
    MPoint mPageLPos;           // 0x34
    u8 unk3C[0x5C - 0x3C];
    MPoint mPageRPos;           // 0x5C
    MPoint mFraction1Pos;       // 0x64
} data_ov006_021939d4;
extern "C" u32 data_ov006_02193a54[];
extern "C" u32 data_ov006_02193ad4;
extern "C" u32 data_ov006_02199180;

extern const u32 data_0208a82c, data_0208a828, data_0208a824;
extern const u32 data_0208a86c, data_0208a868, data_0208a864;
extern const u32 data_0208a83c, data_0208a838, data_0208a834, data_0208a830;
extern const int data_02089aa0, data_02089a9c, data_02089a98;
// The help areas, and the tabs' frames.
extern MRect data_020b5920, data_020b5930, data_020b5940;
extern MRect data_020b58e0[4];

inline DinoUiRvStatusBackButton::DinoUiRvStatusBackButton(DinoUiRvStatusMenu *owner)
    : DinoUiRvStatusMenuItem(owner, data_ov006_021939d4.mBackPath, data_ov006_021939d4.mBackPos, data_0208a82c) {
    SetKeys(2);
    SetMode(4);
    mMotion->Play(2, 0);
}

inline DinoUiRvStatusPageLButton::DinoUiRvStatusPageLButton(DinoUiRvStatusMenu *owner)
    : DinoUiRvStatusMenuItem(owner, data_ov006_021939d4.mPageLPath, data_ov006_021939d4.mPageLPos, data_0208a828) {
    SetKeys(0x200);
    SetNoFocus(TRUE);
}

inline DinoUiRvStatusPageRButton::DinoUiRvStatusPageRButton(DinoUiRvStatusMenu *owner)
    : DinoUiRvStatusMenuItem(owner, data_ov006_021939d4.mPageRPath, data_ov006_021939d4.mPageRPos, data_0208a824) {
    SetKeys(0x100);
    SetNoFocus(TRUE);
}

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = defs.Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

void DinoUiRvStatusMenu::OnStart() {
    DinoCreatureState creature;
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    gLcd->mMain->mBlend->Reset();
    mKasekiDefs = gDinoResource->LoadKasekiDefs();
    mOam = new MSmartOam(0x14, 0, TRUE);
    mBack = new DinoUiRvStatusBackButton(this);
    AddItem(mBack);
    if (mLastPage > 0) {
        mPageL = new DinoUiRvStatusPageLButton(this);
        AddItem(mPageL);
        mPageR = new DinoUiRvStatusPageRButton(this);
        AddItem(mPageR);
    }
    mUnkE4 = new MParallelTaskSet();
    mParallel.AddTail(mUnkE4);
    mParallel.AddTail(mHelp11C = new DinoHelpDummyItem(GetDisplay(), data_020b5920,
                                                       std::string(gDinoResource->GetText(data_0208a86c))));
    mParallel.AddTail(
        new DinoHelpDummyItem(GetDisplay(), data_020b5930, std::string(gDinoResource->GetText(data_0208a868))));
    mParallel.AddTail(mHelp120 = new DinoHelpDummyItem(GetDisplay(), data_020b5940,
                                                       std::string(gDinoResource->GetText(data_0208a864))));
    // A local static the game fills in on the first call.
    u32 guard = data_ov006_02199180;
    if (!(guard & 1)) {
        DINO_POOL_WORD(data_ov006_02193a54, 0x74) = data_0208a83c;
        DINO_POOL_WORD(data_ov006_02193a54, 0x78) = data_0208a838;
        DINO_POOL_WORD(data_ov006_02193a54, 0x7c) = data_0208a834;
        DINO_POOL_WORD(data_ov006_02193ad4, 0) = data_0208a830;
        DINO_POOL_WORD(data_ov006_02199180, 0) = guard | 1;
    }
    int i = 0;
    const MRect *frames = data_020b58e0;
    const u32 *texts = (const u32 *)&data_ov006_02193a54[0x74 / 4];
    for (; i < 4; i++) {
        mTabItems[i] = new DinoUiPartsTabIconMenuItem(*texts, *frames);
        AddItem(mTabItems[i]);
        texts++;
        frames++;
    }
    AddLink(mTabItems[0], 3, mBack);
    AddLink(mBack, 1, mTabItems[0]);
    RefreshTab();
    if (mUnk10A) {
        mFraction = new DinoUiFieldMenuFraction4(mOam, data_ov006_021939d4.mFraction3Pos, 3);
        mParallel.AddTail(mFraction);
    } else {
        mFraction = new DinoUiFieldMenuFraction4(mOam, data_ov006_021939d4.mFraction1Pos, 1);
        mParallel.AddTail(mFraction);
    }
    mFraction->SetNumerator(mPage + 1);
    mFraction->SetDenominator(mLastPage + 1);
    mCursorTasks = new MParallelTaskSet();
    mCursor = new DinoUiFieldMenuCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(0);
    mCursor->Hide();
    mParallel.AddTail(mCursorTasks);
    mParallel.AddTail(mOam);
    mInfo = new DinoUiRvInformation(mUnk10A);
    mParallel.AddTail(mInfo);
    gDinoBackup->GetCreature(mCreatures[mPage], &creature);
    mInfo->Unk02156ee8(*(DinoCreatureInfo *)&creature);
    FocusItem(mTabItems[0]);
    mTabItems[1]->SetKeys(0x400);
    mTabItems[3]->SetKeys(0x800);
    mUnkF4 = new MParallelTaskSet();
    mParallel.AddTail(mUnkF4);
    mUnkF4->AddTail(gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(data_02089aa0, data_02089a9c));
    mSerial.AddTail(new MTimer(data_02089a98));
}

DinoUiRvStatusMenuItem::DinoUiRvStatusMenuItem(DinoUiRvStatusMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

void DinoUiRvStatusMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ShowPage();
}

void DinoUiRvStatusMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ShowPage();
}

#pragma auto_inline on

DinoUiRvStatusMenuItem::~DinoUiRvStatusMenuItem() {
}

DinoUiRvStatusMenu::~DinoUiRvStatusMenu() {
}

DinoUiRvStatusBackButton::~DinoUiRvStatusBackButton() {
}

DinoUiRvStatusPageLButton::~DinoUiRvStatusPageLButton() {
}

DinoUiRvStatusPageRButton::~DinoUiRvStatusPageRButton() {
}

MRect DinoUiRvStatusMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiRvStatusMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiRvStatusMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiRvStatusMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiRvStatusMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiRvStatusBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiRvStatusBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiRvStatusPageRButton::OnDecide() {
    mOwner->PageRight();
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

void DinoUiRvStatusPageLButton::OnDecide() {
    mOwner->PageLeft();
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

void DinoUiRvStatusMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 4; i++) {
        if (item == mTabItems[i]) {
            SelectTab(i);
        }
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
#pragma auto_inline off
template class MCallTask0<DinoUiRvStatusMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiRvStatusMenu *obj, void (DinoHelpBase::*func)());
template class MCallTask1<DinoUiRvStatusMenu, DinoUiRvStatusMenu, u8>;
template MTask *MCreateCallTask(DinoUiRvStatusMenu *obj, void (DinoUiRvStatusMenu::*func)(u8), u8 a0);
#pragma auto_inline reset

// Where the cursor sits on each tab.
extern MPoint data_020b5890[4];

void DinoUiRvStatusMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (int i = 0; i < 4; i++) {
        if (item == mTabItems[i]) {
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

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoUiRvStatusMenu::*SetInfoPage_00)(u8);
} data_ov006_021939f0;
extern "C" struct {
    void (DinoHelpBase::*Enable_00)();
} data_ov006_02193a48;

extern "C" struct {
    void (DinoHelpBase::*Enable_00)();
} data_ov006_02193a10;

// Shows the page's creature, with help back once the information has.
void DinoUiRvStatusMenu::ShowPage() {
    mFraction->SetNumerator(mPage + 1);
    DinoCreatureState creature;
    gDinoBackup->GetCreature(mCreatures[mPage], &creature);
    mInfo->Unk02156ee8(*(DinoCreatureInfo *)&creature);
    GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial),
                         MCreateCallTask(this, data_ov006_02193a10.Enable_00));
    Disable();
    RefreshTab();
}

void DinoUiRvStatusMenu::SelectTab(u8 tab) {
    if (tab > 3 || mTab == tab) {
        return;
    }
    mTab = tab;
    DinoSoundPlaySe(0x94, 0);
    GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov006_021939f0.SetInfoPage_00, tab));
    mTabItems[tab % 4]->SetKeys(0);
    mTabItems[(tab + 1) % 4]->SetKeys(0x400);
    mTabItems[(tab + 2) % 4]->SetKeys(0);
    mTabItems[(tab + 3) % 4]->SetKeys(0x800);
    Disable();
    RefreshTab();
}

void DinoUiRvStatusMenu::SetInfoPage(u8 page) {
    mInfo->SetPage(page);
    if (page == 3) {
        mFraction->SetPriority(3);
    } else {
        mFraction->SetPriority(2);
    }
    GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial),
                         MCreateCallTask(this, data_ov006_02193a48.Enable_00));
    Disable();
}

extern const int data_02089a8c, data_02089a90, data_02089a94;

void DinoUiRvStatusMenu::OnBackDecided() {
    if (mResult != NULL) {
        *mResult = mPage;
    }
    GetSerial()->AddTail(new MTimer(data_02089a94), MTaskSetWaitEmptyTask::Create(mUnkF4),
                         gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(data_02089a90, data_02089a8c),
                         mInfo->CreateWaitModelTask(), MTaskExitTransfer::Create(&(MTask &)*this));
}

// The file's static initialization (see dino_static_init.h).
static struct DinoUiRvStatusMenuItemStaticInit {
    DinoUiRvStatusMenuItemStaticInit() {
        DINO_POOL_WORD(data_ov006_021939d4, 0x6c) = 128;
        DINO_POOL_WORD(data_ov006_021939d4, 0x70) = 176;
        DINO_POOL_WORD(data_ov006_021939d4, 0x44) = 8;
        DINO_POOL_WORD(data_ov006_021939d4, 0x48) = 150;
        DINO_POOL_WORD(data_ov006_021939d4, 0x14) = 64;
        DINO_POOL_WORD(data_ov006_021939d4, 0x18) = 112;
        DINO_POOL_WORD(data_ov006_02193a54, 0xc) = 19;
        DINO_POOL_WORD(data_ov006_02193a54, 0x10) = 179;
        DINO_POOL_WORD(data_ov006_021939d4, 0x54) = 27;
        DINO_POOL_WORD(data_ov006_021939d4, 0x58) = 0;
        DINO_POOL_WORD(data_ov006_021939d4, 0x34) = 128;
        DINO_POOL_WORD(data_ov006_021939d4, 0x38) = 96;
        DINO_POOL_WORD(data_ov006_021939d4, 0x5c) = 128;
        DINO_POOL_WORD(data_ov006_021939d4, 0x60) = 96;
        DINO_POOL_WORD(data_ov006_021939d4, 0x24) = 128;
        DINO_POOL_WORD(data_ov006_021939d4, 0x28) = 98;
        DINO_POOL_WORD(data_ov006_021939d4, 0x64) = 246;
        DINO_POOL_WORD(data_ov006_021939d4, 0x68) = 24;
        DINO_POOL_WORD(data_ov006_021939d4, 0x2c) = 230;
        DINO_POOL_WORD(data_ov006_021939d4, 0x30) = 24;
    }
} sDinoUiRvStatusMenuItemStaticInit;

// (Off at the end of the file, where template members are instantiated: the game calls them out of line.)
#pragma auto_inline off
