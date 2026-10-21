// DinoUiPartsTabIconMenuItem, DinoUiStatusMenuItem, DinoUiStatusChangeModeHelpTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include <algorithm>
#include "game/dino_ui_parts_reviver_medal_button.h"
#include "game/dino_ui_parts_tab_icon_menu_item_menu.h"
#include "game/dino_ui_parts_tab_icon_menu_item.h"
#include "game/dino_ui_status_menu_item.h"
#include "game/dino_ui_status_change_mode_help_task.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "game/dino_ui_parts_reviver_name.h"
#include "game/dino_ui_parts_reviver_name_count_check_task.h"
#include "game/dino_ui_field_menu_cursor.h"
#include "game/dino_common_window_message_box.h"
#include "game/common_window_message.h"
#include "m/view.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "m/bright.h"
#include "m/smart_oam.h"
#include "m/motion.h"
#include "game/dino_backup.h"
#include "game/dino_team.h"
#include "game/dino_ui_parts_team_tech.h"
#include "game/dino_static_init.h"
#include "game/dino_ui_field_menu_number.h"
#include "game/dino_ui_parts_number8.h"
#include "game/dino_ui_common.h"
#include "game/dino_guide_message.h"
#include "m/font.h"
#include "m/oam_canvas.h"
#include "m/input.h"
#include "m/picture.h"
#include "m/indicator.h"
#include "m/display.h"
#include "m/3d_scene.h"
#include "m/3d.h"
#include "m/anm_model.h"
#include "m/3d_model.h"
#include "m/file.h"

#pragma auto_inline off
DinoUiStatusMenu::DinoUiStatusMenu(int *choice, DinoTeam *team, DinoTeam *team2)
    : MMenu(gLcd->GetBottomDisplay(), NULL, 0), mResult(choice), mTeam(team), mUnk11C((u32)team2), mUnk120(0),
      mSelected(0), mMode(0), mUnk12D(0) {
}
#pragma auto_inline on

DinoUiPartsTabIconMenuItem::~DinoUiPartsTabIconMenuItem() {
}

DinoUiStatusMenuItem::~DinoUiStatusMenuItem() {
}

DinoUiStatusChangeModeHelpTask::~DinoUiStatusChangeModeHelpTask() {
}

DinoUiStatusMenu::~DinoUiStatusMenu() {
}

DinoUiStatusBackButton::~DinoUiStatusBackButton() {
}

DinoUiStatusOKButton::~DinoUiStatusOKButton() {
}

void DinoUiStatusChangeModeHelpTask::Idle() {
}

void DinoUiPartsTabIconMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiPartsTabIconMenuItem::OnDecide() {
}

void DinoUiPartsTabIconMenuItem::OnFocusChanged(BOOL value) {
}

MRect DinoUiStatusMenuItem::GetRect() {
    return GetFrame();
}

MRect DinoUiPartsTabIconMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiStatusMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiPartsTabIconMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiStatusMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiStatusMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiPartsTabIconMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiStatusMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiStatusOKButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnOKDecided();
    DinoSoundPlaySe(0x9A, 0);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
// Where things go on the screen (filled in at start).
struct DinoUiStatusLayout {
    u8 unk00[0xC];
    MPoint backPos;             // 0x0C
    MPoint memberMenuOffset;    // 0x14: from the member's medal
    u8 unk1C[0x2C - 0x1C];
    MPoint memberFocusOffset;   // 0x2C: of the cursor on a member's medal
};
extern DinoUiStatusLayout data_ov006_02190e9c;
struct DinoUiStatusLayout2 {
    u8 unk00[0x8];
    const char *okPath;         // 0x08
    const char *bodyPathFormat; // 0x0C: the player's body model
    const char *headPathFormat; // 0x10: and head model
    const char *backPath;       // 0x14
    const char *rankPath;       // 0x18
    u8 unk1C[0x2C - 0x1C];
    MPoint okPos;               // 0x2C
    u8 unk34[0x54 - 0x34];
    MPoint nameOffset;          // 0x54: of the member's name from its medal
};
extern DinoUiStatusLayout2 data_ov006_02190e1c;
extern "C" const MPoint data_ov006_02191000[5];
extern MPoint data_020b5858[3];

