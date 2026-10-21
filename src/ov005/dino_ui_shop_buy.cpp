// DinoUiShopBuy.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_common_window_yes_no_dialog.h"
#include "m/seed_generator.h"
#include "m/smart_oam.h"

#include "game/dino_ui3_parts_rock_name.h"

#include "game/dino_ui3_parts_cursor.h"

#include "m/obj.h"
#include "game/dino_ui_shop_buy_menu.h"
#include "game/dino_ui_shop_buy.h"
#include "game/dino_ui_shop_buy_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_ui3_parts_money.h"
#include "game/dino_ui3_parts_kaseki_button.h"
#include "game/dino_ui2_parts_kaseki_button.h"
#include "game/dino_backup.h"
#include "nitro/mi.h"
#include "m/input.h"
#include "game/dino_static_init.h"
#include "game/dino_guide_message.h"
#include "game/dino_resource.h"
#include "game/dino_ui_common.h"
#include "m/picture.h"

#pragma auto_inline off

DinoUiShopBuyMenu::DinoUiShopBuyMenu() : MMenu(NULL, 0), mPage(0), mLastPage(0), mTotal(0), mCount(0), mSelected(0xFF) {
    for (int i = 0; i < 8; i++) {
        mFlags[i] = 0;
    }
}

// How long the shop's stock lasts, in frames.
extern "C" const u64 data_02089ad4;

// Once that long has passed, restocks the shop with random picks until it is full.
// NONMATCHING: the entry index gets a second (strength-reduced) counter, as in DinoEventExecShopSet::Transfer.
void DinoUiShopBuyMenu::Restock() {
    if (gDinoBackup->GetPlayFrames() - gDinoBackup->GetUnk160() <= data_02089ad4) {
        return;
    }
    MResource<DinoShopData> shop = gDinoResource->LoadShop(gDinoBackup->GetUnk144());
    while (!gDinoBackup->IsUnk148Full()) {
        MRandomRef random(gDinoResource->GetRandom());
        const DinoEntrySource *chosen;
        for (u32 i = 0; i < shop.Get()->randomCount; i++) {
            const DinoShopData *data = shop.Get();
            const DinoEntrySource *source = (const DinoEntrySource *)((const u8 *)data + data->randomOffset) + i;
            BOOL unsold = TRUE;
            for (int j = 0; j < 8; j++) {
                DinoBackupEntry entry;
                gDinoBackup->GetUnk148(j, &entry);
                if (entry.id != 0 && entry.id == source->id) {
                    unsold = FALSE;
                }
            }
            if (unsold && random.Roll()) {
                chosen = source;
            }
        }
        gDinoBackup->AddUnk148(chosen);
    }
}

DinoUiShopBuyMenuItem::DinoUiShopBuyMenuItem(DinoUiShopBuyMenu *owner, const std::string &path, MPoint pos, u32 textId) {
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

// Lists the fossils for sale.
void DinoUiShopBuyMenu::LoadFossils() {
    mUnkE8.clear();
    for (int i = 0; i < 8; i++) {
        DinoBackupEntry entry;
        gDinoBackup->GetUnk148(i, &entry);
        if (entry.id) {
            mUnkE8.push_back(DinoGetFossilEntry(&entry, mKasekiDefs));
        }
    }
}

// Clears the page's items.
void DinoUiShopBuyMenu::ClearItems() {
    RemoveLink(mLinkFrom, 4);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
}

BOOL DinoUiShopBuyMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ClearItems();
    RefreshPage2();
    RefreshPage3();
    return TRUE;
}

#pragma auto_inline on

#pragma auto_inline off

BOOL DinoUiShopBuyMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ClearItems();
    RefreshPage2();
    RefreshPage3();
    return TRUE;
}

#pragma auto_inline on

DinoUiShopBuy::~DinoUiShopBuy() {
}

DinoUiShopBuyMenuItem::~DinoUiShopBuyMenuItem() {
}

DinoUiShopBuyMenu::~DinoUiShopBuyMenu() {
}

DinoUiShopBuyBackButton::~DinoUiShopBuyBackButton() {
}

DinoUiShopBuyPageLButton::~DinoUiShopBuyPageLButton() {
}

