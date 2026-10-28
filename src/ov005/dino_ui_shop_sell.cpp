// DinoUiShopSell.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma ipa file
#pragma auto_inline on

#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_ui_fossil_entry.h"

#include "game/dino_ui3_parts_rock_name.h"

#include "game/dino_ui3_parts_cursor.h"

#include "m/input.h"

#include "game/dino_ui3_parts_kaseki_button.h"
#include "game/dino_ui2_parts_kaseki_button.h"

#include "game/dino_backup.h"
#include "game/dino_ui3_parts_fraction.h"
#include "nitro/mi.h"

#include "game/dino_ui3_parts_money.h"

#include "m/obj.h"
#include "game/dino_ui_shop_sell_menu.h"
#include "game/dino_ui_shop_sell.h"
#include "game/dino_ui_shop_sell_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_static_init.h"
#include "game/dino_guide_message.h"
#include "game/dino_resource.h"
#include "game/dino_ui_common.h"
#include "m/picture.h"

DinoUiShopSellMenu::DinoUiShopSellMenu() : MMenu(NULL, 0), mPage(0), mLastPage(0), mCount(0), mSelected(0xFF) {
    for (int i = 0; i < 0x40; i++) {
        mFlags[i] = 0;
    }
}

DinoUiShopSell::~DinoUiShopSell() {
}

DinoUiShopSellMenuItem::~DinoUiShopSellMenuItem() {
}

DinoUiShopSellMenu::~DinoUiShopSellMenu() {
}

DinoUiShopSellBackButton::~DinoUiShopSellBackButton() {
}

DinoUiShopSellPageLButton::~DinoUiShopSellPageLButton() {
}

DinoUiShopSellPageRButton::~DinoUiShopSellPageRButton() {
}

DinoUiShopSellSellButton::~DinoUiShopSellSellButton() {
}

void DinoUiShopSell::Idle() {
}

MRect DinoUiShopSellMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiShopSellMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiShopSellMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiShopSellMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiShopSellMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiShopSellBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiShopSellBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiShopSellPageRButton::OnDecide() {
    if (mOwner->PageRight()) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
        DinoSoundPlaySe(0x93, 0);
    }
}

void DinoUiShopSellPageLButton::OnDecide() {
    if (mOwner->PageLeft()) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
        DinoSoundPlaySe(0x93, 0);
    }
}

#pragma auto_inline off
void DinoUiShopSellMenu::OnBackDecided() {
    MTask *fade;
    MTask *exit;
    MTask &self = *this;
    exit = MTaskExitTransfer::Create(&self);
    fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(fade);
    serial->AddTail(exit);
}
#pragma auto_inline on

// Pooled file statics: the button corners.
struct DinoUiShopSellStatics {
    const char *mPaths[2];  // 0x00: the path table's last two ([1] page right)
    MPoint mTopLeft;     // 0x08
    MPoint mSellPos;     // 0x10
    u8 unk18[0x28 - 0x18];
    MPoint mColumn;      // 0x28: from a page button to the next in its row
    MPoint mRow;         // 0x30: to the next row
    u8 unk38[0x40 - 0x38];
    MPoint mPageRPos;    // 0x40
    u8 unk48[0x50 - 0x48];
    MPoint mFirst;       // 0x50: the page's first button
    MPoint mBottomRight; // 0x58
    MPoint mBackPos;     // 0x60
    u8 unk68[0x70 - 0x68];
    MPoint mPageLPos;    // 0x70
};
extern "C" DinoUiShopSellStatics data_ov005_02165ff8;

// The buttons' graphics (the path table, which runs on into the statics above).
extern "C" const char *const data_ov005_02165f78[];
extern "C" const MPoint data_ov005_02166018, data_ov005_02166040;  // the fraction's and the rock name's places
extern "C" const char data_ov005_02166ab0[];    // the background
extern const MRect data_020b5838, data_020b5848; // help rects
extern "C" const u32 data_0208a5f0, data_0208a5f4, data_0208a5f8, data_0208a5fc, data_0208a600, data_0208a604,
    data_0208a608, data_0208a938, data_0208aacc;

inline DinoUiShopSellBackButton::DinoUiShopSellBackButton(DinoUiShopSellMenu *owner)
    : DinoUiShopSellMenuItem(owner, data_ov005_02165f78[17], data_ov005_02165ff8.mBackPos, data_0208a5f4) {
    mMotion->Play(2, 0);
}