// The TU's pooled statics OnStart reads.
extern "C" const MPoint data_ov006_02190e68, data_ov006_02190e50;   // the player's and the team's names
extern "C" const MPoint data_ov006_02190eb8, data_ov006_02190e78;   // the two KP numbers
extern "C" const MPoint data_ov006_02190e98;    // the money
extern "C" const MPoint data_ov006_02190f68[];  // the team tech's places
extern "C" const char data_ov006_021913b8[];    // the background
extern MRect data_020b58b0, data_020b58c0, data_020b58d0;   // the tabs' frames
extern const u32 data_0208a884, data_0208a888, data_0208a88c, data_0208a890, data_0208a894, data_0208a898,
    data_0208a89c, data_0208a8a0, data_0208a8a4, data_0208a8a8, data_0208a8ac, data_0208a8b0, data_0208a8b4,
    data_0208a8b8, data_0208a968, data_0208aa4c;
extern const int data_02089a10, data_02089a24, data_02089a28, data_02089a38, data_02089a3c, data_02089a40;
void DinoGetBattleKP(u32 rank, u32 *a, u32 *b);

inline DinoUiStatusBackButton::DinoUiStatusBackButton(DinoUiStatusMenu *owner)
    : DinoUiStatusMenuItem(owner, data_ov006_02190e1c.backPath, data_ov006_02190e9c.backPos, data_0208a884) {
    SetKeys(2);
    SetMode(4);
}

inline DinoUiStatusOKButton::DinoUiStatusOKButton(DinoUiStatusMenu *owner)
    : DinoUiStatusMenuItem(owner, data_ov006_02190e1c.okPath, data_ov006_02190e1c.okPos, data_0208a888) {
    SetKeys(8);
}

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = defs.Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