DinoUiShopBuyPageRButton::~DinoUiShopBuyPageRButton() {
}

DinoUiShopBuyBuyButton::~DinoUiShopBuyBuyButton() {
}

void DinoUiShopBuy::Idle() {
}

MRect DinoUiShopBuyMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiShopBuyMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiShopBuyMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiShopBuyMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiShopBuyMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiShopBuyBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiShopBuyBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiShopBuyPageRButton::OnDecide() {
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

void DinoUiShopBuyPageLButton::OnDecide() {
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

void DinoUiShopBuyMenu::OnBackDecided() {
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

// Pooled file statics: the button corners.
struct DinoUiShopBuyStatics {
    const char *mPaths[5];  // 0x00: [1] page right, [2] back, [3] page left, [4] buy
    MPoint mBottomRight; // 0x14
    MPoint mRow;         // 0x1C: to the next row
    MPoint mPageRPos;    // 0x24
    MPoint mPageLPos;    // 0x2C
    MPoint mTopLeft;     // 0x34
    MPoint mBackPos;     // 0x3C
    u8 unk44[0x54 - 0x44];
    MPoint mFirst;       // 0x54: the page's first button
    MPoint mColumn;      // 0x5C: from a page button to the next in its row
    u8 unk64[0x74 - 0x64];
    MPoint mBuyPos;      // 0x74
};
extern "C" DinoUiShopBuyStatics data_ov005_02166ac8;

extern "C" const MPoint data_ov005_02166b34;   // the rock name's position (in the pool, at 0x6C)
extern "C" const char data_ov005_02166f6c[];    // the background
extern const MRect data_020b5838, data_020b5848; // help rects
extern "C" const u32 data_0208a60c, data_0208a610, data_0208a614, data_0208a618, data_0208a61c, data_0208a620,
    data_0208a93c, data_0208aacc;

inline DinoUiShopBuyBackButton::DinoUiShopBuyBackButton(DinoUiShopBuyMenu *owner)
    : DinoUiShopBuyMenuItem(owner, data_ov005_02166ac8.mPaths[2], data_ov005_02166ac8.mBackPos, data_0208a60c) {
    mMotion->Play(2, 0);
}

inline DinoUiShopBuyPageLButton::DinoUiShopBuyPageLButton(DinoUiShopBuyMenu *owner)
    : DinoUiShopBuyMenuItem(owner, data_ov005_02166ac8.mPaths[3], data_ov005_02166ac8.mPageLPos, data_0208aacc) {
    SetKeys(0x200);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiShopBuyPageRButton::DinoUiShopBuyPageRButton(DinoUiShopBuyMenu *owner)
    : DinoUiShopBuyMenuItem(owner, data_ov005_02166ac8.mPaths[1], data_ov005_02166ac8.mPageRPos, data_0208aacc) {
    SetKeys(0x100);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiShopBuyBuyButton::DinoUiShopBuyBuyButton(DinoUiShopBuyMenu *owner)
    : DinoUiShopBuyMenuItem(owner, data_ov005_02166ac8.mPaths[4], data_ov005_02166ac8.mBuyPos, data_0208a61c) {
}

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = defs.Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

// Restocks, lists the fossils for sale and builds the page, the help, the cursor, the rock name and the money.
#pragma opt_common_subs off
void DinoUiShopBuyMenu::OnStart() {
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    DinoDimScreen();
    mKasekiDefs = gDinoResource->LoadKasekiDefs();
    mLastPage = 0;
    mParallel.AddTail(mBackground = new MTextPicture(std::string(data_ov005_02166f6c), TRUE, false));
    mBackground->Show();
    mBackground->SetPriority(2);
    mOam = new MSmartOam(0x50, 0, TRUE);
    DinoHelpDummyItem *money, *list, *total;
    mParallel.AddTail(new DinoHelpDummyItem(GetDisplay(), MRect(0x1E, 0xC, 0xE2, 0x6C),
                                            std::string(gDinoResource->GetText(data_0208a620))));
    mParallel.AddTail(list = new DinoHelpDummyItem(GetDisplay(), MRect(0x13, 0x90, 0xF1, 0x9F),
                                                   std::string(gDinoResource->GetText(data_0208a618))));
    mParallel.AddTail(money = new DinoHelpDummyItem(GetDisplay(), data_020b5848,
                                                    std::string(gDinoResource->GetText(data_0208a614))));
    mParallel.AddTail(total = new DinoHelpDummyItem(GetDisplay(), data_020b5838,
                                                    std::string(gDinoResource->GetText(data_0208a610))));
    mUnkC8 = new DinoUiShopBuyBackButton(this);
    AddItem(mUnkC8);
    if (mLastPage > 0) {
        mPageL = new DinoUiShopBuyPageLButton(this);
        AddItem(mPageL);
        mPageR = new DinoUiShopBuyPageRButton(this);
        AddItem(mPageR);
    }
    mLinkFrom = new DinoUiShopBuyBuyButton(this);
    AddItem(mLinkFrom);
    AddEntry((u32)(DinoHelpItem *)mLinkFrom, 1, 0);
    AddEntry((u32)list, 3, 0);
    AddEntry((u32)list, 1, 0);
    AddEntry((u32)money, 4, (u32)list);
    AddEntry((u32)money, 1, (u32)total);
    AddEntry((u32)total, 2, 0);
    AddEntry((u32)total, 3, (u32)money);
    AddEntry((u32)(DinoHelpItem *)mUnkC8, 4, (u32)list);
    gDinoBackup->CompactUnk148();
    Restock();
    LoadFossils();
    RefreshPage2();
    mCursorTasks = new MParallelTaskSet();
    mCursor = new DinoUi3PartsCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(1);
    mParallel.AddTail(mCursorTasks);
    mParallel.AddTail(mOam);
    mRockName = new DinoUi3PartsRockName(data_ov005_02166b34);
    mParallel.AddTail(mRockName);
    mMoney = new DinoUi3PartsMoney();
    mParallel.AddTail(mMoney);
    mFunds = gDinoBackup->GetSystemVar(3);
    RefreshPage3();
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208a93c))));
    AddLink(mLinkFrom, 3, NULL);
    AddLink(mLinkFrom, 1, NULL);
    AddLink(mLinkFrom, 2, mUnkC8);
    AddLink(mUnkC8, 4, mLinkFrom);
    AddLink(mUnkC8, 3, NULL);
    if (mItems.size() == 0) {
        FocusItem(mUnkC8);
    } else {
        FocusItem((MMenuItem *)mItems[0]);
    }
    mSerial.AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(30, 0));
}
#pragma opt_common_subs reset

