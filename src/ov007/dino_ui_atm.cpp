// DinoUiATM, DinoUiATMTeamButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/obj.h"
#include "m/display.h"
#include "m/lcd.h"
#include "game/dino_sound.h"
#include "game/dino_ui_atm_menu.h"
#include "game/dino_ui_atm_cursor.h"
#include "game/dino_ui_field_menu_cursor.h"
#include "game/common_window_message.h"
#include "game/dino_ui_atm.h"
#include "game/dino_ui_atm_team_button.h"
#include "game/dino_ui_atm_back_button.h"
#include "game/dino_backup.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/bright.h"
#include "m/indicator.h"
#include "m/bg.h"
#include "game/backup_validater.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_resource.h"
#include "m/text_codec.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_ui_parts_team_indicator.h"
#include "game/dino_ui_parts_team_info.h"
#include "game/dino_ui_keyboard_menu.h"
#include "game/dino_ui_organization_menu.h"

DinoUiATMMenu::DinoUiATMMenu(u32 *result, u8 arg1, DinoUiPartsTeamInfo *teamInfo, u8 arg3) : MMenu(NULL, 0) {
    mTeamInfo = teamInfo;
    mResult = result;
    mUnk114 = 10;
    mUnk118 = 0;
    mUnk11C = 0;
    mUnk120 = -2;
    mUnk124 = arg1;
    mUnk125 = arg3;
}

DinoUiATM::~DinoUiATM() {
}

DinoUiATMTeamButton::~DinoUiATMTeamButton() {
}

DinoUiATMBackButton::~DinoUiATMBackButton() {
}

DinoUiATMMenu::~DinoUiATMMenu() {
}

void DinoUiATMMenu::OnIdle() {
}

extern "C" void func_ov007_02173694(DinoUiATMMenu *menu, int team);
void DinoUiATMMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 3; i++) {
        if (item == mTeamButtons[i]) {
            if (mUnk11C == 0) {
                mUnk118 = i + 1;
                DinoTeam team;
                gDinoBackup->GetTeam(mUnk118, &team);
                u32 j;
                int count = 0;
                for (j = 0; j < 5; j++) {
                    if (team.slots[j] != 0) {
                        count++;
                    }
                }
                if (count == 0) {
                    *mResult = mUnk118 + 300;
                    MTask *exit = CreateExitTransfer();
                    MTask *fade = gLcd->mSub->mBright->CreateFadeOutToBlack(30, 0);
                    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
                    fades->AddTail(fade);
                    MSerialTaskSet *serial = &mSerial;
                    serial->AddTail(fades);
                    serial->AddTail(exit);
                } else {
                    func_ov007_02173694(this, i + 1);
                }
            } else if (mUnk11C == 1) {
                MoveTeam(i + 1);
            }
            return;
        }
    }
}

void DinoUiATMTeamButton::OnDecide() {
}

MRect DinoUiATMBackButton::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiATMBackButton::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiATMTeamButton::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiATMBackButton::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiATMBackButton::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiATM::Idle() {
    (this->*mState)();
}

extern "C" struct {
    void (DinoUiATMMenu::*ExitMoveMode_00)();
    void (DinoUiATMMenu::*Leave_08)();
    u8 unk10[0x8];
    void (DinoBackup::*Save_18)();
} data_ov007_0218cc38;
extern const u32 data_0208ab54;

