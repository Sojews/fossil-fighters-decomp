// DinoUiOrderSelect.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/blend.h"
#include "game/dino_sound.h"
#include "game/dino_ui_order_select.h"
#include "game/dino_ui_rv_information.h"
#include "game/dino_ui_field_menu_cursor.h"
#include "types.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/bright.h"
#include "m/obj.h"
#include "m/bg.h"
#include "game/dino_ui_parts_reviver_medal_button.h"
#include "game/dino_ui_parts_reviver_name.h"
#include "game/common_window_message.h"
#include "game/dino_common_window_message_box.h"
#include <algorithm>
#include "m/smart_oam.h"
#include "m/font.h"
#include "m/indicator.h"
#include "m/oam_canvas.h"
#include "game/dino_team.h"
#include "game/dino_ui_parts_team_tech.h"
#include "m/call_task.h"
#include "m/input.h"
#include <printf.h>
#include "game/dino_static_init.h"
#include "game/dino_ui_parts_black_belt.h"
#include "game/dino_ui_common.h"
#include "game/dino_guide_message.h"
#include "game/dino_ui_parts_tab_icon_menu_item.h"
#include "m/picture.h"

DinoUiOrderSelect::~DinoUiOrderSelect() {
}

DinoUiOrderSelect::DinoUiOrderSelect(u32 team, const DinoUiOrderSelectInfo &info, u8 unk110, u8 unk111, u32 unk114,
                                     u8 unk112)
    : MProcess(NULL), mTeam(team), mInfo(info), mUnk110(unk110), mUnk111(unk111), mUnk112(unk112), mUnk114(unk114) {
}

// Sets up both screens and opens the menu.
void DinoUiOrderSelect::Idle() {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 0, 3, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->mSub->ResetHooks();
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mSub->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
    GetSerial()->AddTail(new DinoUiOrderSelectMenu(mTeam, &mInfo, mUnk110, mUnk111, mUnk114, mUnk112),
                         MTaskExitTransfer::Create(this));
}

DinoUiOrderSelectMenuItem::~DinoUiOrderSelectMenuItem() {
}

DinoUiOrderSelectBackButton::~DinoUiOrderSelectBackButton() {
}

DinoUiOrderSelectDecideButton::~DinoUiOrderSelectDecideButton() {
}

MRect DinoUiOrderSelectMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiOrderSelectMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiOrderSelectMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiOrderSelectBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiOrderSelectMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

const char *DinoUiOrderSelectMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiOrderSelectDecideButton::OnDecide() {
    mMotion->Play(3, 1);
    mOwner->Confirm();
    DinoSoundPlaySe(0x98, 0);
}

void DinoUiOrderSelectBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    mOwner->Confirm();
    DinoSoundPlaySe(0x90, 0);
}

// DinoUiOrderSelectMenu's OnStart/OnFocus/OnDecide (not decompiled yet) queue these call tasks.
#pragma auto_inline off
template MTask *MCreateCallTask(DinoUiOrderSelectMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiOrderSelectMenu *obj, void (DinoUiOrderSelectMenu::*func)());
template MTask *MCreateCallTask(DinoUiOrderSelectMenu *obj, void (DinoUiOrderSelectMenu::*func)(u8), u8 a1);
#pragma auto_inline reset

#pragma auto_inline off

