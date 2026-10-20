// DinoUiItemItemButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_resource.h"
#include "m/indicator.h"
#include "m/smart_oam.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_ui_item_item_button_menu.h"
#include "m/console.h"
#include "game/dino_ui_field_menu_cursor.h"
#include "game/dino_ui_item_item_button.h"
#include "game/dino_ui_item_menu_item.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_static_init.h"

#pragma auto_inline off
DinoUiItemMenu::DinoUiItemMenu(int *choice, DinoTeam *team, DinoTeam *team2)
    : MMenu(gLcd->GetBottomDisplay(), NULL, 0), mResult(choice), mOldTeam(team), mTeam(team2), mPage(0), mLastPage(0) {
}
#pragma auto_inline on

DinoUiItemItemButton::~DinoUiItemItemButton() {
}

DinoUiItemMenuItem::~DinoUiItemMenuItem() {
}

DinoUiItemMenu::~DinoUiItemMenu() {
}

DinoUiItemBackButton::~DinoUiItemBackButton() {
}

DinoUiItemOKButton::~DinoUiItemOKButton() {
}

DinoUiItemPageLButton::~DinoUiItemPageLButton() {
}

DinoUiItemPageRButton::~DinoUiItemPageRButton() {
}

MRect DinoUiItemMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiItemMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiItemMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiItemMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiItemMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiItemOKButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiItemBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiItemBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiItemPageRButton::OnDecide() {
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

void DinoUiItemPageLButton::OnDecide() {
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

void DinoUiItemOKButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnOKDecided();
    DinoSoundPlaySe(0x9A, 0);
}

void DinoUiItemItemButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mView->Show();
    } else {
        mView->Hide();
    }
}

void DinoUiItemMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 3; i++) {
        if (item == mTabItems[i]) {
            SelectTab(i);
        }
    }
}


void DinoUiItemMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    mFraction->SetNumerator(mPage + 1);
    RefreshPage();
    MPoint cursor = GetCursor();
    FocusItem(mUnkB8);
    FocusNearestItem(cursor);
}

void DinoUiItemMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    mFraction->SetNumerator(mPage + 1);
    RefreshPage();
    MPoint cursor = GetCursor();
    FocusItem(mUnkB8);
    FocusNearestItem(cursor);
}

MRect DinoUiItemItemButton::GetFrame() {
    return MRect(mPos.Offset(-80, -8), mPos.Offset(80, 8));
}

extern const int data_02089a20; // fade frames
extern const int data_02089a88;

void DinoUiItemMenu::SelectTab(int tab) {
    if (tab < 0 || tab > 2) {
        return;
    }
    switch (tab) {
    case 0: {
        *mResult = 1;
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a20, data_02089a88);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
        DinoSoundPlaySe(0x94, 0);
        break;
    }
    case 1: {
        *mResult = 2;
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a20, data_02089a88);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
        DinoSoundPlaySe(0x94, 0);
        break;
    }
    case 2:
        break;
    }
}

extern const int data_02089a34;
extern const u32 data_0208aa3c; // "the team was changed" text
extern const int data_02089a30;
extern const int data_02089a2c;

void DinoUiItemMenu::OnBackDecided() {
    *mResult = 9;
    mSerial.AddTail(new MTimer(data_02089a34));
    if (DinoTeamDiffers(mOldTeam, mTeam)) {
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(128, 96), gDinoResource->GetText(data_0208aa3c), 4));
    }
    MTask &self = *this;
    MTask *exit = MTaskExitTransfer::Create(&self);
    MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a30, data_02089a2c);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fade);
    serial->AddTail(exit);
}

extern const u32 data_0208aae0;

void DinoUiItemMenu::OnOKDecided() {
    *mResult = 0;
    if (mOldTeam->slots[0]) {
        if (DinoTeamDiffers(mOldTeam, mTeam)) {
            mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(128, 96), gDinoResource->GetText(data_0208aae0), 4));
        } else {
            mSerial.AddTail(new MTimer(30));
        }
        MTask *fade;
        MTask *exit;
        MTask &self = *this;
        exit = MTaskExitTransfer::Create(&self);
        fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
    } else {
        MTask *timer;
        MTask *fade;
        MTask *exit;
        MTask &self = *this;
        exit = MTaskExitTransfer::Create(&self);
        fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
        timer = new MTimer(30);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(timer);
        serial->AddTail(fade);
        serial->AddTail(exit);
    }
}

#pragma auto_inline off

// Shows the page's items' names.
void DinoUiItemMenu::RefreshPage() {
    MResource<DinoKeyItemDefs> defs = gDinoResource->LoadKeyItemDefs();
    for (int i = 0; i < 5; i++) {
        u32 index = mPage * 5 + i;
        if (mKeyItems.size() > index) {
            u32 id = mKeyItems[index];
            const DinoKeyItemDefs *data = defs.Get();
            const DinoKeyItemDef *def = (const DinoKeyItemDef *)((const u8 *)data + data->tableOffset) + id;
            mLabels[i]->SetText(gDinoResource->GetText(def->nameText));
            mItems[i]->Enable();
        } else {
            mLabels[i]->SetText("");
            mItems[i]->Enable();
        }
    }
}