inline DinoUiShopSellPageLButton::DinoUiShopSellPageLButton(DinoUiShopSellMenu *owner)
    : DinoUiShopSellMenuItem(owner, data_ov005_02165f78[21], data_ov005_02165ff8.mPageLPos, data_0208aacc) {
    SetKeys(0x200);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiShopSellPageRButton::DinoUiShopSellPageRButton(DinoUiShopSellMenu *owner)
    : DinoUiShopSellMenuItem(owner, data_ov005_02165ff8.mPaths[1], data_ov005_02165ff8.mPageRPos, data_0208aacc) {
    SetKeys(0x100);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiShopSellSellButton::DinoUiShopSellSellButton(DinoUiShopSellMenu *owner)
    : DinoUiShopSellMenuItem(owner, data_ov005_02165f78[23], data_ov005_02165ff8.mSellPos, data_0208a604) {
}

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = defs.Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

// Lists the containers' fossils (a page per container) and builds the page, the help, the cursor, the page
// fraction, the rock name and the money.
#pragma opt_common_subs off
void DinoUiShopSellMenu::OnStart() {
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    DinoDimScreen();
    mKasekiDefs = gDinoResource->LoadKasekiDefs();
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, FALSE);
    mLastPage = player.containerNum - 1;
    mParallel.AddTail(mBackground = new MTextPicture(std::string(data_ov005_02166ab0), TRUE, false));
    mBackground->Show();
    mBackground->SetPriority(2);
    mOam = new MSmartOam(0x50, 0, TRUE);
    DinoHelpDummyItem *top, *list, *money, *total, *rocks;
    mParallel.AddTail(top = new DinoHelpDummyItem(GetDisplay(), MRect(0x1E, 0xC, 0xE2, 0x6C),
                                                  std::string(gDinoResource->GetText(data_0208a608))));
    mParallel.AddTail(list = new DinoHelpDummyItem(GetDisplay(), MRect(0x13, 0x90, 0xF1, 0x9F),
                                                   std::string(gDinoResource->GetText(data_0208a600))));
    mParallel.AddTail(money = new DinoHelpDummyItem(GetDisplay(), data_020b5848,
                                                    std::string(gDinoResource->GetText(data_0208a5fc))));
    mParallel.AddTail(total = new DinoHelpDummyItem(GetDisplay(), data_020b5838,
                                                    std::string(gDinoResource->GetText(data_0208a5f8))));
    mParallel.AddTail(rocks = new DinoHelpDummyItem(GetDisplay(), MRect(0xE9, 0x5D, 0xF7, 0x6E),
                                                    std::string(gDinoResource->GetText(data_0208a5f0))));
    mBack = new DinoUiShopSellBackButton(this);
    AddItem(mBack);
    gDinoBackup->CompactUnk98();
    LoadFossils();
    if (mLastPage > 0) {
        mPageL = new DinoUiShopSellPageLButton(this);
        AddItem(mPageL);
        mPageR = new DinoUiShopSellPageRButton(this);
        AddItem(mPageR);
    }
    mLinkFrom = new DinoUiShopSellSellButton(this);
    AddItem(mLinkFrom);
    AddEntry((u32)(DinoHelpItem *)mLinkFrom, 1, (u32)rocks);
    AddEntry((u32)list, 3, 0);
    AddEntry((u32)list, 1, 0);
    AddEntry((u32)money, 4, (u32)list);
    AddEntry((u32)money, 1, (u32)total);
    AddEntry((u32)total, 2, 0);
    AddEntry((u32)total, 3, (u32)money);
    AddEntry((u32)(DinoHelpItem *)mBack, 4, (u32)list);
    AddEntry((u32)rocks, 3, (u32)top);
    AddEntry((u32)money, 1, (u32)total);
    AddEntry((u32)total, 3, (u32)money);
    AddEntry((u32)(DinoHelpItem *)mBack, 4, (u32)list);
    mFraction = new DinoUi3PartsFraction(mOam, data_ov005_02166018);
    mParallel.AddTail(mFraction);
    mFraction->SetNumerator(mPage + 1);
    mFraction->SetDenominator(player.containerNum);
    RefreshPage2();
    mCursorTasks = new MParallelTaskSet();
    mCursor = new DinoUi3PartsCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(1);
    mParallel.AddTail(mCursorTasks);
    mParallel.AddTail(mOam);
    mRockName = new DinoUi3PartsRockName(data_ov005_02166040);
    mParallel.AddTail(mRockName);
    mMoney = new DinoUi3PartsMoney();
    mParallel.AddTail(mMoney);
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208a938))));
    AddLink(mLinkFrom, 3, NULL);
    AddLink(mLinkFrom, 1, NULL);
    AddLink(mLinkFrom, 2, mBack);
    AddLink(mBack, 4, mLinkFrom);
    AddLink(mBack, 3, NULL);
    if (mItems.size() == 0) {
        FocusItem(mBack);
    } else {
        FocusItem((MMenuItem *)mItems[0]);
    }
    mSerial.AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(30, 0));
}
#pragma opt_common_subs reset