DinoUiOrderSelectMenuItem::DinoUiOrderSelectMenuItem(DinoUiOrderSelectMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(2);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

DinoUiOrderSelectMenu::~DinoUiOrderSelectMenu() {
    if (mPictures[0] != NULL) {
        for (int i = 0; i < 4; i++) {
            delete mPictures[i];
        }
    }
    gCurrentDisplay->mBlend->Reset();
}

// Where the team's medals go.
extern "C" const MPoint data_ov006_02197340[5];
extern const u32 kCreatureLevelLimits[2];

// A creature's experience, up to what the top level needs.
static inline u32 GetCreatureTotal(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    u32 sum = creature.unk0_8 + (creature.parts[0].level + creature.parts[1].level + creature.parts[2].level +
                                 creature.parts[3].level);
    return std::min(kCreatureLevelLimits[0] * 11 + 50, sum);
}

static inline u32 GetCreatureLevel(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    return (GetCreatureTotal(creature) - 50) / kCreatureLevelLimits[0] + 1;
}

// Where the opponent's medals go: beside four tabs, or three.
extern "C" const MPoint data_ov006_02197318[5], data_ov006_021972f0[5];

void DinoUiOrderSelectMenu::CreateOpponentItems() {
    for (int i = 0; i < 5; i++) {
        if (mOpponent.creatures[i].index) {
            u8 maxed;
            if (GetCreatureLevel(mOpponent.creatures[i]) == 12) {
                maxed = TRUE;
            } else {
                maxed = FALSE;
            }
            mUnkF8[i] = new DinoUiPartsReviverMedalButton(
                mOam, mHasFourthTab ? data_ov006_02197318[i] : data_ov006_021972f0[i], mOpponent.creatures[i].index,
                maxed, (mOpponent.creatures[i].unk14 & 1) ? (u8)TRUE : (u8)FALSE);
            AddItem(mUnkF8[i]);
        } else {
            mUnkF8[i] = new DinoUiPartsReviverMedalButton(
                mOam, mHasFourthTab ? data_ov006_02197318[i] : data_ov006_021972f0[i], 0xFFFF, 0, 0);
            AddItem(mUnkF8[i]);
            mUnkF8[i]->Disable();
        }
    }
    if (mOpponent.creatures[4].index) {
        AddLink(mUnkF8[0], 3, mUnkF8[4]);
        AddLink(mUnkF8[4], 1, mUnkF8[0]);
    } else if (mOpponent.creatures[3].index) {
        AddLink(mUnkF8[0], 3, mUnkF8[3]);
        AddLink(mUnkF8[3], 1, mUnkF8[0]);
    } else {
        AddLink(mUnkF8[0], 3, mUnkF8[2]);
        AddLink(mUnkF8[2], 1, mUnkF8[0]);
    }
}

// Where the names go (the opponent's beside four tabs, or three).
extern "C" const MPoint data_ov006_02197190, data_ov006_02197158, data_ov006_02197180, data_ov006_02197178;

// (No common subexpressions: the game reads gCurrentDisplay again after copying each canvas's size.)
#pragma opt_common_subs off
void DinoUiOrderSelectMenu::CreateNameLabels(const wchar_t *playerName) {
    MFont *font = gDinoResource->CreateFont12(0, 10, 1);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, MPoint(0xA0, 0x10), gDinoResource->GetUnkTable(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    MIndicator *label = new MIndicator(canvas, font, 0);
    mParallel.AddTail(label);
    label->SetPosition(data_ov006_02197190);
    label->SetPriority(3);
    label->Show();
    label->SetText(std::wstring(playerName));
    canvas = new MObjCanvas(gCurrentDisplay, MPoint(0xA0, 0x10), gDinoResource->GetUnkTable(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    label = new MIndicator(canvas, font, 0);
    mParallel.AddTail(label);
    if (mHasFourthTab) {
        label->SetPosition(data_ov006_02197158);
    } else {
        label->SetPosition(data_ov006_02197180);
    }
    label->SetPriority(3);
    label->Show();
    label->SetText(std::string(mOpponent.name));
    canvas = new MObjCanvas(gCurrentDisplay, MPoint(0xA0, 0x10), gDinoResource->GetUnkTable(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    label = new MIndicator(canvas, font, 0);
    mParallel.AddTail(label);
    label->SetPosition(data_ov006_02197178);
    label->SetPriority(3);
    label->Show();
    label->SetText(std::wstring((const wchar_t *)mTeam.name));
}
#pragma opt_common_subs reset

void DinoUiOrderSelectMenu::OnIdle() {
    mUnk1F6 = TRUE;
    if (GetInput()->GetPressed() & 0x800) {
        PageLeft();
    } else if (GetInput()->GetPressed() & 0x400) {
        PageRight();
    }
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoHelpBase::*Enable_00)();
    void (DinoUiOrderSelectMenu::*SetInfoPage_08)(u8);
} data_ov006_02197110;

void DinoUiOrderSelectMenu::SetInfoPage(u8 page) {
    mInfo->SetPage(page);
    GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial),
                         MCreateCallTask(this, data_ov006_02197110.Enable_00));
    mUnk1BC->Clear();
    Disable();
}

void DinoUiOrderSelectMenu::PageLeft() {
    u8 count = mHasFourthTab ? 4 : 3;
    SetPage((mPage + count - 1) % count);
    if (mFocusTabs) {
        FocusItem(mTabs[mPage]);
    }
}

void DinoUiOrderSelectMenu::PageRight() {
    u8 count = mHasFourthTab ? 4 : 3;
    SetPage((mPage + 1) % count);
    if (mFocusTabs) {
        FocusItem(mTabs[mPage]);
    }
}

void DinoUiOrderSelectMenu::ClearItems() {
    RemoveLink(mOKButton, 4);
    RemoveLink(mBackButton, 4);
    RemoveLink(mUnkF8[mHasFourthTab ? 3 : 2], 2);
    for (int i = 0; i < 5; i++) {
        RemoveItem(mItems[i]);
        delete mItems[i];
    }
    mItemsLinked = FALSE;
}

void DinoUiOrderSelectMenu::RefreshItems() {
    for (int i = 0; i < 5; i++) {
        mItems[i]->Enable();
    }
    for (int j = 0; j < 5; j++) {
        if (mOpponent.creatures[j].index) {
            mUnkF8[j]->Enable();
        }
    }
    mBackButton->MMenuItem::Disable();
    mBackButton->DinoHelpItem::Disable();
    mBackButton->mMotion->Hide();
    mOKButton->MMenuItem::Enable();
    mOKButton->DinoHelpItem::Enable();
    mOKButton->mMotion->Show();
    RemoveLink(mTabs[0], 3);
    AddLink(mTabs[0], 3, mOKButton);
    if (mHasFourthTab) {
        RemoveLink(mTabs[3], 1);
        AddLink(mTabs[3], 1, mOKButton);
    } else {
        RemoveLink(mTabs[2], 1);
        AddLink(mTabs[2], 1, mOKButton);
    }
    LinkItems();
}

DinoUiOrderSelectMenu::DinoUiOrderSelectMenu(u32 team, const DinoUiOrderSelectInfo *opponent, u8 hasFourthTab,
                                             u8 unk1F7, u32 unk1F8, u8 unk1FD)
    : MMenu(NULL, 0), mUnkD0(NULL), mTeamIndex(team), mOpponent(*opponent), mHasFourthTab(hasFourthTab),
      mFocusIndex(0), mUnk1F0(0), mPage(0xFF), mFocusTabs(0), mUnk1F6(0), mUnk1F7(unk1F7), mUnk1F8(unk1F8),
      mItemsLinked(0), mUnk1FD(unk1FD), mUnk204(0xFF), mUnk205(0) {
    for (int i = 0; i < 4; i++) {
        mPictures[i] = NULL;
    }
}

void DinoUiOrderSelectMenu::CreateItems() {
    for (int i = 0; i < 5; i++) {
        if (mTeam.slots[i]) {
            DinoCreatureState creature;
            gDinoBackup->GetCreature(mTeam.slots[i], &creature);
            u8 maxed;
            if (GetCreatureLevel(creature) == 12) {
                maxed = TRUE;
            } else {
                maxed = FALSE;
            }
            mItems[i] = new DinoUiPartsReviverMedalButton(mOam, data_ov006_02197340[i], mTeam.slots[i], maxed, 0);
            AddItem(mItems[i]);
        } else {
            mItems[i] = new DinoUiPartsReviverMedalButton(mOam, data_ov006_02197340[i], 0, 0, 0);
            AddItem(mItems[i]);
            mItems[i]->Disable();
        }
        AddLink(mItems[i], 2, mTabs[mPage]);
    }
    if (mItems[3]->IsEnabled()) {
        AddLink(mUnkF8[mHasFourthTab ? 3 : 2], 2, mItems[3]);
    } else if (mItems[2]->IsEnabled()) {
        AddLink(mUnkF8[mHasFourthTab ? 3 : 2], 2, mItems[2]);
    } else {
        AddLink(mUnkF8[mHasFourthTab ? 3 : 2], 2, mItems[4]);
    }
}

// Links the items left and right to the nearest enabled ones (round the row), and the buttons up to the last.
void DinoUiOrderSelectMenu::LinkItems() {
    MMenuItem *left[5];
    MMenuItem *right[5];
    int i;
    for (i = 0; i < 5; i++) {
        left[i] = mItems[(i + 4) % 5];
        right[i] = mItems[(i + 1) % 5];
    }
    int first = 0;
    for (i = 0; i < 5; i++) {
        if (mItems[i]->IsEnabled()) {
            first = i;
            break;
        }
    }
    for (i = 0; i < 5; i++) {
        int k = first + i;
        if (!mItems[k % 5]->IsEnabled()) {
            left[(k + 1) % 5] = left[k % 5];
        }
    }
    int start = (first + 1) % 5;
    for (i = 4; i >= 0; i--) {
        int k = start + i;
        if (!mItems[k % 5]->IsEnabled()) {
            right[(k + 4) % 5] = right[k % 5];
        }
    }
    if (mItemsLinked) {
        RemoveLink(mBackButton, 4);
        RemoveLink(mOKButton, 4);
        for (i = 0; i < 5; i++) {
            RemoveLink(mItems[i], 3);
            RemoveLink(mItems[i], 1);
        }
    }
    if (mItems[4]->IsEnabled()) {
        AddLink(mBackButton, 4, mItems[4]);
        AddLink(mOKButton, 4, mItems[4]);
    } else if (mItems[3]->IsEnabled()) {
        AddLink(mBackButton, 4, mItems[3]);
        AddLink(mOKButton, 4, mItems[3]);
    } else {
        AddLink(mBackButton, 4, mItems[2]);
        AddLink(mOKButton, 4, mItems[2]);
    }
    for (i = 0; i < 5; i++) {
        AddLink(mItems[i], 3, left[i]);
        AddLink(mItems[i], 1, right[i]);
    }
    mItemsLinked = TRUE;
}

void DinoUiOrderSelectMenu::SelectItem(int index) {
    int i;
    for (i = 0; i < 5; i++) {
        mItems[i]->Enable();
    }
    mItems[index]->Disable();
    for (i = 0; i < 5; i++) {
        mUnkF8[i]->Disable();
    }
    mBackButton->MMenuItem::Enable();
    mBackButton->DinoHelpItem::Enable();
    mBackButton->mMotion->Show();
    mOKButton->MMenuItem::Disable();
    mOKButton->DinoHelpItem::Disable();
    mOKButton->mMotion->Hide();
    RemoveLink(mTabs[0], 3);
    AddLink(mTabs[0], 3, mBackButton);
    if (mHasFourthTab) {
        RemoveLink(mTabs[3], 1);
        AddLink(mTabs[3], 1, mBackButton);
    } else {
        RemoveLink(mTabs[2], 1);
        AddLink(mTabs[2], 1, mBackButton);
    }
    LinkItems();
}

void DinoUiOrderSelectMenu::SetPage(u8 page) {
    if (page > 3 || (!mHasFourthTab && page == 3)) {
        return;
    }
    BOOL first = FALSE;
    if (mPage == 0xFF) {
        first = TRUE;
    }
    if (mPage == page) {
        return;
    }
    mPage = page;
    if (!first) {
        DinoSoundPlaySe(0x94, 0);
    }
    if (mUnkD0 != NULL) {
        mParallel.Remove(mUnkD0);
        delete mUnkD0;
    }
    mUnkD0 = new MTextPicture(*mPictures[page], TRUE, false);
    mParallel.AddTail(mUnkD0);
    mUnkD0->Show();
    mUnkD0->SetPriority(3);
    for (int i = 0; i < 5; i++) {
        RemoveLink(mItems[i], 2);
        AddLink(mItems[i], 2, mTabs[page]);
    }
    Disable();
    ShowPage(0xFF);
    if (first) {
        SetInfoPage(page);
        return;
    }
    GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov006_02197110.SetInfoPage_08, page));
}

extern "C" struct {
    void (DinoUiOrderSelectMenu::*FinishPage_00)();
} data_ov006_02197138;
extern const u32 data_0208aad0;

// Where the cursor sits on each tab.
extern MPoint data_020b5890[4];
// (The TU's pooled offsets and the method pointers this function's call tasks read.)
extern "C" struct {
    u8 unk00[0x5C];
    MPoint nameOffset;      // 0x5C: of the creature's name from its medal
} data_ov006_021970cc;
extern "C" struct {
    u8 unk00[0x24];
    MPoint buttonPos;       // 0x24: the back and decide buttons' (one at a time)
    u8 unk2C[0x54 - 0x2C];
    MPoint cursorOffset;    // 0x54: of the cursor's frame on a medal
} data_ov006_0219714c;
extern "C" struct {
    void (DinoHelpBase::*Enable)();
} data_ov006_021970f8, data_ov006_02197148;
extern "C" void func_ov003_02156eac(DinoUiRvInformation *info, MFx32 value);

// More of the TU's pooled statics, for OnStart.
extern "C" u32 data_ov006_02196ecc[], data_ov006_02196fcc[];  // path pools (the back button's, the decide button's)
extern "C" const MPoint data_ov006_02197278[];  // the team tech's places
extern "C" std::string data_ov006_02197368[4], data_ov006_02197398[4];   // the pages' pictures (with/without the 4th)
extern "C" struct {
    void (DinoHelpBase::*Disable)();
} data_ov006_021971a8, data_ov006_021971b8;
extern "C" struct {
    void (DinoHelpBase::*Enable)();
} data_ov006_02197198, data_ov006_021971b0;
// OnStart's function-local static: the tabs' help texts, set on the first call (0x1C into data_ov006_0219724c).
extern "C" u32 data_ov006_0219724c[];
extern "C" u32 data_ov006_02197268[4];
extern "C" u32 data_ov006_021991ac;
extern MRect data_020b58e0[4];  // the tabs' frames
extern const u32 data_0208a504, data_0208a508, data_0208a50c, data_0208a510, data_0208a514, data_0208a518,
    data_0208a51c, data_0208a520, data_0208a524, data_0208a528, data_0208a52c, data_0208a554, data_0208a558,
    data_0208a55c, data_0208a710, data_0208a940, data_0208ab18;

inline DinoUiOrderSelectBackButton::DinoUiOrderSelectBackButton(DinoUiOrderSelectMenu *owner)
    : DinoUiOrderSelectMenuItem(owner, ((const char **)data_ov006_02196ecc)[5], data_ov006_0219714c.buttonPos,
                                data_0208ab18) {
    SetKeys(2);
    SetMode(4);
    mMotion->Play(2, 0);
    mMotion->GetUnit()->SetMode(1);
}

inline DinoUiOrderSelectDecideButton::DinoUiOrderSelectDecideButton(DinoUiOrderSelectMenu *owner)
    : DinoUiOrderSelectMenuItem(owner, ((const char **)data_ov006_02196fcc)[7], data_ov006_0219714c.buttonPos,
                                data_0208a504) {
    SetKeys(8);
    mMotion->GetUnit()->SetMode(1);
}

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(const MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = const_cast<MResource<DinoExcavateDefs> &>(defs).Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

#pragma auto_inline off
// The help areas, the back (hidden) and decide buttons, the tabs, both teams, the cursor, the name, the information,
// the pages' pictures and the first page; then fades in from white, and shows the battle's rules if it has any.
void DinoUiOrderSelectMenu::OnStart() {
    SetLinkFallback(1);
    GetInput()->SetRepeat(0xF0, GetExcavate7C(gDinoResource->GetExcavateDefs())[7],
                          GetExcavate7C(gDinoResource->GetExcavateDefs())[8]);
    mKasekiDefs = gDinoResource->LoadKasekiDefs();
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, mUnk1FD);
    gDinoBackup->GetTeam(mTeamIndex, &mTeam);
    mOam = new MSmartOam(0x29, 0, TRUE);
    mUnk200 = new MParallelTaskSet();
    mParallel.AddTail(mUnk200);
    mParallel.AddTail(mHelp21C = new DinoHelpDummyItem(gLcd->GetTopDisplay(), MRect(0x1B, 3, 0x43, 0x12),
                                                       std::string(gDinoResource->GetText(data_0208a55c))));
    mParallel.AddTail(new DinoHelpDummyItem(gLcd->GetTopDisplay(), MRect(0x44, 3, 0x53, 0x13),
                                            std::string(gDinoResource->GetText(data_0208a558))));
    mParallel.AddTail(mHelp220 = new DinoHelpDummyItem(gLcd->GetTopDisplay(), MRect(0x58, 3, 0xEC, 0x13),
                                                       std::string(gDinoResource->GetText(data_0208a554))));
    mParallel.AddTail(mHelps[0] = new DinoHelpDummyItem(GetDisplay(), MRect(0xE, 0xA, 0xF2, 0x2D),
                                                        std::string(gDinoResource->GetText(data_0208a52c))));
    mParallel.AddTail(mHelps[1] = new DinoHelpDummyItem(GetDisplay(), MRect(0x94, 0x30, 0xF6, 0x3C),
                                                        std::string(gDinoResource->GetText(data_0208a528))));
    mParallel.AddTail(mHelps[2] = new DinoHelpDummyItem(GetDisplay(), MRect(0x19, 0x51, 0x5A, 0x5E),
                                                        std::string(gDinoResource->GetText(data_0208a524))));
    mParallel.AddTail(mHelps[3] = new DinoHelpDummyItem(GetDisplay(), MRect(0x26, 0x65, 0x9C, 0x72),
                                                        std::string(gDinoResource->GetText(data_0208a520))));
    mParallel.AddTail(mHelps[4] = new DinoHelpDummyItem(GetDisplay(), MRect(0x1C, 0x78, 0x48, 0x9B),
                                                        std::string(gDinoResource->GetText(data_0208a51c))));
    mParallel.AddTail(mHelps[5] = new DinoHelpDummyItem(GetDisplay(), MRect(0x4A, 0x78, 0x9B, 0x9B),
                                                        std::string(gDinoResource->GetText(data_0208a518))));
    mParallel.AddTail(mHelps[6] = new DinoHelpDummyItem(GetDisplay(), MRect(0xA4, 0x78, 0xEE, 0x9B),
                                                        std::string(gDinoResource->GetText(data_0208a514))));
    mBackButton = new DinoUiOrderSelectBackButton(this);
    AddItem(mBackButton);
    mOKButton = new DinoUiOrderSelectDecideButton(this);
    AddItem(mOKButton);
    mBackButton->MMenuItem::Disable();
    mBackButton->DinoHelpItem::Disable();
    mBackButton->mMotion->Hide();
    u32 guard = data_ov006_021991ac;
    if (!(guard & 1)) {
        DINO_POOL_WORD(data_ov006_0219724c, 0x1c) = data_0208a510;
        DINO_POOL_WORD(data_ov006_0219724c, 0x20) = data_0208a50c;
        DINO_POOL_WORD(data_ov006_0219724c, 0x24) = data_0208a508;
        DINO_POOL_WORD(data_ov006_0219724c, 0x28) = data_0208a710;
        DINO_POOL_WORD(data_ov006_021991ac, 0) = guard | 1;
    }
    for (int i = 0; i < 4; i++) {
        mTabs[i] = new DinoUiPartsTabIconMenuItem(data_ov006_02197268[i], data_020b58e0[i]);
        AddItem(mTabs[i]);
    }
    AddEntry((u32)mHelps[1], 2, (u32)mHelps[2]);
    AddEntry((u32)mHelps[1], 3, (u32)mHelps[2]);
    AddEntry((u32)mHelps[2], 4, (u32)mHelps[1]);
    AddEntry((u32)mHelps[2], 2, (u32)mHelps[3]);
    AddEntry((u32)mHelps[2], 1, (u32)mHelps[1]);
    AddEntry((u32)mHelps[3], 3, 0);
    AddEntry((u32)mHelps[3], 1, 0);
    AddEntry((u32)mHelps[4], 4, (u32)mHelps[3]);
    AddEntry((u32)mHelps[4], 2, (u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabs[0]);
    AddEntry((u32)mHelps[5], 2, (u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabs[2]);
    AddEntry((u32)mHelps[6], 4, (u32)mHelps[3]);
    AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabs[2], 4, (u32)mHelps[5]);
    AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabs[2], 1, (u32)(DinoHelpItem *)mOKButton);
    AddEntry((u32)(DinoHelpItem *)mOKButton, 3, (u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabs[2]);
    if (mHasFourthTab) {
        AddLink(mBackButton, 3, mTabs[3]);
        AddLink(mOKButton, 3, mTabs[3]);
        AddLink(mTabs[3], 1, mOKButton);
    } else {
        mTabs[3]->Disable();
        ((DinoUiPartsTabIconMenuItem *)mTabs[3])->DinoHelpItem::Disable();
        AddLink(mBackButton, 3, mTabs[2]);
        AddLink(mOKButton, 3, mTabs[2]);
        AddLink(mTabs[2], 1, mOKButton);
    }
    AddLink(mBackButton, 1, mTabs[0]);
    AddLink(mOKButton, 1, mTabs[0]);
    AddLink(mTabs[0], 3, mOKButton);
    CreateOpponentItems();
    CreateNameLabels((const wchar_t *)player.name);
    mCursorTasks = new MParallelTaskSet();
    mCursor = new DinoUiFieldMenuCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(0);
    mCursor->Hide();
    mParallel.AddTail(mCursorTasks);
    mParallel.AddTail(mOam);
    mName = new DinoUiPartsReviverName();
    mParallel.AddTail(mName);
    mName->Hide();
    mUnk1BC = new MSerialTaskSet();
    mParallel.AddTail(mUnk1BC);
    mInfo = new DinoUiRvInformation(0);
    mParallel.AddTail(mInfo);
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208a940))));
    mParallel.AddTail(new DinoUiPartsBlackBelt());
    mTech = new DinoUiPartsTeamTech(data_ov006_02197278);
    mParallel.AddTail(mTech);
    mTech->Hide();
    if (mHasFourthTab) {
        mInfo->SetUnk1AC(3);
        for (int i = 0; i < 4; i++) {
            mPictures[i] = new MPictureFile(data_ov006_02197368[i], 7);
        }
    } else {
        for (int i = 0; i < 4; i++) {
            mPictures[i] = new MPictureFile(data_ov006_02197398[i], 7);
        }
    }
    SetPage(0);
    CreateItems();
    RefreshItems();
    FocusItem(mItems[0]);
    SetSelectedItem((DinoUiPartsTabIconMenuItem *)mTabs[0]);
    DinoDimScreen();
    MTask *main;
    MTask *sub;
    sub = gLcd->mSub->mBright->CreateFadeInFromWhite(30, 0);
    main = gLcd->mMain->mBright->CreateFadeInFromWhite(30, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(main);
    fades->AddTail(sub);
    mSerial.AddTail(fades);
    mCursor->Hide();
    MTask *play;
    MTask *show;
    show = mCursor->CreateShowTransfer();
    play = mCursor->CreatePlayTask();
    MParallelTaskSet *cursor = new MParallelTaskSet(TRUE);
    cursor->AddTail(play);
    cursor->AddTail(show);
    mSerial.AddTail(cursor);
    if (!mUnk1F7) {
        DinoSoundLoadScene(0xE);
        DinoSoundPlayBgm(0x6A, 0);
    }
    if (mUnk1F8 != 0) {
        MResource<DinoBattleData> battle = gDinoResource->LoadBattle(mUnk1F8);
        if (battle.Get()->unk44 != 0) {
            GetSerial()->AddTail(MCreateCallTask(this, data_ov006_02197198.Enable),
                            new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), GetUnk44Text(), 0xB4, 4),
                            MCreateCallTask(this, data_ov006_021971a8.Disable));
        }
        if (battle.Get()->unk3C != 0) {
            GetSerial()->AddTail(MCreateCallTask(this, data_ov006_021971b0.Enable),
                            new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), GetUnk3CText(), 0xB4, 4),
                            MCreateCallTask(this, data_ov006_021971b8.Disable));
        }
    }
    RefreshOKButton();
}
#pragma auto_inline reset

