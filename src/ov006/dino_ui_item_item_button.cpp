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
#include "m/input.h"
#include "game/dino_backup.h"
#include "m/resource.h"
#include "game/dino_ui_parts_tab_icon_menu_item.h"
#include "m/picture.h"
#include "game/dino_guide_message.h"
#include "game/dino_ui_common.h"
#include "m/oam_canvas.h"
#include "m/obj.h"

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
            u32 id = (u32)mKeyItems[index];
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


static inline const DinoKeyItemDef *GetKeyItemDef(const DinoKeyItemDefs *defs, u32 id) {
    return (const DinoKeyItemDef *)((const u8 *)defs + defs->tableOffset) + id;
}

// OnStart's pooled layout (see the static initializer below): path pointers and positions.
extern "C" u32 data_ov006_021934b4[];
extern "C" u32 data_ov006_02193534[];
#define DINO_UI_ITEM_PATH(i) (((const char **)data_ov006_021934b4)[i])
#define DINO_UI_ITEM_POINT(pool, off) (*(const MPoint *)((const u8 *)(pool) + (off)))
extern MPoint data_ov006_021934dc;      // the page fraction's place
extern MPoint data_ov006_02193524;      // the description's place
extern const MRect data_020b58b0, data_020b58c0, data_020b58d0;     // the tabs' frames
extern "C" const char data_ov006_021939bc[], data_ov006_021939d0[];   // the background picture, ""
extern const u32 data_0208a8bc, data_0208a8c0, data_0208a8c4, data_0208a8c8, data_0208a8cc, data_0208a8d0,
    data_0208a8d4, data_0208a8d8, data_0208a960, data_0208aabc;