#pragma opt_common_subs off
#pragma auto_inline off
// The background, the help areas, the back and OK buttons and the tabs, the team (unless it's the tutorial), the
// player's and the team's names, the rank, the KP and the money, the cursor and the name; then the fade in (and, for
// an empty team at a battle, a message).
void DinoUiStatusMenu::OnStart() {
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, FALSE);
    mUnk108 = new MParallelTaskSet();
    mParallel.AddTail(mUnk108);
    mHelp = NULL;
    mParallel.AddTail(mBackground = new MTextPicture(std::string(data_ov006_021913b8), TRUE, false));
    mBackground->Show();
    mBackground->SetPriority(3);
    mOam = new MSmartOam(0x28, 0, TRUE);
    DinoHelpDummyItem *player1, *player2, *player3, *player4, *player5, *team, *tab1, *tab3;
    mParallel.AddTail(player1 = new DinoHelpDummyItem(GetDisplay(), MRect(8, 8, 0x62, 0x15),
                                                      std::string(gDinoResource->GetText(data_0208a8b8))));
    mParallel.AddTail(player2 = new DinoHelpDummyItem(GetDisplay(), MRect(0xC, 0x15, 0xB2, 0x37),
                                                      std::string(gDinoResource->GetText(data_0208a8b4))));
    mParallel.AddTail(player3 = new DinoHelpDummyItem(GetDisplay(), MRect(0xC, 0x39, 0xB2, 0x47),
                                                      std::string(gDinoResource->GetText(data_0208a8b0))));
    mParallel.AddTail(player4 = new DinoHelpDummyItem(GetDisplay(), MRect(0xC, 0x49, 0xB2, 0x57),
                                                      std::string(gDinoResource->GetText(data_0208a8ac))));
    mParallel.AddTail(player5 = new DinoHelpDummyItem(GetDisplay(), MRect(0xC, 0x59, 0xB2, 0x68),
                                                      std::string(gDinoResource->GetText(data_0208a8a8))));
    mParallel.AddTail(team = new DinoHelpDummyItem(GetDisplay(), MRect(8, 0x72, 0x8F, 0x7F),
                                                   std::string(gDinoResource->GetText(data_0208a8a4))));
    mParallel.AddTail(tab1 = new DinoHelpDummyItem(GetDisplay(), MRect(0x15, 0x83, 0x43, 0xAB),
                                                   std::string(gDinoResource->GetText(data_0208a8a0))));
    mParallel.AddTail(new DinoHelpDummyItem(GetDisplay(), MRect(0x45, 0x83, 0x99, 0xAB),
                                            std::string(gDinoResource->GetText(data_0208a89c))));
    mParallel.AddTail(tab3 = new DinoHelpDummyItem(GetDisplay(), MRect(0x9D, 0x83, 0xEA, 0xAB),
                                                   std::string(gDinoResource->GetText(data_0208a898))));
    mUnkB8 = new DinoUiStatusBackButton(this);
    AddItem(mUnkB8);
    mOKButton = new DinoUiStatusOKButton(this);
    AddItem(mOKButton);
    mTabItems[0] = new DinoUiPartsTabIconMenuItem(data_0208a894, data_020b58b0);
    AddItem(mTabItems[0]);
    mTabItems[1] = new DinoUiPartsTabIconMenuItem(data_0208a890, data_020b58c0);
    AddItem(mTabItems[1]);
    mTabItems[2] = new DinoUiPartsTabIconMenuItem(data_0208a88c, data_020b58d0);
    AddItem(mTabItems[2]);
    AddLink(mUnkB8, 3, mOKButton);
    AddLink(mOKButton, 1, mUnkB8);
    AddLink(mOKButton, 3, mTabItems[2]);
    AddLink(mTabItems[2], 1, mOKButton);
    AddLink(mTabItems[0], 3, mUnkB8);
    AddLink(mUnkB8, 1, mTabItems[0]);
    AddEntry((u32)player1, 1, 0);
    AddEntry((u32)player2, 3, 0);
    AddEntry((u32)player3, 1, 0);
    AddEntry((u32)player4, 1, 0);
    AddEntry((u32)player5, 1, 0);
    AddEntry((u32)team, 1, 0);
    AddEntry((u32)tab1, 2, (u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mTabItems[0]);
    AddEntry((u32)tab3, 4, (u32)team);
    if (!gDinoBackup->GetSystemFlag(2)) {
        CreateItems();
        RefreshItems();
    }
    MFont *font = gDinoResource->CreateFont12(0, 10, 1);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, MPoint(0xA0, 0x10), gDinoResource->GetUnkTable(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    mPlayerName = new MIndicator(canvas, font, 0);
    mParallel.AddTail(mPlayerName);
    mPlayerName->SetPosition(data_ov006_02190e68);
    mPlayerName->SetPriority(3);
    mPlayerName->Show();
    mPlayerName->SetText(std::wstring((const wchar_t *)player.name));
    canvas = new MObjCanvas(gCurrentDisplay, MPoint(0xA0, 0x10), gDinoResource->GetUnkTable(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    mTeamName = new MIndicator(canvas, font, 0);
    mParallel.AddTail(mTeamName);
    mTeamName->SetPosition(data_ov006_02190e50);
    mTeamName->SetPriority(3);
    mTeamName->Show();
    mTeamName->SetText(std::wstring((const wchar_t *)mTeam->name));
    mRank = new MMotion(std::string(data_ov006_02190e1c.rankPath), mOam, 2, 1);
    mRank->Play(player.horiderLevel, 0);
    mRank->SetPosition(0x84, 0x60);
    mRank->SetPriority(3);
    mRank->Show();
    mParallel.AddTail(mRank);
    mRank->GetUnit()->SetMode(1);
    u32 kp, maxKP;
    DinoGetBattleKP(player.horiderLevel, &kp, &maxKP);
    mKP[0] = new DinoUiFieldMenuNumber(mOam, data_ov006_02190eb8);
    mKP[0]->SetValue(kp);
    mParallel.AddTail(mKP[0]);
    mKP[1] = new DinoUiFieldMenuNumber(mOam, data_ov006_02190e78);
    mKP[1]->SetValue(maxKP);
    mParallel.AddTail(mKP[1]);
    DinoUiPartsNumber8 *money = new DinoUiPartsNumber8(data_ov006_02190e98);
    money->SetValue(player.money);
    mParallel.AddTail(money);
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
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208a968))));
    mUnk110 = new MSerialTaskSet();
    mParallel.AddTail(mUnk110);
    mTech = new DinoUiPartsTeamTech(data_ov006_02190f68);
    mParallel.AddTail(mTech);
    mTech->Hide();
    CreatePlayerModel();
    FocusItem(mTabItems[0]);
    mTabItems[1]->SetKeys(0x400);
    mTabItems[2]->SetKeys(0x800);
    RefreshOKButton();
    DinoDimScreen();
    mUnk130 = new MParallelTaskSet();
    mParallel.AddTail(mUnk130);
    if (*mResult == 99) {
        mUnk130->AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(data_02089a40, data_02089a3c));
        mCursor->Hide();
        mSerial.AddTail(new MTimer(data_02089a38), mCursor->CreateShowTransfer(), mCursor->CreatePlayTask());
    } else {
        mSerial.AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(data_02089a28, data_02089a24));
    }
    if (*mResult == 98 && mTeam->slots[0] == 0) {
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), std::string(gDinoResource->GetText(data_0208aa4c)),
                                                       data_02089a10, 4));
    }
    *mResult = 0;
}
#pragma opt_common_subs reset
#pragma auto_inline reset