// Moves the cursor to the item. A member of either team also shows its creature's information, page and name (the
// player's also raise the team tech's member); an empty place hides them.
void DinoUiOrderSelectMenu::OnFocus(MMenuItem *item) {
    int i;
    mUnk1F6 = FALSE;
    mFocusTabs = FALSE;
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (i = 0; i < 4; i++) {
        if (item == mTabs[i]) {
            frame.MoveTo(data_020b5890[i]);
            mCursor->SetMode(2);
            mFocusTabs = TRUE;
        }
    }
    for (i = 0; i < 5; i++) {
        if (item == mItems[i] || item == mUnkF8[i]) {
            frame.Offset(data_ov006_0219714c.cursorOffset);
            break;
        }
    }
    if (i == 5) {
        SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    mCursor->Show();
    mTech->Lower();
    for (i = 0; i < 5; i++) {
        if (item == mItems[i]) {
            if (mTeam.slots[i] != 0) {
                if (!mHasFourthTab) {
                    mInfo->SetUnk1AC(0);
                }
                DinoCreatureState creature;
                gDinoBackup->GetCreature(mTeam.slots[i], &creature);
                mInfo->SetUnk1B1(0);
                mInfo->Unk02156ee8(*(DinoCreatureInfo *)&creature);
                MTask *enable = MCreateCallTask(this, data_ov006_021970f8.Enable);
                mSerial.AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial), enable);
                Disable();
                ShowPage(mTeam.slots[i]);
                if (i < 3) {
                    mTech->Raise((u8)i);
                }
                mName->SetCreature(mTeam.slots[i]);
                mName->SetPosition(data_ov006_02197340[i] + data_ov006_021970cc.nameOffset);
            } else {
                mInfo->Unk021571bc();
                ShowPage(1);
                mName->Hide();
            }
            return;
        }
    }
    for (i = 0; i < 5; i++) {
        if (item == mUnkF8[i]) {
            if (mOpponent.creatures[i].index != 0) {
                if (!mHasFourthTab) {
                    mInfo->SetUnk1AC(2);
                    if (mUnk1F8 != 0) {
                        MResource<DinoBattleData> battle = gDinoResource->LoadBattle(mUnk1F8);
                        if (((const u8 *)battle.Get())[0x58] != 0) {
                            mInfo->SetUnk1AC(1);
                        }
                    }
                }
                if (!mHasFourthTab) {
                    mInfo->SetUnk1B1(1);
                    func_ov003_02156eac(mInfo, mOpponent.unk84[i]);
                }
                mInfo->Unk02156ee8(*(DinoCreatureInfo *)&mOpponent.creatures[i]);
                MTask *enable = MCreateCallTask(this, data_ov006_02197148.Enable);
                mSerial.AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial), enable);
                Disable();
                ShowPage(mOpponent.creatures[i].index);
                mName->SetCreature(mOpponent.creatures[i].index);
                if (mHasFourthTab) {
                    mName->SetPosition(data_ov006_02197318[i] + data_ov006_021970cc.nameOffset);
                } else {
                    mName->SetPosition(data_ov006_021972f0[i] + data_ov006_021970cc.nameOffset);
                }
            } else {
                mInfo->Unk021571bc();
                ShowPage(1);
                mName->Hide();
            }
            return;
        }
    }
    mName->Hide();
}