MRect DinoUiShopBuyBuyButton::GetFrame() {
    return MRect(data_ov005_02166ac8.mTopLeft, data_ov005_02166ac8.mBottomRight);
}

void DinoUiShopBuyBuyButton::OnDecide() {
    if (mOwner->Buy()) {
        DinoSoundPlaySe(0x98, 0);
        mMotion->Play(3, 1);
        MTask *play = mMotion->CreatePlayTransfer(2, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
    }
}

void DinoUiShopBuyBuyButton::OnFocusChanged(BOOL focused) {
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
template class MCallTask0<DinoUiShopBuyMenu, DinoUiShopBuyMenu>;

DinoUiShopBuy::DinoUiShopBuy() : MProcess(NULL) {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->VSyncNow();
    mSerial.AddTail(new DinoUiShopBuyMenu());
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

#pragma auto_inline off

template MTask *MCreateCallTask(DinoUiShopBuyMenu *obj, void (DinoUiShopBuyMenu::*func)());

u8 DinoUiShopBuyMenu::GetFlag(int index) {
    return mFlags[index];
}

// Makes the page's buttons (blank past the last fossil) and links them in a grid, the top row to the back button.
void DinoUiShopBuyMenu::RefreshPage2() {
    mItems.clear();
    for (int i = 0; i < 8; i++) {
        DinoUi3PartsKasekiButton *button;
        if (mUnkE8.size() > i + mPage * 8) {
            button = new DinoUi3PartsKasekiButton(
                mOam,
                MPoint(data_ov005_02166ac8.mFirst.x + data_ov005_02166ac8.mColumn.x * (i % 4) +
                           data_ov005_02166ac8.mRow.x * (i / 4),
                       data_ov005_02166ac8.mFirst.y + data_ov005_02166ac8.mColumn.y * (i % 4) +
                           data_ov005_02166ac8.mRow.y * (i / 4)),
                (const DinoUiKasekiIcon &)mUnkE8[i + mPage * 8], GetFlag(i + mPage * 8));
        } else {
            button = new DinoUi3PartsKasekiButton(
                mOam,
                MPoint(data_ov005_02166ac8.mFirst.x + data_ov005_02166ac8.mColumn.x * (i % 4) +
                           data_ov005_02166ac8.mRow.x * (i / 4),
                       data_ov005_02166ac8.mFirst.y + data_ov005_02166ac8.mColumn.y * (i % 4) +
                           data_ov005_02166ac8.mRow.y * (i / 4)),
                (const DinoUiKasekiIcon &)DinoUi2GetEmptyKasekiInfo(), 0);
            button->HideMotion();
        }
        mItems.push_back(button);
        AddItem(button);
    }
    DinoUi2LinkKasekiGrid(this, (std::vector<MMenuItem *> &)mItems, mLinkFrom);
    AddLink((MMenuItem *)mItems[0], 4, mUnkC8);
    AddLink((MMenuItem *)mItems[1], 4, mUnkC8);
    AddLink((MMenuItem *)mItems[2], 4, mUnkC8);
    AddLink((MMenuItem *)mItems[3], 4, mUnkC8);
    AddLink(mLinkFrom, 4, (MMenuItem *)mItems[4]);
}

u32 DinoUiShopBuyMenu::GetPrice(int index) {
    DinoBackupEntry entry;
    gDinoBackup->GetUnk148(index, &entry);
    u32 id = entry.id;
    const DinoKasekiDefs *defs = mKasekiDefs.Get();
    return ((const DinoKasekiDef *)((const u8 *)defs + ((const u32 *)((const u8 *)defs + defs->tableOffset))[id]))->price;
}

// Chooses or unchooses the item, keeping the total.
void DinoUiShopBuyMenu::SetFlag(int index, u8 flag) {
    if (flag != mFlags[index]) {
        mFlags[index] = flag;
        if (flag) {
            mTotal += GetPrice(index);
            mCount++;
            DinoSoundPlaySe(0x9C, 0);
        } else {
            mTotal -= GetPrice(index);
            mCount--;
            DinoSoundPlaySe(0x9D, 0);
        }
    }
}

// The items whose price would go over the money (or that there's no room for) can't be chosen.
void DinoUiShopBuyMenu::RefreshPage3() {
    mMoney->SetChange(mTotal);
    for (int i = 0; i < mItems.size(); i++) {
        if (!GetFlag(mPage * 8 + i) &&
            (mTotal + GetPrice(mPage * 8 + i) > mFunds || mCount >= gDinoBackup->CountFreeUnk98())) {
            ((DinoUi3PartsKasekiButton *)mItems[i])->SetActive(FALSE);
        } else {
            ((DinoUi3PartsKasekiButton *)mItems[i])->SetActive(TRUE);
        }
    }
}

// Moves the cursor to the item, and shows the fossil's name (marked if it can't be bought now) and price.
void DinoUiShopBuyMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
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
            if (index >= mUnkE8.size()) {
                continue;
            }
            DinoBackupEntry entry;
            gDinoBackup->GetUnk148(index, &entry);
            if (!GetFlag(mPage * 8 + i) &&
                (mTotal + GetPrice(mPage * 8 + i) > mFunds || mCount >= gDinoBackup->CountFreeUnk98())) {
                mRockName->SetEntry(&entry, TRUE);
            } else {
                mRockName->SetEntry(&entry, FALSE);
            }
            mMoney->SetPrice(GetPrice(mPage * 8 + i));
            mSelected = mPage * 8 + i;
            return;
        }
    }
    mRockName->Clear();
    mMoney->SetPrice(0);
    mSelected = 0xFF;
}