// Moves the cursor to the item; a member's medal also shows the member's name and (in the team view) raises its
// technique, or (while a replacement is picked) restarts the help.
void DinoUiStatusMenu::OnFocus(MMenuItem *item) {
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
        if (item == mMembers[i]) {
            frame.Offset(data_ov006_02190e9c.memberFocusOffset);
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    mCursor->Show();
    for (i = 0; i < 5; i++) {
        if (item == mMembers[i] && mTeam->slots[i]) {
            mName->SetCreature(mTeam->slots[i]);
            mName->SetPosition(data_ov006_02191000[i] + data_ov006_02190e1c.nameOffset);
            if (mMode == 0) {
                if (i < 3) {
                    mTech->Raise((u8)i);
                }
                return;
            }
            DinoUiStatusChangeModeHelpTask *help = mHelp;
            if (help != NULL) {
                DinoUiPartsReviverName *name = mName;
                help->Disable();
                help->mSerial.Clear();
                help->GetSerial()->AddTail(new DinoUiPartsReviverNameCountCheckTask(name),
                                           MCreateCallTask(help, &DinoHelpBase::Enable));
            }
            return;
        }
    }
    mName->Hide();
    mUnk110->Clear();
    Enable();
    mTech->Lower();
}

#pragma auto_inline off
template class MCallTask0<DinoUiStatusMenu, DinoUiStatusMenu>;
template MTask *MCreateCallTask(DinoUiStatusMenu *obj, void (DinoUiStatusMenu::*func)());
template class MCallTask0<DinoUiStatusChangeModeHelpTask, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiStatusChangeModeHelpTask *obj, void (DinoHelpBase::*func)());
#pragma auto_inline reset

void DinoUiStatusBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

MRect DinoUiPartsTabIconMenuItem::GetFrame() {
    return mFrame;
}

void DinoUiStatusMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 3; i++) {
        if (item == mTabItems[i]) {
            SelectTab(i);
            return;
        }
    }
    for (int i = 0; i < 5; i++) {
        if (item == mMembers[i]) {
            if (mMode == 1) {
                DecideMember(i);
            } else if (mTeam->slots[i]) {
                mSelected = i;
                SelectMember(i);
            }
            return;
        }
    }
}

#pragma auto_inline off