// Fades out and leaves, saving the teams first if they changed.
void DinoUiATMMenu::Leave() {
    DinoHelpBase::Disable();
    if (mUnk114 == 1) {
        for (int i = 0; i < 3; i++) {
            DinoTeam team;
            gDinoBackup->GetTeam(i + 1, &team);
            gDinoBackup->SetTeam(i + 4, &team);
        }
        MTask *wait = new DinoCommonWindowMessageWait1(std::string(gDinoResource->GetText(data_0208ab54)),
                                                       MPoint(0x80, 0x8C), 4, 0);
        mParallel.AddTail(wait);
        MTask *waitExit = MTaskExitTransfer::Create(wait);
        MTask *validate = new DinoBackupWriteValidater(wait, NULL);
        MTask *save = MCreateBgCallTask(gDinoBackup, data_ov007_0218cc38.Save_18, 1);
        MTask *validateSave = new DinoBackupWriteValidater(wait, NULL);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(validateSave);
        serial->AddTail(save);
        serial->AddTail(validate);
        serial->AddTail(waitExit);
    }
    MTask *fadeMain;
    MTask *fadeSub;
    MTask *stopBgm;
    MTask *exit;
    exit = CreateExitTransfer();
    stopBgm = DinoSoundCreateStopBgmTask(-1, 30);
    fadeSub = gLcd->mSub->mBright->CreateFadeOutToBlack(30, 0);
    fadeMain = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(fadeMain);
    fades->AddTail(fadeSub);
    fades->AddTail(stopBgm);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
#pragma auto_inline off
template class MBgCallTask0<DinoBackup>;
template MTask *MCreateBgCallTask(DinoBackup *obj, void (DinoBackup::*func)(), int thread);
template class MCallTask0<DinoUiATMMenu, DinoUiATMMenu>;
template MTask *MCreateCallTask(DinoUiATMMenu *obj, void (DinoUiATMMenu::*func)());
#pragma auto_inline reset

void DinoUiATMBackButton::OnFocusChanged(BOOL focused) {
    mSerial.Clear();
    if (focused) {
        mMotion->Play(3, 0);
        mMotion2->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
        mMotion2->Play(2, 0);
    }
}

void DinoUiATMTeamButton::OnFocusChanged(BOOL focused) {
    mSerial.Clear();
    if (focused) {
        MTask *show = mMotion->CreateShowTransfer();
        MTask *timer = new MTimer(10);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(timer);
        serial->AddTail(show);
    } else {
        mMotion->Hide();
    }
}

void DinoUiATMBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mMotion2->Play(4, 1);
    MTask *play2 = mMotion2->CreatePlayTransfer(3, 0);
    MTask *play = mMotion->CreatePlayTransfer(3, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    serial->AddTail(play2);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x38];
    void (DinoUiATMMenu::*ClearTeam_38)();
    u8 unk40[0x8];
    void (DinoUiATM::*StateResult_48)();
    void (DinoUiATM::*StateMenu_50)();
    u8 unk58[0x8];
    void (DinoUiATM::*StateSetup_60)();
    u8 unk68[0x8];
    void (DinoUiATM::*StateNamed_70)();
    void (DinoUiATM::*StateResult_78)();
} data_ov007_0218cb60;
extern "C" struct {
    void (DinoUiATM::*StateMenu_00)();
    void (DinoUiATM::*StateSetup_08)();
    void (DinoUiATM::*StateSetup_10)();
    void (DinoUiATM::*Start_18)();
    u8 unk20[0x38 - 0x20];
    MPoint mCursorOffset;   // 0x38: from a team's button
    u8 unk40[0x48 - 0x40];
    MPoint mFocusOffset;    // 0x48: from the focused team's frame
} data_ov007_0218cbe0;

DinoUiATM::DinoUiATM(u8 arg) : MProcess(gLcd->mSub), mUnk7C(arg) {
    mState = data_ov007_0218cbe0.Start_18;
}

void DinoUiATM::StateStart() {
    mResult = 0;
    mTasks = new MParallelTaskSet;
    mParallel.AddTail(mTasks);
    mState = data_ov007_0218cbe0.StateSetup_10;
}