// A tab turns the page. A member picks the member to swap (asking with whom); in swap mode a member swaps places with
// it.
void DinoUiOrderSelectMenu::OnDecide(MMenuItem *item) {
    int i;
    for (i = 0; i < 4; i++) {
        if (item == mTabs[i]) {
            SetPage((u8)i);
        }
    }
    if (mUnk1F0 == 1) {
        for (i = 0; i < 5; i++) {
            if (item == mItems[i]) {
                u8 member = mTeam.slots[i];
                u8 other = mTeam.slots[mFocusIndex];
                mTeam.slots[mFocusIndex] = member;
                mTeam.slots[i] = other;
                ((DinoUiPartsReviverMedalButton *)mItems[i])->PlayDecided();
                mMessage->mUnk98 = 15;
                mName->Hide();
                MParallelTaskSet *moves = new MParallelTaskSet(TRUE);
                moves->AddTail(((DinoUiPartsReviverMedalButton *)mItems[i])->CreateFlyInTask(data_ov006_02197340[mFocusIndex]));
                moves->AddTail(((DinoUiPartsReviverMedalButton *)mItems[mFocusIndex])->CreateFlyInTask(data_ov006_02197340[i]));
                GetSerial()->AddTail(new MTimer(5), moves, MTaskExitTransfer::Create(mMessage),
                                     MCreateCallTask(this, data_ov006_02197138.FinishPage_00));
                mUnk1BC->Clear();
                Disable();
                return;
            }
        }
        return;
    }
    for (i = 0; i < 5; i++) {
        if (item == mItems[i] && mTeam.slots[i]) {
            if (!mUnk1F6) {
                return;
            }
            mFocusIndex = i;
            mUnk1F0 = 1;
            mMessage = new DinoCommonWindowMessage(MPoint(0x80, 0x60), std::string(gDinoResource->GetText(data_0208aad0)), 4, 0);
            mParallel.AddTail(mMessage);
            SelectItem(mFocusIndex);
            ((DinoUiPartsReviverMedalButton *)mItems[mFocusIndex])->PlayDecided();
            if (i < 3) {
                mTech->Hide();
            }
            FocusNearestItem(GetCursor());
            return;
        }
    }
}

