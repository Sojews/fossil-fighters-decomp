// DispBlower, OptionKeyCheckTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_backup.h"
#include "game/dino_sound.h"
#include "game/disp_blower_menu.h"
#include "game/disp_blower.h"
#include "game/option_key_check_task.h"
#include "game/dino_ui_option_menu_item.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

extern const char *data_ov004_0217520c[]; // motion file paths

DispBlower::~DispBlower() {
}

OptionKeyCheckTask::~OptionKeyCheckTask() {
}

DinoUiOptionMenuItem::~DinoUiOptionMenuItem() {
}

DinoUiOptionSpeedMenuItem::~DinoUiOptionSpeedMenuItem() {
}

BGMVolumeButton::~BGMVolumeButton() {
}

SEVolumeButton::~SEVolumeButton() {
}

MicTestButton::~MicTestButton() {
}

DinoUiOptionBackButton::~DinoUiOptionBackButton() {
}

DinoUiOptionOkButton::~DinoUiOptionOkButton() {
}

DinoUiOptionPower::~DinoUiOptionPower() {
    MSleepHook::Unregister();
}

DinoUiOptionMenu::~DinoUiOptionMenu() {
}

void DinoUiOptionMenu::OnIdle() {
}

void DinoUiOptionPower::OnWake() {
}

void DinoUiOptionPower::Execute() {
}

MRect DinoUiOptionMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiOptionMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiOptionMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiOptionMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiOptionOkButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 1);
    } else {
        mMotion->Play(2, 1);
    }
}

void DinoUiOptionBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 1);
    } else {
        mMotion->Play(2, 1);
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiOptionMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiOptionMenu *obj, void (DinoHelpBase::*func)());

void DinoUiOptionMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        return;
    }
    if (mSilentFocus) {
        mSilentFocus = FALSE;
    } else {
        DinoSoundPlaySe(0x38E, 0);
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
}

void DinoUiOptionMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(1, 0);
        mMotion->Show();
    } else {
        mMotion->Hide();
    }
}

void DinoUiOptionOkButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    DinoSoundPlaySe(0x390, 0);
    mOwner->OnOKDecided();
}

void DinoUiOptionBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    DinoSoundPlaySe(0x38F, 0);
    mOwner->OnBackDecided();
}

void MicTestButton::OnDecide() {
    DinoSoundPlaySe(0x390, 0);
}

MRect BGMVolumeButton::GetFrame() {
    MPoint p(mPos.x, mPos.y);
    return MRect(MPoint(p.x - 8, p.y - 8), MPoint(p.x + 8, p.y + 8));
}

MRect SEVolumeButton::GetFrame() {
    MPoint p(mPos.x, mPos.y);
    return MRect(MPoint(p.x - 8, p.y - 8), MPoint(p.x + 8, p.y + 8));
}

void BGMVolumeButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        if (mMotion == NULL) {
            mMotion = new MMotion(data_ov004_0217520c[2], mOwner->GetOam(), 2, 1);
            mMotion->SetPosition(mPos);
            mParallel.AddTail(mMotion);
        }
        mMotion->Play(1, 0);
        mMotion->Show();
        mSerial.AddTail(mMotion->CreateShowTransfer());
        mMotion->Play(1, 0);
        mOwner->SetBgmVolume(mLevel);
    } else if (mMotion) {
        mSerial.Clear();
        mParallel.Remove(mMotion);
        delete mMotion;
        mMotion = NULL;
    }
}

void SEVolumeButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        if (mMotion == NULL) {
            mMotion = new MMotion(data_ov004_0217520c[2], mOwner->GetOam(), 2, 1);
            mMotion->SetPosition(mPos);
            mParallel.AddTail(mMotion);
        }
        mMotion->Play(1, 0);
        mMotion->Show();
        mSerial.AddTail(mMotion->CreateShowTransfer());
        mMotion->Play(1, 0);
        mOwner->SetSeVolume(mLevel);
    } else if (mMotion) {
        mSerial.Clear();
        mParallel.Remove(mMotion);
        delete mMotion;
        mMotion = NULL;
    }
}

void BGMVolumeButton::OnDecide() {
    if (mMotion == NULL) {
        mMotion = new MMotion(data_ov004_0217520c[2], mOwner->GetOam(), 2, 1);
        mMotion->SetPosition(mPos);
        mParallel.AddTail(mMotion);
    }
    mMotion->Play(1, 0);
    mMotion->Show();
    mSerial.AddTail(mMotion->CreateShowTransfer());
    if (mMotion) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
    }
    mOwner->SetBgmVolume(mLevel);
    DinoSoundPlaySe(0x38E, 0);
}

void SEVolumeButton::OnDecide() {
    if (mMotion == NULL) {
        mMotion = new MMotion(data_ov004_0217520c[2], mOwner->GetOam(), 2, 1);
        mMotion->SetPosition(mPos);
        mParallel.AddTail(mMotion);
    }
    mMotion->Play(1, 0);
    mMotion->Show();
    mSerial.AddTail(mMotion->CreateShowTransfer());
    if (mMotion) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
    }
    mOwner->SetSeVolume(mLevel);
    DinoSoundPlaySe(0x38E, 0);
}

void DinoUiOptionMenu::OnPreLoop() {
    if (mDelay) {
        mDelay--;
    }
}

void DinoUiOptionMenu::OnOKDecided() {
    mUnk13C->Exit();
    MTask &self = *this;
    MTask *exit = MTaskExitTransfer::Create(&self);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(exit);
}

void DinoUiOptionPower::OnSleep() {
    mMenu->Sleep();
}

void OptionKeyCheckTask::Execute() {
    mMenu->CheckKeys();
}

void DinoUiOptionMenu::OnBackDecided() {
    gDinoBackup->SetUnk34_16(mOldSpeed);
    gDinoBackup->SetTargetVolume(mOldBgmVolume);
    gDinoBackup->SetVolume(mOldSeVolume);
    mUnk13C->Exit();
    MTask &self = *this;
    MTask *exit = MTaskExitTransfer::Create(&self);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(exit);
}

void DinoUiOptionMenu::SetBgmVolume(u8 level) {
    if (level < 11) {
        mBgmMotion->Play(level + 1, 0);
        mBgmVolume = level;
        gDinoBackup->SetTargetVolume(mBgmVolume);
        for (int i = 0; i < 3; i++) {
            RemoveLink(mTopItems[i], 2);
            AddLink(mTopItems[i], 2, mBgmItems[level]);
        }
        for (int i = 0; i < 11; i++) {
            RemoveLink(mSeItems[i], 4);
            AddLink(mSeItems[i], 4, mBgmItems[level]);
        }
    }
}

void DinoUiOptionMenu::SetSeVolume(u8 level) {
    if (level < 11) {
        mSeMotion->Play(level + 1, 0);
        mSeVolume = level;
        gDinoBackup->SetVolume(mSeVolume);
        for (int i = 0; i < 11; i++) {
            RemoveLink(mBgmItems[i], 2);
            AddLink(mBgmItems[i], 2, mSeItems[mSeVolume]);
        }
        RemoveLink(mBelowSeItem, 4);
        AddLink(mBelowSeItem, 4, mSeItems[mSeVolume]);
    }
}