MRect DinoUiShopSellSellButton::GetFrame() {
    return MRect(data_ov005_02165ff8.mTopLeft, data_ov005_02165ff8.mBottomRight);
}

void DinoUiShopSellSellButton::OnDecide() {
    if (mOwner->Sell()) {
        DinoSoundPlaySe(0x98, 0);
        mMotion->Play(3, 1);
        MTask *play = mMotion->CreatePlayTransfer(2, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
    }
}

void DinoUiShopSellSellButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        if (mOwner->mCount) {
            mMotion->Play(2, 0);
        } else {
            mMotion->Play(1, 0);
        }
    } else {
        mMotion->Play(1, 0);
        mSerial.Clear();
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiShopSellMenu, DinoUiShopSellMenu>;

#pragma auto_inline off
BOOL DinoUiShopSellMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    if (mFocused != mLinkFrom) {
        FocusNearestItem(mCursor->GetPosition());
    }
    return TRUE;
}
#pragma auto_inline on

#pragma auto_inline off
BOOL DinoUiShopSellMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    if (mFocused != mLinkFrom) {
        FocusNearestItem(mCursor->GetPosition());
    }
    return TRUE;
}
#pragma auto_inline on

#pragma auto_inline off

DinoUiShopSellMenuItem::DinoUiShopSellMenuItem(DinoUiShopSellMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(1);
    mMotion->Show();
    mMotion->GetUnit()->SetMode(1);
    mParallel.AddTail(mMotion);
}

// Lists the fossils in the containers.
void DinoUiShopSellMenu::LoadFossils() {
    mFossils.clear();
    for (int i = 0; i < 0x40; i++) {
        DinoBackupEntry entry;
        gDinoBackup->GetUnk98(i, &entry);
        if (entry.id) {
            mFossils.push_back(DinoGetFossilEntry(&entry, mKasekiDefs));
        }
    }
    if (mFossils.size() == 0) {
        mLastPage = 0;
    } else {
        mLastPage = (mFossils.size() - 1) / 8;
    }
}

void DinoUiShopSellMenu::ClearItems() {
    RemoveLink(mLinkFrom, 4);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
}