void DinoUiOrderSelectMenu::FinishPage() {
    mUnk1F0 = 0;
    ClearItems();
    CreateItems();
    RefreshItems();
    Enable();
    RefreshOKButton();
    FocusNearestItem(GetCursor());
}

void DinoUiOrderSelectMenu::FinishTab() {
    RefreshItems();
    FocusItem(mItems[mFocusIndex]);
    RefreshOKButton();
}

extern "C" struct {
    void (DinoUiOrderSelectMenu::*FinishTab_00)();
} data_ov006_02197108;
extern const u32 data_0208aa44, data_0208aa48, data_0208aa4c;

void DinoUiOrderSelectMenu::Confirm() {
    if (mUnk1F0 == 1) {
        mUnk1F0 = 0;
        mMessage->Exit();
        ((DinoUiPartsReviverMedalButton *)mItems[mFocusIndex])->PlayNormal();
        GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(&mBackButton->mSerial),
                             MCreateCallTask(this, data_ov006_02197108.FinishTab_00));
        return;
    }
    switch (IsTeamInvalid()) {
    case 0: {
        DinoSoundStopBgm(-1, 30);
        gDinoBackup->SetTeam(mTeamIndex, &mTeam);
        MTask *fadeMain;
        MTask *fadeSub;
        MTask *wait;
        MTask *exit = CreateExitTransfer();
        wait = mInfo->CreateWaitModelTask();
        fadeSub = gLcd->mSub->mBright->CreateFadeOutToWhite(90, 0);
        fadeMain = gLcd->mMain->mBright->CreateFadeOutToWhite(90, 0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeMain);
        fades->AddTail(fadeSub);
        GetSerial()->AddTail(new MTimer(30), fades, wait, exit);
        break;
    }
    case 1: {
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x60),
                                                       std::string(gDinoResource->GetText(data_0208aa4c)), 0x78, 4));
        break;
    }
    case 2: {
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x60),
                                                       std::string(gDinoResource->GetText(data_0208aa48)), 0x78, 4));
        break;
    }
    case 3: {
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x60),
                                                       std::string(gDinoResource->GetText(data_0208aa44)), 0x78, 4));
    }
    case 4: {
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), GetUnk44Text(), 0x78, 4));
        break;
    }
    case 5: {
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), GetUnk3CText(), 0x78, 4));
        break;
    }
    }
}