extern const int data_02089a24, data_02089a28;

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(const MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = const_cast<MResource<DinoExcavateDefs> &>(defs).Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

inline DinoUiItemBackButton::DinoUiItemBackButton(DinoUiItemMenu *owner)
    : DinoUiItemMenuItem(owner, DINO_UI_ITEM_PATH(5), DINO_UI_ITEM_POINT(data_ov006_02193534, 8), data_0208a8bc) {
    SetKeys(2);
    SetMode(4);
    mMotion->Play(2, 0);
}

inline DinoUiItemOKButton::DinoUiItemOKButton(DinoUiItemMenu *owner)
    : DinoUiItemMenuItem(owner, DINO_UI_ITEM_PATH(1), DINO_UI_ITEM_POINT(data_ov006_021934b4, 0x18), data_0208a8c0) {
    SetKeys(8);
    mMotion->Play(2, 0);
}

inline DinoUiItemPageLButton::DinoUiItemPageLButton(DinoUiItemMenu *owner)
    : DinoUiItemMenuItem(owner, DINO_UI_ITEM_PATH(4), DINO_UI_ITEM_POINT(data_ov006_021934b4, 0x30), data_0208aabc) {
    SetKeys(0x200);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiItemPageRButton::DinoUiItemPageRButton(DinoUiItemMenu *owner)
    : DinoUiItemMenuItem(owner, DINO_UI_ITEM_PATH(3), DINO_UI_ITEM_POINT(data_ov006_021934b4, 0x48), data_0208aabc) {
    SetKeys(0x100);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiItemItemButton::DinoUiItemItemButton(DinoUiItemMenu *owner, MPoint pos) {
    mOwner = owner;
    mPos = pos;
    mView = new MMotion(std::string(DINO_UI_ITEM_PATH(2)), mOwner->GetOam(), 2, 1);
    mView->Play(1, 0);
    mView->SetPosition(pos);
    mView->SetPriority(2);
    mView->Hide();
    mParallel.AddTail(mView);
    mView->GetUnit()->SetMode(1);
}

// &mParallel through a non-member inline: the label loop hoists it into the frame's lowest slot, as the game does.
static inline MTaskSet *ItemParallel(DinoUiItemMenu *menu) {
    return &menu->mParallel;
}

// The key items the player has; the background, the back and OK (and page) buttons, the tabs, the help areas, the
// page fraction, the cursor, the page's labels and item buttons and the description; focuses the third tab.
#pragma opt_common_subs off
void DinoUiItemMenu::OnStart() {
    SetLinkFallback(1);
    GetInput()->SetRepeat(0xF0, GetExcavate7C(gDinoResource->GetExcavateDefs())[7],
                          GetExcavate7C(gDinoResource->GetExcavateDefs())[8]);
    DinoDimScreen();
    mKeyItems.clear();
    MResource<DinoKeyItemDefs> defs = gDinoResource->LoadKeyItemDefs();
    u32 i = 1;
    for (u32 id = i; id < gDinoResource->GetKeyItemCount(); id = ++i) {
        if (GetKeyItemDef(defs.Get(), id)->nameText != 0 && gDinoBackup->GetKeyItem(i) == 1) {
            mKeyItems.push_back((void *&)i);
        }
    }
    if (mKeyItems.size() == 0) {
        mLastPage = 0;
    } else {
        mLastPage = (mKeyItems.size() - 1) / 5;
    }
    mParallel.AddTail(mBackground = new MTextPicture(std::string(data_ov006_021939bc), TRUE, false));
    mBackground->Show();
    mBackground->SetPriority(2);
    mOam = new MSmartOam(0x3C, 0, TRUE);
    mUnkB8 = new DinoUiItemBackButton(this);
    AddItem(mUnkB8);
    mOKButton = new DinoUiItemOKButton(this);
    AddItem(mOKButton);
    RefreshOKButton();
    if (mLastPage > 0) {
        mPageL = new DinoUiItemPageLButton(this);
        AddItem(mPageL);
        mPageR = new DinoUiItemPageRButton(this);
        AddItem(mPageR);
    }
    mTabItems[0] = new DinoUiPartsTabIconMenuItem(data_0208a8cc, data_020b58b0);
    AddItem(mTabItems[0]);
    mTabItems[1] = new DinoUiPartsTabIconMenuItem(data_0208a8c8, data_020b58c0);
    AddItem(mTabItems[1]);
    mTabItems[2] = new DinoUiPartsTabIconMenuItem(data_0208a8c4, data_020b58d0);
    AddItem(mTabItems[2]);
    mParallel.AddTail(new DinoHelpDummyItem(GetDisplay(), MRect(0x19, 0x1C, 0xE6, 0x7C),
                                            std::string(gDinoResource->GetText(data_0208a8d8))));
    mParallel.AddTail(new DinoHelpDummyItem(GetDisplay(), MRect(0xE7, 0x65, 0xFA, 0x7B),
                                            std::string(gDinoResource->GetText(data_0208a8d4))));
    DinoHelpDummyItem *help;
    mParallel.AddTail(help = new DinoHelpDummyItem(GetDisplay(), MRect(0xF, 0x84, 0xF1, 0xAA),
                                                   std::string(gDinoResource->GetText(data_0208a8d0))));
    AddEntry((u32)help, 3, 0);
    AddEntry((u32)help, 1, 0);
    AddEntry((u32)help, 2, (u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabItems[2]);
    AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabItems[0], 4, (u32)help);
    AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabItems[1], 4, (u32)help);
    AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabItems[2], 4, (u32)help);
    AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabItems[2], 1, (u32)(DinoHelpItem *)mOKButton);
    mFraction = new DinoUiFieldMenuFraction(mOam, data_ov006_021934dc);
    mParallel.AddTail(mFraction);
    mFraction->SetNumerator(mPage + 1);
    mFraction->SetDenominator(mLastPage + 1);
    mCursorTasks = new MParallelTaskSet();
    mCursor = new DinoUiFieldMenuCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(1);
    mCursor->Hide();
    mParallel.AddTail(mCursorTasks);
    mParallel.AddTail(mOam);
    MFont *font = gDinoResource->CreateFont12(0, 1, 0);
    for (int i = 0; i < 5; i++) {
        MTaskSet *tasks = ItemParallel(this);
        MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, MPoint(0xA0, 0x10), gDinoResource->GetUnkTable(), TRUE);
        canvas->GetOamUnit()->SetMode(1);
        mLabels[i] = new MIndicator(canvas, font, 0);
        tasks->AddTail(mLabels[i]);
        mLabels[i]->SetPosition(MPoint(DINO_UI_ITEM_POINT(data_ov006_02193534, 0x18).x +
                                           DINO_UI_ITEM_POINT(data_ov006_02193534, 0).x * i,
                                       DINO_UI_ITEM_POINT(data_ov006_02193534, 0x18).y +
                                           DINO_UI_ITEM_POINT(data_ov006_02193534, 0).y * i));
        mLabels[i]->SetPriority(2);
        mLabels[i]->Show();
        u32 index = mPage * 5 + i;
        if (mKeyItems.size() > index) {
            u32 id = (u32)mKeyItems[index];
            mLabels[i]->SetText(std::string(gDinoResource->GetText(GetKeyItemDef(defs.Get(), id)->nameText)));
        } else {
            mLabels[i]->SetText(std::string(data_ov006_021939d0));
        }
        mItems[i] = new DinoUiItemItemButton(this, MPoint(DINO_UI_ITEM_POINT(data_ov006_021934b4, 0x50).x +
                                                              DINO_UI_ITEM_POINT(data_ov006_02193534, 0).x * i,
                                                          DINO_UI_ITEM_POINT(data_ov006_021934b4, 0x50).y +
                                                              DINO_UI_ITEM_POINT(data_ov006_02193534, 0).y * i));
        AddItem(mItems[i]);
    }
    AddLink(mTabItems[2], 1, mOKButton);
    AddLink(mTabItems[0], 3, mUnkB8);
    AddLink(mUnkB8, 1, mTabItems[0]);
    AddLink(mItems[0], 4, mUnkB8);
    AddLink(mTabItems[0], 4, mItems[4]);
    AddLink(mTabItems[0], 2, mItems[0]);
    AddLink(mTabItems[1], 2, mItems[0]);
    AddLink(mTabItems[2], 2, mItems[0]);
    AddLink(mOKButton, 2, mItems[0]);
    AddLink(mUnkB8, 2, mItems[0]);
    AddLink(mItems[4], 2, mTabItems[2]);
    AddLink(mItems[4], 3, NULL);
    AddLink(mItems[3], 3, NULL);
    AddLink(mItems[4], 1, NULL);
    AddLink(mItems[3], 1, NULL);
    mConsole = new MConsole(new MObjCanvas(gCurrentDisplay, MPoint(0xD8, 0x20), gDinoResource->GetUnkTable(), TRUE),
                            font, MPoint(0, 3), MFx32(0));
    mConsole->SetUnk44(0);
    mConsole->SetPosition(data_ov006_02193524);
    mConsole->Show();
    mParallel.AddTail(mConsole);
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208a960))));
    mTabItems[1]->SetKeys(0x800);
    mTabItems[0]->SetKeys(0x400);
    FocusItem(mTabItems[2]);
    *mResult = 0;
    mSerial.AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(data_02089a28, data_02089a24));
}
#pragma opt_common_subs reset

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
            u32 id = (u32)mKeyItems[mPage * 5 + i];
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