// (The game calls the menu's constructor defined above.)
#pragma dont_inline on
// Sets up both screens, with the team info on the top one, and opens the menu.
void DinoUiATM::StateSetup() {
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mMain->Setup(5, 0, 3, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    mTeamInfo = new DinoUiPartsTeamInfo;
    mTasks->AddTail(mTeamInfo);
    gLcd->mMain->mBright->SetMode(2);
    gLcd->mMain->mBright->SetFactor(16);
    gLcd->mSub->ResetHooks();
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mSub->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    mSerial.AddTail(new DinoUiATMMenu(&mResult, mUnk7C, mTeamInfo, 1));
    mState = data_ov007_0218cb60.StateResult_78;
}

void DinoUiATM::StateMenu() {
    gLcd->mSub->ResetHooks();
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mSub->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    mSerial.AddTail(new DinoUiATMMenu(&mResult, mUnk7C, mTeamInfo, 0));
    mState = data_ov007_0218cb60.StateResult_48;
}

// After naming a new team: back to the menu if it was left unnamed, or on to its members.
void DinoUiATM::StateNamed() {
    DinoTeam team;
    gDinoBackup->GetTeam(mResult, &team);
    if (team.name[0] == 0) {
        mState = data_ov007_0218cbe0.StateMenu_00;
        return;
    }
    mTasks->Clear();
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mMain->Setup(5, 0, 3, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mSub->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
    mSerial.AddTail(new DinoUiOrganizationMenu(mResult));
    mState = data_ov007_0218cbe0.StateSetup_08;
}
#pragma dont_inline reset

// What the menu chose: under 100 leaves, then 100 + team renames the team, 200 + team edits it and 300 + team names
// a new one.
void DinoUiATM::StateResult() {
    if (mResult < 100) {
        Exit();
    } else if (mResult < 200) {
        mResult -= 100;
        gLcd->GetTopDisplay()->mBright->Reset();
        gLcd->GetBottomDisplay()->mBright->Reset();
        mSerial.AddTail(new DinoUiKeyboardMenu(1, mResult, 1));
        mState = data_ov007_0218cb60.StateMenu_50;
    } else if (mResult < 300) {
        mResult -= 200;
        mTasks->Clear();
        mSerial.AddTail(new DinoUiOrganizationMenu(mResult));
        mState = data_ov007_0218cb60.StateSetup_60;
    } else {
        mResult -= 300;
        DinoTeam team;
        gDinoBackup->GetTeam(mResult, &team);
        team.name[0] = 0;
        gDinoBackup->SetTeam(mResult, &team);
        gLcd->GetTopDisplay()->mBright->Reset();
        gLcd->GetBottomDisplay()->mBright->Reset();
        mSerial.AddTail(new DinoUiKeyboardMenu(1, mResult, 1));
        mState = data_ov007_0218cb60.StateNamed_70;
    }
}


// Shows each team's name, or a blank for an empty team, and marks the current team.
void DinoUiATMMenu::RefreshTeams() {
    for (int i = 0; i < 3; i++) {
        DinoTeam team;
        gDinoBackup->GetTeam(i + 1, &team);
        u32 j;
        int count = 0;
        for (j = 0; j < 5; j++) {
            if (team.slots[j] != 0) {
                count++;
            }
        }
        if (count == 0) {
            mTeamNames[i]->SetText(std::wstring(L" "));
            mTeamTexts[i]->SetText(std::string(" "));
            mEmptyMarks[i]->Show();
        } else {
            mTeamNames[i]->SetText(std::wstring((const wchar_t *)team.name));
            mTeamTexts[i]->SetText(std::wstring((const wchar_t *)team.name));
            mEmptyMarks[i]->Hide();
        }
        if (i + 1 == gDinoBackup->GetUnk34_26()) {
            mTeamNames[i]->Show();
            mTeamTexts[i]->Hide();
        } else {
            mTeamTexts[i]->Show();
            mTeamNames[i]->Hide();
        }
    }
}

void DinoUiATMMenu::ExitMoveMode() {
    DinoUiATMBackButton *back = mBackButton;
    back->mMotion->Show();
    back->mMotion2->Hide();
    back->SetMode(4);
    RefreshTeams();
    mUnk11C = 0;
    LinkTeamButtons();
}

// " " (between the team's name and the message).
extern "C" const char data_ov007_0218e9e4[];
extern const u32 data_0208aa80, data_0208aa84, data_0208aa8c, data_0208aa90;

// Leaves (asking first if the teams changed); while a team is moved, stops moving it.
// NONMATCHING: the exit transfer and the fades' set swap registers (r4/r5).
void DinoUiATMMenu::OnBackDecided() {
    if (mUnk11C == 0) {
        *mResult = mUnk118;
        mFocusCursor->Hide();
        u8 changed = FALSE;
        if (mUnk124) {
            for (int t = 0; t < 3; t++) {
                DinoTeam team;
                DinoTeam saved;
                gDinoBackup->GetTeam(t + 1, &team);
                gDinoBackup->GetTeam(t + 4, &saved);
                for (int i = 0; i < 5; i++) {
                    if (team.slots[i] != saved.slots[i]) {
                        changed = TRUE;
                    }
                }
                u32 j;
                u32 count = 0;
                for (j = 0; j < 5; j++) {
                    if (team.slots[j]) {
                        count++;
                    }
                }
                if (count) {
                    for (int i = 0; i < 16; i++) {
                        if (team.name[i] != saved.name[i]) {
                            changed = TRUE;
                        }
                    }
                }
            }
        }
        if (changed) {
            mUnk114 = 0;
            DinoCommonWindowYesNoDialog *dialog =
                new DinoCommonWindowYesNoDialog(std::string(gDinoResource->GetText(data_0208aa8c)), &mUnk114);
            dialog->SetLabels2(std::string(gDinoResource->GetText(data_0208aa84)),
                               std::string(gDinoResource->GetText(data_0208aa80)));
            mSerial.AddTail(dialog);
            mSerial.AddTail(MCreateCallTask(this, data_ov007_0218cc38.Leave_08));
            return;
        }
        if (!mUnk124) {
            DinoTeam team;
            gDinoBackup->GetTeam(gDinoBackup->GetUnk34_26(), &team);
            std::string name = MEncodeText(std::wstring((const wchar_t *)team.name), 0);
            mSerial.AddTail(new DinoCommonWindowMessageBox(
                MPoint(0x80, 0x60), name + data_ov007_0218e9e4 + gDinoResource->GetText(data_0208aa90), 0x78, 4));
        }
        MTask *fadeMain;
        MTask *fadeSub;
        MTask *stop;
        MTask *exit = CreateExitTransfer();
        stop = DinoSoundCreateStopBgmTask(-1, 30);
        fadeSub = gLcd->mSub->mBright->CreateFadeOutToBlack(30, 0);
        fadeMain = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeMain);
        fades->AddTail(fadeSub);
        fades->AddTail(stop);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fades);
        serial->AddTail(exit);
    } else if (mUnk11C == 1) {
        DinoUiATMBackButton *back = mBackButton;
        back->mMotion->Show();
        back->mMotion2->Hide();
        back->SetMode(4);
        mUnk11C = 0;
        LinkTeamButtons();
        mMessage->Exit();
    }
}