#pragma opt_strength_reduction off
// Why the team can't fight the battle: 1 without a leader, 4 with too few of the creatures the rule names a kind of,
// 5 without one of the creatures the rule names; 0 if it can.
BOOL DinoUiOrderSelectMenu::IsTeamInvalid() {
    u8 members = 0;
    if (mTeam.slots[0] != 0) {
        members += 2;
    }
    if (members == 0) {
        return 1;
    }
    if (mUnk1F8 != 0) {
        MResource<DinoBattleData> battle = gDinoResource->LoadBattle(mUnk1F8);
        if (battle.Get()->unk44 != 0 && battle.Get()->unk48 >= 1 && battle.Get()->unk48 <= 3) {
            u32 count = 0;
            for (int i = 0; i < 3; i++) {
                if (mTeam.slots[i] != 0 &&
                    gDinoResource->GetCreatureDef(mTeam.slots[i])->unk0D == battle.Get()->GetUnk44()) {
                    count++;
                }
            }
            if (battle.Get()->unk48 > count) {
                return 4;
            }
        }
        if (battle.Get()->unk3C != 0) {
            for (u32 i = 0; i < battle.Get()->unk3C; i++) {
                if (mTeam.slots[0] != battle.Get()->GetUnk40()[i] && mTeam.slots[1] != battle.Get()->GetUnk40()[i] &&
                    mTeam.slots[2] != battle.Get()->GetUnk40()[i]) {
                    return 5;
                }
            }
        }
    }
    return 0;
}
#pragma opt_strength_reduction reset