DinoUiStatusMenuItem::DinoUiStatusMenuItem(DinoUiStatusMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(2, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(2);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    mMotion->GetUnit()->SetMode(1);
}

void DinoUiStatusMenu::FinishPage() {
    ClearItems();
    CreateItems();
    RefreshItems();
    RefreshOKButton();
    FocusNearestItem(GetCursor());
}

void DinoUiStatusMenu::FinishTab() {
    mMembers[mSelected]->PlayNormal();
    FocusItem(mMembers[mSelected]);
    RefreshOKButton();
}

void DinoUiStatusMenu::ClearItems() {
    RemoveLink(mUnkB8, 4);
    for (int i = 0; i < 5; i++) {
        RemoveItem(mMembers[i]);
        delete mMembers[i];
    }
    mUnk12D = 0;
}

// The OK button lights up when the first member can be picked; the technique shows when the team has one.
void DinoUiStatusMenu::RefreshOKButton() {
    if (mTeam->slots[0]) {
        mOKButton->mMotion->GetUnit()->SetMode(1);
    } else {
        mOKButton->mMotion->GetUnit()->SetMode(0);
    }
    if (!gDinoBackup->GetSystemFlag(2)) {
        if (DinoTeamHasTech(mTeam)) {
            mTech->Show();
        } else {
            mTech->Hide();
        }
    }
}

void DinoUiStatusMenu::RefreshItems() {
    for (int i = 0; i < 5; i++) {
        mMembers[i]->Enable();
    }
    for (int j = 0; j < 3; j++) {
        mTabItems[j]->Enable();
    }
    mOKButton->MMenuItem::Enable();
    LinkMembers();
    for (int i = 0; i < 5; i++) {
        RemoveLink(mMembers[i], 2);
        AddLink(mMembers[i], 2, mTabItems[0]);
    }
    mUnk108->Clear();
    mHelp = NULL;
}

// (Data the item positions come from.)
extern "C" const MPoint data_ov006_02191000[5];
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

void DinoUiStatusMenu::CreateItems() {
    for (int i = 0; i < 5; i++) {
        DinoCreatureState creature;
        gDinoBackup->GetCreature(mTeam->slots[i], &creature);
        u8 maxed;
        if (GetCreatureLevel(creature) == 12) {
            maxed = TRUE;
        } else {
            maxed = FALSE;
        }
        mMembers[i] = new DinoUiPartsReviverMedalButton(mOam, data_ov006_02191000[i], mTeam->slots[i], maxed, 0);
        AddItem(mMembers[i]);
        mMembers[i]->SetMode(2);
        AddLink(mMembers[i], 2, mTabItems[0]);
        AddLink(mMembers[i], 4, mUnkB8);
    }
    AddLink(mTabItems[0], 2, mMembers[0]);
    AddLink(mTabItems[1], 2, mMembers[0]);
    AddLink(mTabItems[2], 2, mMembers[0]);
    AddLink(mOKButton, 2, mMembers[0]);
    AddLink(mUnkB8, 2, mMembers[0]);
}

// Links the members left and right to the nearest enabled ones (round the row), and the tabs and buttons down to the
// first enabled member.
void DinoUiStatusMenu::LinkMembers() {
    MMenuItem *left[5];
    MMenuItem *right[5];
    int i;
    for (i = 0; i < 5; i++) {
        left[i] = mMembers[(i + 4) % 5];
        right[i] = mMembers[(i + 1) % 5];
    }
    int first = 0;
    for (i = 0; i < 5; i++) {
        if (mMembers[i]->IsEnabled()) {
            first = i;
            break;
        }
    }
    for (i = 0; i < 5; i++) {
        int k = first + i;
        if (!mMembers[k % 5]->IsEnabled()) {
            left[(k + 1) % 5] = left[k % 5];
        }
    }
    int start = (first + 1) % 5;
    for (i = 4; i >= 0; i--) {
        int k = start + i;
        if (!mMembers[k % 5]->IsEnabled()) {
            right[(k + 4) % 5] = right[k % 5];
        }
    }
    if (mUnk12D) {
        RemoveLink(mUnkB8, 4);
        for (i = 0; i < 5; i++) {
            RemoveLink(mMembers[i], 3);
            RemoveLink(mMembers[i], 1);
        }
    }
    if (mMembers[4]->IsEnabled()) {
        AddLink(mUnkB8, 4, mMembers[4]);
    } else if (mMembers[3]->IsEnabled()) {
        AddLink(mUnkB8, 4, mMembers[3]);
    } else {
        AddLink(mUnkB8, 4, mMembers[2]);
    }
    for (i = 0; i < 5; i++) {
        AddLink(mMembers[i], 3, left[i]);
        AddLink(mMembers[i], 1, right[i]);
    }
    if (mMembers[0]->IsEnabled()) {
        RemoveLink(mTabItems[0], 2);
        RemoveLink(mTabItems[1], 2);
        RemoveLink(mTabItems[2], 2);
        RemoveLink(mOKButton, 2);
        RemoveLink(mUnkB8, 2);
        AddLink(mTabItems[0], 2, mMembers[0]);
        AddLink(mTabItems[1], 2, mMembers[0]);
        AddLink(mTabItems[2], 2, mMembers[0]);
        AddLink(mOKButton, 2, mMembers[0]);
        AddLink(mUnkB8, 2, mMembers[0]);
    } else {
        RemoveLink(mTabItems[0], 2);
        RemoveLink(mTabItems[1], 2);
        RemoveLink(mTabItems[2], 2);
        RemoveLink(mOKButton, 2);
        RemoveLink(mUnkB8, 2);
        AddLink(mTabItems[0], 2, mMembers[1]);
        AddLink(mTabItems[1], 2, mMembers[1]);
        AddLink(mTabItems[2], 2, mMembers[1]);
        AddLink(mOKButton, 2, mMembers[1]);
        AddLink(mUnkB8, 2, mMembers[1]);
    }
    mUnk12D = 1;
}

extern const int data_02089a20; // fade frames
extern const int data_02089a88;

// The player's character data (as DinoField reads it).
struct DinoStatusCharaData {
    u32 unk00;
    u32 bodyModelCount; // 0x04
    u32 bodyModelNames; // 0x08: offset of the offsets of their names
    u32 headModelCount; // 0x0C
    u32 headModelNames; // 0x10
};

// The 3D scene's heap (in ov003's memory), the body's rotation and the joint the head goes on.
extern "C" u8 data_ov003_021624cc[];
extern "C" const VecFx32 data_ov006_02190040;
extern "C" const char data_ov006_021913cc[];

// The player's model in the corner: the body, then the head (or the head mask worn) on its head joint.
void DinoUiStatusMenu::CreatePlayerModel() {
    char path[0x100];
    MResource<DinoCharaData> chara = gDinoResource->LoadChara(1);
    DinoCharaState state;
    gDinoBackup->GetChara(1, &state);
    u32 body;
    if ((body = state.bodyModel) >= ((const DinoStatusCharaData *)chara.Get())->bodyModelCount) {
        return;
    }
    const u8 *data = (const u8 *)chara.Get();
    OS_SPrintf(path, data_ov006_02190e1c.bodyPathFormat,
               data + ((const u32 *)(data + ((const DinoStatusCharaData *)data)->bodyModelNames))[body], 1);
    if (!MFileExists(std::string(path))) {
        return;
    }
    M3DScene *scene = new M3DScene((u32)(data_ov003_021624cc + 4), 0xF618);
    scene->mUnk14 = 1;
    scene->mUnk18 = 0x12C000;
    scene->mM3d->mBgCntBits.priority = 3;
    scene->SetUnk80(-0x56, 0);
    VecFx32 v;
    v.x = 0;
    v.y = 0x4CC;
    v.z = 0;
    scene->SetCameraTarget(&v);
    v.x = 0;
    v.y = 2 << 12;
    v.z = 7 << 12;
    scene->SetCameraPos(&v);
    MAnmModel *model = new MAnmModel(scene);
    model->SetFrames(0, 0x40000000);
    model->Load(path, 0);
    model->mUnkF4 = 1;
    model->mFollowParent = TRUE;
    model->SetRotRef(&data_ov006_02190040);
    mParallel.AddTail(model);
    int joint = model->mModel->FindJoint(data_ov006_021913cc);
    if (state.headModel >= ((const DinoStatusCharaData *)chara.Get())->headModelCount) {
        return;
    }
    if (gDinoBackup->GetSystemVar(4)) {
        MResource<DinoHeadMaskDefs> masks = gDinoResource->LoadHeadMaskDefs();
        u32 id = gDinoBackup->GetSystemVar(4);
        const u8 *defs = (const u8 *)masks.Get();
        const DinoHeadMaskDef *mask =
            (const DinoHeadMaskDef *)(defs + ((const u32 *)(defs + ((const DinoHeadMaskDefs *)defs)->tableOffset))[id]);
        OS_SPrintf(path, data_ov006_02190e1c.headPathFormat, (const char *)mask + mask->modelName);
    } else {
        u32 head = state.headModel;
        const u8 *data = (const u8 *)chara.Get();
        OS_SPrintf(path, data_ov006_02190e1c.headPathFormat,
                   data + ((const u32 *)(data + ((const DinoStatusCharaData *)data)->headModelNames))[head]);
    }
    MAnmModel *head = new MAnmModel(scene);
    head->SetFrames(0, 0x40000000);
    head->Load(path, 0);
    head->SetParent(model, joint);
    head->Run();
    head->mUnkF4 = 1;
    head->mFollowParent = TRUE;
    head->SetUnk145();
    mParallel.AddTail(head);
    mParallel.AddTail(scene);
}

void DinoUiStatusMenu::SelectTab(int tab) {
    if (tab < 0 || tab > 2) {
        return;
    }
    switch (tab) {
    case 0:
        break;
    case 1: {
        *mResult = 2;
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a20, data_02089a88);
        GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(mUnk130), fade, exit);
        DinoSoundPlaySe(0x94, 0);
        break;
    }
    case 2: {
        *mResult = 3;
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a20, data_02089a88);
        GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(mUnk130), fade, exit);
        DinoSoundPlaySe(0x94, 0);
        break;
    }
    }
}