DinoUiShopSell::DinoUiShopSell() : MProcess(NULL) {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    mSerial.AddTail(new DinoUiShopSellMenu());
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

#pragma auto_inline off

template MTask *MCreateCallTask(DinoUiShopSellMenu *obj, void (DinoUiShopSellMenu::*func)());

u8 DinoUiShopSellMenu::GetFlag(int index) {
    return mFlags[index];
}

extern const u32 data_0208a1f4;

// Makes the page's buttons (blank past the last fossil) and links them in a grid, the top row to the back button.
// (Pushed through a reference, which gives the game's order for push_back's byte temps under -ipa file.)
static inline void DinoPushItem(std::vector<void *> &v, void *const &x) {
    v.push_back(x);
}

void DinoUiShopSellMenu::RefreshPage2() {
    mItems.clear();
    for (int i = 0; i < 8; i++) {
        DinoUi3PartsKasekiButton *button;
        if (mFossils.size() > i + mPage * 8) {
            button = new DinoUi3PartsKasekiButton(
                mOam,
                MPoint(data_ov005_02165ff8.mFirst.x + data_ov005_02165ff8.mColumn.x * (i % 4) +
                           data_ov005_02165ff8.mRow.x * (i / 4),
                       data_ov005_02165ff8.mFirst.y + data_ov005_02165ff8.mColumn.y * (i % 4) +
                           data_ov005_02165ff8.mRow.y * (i / 4)),
                (const DinoUiKasekiIcon &)mFossils[i + mPage * 8], GetFlag(i + mPage * 8));
        } else {
            button = new DinoUi3PartsKasekiButton(
                mOam,
                MPoint(data_ov005_02165ff8.mFirst.x + data_ov005_02165ff8.mColumn.x * (i % 4) +
                           data_ov005_02165ff8.mRow.x * (i / 4),
                       data_ov005_02165ff8.mFirst.y + data_ov005_02165ff8.mColumn.y * (i % 4) +
                           data_ov005_02165ff8.mRow.y * (i / 4)),
                (const DinoUiKasekiIcon &)DinoUi2GetEmptyKasekiInfo(), 0);
            button->HideMotion();
        }
        DinoPushItem(mItems, button);
        AddItem(button);
    }
    DinoUi2LinkKasekiGrid(this, (std::vector<MMenuItem *> &)mItems, mLinkFrom);
    AddLink((MMenuItem *)mItems[0], 4, mBack);
    AddLink((MMenuItem *)mItems[1], 4, mBack);
    AddLink((MMenuItem *)mItems[2], 4, mBack);
    AddLink((MMenuItem *)mItems[3], 4, mBack);
    AddLink(mLinkFrom, 4, (MMenuItem *)mItems[4]);
}

// What the fossil sells for: cleaned ones by their score (100 is full price), uncleaned ones by their own price.
u32 DinoUiShopSellMenu::GetPrice(int index) {
    DinoBackupEntry entry;
    gDinoBackup->GetUnk98(index, &entry);
    u32 price = GetKasekiDef(entry.id)->sellPrice;
    if (!entry.active) {
        if (entry.unk10_0 == 1) {
            price = data_0208a1f4;
        } else if (entry.type == 1) {
            price = GetKasekiDef(entry.id)->unk20;
        }
    } else if (entry.type == 1) {
        u32 score = entry.score;
        if (score < 100) {
            price = price * 75 / 100;
            price = (price * score / 99 + 99) / 100 * 100;
        }
    }
    return price;
}

// Chooses or unchooses the fossil.
void DinoUiShopSellMenu::SetFlag(int index, u8 flag) {
    if (flag != mFlags[index]) {
        mFlags[index] = flag;
        if (flag) {
            mCount++;
            DinoSoundPlaySe(0x9C, 0);
        } else {
            mCount--;
            DinoSoundPlaySe(0x9D, 0);
        }
    }
}

void DinoUiShopSellMenu::RefreshTotal() {
    int total = 0;
    for (int i = 0; i < 0x40; i++) {
        if (mFlags[i] == 1) {
            total += GetPrice(i);
        }
    }
    mMoney->SetChange(total);
}

// Moves the cursor to the item, and shows the fossil's name and price.
void DinoUiShopSellMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    mFocused = item;
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    if (mCursor->IsHidden()) {
        mCursor->Show();
        mCursor->SetFrame(item->GetFrame());
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), item->GetFrame(), 10, -5));
    }
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            u32 index = mPage * 8 + i;
            if (index >= mFossils.size()) {
                continue;
            }
            DinoBackupEntry entry;
            gDinoBackup->GetUnk98(index, &entry);
            mRockName->SetEntry(&entry, FALSE);
            mMoney->SetPrice(GetPrice(mPage * 8 + i));
            mSelected = mPage * 8 + i;
            return;
        }
    }
    mRockName->Clear();
    mMoney->SetPrice(0);
    mSelected = 0xFF;
}

extern const u32 data_0208a9e0, data_0208a9e4, data_0208a9e8, data_0208aa04, data_0208aa08;

// Asks whether to sell the chosen fossils.
BOOL DinoUiShopSellMenu::Sell() {
    if (mCount == 0) {
        DinoSoundPlaySe(0xA4, 0);
        return FALSE;
    }
    mCursor->Hide();
    mConfirmed = FALSE;
    DinoCommonWindowYesNoDialog *dialog =
        new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208a9e8), &mConfirmed);
    dialog->SetLabels(gDinoResource->GetText(data_0208aa08), gDinoResource->GetText(data_0208aa04));
    dialog->SetLabels2(gDinoResource->GetText(data_0208a9e4), gDinoResource->GetText(data_0208a9e0));
    MTask *finish = MCreateCallTask(this, &DinoUiShopSellMenu::FinishSell);
    MTask *show = mCursor->CreateShowTransfer();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(dialog);
    serial->AddTail(show);
    serial->AddTail(finish);
    return TRUE;
}