extern const u32 data_0208a9f8, data_0208a9fc, data_0208aa00, data_0208aa04, data_0208aa08;

// Asks whether to buy the chosen fossils.
BOOL DinoUiShopBuyMenu::Buy() {
    if (mCount == 0) {
        DinoSoundPlaySe(0xA4, 0);
        return FALSE;
    }
    mCursor->Hide();
    mConfirmed = FALSE;
    DinoCommonWindowYesNoDialog *dialog =
        new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208aa00), &mConfirmed);
    dialog->SetLabels(gDinoResource->GetText(data_0208aa08), gDinoResource->GetText(data_0208aa04));
    dialog->SetLabels2(gDinoResource->GetText(data_0208a9fc), gDinoResource->GetText(data_0208a9f8));
    MTask *finish = MCreateCallTask(this, &DinoUiShopBuyMenu::FinishBuy);
    MTask *show = mCursor->CreateShowTransfer();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(dialog);
    serial->AddTail(show);
    serial->AddTail(finish);
    return TRUE;
}

// After the yes/no: buys the chosen fossils there's money and room for, and starts the page over.
void DinoUiShopBuyMenu::FinishBuy() {
    if (mConfirmed == TRUE) {
        DinoSoundPlaySe(0x107, 0);
        for (int i = 0; i < 8; i++) {
            if (mFlags[i]) {
                mFlags[i] = FALSE;
                DinoBackupEntry entry;
                gDinoBackup->GetUnk148(i, &entry);
                u32 price = GetKasekiDef(entry.id)->price;
                if (!gDinoBackup->IsUnk98Full() && price <= mFunds) {
                    mFunds = mFunds - price;
                    gDinoBackup->SetSystemVar(3, mFunds);
                    gDinoBackup->SetUnk98(gDinoBackup->FindFreeUnk98(), &entry);
                    MI_CpuFill8(&entry, 0, sizeof(entry));
                    gDinoBackup->SetUnk148(i, &entry);
                }
            }
        }
        mCount = 0;
        gDinoBackup->CompactUnk148();
        LoadFossils();
        ClearItems();
        RefreshPage2();
        mTotal = 0;
        mMoney->RefreshMoney();
        RefreshPage3();
        gDinoBackup->MarkUnk160();
        DinoUiShopBuyBuyButton *buy = mLinkFrom;
        buy->mSerial.Clear();
        buy->mMotion->Play(1, 0);
    }
}