// The OK button lights up when the team can fight; the technique shows when the team has one.
void DinoUiOrderSelectMenu::RefreshOKButton() {
    if (!IsTeamInvalid()) {
        mOKButton->mMotion->GetUnit()->SetMode(1);
    } else {
        mOKButton->mMotion->GetUnit()->SetMode(0);
    }
    if (DinoTeamHasTech(&mTeam)) {
        mTech->Show();
    } else {
        mTech->Hide();
    }
}


extern const u32 data_0208aae8;
extern const u32 data_0208aaec;
extern const u32 data_0208aaf0;
extern const u32 data_0208aaf4;
extern const u32 data_0208aaf8;
extern const u32 data_0208aafc;
extern const u32 data_0208ab00;
extern const u32 data_0208ab04;
extern const u32 data_0208ab08;

std::string DinoUiOrderSelectMenu::GetUnk44Text() {
    char text[0x80];
    text[0] = 0;
    if (mUnk1F8 != 0) {
        MResource<DinoBattleData> battle = gDinoResource->LoadBattle(mUnk1F8);
        if (battle.Get()->unk44 != 0) {
            u32 arg;
            switch (battle.Get()->unk44) {
            case 1:
                arg = data_0208aafc;
                break;
            case 2:
                arg = data_0208aaf8;
                break;
            case 3:
                arg = data_0208aaf4;
                break;
            case 4:
                arg = data_0208aaf0;
                break;
            case 5:
                arg = data_0208aaec;
                break;
            case 6:
            default:
                arg = data_0208aae8;
                break;
            }
            u32 format;
            switch (battle.Get()->unk48) {
            case 1:
                format = data_0208ab08;
                break;
            case 2:
                format = data_0208ab04;
                break;
            case 3:
            default:
                format = data_0208ab00;
                break;
            }
            sprintf(text, gDinoResource->GetText(format), gDinoResource->GetText(arg));
        }
    }
    return std::string(text);
}

extern const u32 data_0208ab0c;
extern const u32 data_0208ab10;
extern const u32 data_0208ab14;

std::string DinoUiOrderSelectMenu::GetUnk3CText() {
    char text[0x80];
    if (mUnk1F8 != 0) {
        MResource<DinoBattleData> battle = gDinoResource->LoadBattle(mUnk1F8);
        switch (battle.Get()->unk3C) {
        case 1:
            sprintf(text, gDinoResource->GetText(data_0208ab14),
                    gDinoResource->GetCreatureText08(battle.Get()->GetUnk40()[0]));
            break;
        case 2: {
            u32 b = battle.Get()->GetUnk40()[1];
            u32 a = battle.Get()->GetUnk40()[0];
            sprintf(text, gDinoResource->GetText(data_0208ab10), gDinoResource->GetCreatureText08(a),
                    gDinoResource->GetCreatureText08(b));
            break;
        }
        case 3: {
            u32 c = battle.Get()->GetUnk40()[2];
            u32 b = battle.Get()->GetUnk40()[1];
            u32 a = battle.Get()->GetUnk40()[0];
            sprintf(text, gDinoResource->GetText(data_0208ab0c), gDinoResource->GetCreatureText08(a),
                    gDinoResource->GetCreatureText08(b), gDinoResource->GetCreatureText08(c));
            break;
        }
        default:
            sprintf(text, "");
            break;
        }
    }
    return std::string(text);
}

extern const u32 data_0208a4fc, data_0208a500, data_0208a530, data_0208a534, data_0208a538, data_0208a53c, data_0208a540, data_0208a544, data_0208a548, data_0208a54c, data_0208a550, data_0208a6a4, data_0208a6a8, data_0208a6ac, data_0208a6b0, data_0208a6b4, data_0208a6b8, data_0208a6bc, data_0208a6c0, data_0208a6c4, data_0208a6c8, data_0208a6cc, data_0208a6d0, data_0208a6d4, data_0208a6d8, data_0208a6dc, data_0208a6e0, data_0208a6e4, data_0208a6e8, data_0208a6ec, data_0208a6f0, data_0208a6f4, data_0208a6f8, data_0208a6fc;
extern MRect data_020b5950, data_020b5960, data_020b5970, data_020b5980, data_020b5990, data_020b59a0, data_020b59b0, data_020b59c0, data_020b59d0, data_020b59e0, data_020b59f0, data_020b5a00, data_020b5a10, data_020b5a20, data_020b5a30, data_020b5a40, data_020b5a50, data_020b5a60, data_020b5a70, data_020b5a80;