// After the yes/no: sells the chosen fossils, and fixes up the pages.
void DinoUiShopSellMenu::FinishSell() {
    if (mConfirmed == TRUE) {
        DinoSoundPlaySe(0x107, 0);
        DinoBackupEntry empty;
        MI_CpuFill8(&empty, 0, sizeof(empty));
        for (int i = 0; i < 0x40; i++) {
            if (mFlags[i]) {
                mFlags[i] = FALSE;
                u32 price = GetPrice(i);
                gDinoBackup->SetSystemVar(3, price + gDinoBackup->GetSystemVar(3));
                gDinoBackup->SetUnk98(i, &empty);
            }
        }
        mCount = 0;
        gDinoBackup->CompactUnk98();
        if (mLastPage > 0) {
            if (gDinoBackup->CountUsedUnk98() <= 8) {
                mPageL->MMenuItem::Disable();
                mPageL->mMotion->Hide();
                mPageR->MMenuItem::Disable();
                mPageR->mMotion->Hide();
                mPage = 0;
                mFraction->SetNumerator(mPage + 1);
            } else {
                int last = (gDinoBackup->CountUsedUnk98() - 1) / 8;
                if (mPage > last) {
                    mPage = last;
                    mFraction->SetNumerator(mPage + 1);
                }
            }
        }
        LoadFossils();
        ClearItems();
        RefreshPage2();
        mMoney->RefreshMoney();
        RefreshTotal();
        DinoUiShopSellSellButton *sell = mLinkFrom;
        sell->mSerial.Clear();
        sell->mMotion->Play(1, 0);
    }
}

// A fossil button: chooses or unchooses it.
void DinoUiShopSellMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            int index = mPage * 8 + i;
            if (index < mFossils.size()) {
                u8 flag = !GetFlag(index);
                SetFlag(mPage * 8 + i, flag);
                ((DinoUi3PartsKasekiButton *)mItems[i])->SetMarked(flag);
                RefreshTotal();
            }
            return;
        }
    }
}

// X jumps to the sell button; B unchooses the selected fossil, or goes to (and then presses) the back button.
void DinoUiShopSellMenu::OnIdle() {
    if (GetInput()->GetPressed() & 8) {
        FocusItem(mLinkFrom);
    }
    if (GetInput()->GetPressed() & 2) {
        if (mSelected != 0xFF && GetFlag(mSelected)) {
            SetFlag(mSelected, FALSE);
            ((DinoUi3PartsKasekiButton *)mItems[mSelected % 8])->SetMarked(FALSE);
            RefreshTotal();
            return;
        }
        if (mBack->IsFocused()) {
            mBack->Decide();
        } else {
            FocusItem(mBack);
        }
    }
}