extern const u32 data_0208aad4, data_0208aad8, data_0208aadc;
extern const u32 data_0208a878, data_0208a87c, data_0208a880;

// The menu for a team member.
void DinoUiStatusMenu::SelectMember(int index) {
    mUnk120 = 0;
    mCursor->Hide();
    DinoCommonWindowMenu *menu = new DinoCommonWindowMenu(
        &mUnk120, data_ov006_02191000[index] + data_ov006_02190e9c.memberMenuOffset, NULL, 0);
    menu->AddChoice(1, gDinoResource->GetText(data_0208aadc), gDinoResource->GetText(data_0208a880));
    menu->AddChoice(2, gDinoResource->GetText(data_0208aad8), gDinoResource->GetText(data_0208a87c));
    menu->AddChoice(-2, gDinoResource->GetText(data_0208aad4), gDinoResource->GetText(data_0208a878));
    GetSerial()->AddTail(menu, mCursor->CreateShowTransfer(),
                         MCreateCallTask(this, &DinoUiStatusMenu::HandleMemberResult));
}

extern const u32 data_0208aad0;

void DinoUiStatusMenu::HandleMemberResult() {
    if (mUnk120 == 1) {
        mMode = 1;
        mMessage = new DinoCommonWindowMessage(MPoint(128, 96), std::string(gDinoResource->GetText(data_0208aad0)), 4, 0);
        mParallel.AddTail(mMessage);
        PickReplacement(mSelected);
        mMembers[mSelected]->PlayDecided();
        FocusNearestItem(GetCursor());
        if (mSelected < 3) {
            mTech->Hide();
        }
    } else if (mUnk120 == 2) {
        *mResult = mSelected + 4;
        GetSerial()->AddTail(gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0),
                             MTaskExitTransfer::Create(&(MTask &)*this));
    } else if (mUnk120 == -2) {
        mMembers[mSelected]->PlayFocused();
    }
    mUnk120 = 0;
}

