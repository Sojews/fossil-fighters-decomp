// DinoUiATM, DinoUiATMTeamButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/obj.h"
#include "m/display.h"
#include "m/lcd.h"
#include "game/dino_sound.h"
#include "game/dino_ui_atm_menu.h"
#include "game/dino_ui_atm.h"
#include "game/dino_ui_atm_team_button.h"
#include "game/dino_ui_atm_back_button.h"
#include "game/dino_backup.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/bright.h"
#include "m/indicator.h"
#include "game/dino_ui_parts_team_indicator.h"

DinoUiATMMenu::DinoUiATMMenu(u32 *result, u8 arg1, u32 arg2, u8 arg3) : MMenu(NULL, 0) {
    mUnk108 = arg2;
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

extern "C" void func_ov007_02172e08(DinoUiATMMenu *menu, int team);
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
                func_ov007_02172e08(this, i + 1);
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

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoBackup>;
template MTask *MCreateBgCallTask(DinoBackup *obj, void (DinoBackup::*func)(), int thread);
template class MCallTask0<DinoUiATMMenu, DinoUiATMMenu>;
template MTask *MCreateCallTask(DinoUiATMMenu *obj, void (DinoUiATMMenu::*func)());

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
    u8 unk00[0x10];
    void (DinoUiATM::*Unk021741c4_10)();
    void (DinoUiATM::*Start_18)();
} data_ov007_0218cbe0;
extern "C" struct {
    u8 unk00[0x38];
    void (DinoUiATMMenu::*ClearTeam_38)();
    u8 unk40[0x8];
    void (DinoUiATM::*Unk021743b8_48)();
} data_ov007_0218cb60;

DinoUiATM::DinoUiATM(u8 arg) : MProcess(gLcd->mSub), mUnk7C(arg) {
    mState = data_ov007_0218cbe0.Start_18;
}

void DinoUiATM::StateStart() {
    mResult = 0;
    mTasks = new MParallelTaskSet;
    mParallel.AddTail(mTasks);
    mState = data_ov007_0218cbe0.Unk021741c4_10;
}

// (The game calls the menu's constructor defined above.)
#pragma dont_inline on
void DinoUiATM::StateMenu() {
    gLcd->mSub->ResetHooks();
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mSub->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    mSerial.AddTail(new DinoUiATMMenu(&mResult, mUnk7C, mUnk88, 0));
    mState = data_ov007_0218cb60.Unk021743b8_48;
}
#pragma dont_inline reset

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

#pragma dont_inline on
void DinoUiATMMenu::Unk02173d08() {
    if (mUnk114 == 1) {
        mUnkFC->Hide();
        MTask *clear = MCreateCallTask(this, data_ov007_0218cb60.ClearTeam_38);
        DinoUiATMTeamButton *button = mTeamButtons[(int)mUnk118 - 1];
        button->mMotion->Play(3, 0);
        MTask *wait = button->mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(clear);
    }
}
#pragma dont_inline reset

extern "C" void func_ov003_0215a4ec(u32 obj, u32 team);

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
    func_ov003_0215a4ec(mUnk108, mUnk118);
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