#pragma dont_inline on
void DinoUiATMMenu::Unk02173d08() {
    if (mUnk114 == 1) {
        mCursor->Hide();
        MTask *clear = MCreateCallTask(this, data_ov007_0218cb60.ClearTeam_38);
        DinoUiATMTeamButton *button = mTeamButtons[mUnk118 - 1];
        button->mMotion->Play(3, 0);
        MTask *wait = button->mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(clear);
    }
}
#pragma dont_inline reset

void DinoUiATMMenu::ClearTeam() {
    DinoTeam team;
    gDinoBackup->GetTeam(mUnk118, &team);
    team.name[0] = 0;
    team.slots[0] = 0;
    team.slots[1] = 0;
    team.slots[2] = 0;
    team.slots[3] = 0;
    team.slots[4] = 0;
    gDinoBackup->SetTeam(mUnk118, &team);
    mTeamInfo->SetTeam(mUnk118);
    RefreshTeams();
}

void DinoUiATMMenu::LinkTeamButtons() {
    for (int i = 0; i < 3; i++) {
        mTeamButtons[i]->Enable();
        if (mUnk120 == i + 1) {
            DinoUiATMTeamButton *button = mTeamButtons[i];
            button->mMotion->Play(1, 0);
            button->mMotion->Show();
        } else {
            DinoUiATMTeamButton *button = mTeamButtons[i];
            button->mMotion->Play(1, 0);
            button->mMotion->Hide();
        }
    }
    RemoveLink(mBackButton, 4);
    AddLink(mBackButton, 4, mTeamButtons[2]);
    RemoveLink(mBackButton, 2);
    AddLink(mBackButton, 2, mTeamButtons[0]);
    RemoveLink(mTeamButtons[1], 4);
    AddLink(mTeamButtons[1], 4, mTeamButtons[0]);
    RemoveLink(mTeamButtons[2], 4);
    AddLink(mTeamButtons[2], 4, mTeamButtons[1]);
}

// Where the teams' buttons are.
extern const MPoint data_ov007_0218cd10[3];
extern const int data_020899f4, data_02089a08;

// Moves the team being moved (mUnk118) into the team, with the cursor flying across.
void DinoUiATMMenu::MoveTeam(int team) {
    DinoSoundPlaySe(0x99, 0);
    mTeamButtons[team - 1]->mMotion->Hide();
    mCursor->SetPosition(data_ov007_0218cd10[mUnk118 - 1] + data_ov007_0218cbe0.mCursorOffset);
    mCursor->Show();
    mMessage->mUnk98 = data_020899f4;
    MTask *show = mTeamButtons[team - 1]->mMotion->CreateShowTransfer();
    MTask *exitMessage = MTaskExitTransfer::Create(mMessage);
    MTask *hide = mCursor->mUnk10->CreateHideTransfer();
    MTask *wait = mCursor->mUnk10->CreateWaitStopTask();
    MTask *play = mCursor->mUnk10->CreatePlayTransfer(3, 1);
    MTask *se = DinoSoundCreatePlaySeTask(0xA3, 0);
    MTask *move = mCursor->CreateCoordAnime(mCursor->GetPosition(),
                                            data_ov007_0218cd10[team - 1] + data_ov007_0218cbe0.mCursorOffset,
                                            data_02089a08, 0);
    GetSerial()->AddTail(DinoSoundCreatePlaySeTask(0xA1, 0), move, se, play, wait, hide, exitMessage, show);
    DinoTeam moved;
    gDinoBackup->GetTeam(mUnk118, &moved);
    gDinoBackup->SetTeam(team, &moved);
    mTeamInfo->SetTeam(team);
    mSerial.AddTail(MCreateCallTask(this, data_ov007_0218cc38.ExitMoveMode_00));
}