void DinoUiStatusMenu::PickReplacement(int index) {
    int i;
    for (i = 0; i < 5; i++) {
        mMembers[i]->Enable();
    }
    if (!mTeam->slots[index]) {
        for (int j = 0; j < 5; j++) {
            if (!mTeam->slots[j]) {
                mMembers[j]->Disable();
            }
        }
    }
    mMembers[index]->Disable();
    for (i = 0; i < 3; i++) {
        mTabItems[i]->Disable();
    }
    mOKButton->MMenuItem::Disable();
    LinkMembers();
    for (i = 0; i < 5; i++) {
        RemoveLink(mMembers[i], 2);
        AddLink(mMembers[i], 2, mUnkB8);
    }
    mHelp = new DinoUiStatusChangeModeHelpTask(mUnkB8->GetFrame());
    mUnk108->AddTail(mHelp);
}

extern const u32 data_0208aa4c, data_0208aae0;
extern const int data_02089a2c, data_02089a30, data_02089a34;

void DinoUiStatusMenu::OnOKDecided() {
    if (mMode) {
        return;
    }
    *mResult = 0;
    if (mTeam->slots[0]) {
        if (DinoTeamDiffers(mTeam, (DinoTeam *)mUnk11C)) {
            mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(128, 96),
                                                           std::string(gDinoResource->GetText(data_0208aae0)), 4));
        } else {
            mSerial.AddTail(new MTimer(data_02089a34));
        }
        GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(mUnk130),
                             gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a30, data_02089a2c),
                             MTaskExitTransfer::Create(&(MTask &)*this));
    } else {
        mSerial.AddTail(
            new DinoCommonWindowMessageBox(MPoint(128, 96), std::string(gDinoResource->GetText(data_0208aa4c)), 4));
    }
}

// Swaps the picked member with the one decided on.
void DinoUiStatusMenu::DecideMember(int index) {
    mMode = 0;
    mMessage->mUnk98 = 15;
    mName->Hide();
    MParallelTaskSet *anims = new MParallelTaskSet(TRUE);
    if (mTeam->slots[index]) {
        mMembers[index]->PlayDecided();
        anims->AddTail(mMembers[index]->CreateFlyInTask(data_ov006_02191000[mSelected]));
    } else {
        mMembers[index]->PlayNormal();
    }
    anims->AddTail(mMembers[mSelected]->CreateFlyInTask(data_ov006_02191000[index]));
    GetSerial()->AddTail(new MTimer(5), anims, MTaskExitTransfer::Create(mMessage),
                         MCreateCallTask(this, &DinoUiStatusMenu::FinishPage));
    u8 slot = mTeam->slots[mSelected];
    mTeam->slots[mSelected] = mTeam->slots[index];
    mTeam->slots[index] = slot;
    if (mHelp != NULL) {
        mHelp->mSerial.Clear();
        mHelp->Disable();
    }
}

extern const u32 data_0208aa3c;