// The help areas of the top screen's page (the information panel), by what it shows: the creature, its stats, a fossil,
// or a fossil's part.
void DinoUiOrderSelectMenu::ShowPage(u8 page) {
    u8 mode;
    if (page == 0) {
        return;
    }
    if (page != 0xFF) {
        mUnk205 = page;
    }
    switch (mPage) {
    case 0:
        mode = 0;
        break;
    case 1:
    case 2:
        mode = 1;
        break;
    case 3:
        if (mUnk205 == 0) {
            mode = 2;
        } else {
            int index = (mUnk205 - 1) * 4 + 1;
            u8 uncleanable = mKasekiDefs.Get()->GetDef(index)->IsUncleanable();
            u32 part = mKasekiDefs.Get()->GetDef(index)->GetUnk38();
            if (uncleanable) {
                mode = part + 2;
            } else {
                mode = 2;
            }
        }
        break;
    }
    if (mUnk204 == mode) {
        return;
    }
    mUnk204 = mode;
    mUnk200->Clear();
    switch (mUnk204) {
    case 0:
        mUnk200->AddTail(mPageHelps[0] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5950, std::string(gDinoResource->GetText(data_0208a550))));
        mUnk200->AddTail(mPageHelps[1] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5960, std::string(gDinoResource->GetText(data_0208a54c))));
        mUnk200->AddTail(mPageHelps[2] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5970, std::string(gDinoResource->GetText(data_0208a548))));
        mUnk200->AddTail(new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5980, std::string(gDinoResource->GetText(data_0208a544))));
        mUnk200->AddTail(mPageHelps[4] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5990, std::string(gDinoResource->GetText(data_0208a540))));
        mUnk200->AddTail(mPageHelps[5] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59a0, std::string(gDinoResource->GetText(data_0208a53c))));
        mUnk200->AddTail(mPageHelps[6] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59b0, std::string(gDinoResource->GetText(data_0208a538))));
        mUnk200->AddTail(mPageHelps[7] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59c0, std::string(gDinoResource->GetText(data_0208a534))));
        mUnk200->AddTail(mPageHelps[8] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59d0, std::string(gDinoResource->GetText(data_0208a530))));
        AddEntry((u32)mHelp21C, 2, (u32)mPageHelps[0]);
        AddEntry((u32)mHelp220, 2, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[0], 2, (u32)mPageHelps[1]);
        AddEntry((u32)mPageHelps[0], 3, 0);
        AddEntry((u32)mPageHelps[0], 1, 0);
        AddEntry((u32)mPageHelps[1], 4, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[1], 1, (u32)mPageHelps[2]);
        AddEntry((u32)mPageHelps[2], 1, 0);
        AddEntry((u32)mPageHelps[4], 1, 0);
        AddEntry((u32)mPageHelps[5], 2, (u32)mPageHelps[7]);
        AddEntry((u32)mPageHelps[6], 1, 0);
        AddEntry((u32)mPageHelps[7], 3, 0);
        AddEntry((u32)mPageHelps[7], 1, 0);
        AddEntry((u32)mPageHelps[8], 3, 0);
        AddEntry((u32)mPageHelps[8], 1, 0);
        break;
    case 1:
        mUnk200->AddTail(mPageHelps[0] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59e0, std::string(gDinoResource->GetText(data_0208a500))));
        mUnk200->AddTail(mPageHelps[1] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b59f0, std::string(gDinoResource->GetText(data_0208a4fc))));
        AddEntry((u32)mPageHelps[1], 2, (u32)mHelps[0]);
        break;
    case 2:
        mUnk200->AddTail(mPageHelps[0] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a00, std::string(gDinoResource->GetText(data_0208a6fc))));
        mUnk200->AddTail(new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a10, std::string(gDinoResource->GetText(data_0208a6f8))));
        mUnk200->AddTail(new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a20, std::string(gDinoResource->GetText(data_0208a6f4))));
        mUnk200->AddTail(mPageHelps[3] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a30, std::string(gDinoResource->GetText(data_0208a6f0))));
        mUnk200->AddTail(mPageHelps[4] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a6e0))));
        mUnk200->AddTail(mPageHelps[5] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a6ec))));
        mUnk200->AddTail(mPageHelps[6] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a6e8))));
        mUnk200->AddTail(mPageHelps[7] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a6e4))));
        AddEntry((u32)mPageHelps[0], 2, (u32)mPageHelps[4]);
        AddEntry((u32)mPageHelps[3], 2, (u32)mPageHelps[4]);
        AddEntry((u32)mPageHelps[4], 4, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[4], 3, 0);
        AddEntry((u32)mPageHelps[4], 1, 0);
        AddEntry((u32)mPageHelps[5], 3, 0);
        AddEntry((u32)mPageHelps[5], 1, 0);
        AddEntry((u32)mPageHelps[6], 3, 0);
        AddEntry((u32)mPageHelps[6], 1, 0);
        AddEntry((u32)mPageHelps[7], 3, 0);
        AddEntry((u32)mPageHelps[7], 1, 0);
        break;
    case 3:
    case 4:
    case 5:
        switch (mUnk204) {
        case 3:
            mUnk200->AddTail(mPageHelps[0] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a80, std::string(gDinoResource->GetText(data_0208a6dc))));
            mUnk200->AddTail(mPageHelps[1] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a6cc))));
            mUnk200->AddTail(mPageHelps[2] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a6d8))));
            mUnk200->AddTail(mPageHelps[3] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a6d4))));
            mUnk200->AddTail(mPageHelps[4] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a6d0))));
            break;
        case 4:
            mUnk200->AddTail(mPageHelps[0] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a80, std::string(gDinoResource->GetText(data_0208a6c8))));
            mUnk200->AddTail(mPageHelps[1] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a6c4))));
            mUnk200->AddTail(mPageHelps[2] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a6c0))));
            mUnk200->AddTail(mPageHelps[3] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a6bc))));
            mUnk200->AddTail(mPageHelps[4] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a6b8))));
            break;
        case 5:
            mUnk200->AddTail(mPageHelps[0] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a80, std::string(gDinoResource->GetText(data_0208a6b4))));
            mUnk200->AddTail(mPageHelps[1] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a6b0))));
            mUnk200->AddTail(mPageHelps[2] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a6ac))));
            mUnk200->AddTail(mPageHelps[3] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a6a8))));
            mUnk200->AddTail(mPageHelps[4] = new DinoHelpDummyItem(gLcd->GetTopDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a6a4))));
            break;
        }
        AddEntry((u32)mHelp21C, 2, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[0], 3, 0);
        AddEntry((u32)mPageHelps[0], 1, 0);
        AddEntry((u32)mPageHelps[1], 3, 0);
        AddEntry((u32)mPageHelps[1], 1, 0);
        AddEntry((u32)mPageHelps[2], 3, 0);
        AddEntry((u32)mPageHelps[2], 1, 0);
        AddEntry((u32)mPageHelps[3], 3, 0);
        AddEntry((u32)mPageHelps[3], 1, 0);
        AddEntry((u32)mPageHelps[4], 3, 0);
        AddEntry((u32)mPageHelps[4], 1, 0);
        break;
    }
}