// Shows the team buttons (lit for the selected team) and, while moving a team, disables the one being moved (and
// the current team, if it can't move), then links the back button to the enabled ones.
void DinoUiATMMenu::RefreshButtons() {
    for (int i = 0; i < 3; i++) {
        mTeamButtons[i]->Enable();
        if (mUnk120 == i + 1) {
            DinoUiATMTeamButton *button = mTeamButtons[i];
            button->mMotion->Play(1, 0);
            button->mMotion->Show();
        } else {
            DinoUiATMTeamButton *button = mTeamButtons[i];
            button->mMotion->Play(1, 0);
            button->mMotion->Hide();
        }
    }
    if (mUnk11C == 1) {
        int moving = mUnk118 - 1;
        if (moving / 3 == 0) {
            mTeamButtons[moving]->Disable();
            DinoUiATMTeamButton *button = mTeamButtons[mUnk118 - 1];
            button->mMotion->Play(2, 0);
            button->mMotion->Show();
        }
        if (mUnk124) {
            mTeamButtons[gDinoBackup->GetUnk34_26() - 1]->Disable();
        }
        if (!mTeamButtons[2]->IsEnabled()) {
            RemoveLink(mBackButton, 4);
            if (!mTeamButtons[1]->IsEnabled()) {
                AddLink(mBackButton, 4, mTeamButtons[0]);
            } else {
                AddLink(mBackButton, 4, mTeamButtons[1]);
            }
        }
        if (!mTeamButtons[0]->IsEnabled()) {
            RemoveLink(mBackButton, 2);
            if (!mTeamButtons[1]->IsEnabled()) {
                AddLink(mBackButton, 2, mTeamButtons[2]);
                RemoveLink(mTeamButtons[2], 4);
                AddLink(mTeamButtons[2], 4, mBackButton);
            } else {
                AddLink(mBackButton, 2, mTeamButtons[1]);
                RemoveLink(mTeamButtons[1], 4);
                AddLink(mTeamButtons[1], 4, mBackButton);
            }
        }
        if (mTeamButtons[0]->IsEnabled() && !mTeamButtons[1]->IsEnabled()) {
            RemoveLink(mTeamButtons[2], 4);
            AddLink(mTeamButtons[2], 4, mTeamButtons[0]);
        }
    }
}

// Moves the cursor to the item; on a team, shows its info and puts the team cursor by it (gliding there from
// another team).
void DinoUiATMMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mFocusCursor->Hide();
        mCursor->Hide();
        return;
    }
    if (item == mBackButton) {
        SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    }
    if (mFocusCursor->IsHidden()) {
        mFocusCursor->Show();
        mFocusCursor->SetFrame(item->GetFrame());
    } else {
        mFocusTasks->Clear();
        mFocusTasks->AddTail(mFocusCursor->CreateMoveTask(mFocusCursor->GetFrame(), item->GetFrame(), 10, -5));
    }
    for (int i = 0; i < 3; i++) {
        if (item == mTeamButtons[i]) {
            mTeamInfo->SetTeam(i + 1);
            if (mUnk120 < -1) {
                mCursor->Show();
                mCursor->SetPosition(item->GetFrame().GetTopLeft() + data_ov007_0218cbe0.mFocusOffset);
            } else if (mUnk120 < 1) {
                mCursor->Show();
                mCursor->SetPosition(item->GetFrame().GetTopLeft() + data_ov007_0218cbe0.mFocusOffset);
                DinoSoundPlaySe(0x96, 0);
            } else {
                mCursor->Show();
                mFocusTasks->AddTail(mCursor->CreateCoordAnime(
                    mCursor->GetPosition(), item->GetFrame().GetTopLeft() + data_ov007_0218cbe0.mFocusOffset, 10, -5));
                DinoSoundPlaySe(0x96, 0);
            }
            mUnk120 = i + 1;
            return;
        }
    }
    mUnk120 = -1;
    mCursor->Hide();
}