DinoUiItemMenuItem::DinoUiItemMenuItem(DinoUiItemMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(2);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    mMotion->GetUnit()->SetMode(1);
}

extern MPoint data_020b5858[3];

static inline const DinoKeyItemDef *GetKeyItemDef(const DinoKeyItemDefs *defs, u32 id) {
    return (const DinoKeyItemDef *)((const u8 *)defs + defs->tableOffset) + id;
}

// Moves the cursor to the item (a tab, or an item button), and shows the item's description.
void DinoUiItemMenu::OnFocus(MMenuItem *item) {
    int i;
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (i = 0; i < 3; i++) {
        if (item == mTabItems[i]) {
            frame.MoveTo(data_020b5858[i]);
            mCursor->SetMode(2);
        }
    }
    for (i = 0; i < 5; i++) {
        if (item == mItems[i]) {
            frame.left -= 8;
            frame.top += 8;
            frame.right -= 8;
            frame.bottom += 8;
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        MRect from = mCursor->GetFrame();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(from, frame, 10, -5));
    }
    mCursor->Show();
    mConsole->Clear();
    for (i = 0; i < (mPage == mLastPage ? mKeyItems.size() - mPage * 5 : 5); i++) {
        if (item == mItems[i]) {
            MResource<DinoKeyItemDefs> defs = gDinoResource->LoadKeyItemDefs();
            u32 id = mKeyItems[mPage * 5 + i];
            const DinoKeyItemDefs *data = defs.Get();
            const DinoKeyItemDef *def = (const DinoKeyItemDef *)((const u8 *)data + data->tableOffset) + id;
            mConsole->Print(std::string(gDinoResource->GetText(def->descText)));
        }
    }
}

// The OK button is usable once the team has a first member.
void DinoUiItemMenu::RefreshOKButton() {
    if (mOldTeam->slots[0]) {
        mOKButton->mMotion->GetUnit()->SetMode(1);
    } else {
        mOKButton->mMotion->GetUnit()->SetMode(0);
    }
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov006_021934b4[];
extern "C" u32 data_ov006_02193534[];
static struct DinoUiItemItemButtonStaticInit {
    DinoUiItemItemButtonStaticInit() {
        DINO_POOL_WORD(data_ov006_021934b4, 0x58) = 128;
        DINO_POOL_WORD(data_ov006_021934b4, 0x5c) = 176;
        DINO_POOL_WORD(data_ov006_021934b4, 0x40) = 8;
        DINO_POOL_WORD(data_ov006_021934b4, 0x44) = 150;
        DINO_POOL_WORD(data_ov006_021934b4, 0x60) = 64;
        DINO_POOL_WORD(data_ov006_021934b4, 0x64) = 112;
        DINO_POOL_WORD(data_ov006_021934b4, 0x38) = 19;
        DINO_POOL_WORD(data_ov006_021934b4, 0x3c) = 179;
        DINO_POOL_WORD(data_ov006_021934b4, 0x78) = 27;
        DINO_POOL_WORD(data_ov006_021934b4, 0x7c) = 0;
        DINO_POOL_WORD(data_ov006_021934b4, 0x50) = 128;
        DINO_POOL_WORD(data_ov006_021934b4, 0x54) = 40;
        DINO_POOL_WORD(data_ov006_02193534, 0x18) = 51;
        DINO_POOL_WORD(data_ov006_02193534, 0x1c) = 34;
        DINO_POOL_WORD(data_ov006_02193534, 0x0) = 0;
        DINO_POOL_WORD(data_ov006_02193534, 0x4) = 18;
        DINO_POOL_WORD(data_ov006_021934b4, 0x70) = 20;
        DINO_POOL_WORD(data_ov006_021934b4, 0x74) = 137;
        DINO_POOL_WORD(data_ov006_02193534, 0x8) = 128;
        DINO_POOL_WORD(data_ov006_02193534, 0xc) = 96;
        DINO_POOL_WORD(data_ov006_021934b4, 0x18) = 128;
        DINO_POOL_WORD(data_ov006_021934b4, 0x1c) = 96;
        DINO_POOL_WORD(data_ov006_021934b4, 0x30) = 128;
        DINO_POOL_WORD(data_ov006_021934b4, 0x34) = 96;
        DINO_POOL_WORD(data_ov006_021934b4, 0x48) = 128;
        DINO_POOL_WORD(data_ov006_021934b4, 0x4c) = 96;
        DINO_POOL_WORD(data_ov006_021934b4, 0x28) = 241;
        DINO_POOL_WORD(data_ov006_021934b4, 0x2c) = 114;
    }
} sDinoUiItemItemButtonStaticInit;