#pragma auto_inline off
// The fossil list's vector members the game emits in this file (made explicitly until the code that fills the list is
// decompiled).
template void std::__vec_deleter<DinoUiFossilEntry, std::allocator<DinoUiFossilEntry> >::sub_push_back(const DinoUiFossilEntry &x, Metrowerks::int2type<true>);

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov005_021660f8[];
extern "C" u32 data_ov005_02166178[];
static struct DinoUiShopSellStaticInit {
    DinoUiShopSellStaticInit() {
        DINO_POOL_WORD(data_ov005_021660f8, 0x34) = ((s32 *) data_ov005_02165f78)[0x4 / 4];
        DINO_POOL_WORD(data_ov005_021660f8, 0x38) = ((s32 *) data_ov005_02165f78)[0x4 / 4];
        DINO_POOL_WORD(data_ov005_021660f8, 0x3c) = DINO_POOL_WORD(data_ov005_02165f78, 0x34);
        DINO_POOL_WORD(data_ov005_021660f8, 0x40) = DINO_POOL_WORD(data_ov005_02165ff8, 0x0);
        DINO_POOL_WORD(data_ov005_021660f8, 0x44) = DINO_POOL_WORD(data_ov005_02165f78, 0x24);
        DINO_POOL_WORD(data_ov005_021660f8, 0x48) = DINO_POOL_WORD(data_ov005_02165f78, 0xc);
        DINO_POOL_WORD(data_ov005_021660f8, 0x4c) = DINO_POOL_WORD(data_ov005_02165f78, 0x3c);
        DINO_POOL_WORD(data_ov005_021660f8, 0x50) = DINO_POOL_WORD(data_ov005_02165f78, 0x30);
        DINO_POOL_WORD(data_ov005_021660f8, 0x54) = DINO_POOL_WORD(data_ov005_02165f78, 0x50);
        DINO_POOL_WORD(data_ov005_021660f8, 0x58) = DINO_POOL_WORD(data_ov005_02165f78, 0x1c);
        DINO_POOL_WORD(data_ov005_021660f8, 0x5c) = DINO_POOL_WORD(data_ov005_02165f78, 0x28);
        DINO_POOL_WORD(data_ov005_021660f8, 0x60) = DINO_POOL_WORD(data_ov005_02165f78, 0x58);
        DINO_POOL_WORD(data_ov005_021660f8, 0x64) = DINO_POOL_WORD(data_ov005_02165f78, 0x8);
        DINO_POOL_WORD(data_ov005_021660f8, 0x68) = DINO_POOL_WORD(data_ov005_02165f78, 0x74);
        DINO_POOL_WORD(data_ov005_021660f8, 0x6c) = DINO_POOL_WORD(data_ov005_02165f78, 0x20);
        DINO_POOL_WORD(data_ov005_021660f8, 0x70) = DINO_POOL_WORD(data_ov005_02165f78, 0x2c);
        DINO_POOL_WORD(data_ov005_021660f8, 0x74) = DINO_POOL_WORD(data_ov005_02165f78, 0x18);
        DINO_POOL_WORD(data_ov005_021660f8, 0x78) = DINO_POOL_WORD(data_ov005_02165f78, 0x4c);
        DINO_POOL_WORD(data_ov005_021660f8, 0x7c) = DINO_POOL_WORD(data_ov005_02165f78, 0x10);
        DINO_POOL_WORD(data_ov005_02166178, 0x0) = DINO_POOL_WORD(data_ov005_02165f78, 0x78);
        DINO_POOL_WORD(data_ov005_02166178, 0x4) = DINO_POOL_WORD(data_ov005_02165f78, 0x6c);
        DINO_POOL_WORD(data_ov005_02166178, 0x8) = DINO_POOL_WORD(data_ov005_02165f78, 0x40);
        DINO_POOL_WORD(data_ov005_02166178, 0xc) = DINO_POOL_WORD(data_ov005_02165f78, 0x70);
        DINO_POOL_WORD(data_ov005_02166178, 0x10) = DINO_POOL_WORD(data_ov005_02165f78, 0x64);
        DINO_POOL_WORD(data_ov005_02166178, 0x14) = DINO_POOL_WORD(data_ov005_02165f78, 0x68);
        DINO_POOL_WORD(data_ov005_02166178, 0x18) = DINO_POOL_WORD(data_ov005_02165f78, 0x48);
        DINO_POOL_WORD(data_ov005_02166178, 0x1c) = DINO_POOL_WORD(data_ov005_02165f78, 0x7c);
        DINO_POOL_WORD(data_ov005_02166178, 0x20) = DINO_POOL_WORD(data_ov005_02165f78, 0x60);
        DINO_POOL_WORD(data_ov005_02166178, 0x24) = DINO_POOL_WORD(data_ov005_02165f78, 0x38);
        DINO_POOL_WORD(data_ov005_02166178, 0x28) = DINO_POOL_WORD(data_ov005_02165f78, 0x14);
        DINO_POOL_WORD(data_ov005_02165ff8, 0x60) = 128;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x64) = 96;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x70) = 128;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x74) = 96;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x40) = 128;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x44) = 96;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x50) = 53;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x54) = 35;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x28) = 50;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x2c) = 0;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x30) = 0;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x34) = 50;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x38) = -32;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x3c) = 4;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x20) = 241;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x24) = 103;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x10) = 128;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x14) = 96;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x48) = 24;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x4c) = 144;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x8) = 54;
        DINO_POOL_WORD(data_ov005_02165ff8, 0xc) = 115;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x58) = 202;
        DINO_POOL_WORD(data_ov005_02165ff8, 0x5c) = 139;
    }
} sDinoUiShopSellStaticInit;