void DinoUiStatusMenu::OnBackDecided() {
    if (mMode == 1) {
        mMode = 0;
        mMessage->Exit();
        RefreshItems();
        GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(&mUnkB8->mSerial),
                             MCreateCallTask(this, &DinoUiStatusMenu::FinishTab));
        return;
    }
    Disable();
    *mResult = 9;
    mSerial.AddTail(new MTimer(data_02089a34));
    if (DinoTeamDiffers(mTeam, (DinoTeam *)mUnk11C)) {
        MParallelTaskSet *anims = new MParallelTaskSet(TRUE);
        for (int i = 0; i < 5; i++) {
            if (mTeam->slots[i] != 0 && mTeam->slots[i] != ((DinoTeam *)mUnk11C)->slots[i]) {
                int from = 5;
                for (int j = 0; j < 5; j++) {
                    if (mTeam->slots[i] == ((DinoTeam *)mUnk11C)->slots[j]) {
                        from = j;
                    }
                }
                if (from != 5) {
                    anims->AddTail(mMembers[i]->CreateFlyInTask(data_ov006_02191000[from]));
                }
            }
        }
        GetSerial()->AddTail(
            new DinoCommonWindowMessageBox(MPoint(128, 96), std::string(gDinoResource->GetText(data_0208aa3c)), 4),
            anims);
    }
    GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(mUnk130),
                         gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a30, data_02089a2c),
                         MTaskExitTransfer::Create(&(MTask &)*this));
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov006_02190f9c[];
extern "C" u32 data_ov006_02191008[];
extern "C" u32 data_ov006_02191010[];
extern "C" u32 data_ov006_02191018[];
extern "C" u32 data_ov006_02191020[];
extern "C" u32 data_ov006_02190f1c[];
// NONMATCHING: register choice/scheduling of the pooled stores.
static struct DinoUiPartsTabIconMenuItemStaticInit {
    DinoUiPartsTabIconMenuItemStaticInit() {
        volatile s32 *p = (volatile s32 *) &data_ov006_02190e1c;
        p[0x44 / 4] = 128;
        p[0x48 / 4] = 176;
        p[0x6c / 4] = 8;
        p[0x70 / 4] = 150;
        p = (volatile s32 *) &data_ov006_02190e9c;
        p[0x4 / 4] = 64;
        p[0x8 / 4] = 112;
        DINO_POOL_WORD(data_ov006_02190e1c, 0x4c) = 27;
        DINO_POOL_WORD(data_ov006_02190e1c, 0x50) = 10;
        DINO_POOL_WORD(data_ov006_02190e1c, 0x34) = 36;
        DINO_POOL_WORD(data_ov006_02190e1c, 0x38) = 115;
        DINO_POOL_WORD(data_ov006_02190e9c, 0x1c) = 117;
        DINO_POOL_WORD(data_ov006_02190e9c, 0x20) = 65;
        DINO_POOL_WORD(data_ov006_02190e1c, 0x5c) = 117;
        DINO_POOL_WORD(data_ov006_02190e1c, 0x60) = 81;
        DINO_POOL_WORD(data_ov006_02190e1c, 0x7c) = 89;
        DINO_POOL_WORD(data_ov006_02190e9c, 0x0) = 89;
        DINO_POOL_WORD(data_ov006_02190e1c, 0x2c) = 128;
        DINO_POOL_WORD(data_ov006_02190e1c, 0x30) = 96;
        DINO_POOL_WORD(data_ov006_02190e9c, 0xc) = 128;
        DINO_POOL_WORD(data_ov006_02190e9c, 0x10) = 96;
        p = (volatile s32 *) &data_ov006_02190f9c;
        p[0x64 / 4] = 46;
        p[0x68 / 4] = 152;
        DINO_POOL_WORD(data_ov006_02191008, 0x0) = 91;
        DINO_POOL_WORD(data_ov006_02191008, 0x4) = 152;
        DINO_POOL_WORD(data_ov006_02191010, 0x0) = 128;
        DINO_POOL_WORD(data_ov006_02191010, 0x4) = 152;
        p = (volatile s32 *) &data_ov006_02191018;
        p[0x0 / 4] = 178;
        p[0x4 / 4] = 152;
        DINO_POOL_WORD(data_ov006_02191020, 0x0) = 214;
        DINO_POOL_WORD(data_ov006_02191020, 0x4) = 152;
        DINO_POOL_WORD(data_ov006_02190e9c, 0x14) = -40;
        DINO_POOL_WORD(data_ov006_02190e9c, 0x18) = -88;
        DINO_POOL_WORD(data_ov006_02190e1c, 0x54) = -32;
        DINO_POOL_WORD(data_ov006_02190e1c, 0x58) = 8;
        DINO_POOL_WORD(data_ov006_02190e9c, 0x2c) = -6;
        DINO_POOL_WORD(data_ov006_02190e9c, 0x30) = 12;
        DINO_POOL_WORD(data_ov006_02190f1c, 0x4c) = DINO_POOL_WORD(data_ov006_02190f9c, 0x64);
        DINO_POOL_WORD(data_ov006_02190f1c, 0x50) = DINO_POOL_WORD(data_ov006_02190f9c, 0x68);
        DINO_POOL_WORD(data_ov006_02190f1c, 0x54) = DINO_POOL_WORD(data_ov006_02190f9c, 0x6c);
        DINO_POOL_WORD(data_ov006_02190f1c, 0x58) = DINO_POOL_WORD(data_ov006_02190f9c, 0x70);
        DINO_POOL_WORD(data_ov006_02190f1c, 0x5c) = DINO_POOL_WORD(data_ov006_02190f9c, 0x74);
        DINO_POOL_WORD(data_ov006_02190f1c, 0x60) = DINO_POOL_WORD(data_ov006_02190f9c, 0x78);
    }
} sDinoUiPartsTabIconMenuItemStaticInit;