// A fossil button: chooses it (if the money and the room allow) or unchooses it.
void DinoUiShopBuyMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            int index = mPage * 8 + i;
            if (index < mUnkE8.size()) {
                u8 flag = !GetFlag(index);
                if (flag && mTotal + GetPrice(mPage * 8 + i) > mFunds) {
                    DinoSoundPlaySe(0xA4, 0);
                    return;
                }
                if (flag && mCount >= gDinoBackup->CountFreeUnk98()) {
                    DinoSoundPlaySe(0xA4, 0);
                    return;
                }
                SetFlag(mPage * 8 + i, flag);
                ((DinoUi3PartsKasekiButton *)mItems[i])->SetMarked(flag);
                RefreshPage3();
            }
            return;
        }
    }
}

// X jumps to the buy button; B unchooses the selected item, or goes to (and then presses) the back button.
void DinoUiShopBuyMenu::OnIdle() {
    if (GetInput()->GetPressed() & 8) {
        FocusItem(mLinkFrom);
    }
    if (GetInput()->GetPressed() & 2) {
        if (mSelected != 0xFF && GetFlag(mSelected)) {
            SetFlag(mSelected, FALSE);
            ((DinoUi3PartsKasekiButton *)mItems[mSelected % 8])->SetMarked(FALSE);
            RefreshPage3();
            return;
        }
        if (mUnkC8->IsFocused()) {
            mUnkC8->Decide();
        } else {
            FocusItem(mUnkC8);
        }
    }
}

// The file's static initialization (see dino_static_init.h).
static struct DinoUiShopBuyStaticInit {
    DinoUiShopBuyStaticInit() {
        DINO_POOL_WORD(data_ov005_02166ac8, 0x3c) = 128;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x40) = 96;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x2c) = 128;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x30) = 96;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x24) = 128;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x28) = 96;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x54) = 53;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x58) = 35;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x5c) = 50;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x60) = 0;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x1c) = 0;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x20) = 50;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x74) = 128;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x78) = 96;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x6c) = 24;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x70) = 144;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x34) = 54;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x38) = 115;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x14) = 202;
        DINO_POOL_WORD(data_ov005_02166ac8, 0x18) = 139;
    }
} sDinoUiShopBuyStaticInit;
