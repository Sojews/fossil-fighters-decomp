// DispBlower, OptionKeyCheckTask.
// Mostly the destructors so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_blower.h"
#include "m/input.h"
#include "game/dino_backup.h"
#include "game/dino_sound.h"
#include "game/disp_blower_menu.h"
#include "game/disp_blower.h"
#include "game/option_key_check_task.h"
#include "game/dino_ui_option_menu_item.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/picture.h"
#include "m/smart_oam.h"

// The file's pooled statics (anchor data_ov004_0217520c): motion paths, the callback the menu runs once shown, and
// the layout points (zero in the ROM, set up at startup).
extern "C" struct DispBlowerPool {
    const char *mPaths[14];         // 0x00
    void (DinoHelpBase::*mEnable)(); // 0x38
    MPoint mBgmBase;        // 0x40: the BGM volume row's first button
    MPoint mButtonPos;      // 0x48: OK and back
    MPoint mMarkPos;        // 0x50: the speed marks
    MPoint mSpeedStep;      // 0x58
    u8 unk60[8];
    MPoint mSpeedBase;      // 0x68
    u8 unk70[8];
    MPoint mSeMotionPos;    // 0x78
    u8 unk80[8];
    MPoint mMicPos;         // 0x88
    u8 unk90[8];
    MPoint mBgmMotionPos;   // 0x98
    MPoint mSeBase;         // 0xA0: the SE volume row's first button
    MPoint mSeStep;         // 0xA8
    MPoint mBgmStep;        // 0xB0
} data_ov004_0217520c;

DispBlower::~DispBlower() {
}

extern "C" const u32 data_0208abb8; // "please wait"

// NONMATCHING: the wait message's position temporary sits lower on the stack in the game.
void DispBlower::Execute() {
    MicTestButton *button = mButton;
    button->mLevels[button->mCount++ & 0x3F] = button->mOwner->GetMicStrength();
    if (button->IsFocused()) {
        if (button->mOwner->IsWaiting()) {
            if (button->mMessages->IsEmpty()) {
                for (int i = 0; i < 0x40; i++) {
                    button->mLevels[i] = 0;
                }
                button->mTasks->Clear();
                button->mMeter->Play(1, 1);
                button->mMeter->Hide();
                button->mMessages->AddTail(new DinoCommonWindowMessageWait1(
                    std::string(gDinoResource->GetText(data_0208abb8)), MPoint(0x80, 0x60), 4, 0));
            }
            button->mMotionA0->Hide();
        } else {
            button->mMessages->Clear();
            button->mMotionA0->Play(1, 0);
            button->mMotionA0->Show();
        }
    } else {
        button->mMessages->Clear();
        button->mMotionA0->Hide();
    }
    if (button->mTasks->IsEmpty() && button->mMessages->IsEmpty()) {
        if (button->IsFocused()) {
            u8 level = 0;
            for (int i = 0; i < 0x40; i++) {
                if (button->mLevels[i] > level) {
                    level = button->mLevels[i];
                }
            }
            if (level) {
                button->mMeter->Play(level, 0);
                button->mMeter->Show();
                if (level == 4) {
                    button->mMotionA8->Play(5, 0);
                    button->mTasks->AddTail(new MTimer(0x5A));
                } else {
                    button->mMotionA8->Play(4, 0);
                }
            } else {
                button->mMeter->Hide();
            }
        } else {
            button->mMeter->Hide();
        }
    }
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
// Declared ahead so OnStart calls it instead of inlining it.
template <> MTask *MCreateCallTask(DinoUiOptionMenu *obj, void (DinoHelpBase::*func)());

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

// Pooled file statics.
struct DispBlowerStatics {
    u8 unk00[0x5C];
    MRect mMicTestFrame;    // 0x5C
};
extern "C" DispBlowerStatics data_ov004_0217530c;

MRect MicTestButton::GetFrame() {
    return data_ov004_0217530c.mMicTestFrame;
}

void MicTestButton::OnDecide() {
    DinoSoundPlaySe(0x390, 0);
}

MRect BGMVolumeButton::GetFrame() {
    return MRect(mPos.Offset(-8, -8), mPos.Offset(8, 8));
}

MRect SEVolumeButton::GetFrame() {
    return MRect(mPos.Offset(-8, -8), mPos.Offset(8, 8));
}

void BGMVolumeButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        if (mMotion == NULL) {
            mMotion = new MMotion(data_ov004_0217520c.mPaths[2], mOwner->GetOam(), 2, 1);
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
            mMotion = new MMotion(data_ov004_0217520c.mPaths[2], mOwner->GetOam(), 2, 1);
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
        mMotion = new MMotion(data_ov004_0217520c.mPaths[2], mOwner->GetOam(), 2, 1);
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
        mMotion = new MMotion(data_ov004_0217520c.mPaths[2], mOwner->GetOam(), 2, 1);
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
        RemoveLink(mMicTest, 4);
        AddLink(mMicTest, 4, mSeItems[mSeVolume]);
    }
}

MRect DinoUiOptionSpeedMenuItem::GetFrame() {
    return MRect(mMotion->GetPosition().Offset(-25, -8), mMotion->GetPosition().Offset(25, 8));
}

void DinoUiOptionSpeedMenuItem::OnDecide() {
    if (mMotion) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
    }
    mOwner->SetSpeed(mSpeed);
    DinoSoundPlaySe(0x38E, 0);
}

void DinoUiOptionMenu::CheckKeys() {
    MSerialTaskSet *serial = &mSerial;
    if (serial->IsEmpty()) {
        if (GetInput()->GetPressed() & 2) {
            if (mBackButton) {
                if (mBackButton->IsFocused()) {
                    mBackButton->Decide();
                } else {
                    mBackButton->Focus();
                }
            }
        }
    }
}

void MicTestButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(1, 0);
        mMotion->Show();
        mMotionA4->Play(2, 0);
        mMotionA8->Play(4, 0);
    } else {
        mMotion->Hide();
        mMotionA4->Play(1, 0);
        mMotionA8->Play(3, 0);
    }
    for (int i = 0; i < 0x40; i++) {
        mLevels[i] = 0;
    }
    mTasks->Clear();
    mMeter->Play(1, 1);
    mMeter->Hide();
}

void DinoUiOptionMenu::Sleep() {
    mDelay = 180;
    MicTestButton *mic = mMicTest;
    for (int i = 0; i < 0x40; i++) {
        mic->mLevels[i] = 0;
    }
    mic->mTasks->Clear();
    mic->mMeter->Play(1, 1);
    mic->mMeter->Hide();
}

void DinoUiOptionMenu::SetSpeed(u8 speed) {
    if (speed < 3) {
        for (int i = 0; i < 3; i++) {
            mSpeedMarks[i]->Hide();
        }
        mSpeedMarks[speed]->Show();
        mSpeed = speed;
        gDinoBackup->SetUnk34_16(mSpeed);
        for (int i = 0; i < 11; i++) {
            RemoveLink(mBgmItems[i], 4);
            AddLink(mBgmItems[i], 4, mTopItems[mSpeed]);
        }
        RemoveLink(mUnk128, 2);
        AddLink(mUnk128, 2, mTopItems[mSpeed]);
        RemoveLink(mBackButton, 2);
        AddLink(mBackButton, 2, mTopItems[mSpeed]);
    }
}

DinoUiOptionMenu::DinoUiOptionMenu() : MLooperPreLoopHook(FALSE, 0), MMenu(NULL, 0) {
    mSpeed = 0;
    mBgmVolume = 0;
    mSeVolume = 0;
    mSilentFocus = TRUE;
    SetDeferStart(TRUE);
    mBackButton = NULL;
    mDelay = 180;
    MLooperPreLoopHook::Enable();
}

u8 DinoUiOptionMenu::GetMicStrength() {
    if (IsWaiting()) {
        return 0;
    }
    return mBlower->GetStrength();
}

BOOL DinoUiOptionMenu::IsWaiting() {
    if (mDelay) {
        return TRUE;
    }
    return FALSE;
}

#pragma auto_inline off

extern "C" const u32 data_0208ab98; // "back"
extern "C" const u32 data_0208ab9c; // "OK"
extern "C" const u32 data_0208aba0; // the mic test's help
extern "C" const u32 data_0208aba4; // the SE volume's help
extern "C" const u32 data_0208aba8; // the BGM volume's help
extern "C" const char data_ov004_021759c4[];  // the title picture
extern "C" std::string data_ov004_021772b4[3]; // the speed marks' motions
extern "C" u32 data_ov004_021752e4[3];         // the speed items' help texts

static inline const int *GetExcavate7C(MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = defs.Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

inline DinoUiOptionMenuItem::DinoUiOptionMenuItem(DinoUiOptionMenu *owner, u32 textId) {
    mOwner = owner;
    mMotion = NULL;
    mTextId = textId;
}

inline DinoUiOptionSpeedMenuItem::DinoUiOptionSpeedMenuItem(DinoUiOptionMenu *owner, u8 speed)
    : DinoUiOptionMenuItem(owner, data_ov004_0217520c.mPaths[12],
                           MPoint(data_ov004_0217520c.mSpeedBase.x + data_ov004_0217520c.mSpeedStep.x * speed,
                                  data_ov004_0217520c.mSpeedBase.y + data_ov004_0217520c.mSpeedStep.y * speed),
                           data_ov004_021752e4[speed]) {
    mSpeed = speed;
}

inline BGMVolumeButton::BGMVolumeButton(DinoUiOptionMenu *owner, u8 level, u32 textId)
    : DinoUiOptionMenuItem(owner, textId) {
    mLevel = level;
    mPos = MPoint(data_ov004_0217520c.mBgmBase.x + data_ov004_0217520c.mBgmStep.x * level,
                  data_ov004_0217520c.mBgmBase.y + data_ov004_0217520c.mBgmStep.y * level);
    DinoHelpItem::Disable();
}

inline SEVolumeButton::SEVolumeButton(DinoUiOptionMenu *owner, int level, u32 textId)
    : DinoUiOptionMenuItem(owner, textId) {
    mLevel = level;
    mPos = MPoint(data_ov004_0217520c.mSeBase.x + data_ov004_0217520c.mSeStep.x * level,
                  data_ov004_0217520c.mSeBase.y + data_ov004_0217520c.mSeStep.y * level);
    DinoHelpItem::Disable();
}

// Six tasks into one set (the arguments are evaluated last to first; the set is kept in a register).
static inline void DinoAddTail6(MTaskSet *set, MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f) {
    MTaskSet *self = set;
    self->AddTail(a);
    self->AddTail(b);
    self->AddTail(c);
    self->AddTail(d);
    self->AddTail(e);
    self->AddTail(f);
}

inline MicTestButton::MicTestButton(DinoUiOptionMenu *owner, u32 textId)
    : DinoUiOptionMenuItem(owner, data_ov004_0217520c.mPaths[1], data_ov004_0217520c.mMicPos, textId) {
    for (int i = 0; i < 0x40; i++) {
        mLevels[i] = 0;
    }
    mCount = 0;
    mMotion->SetPriority(2);
    mMotionA0 = new MMotion(data_ov004_0217520c.mPaths[11], mOwner->GetOam(), 2, 1);
    mMotionA0->Play(1, 0);
    mMotionA0->SetPosition(data_ov004_0217520c.mMicPos);
    mMotionA0->SetPriority(2);
    mMotionA0->Hide();
    mMeter = new MMotion(data_ov004_0217520c.mPaths[7], mOwner->GetOam(), 2, 1);
    mMeter->Play(1, 0);
    mMeter->SetPosition(data_ov004_0217520c.mMicPos);
    mMeter->SetPriority(2);
    mMeter->Hide();
    mMotionA4 = new MMotion(data_ov004_0217520c.mPaths[10], mOwner->GetOam(), 2, 1);
    mMotionA4->Play(1, 0);
    mMotionA4->SetPosition(data_ov004_0217520c.mMicPos);
    mMotionA4->SetPriority(2);
    mMotionA4->Show();
    mMotionA8 = new MMotion(data_ov004_0217520c.mPaths[10], mOwner->GetOam(), 2, 1);
    mMotionA8->Play(3, 0);
    mMotionA8->SetPosition(data_ov004_0217520c.mMicPos);
    mMotionA8->SetPriority(2);
    mMotionA8->Show();
    mTasks = new MSerialTaskSet;
    mMessages = new MParallelTaskSet;
    MTask *msgs, *meter, *a8, *a4, *a0;
    meter = mMeter;
    a8 = mMotionA8;
    a4 = mMotionA4;
    a0 = mMotionA0;
    msgs = mMessages;
    DinoAddTail6(&mParallel, mTasks, msgs, a0, a4, a8, meter);
    DinoHelpItem::Disable();
    mParallel.AddTail(new DispBlower(this));
}

inline DinoUiOptionBackButton::DinoUiOptionBackButton(DinoUiOptionMenu *owner, u32 textId)
    : DinoUiOptionMenuItem(owner, textId) {
    mMotion = new MMotion(data_ov004_0217520c.mPaths[3], mOwner->GetOam(), 2, 1);
    mMotion->Play(2, 0);
    mMotion->SetPosition(data_ov004_0217520c.mButtonPos);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    DinoHelpItem::Enable();
}

inline DinoUiOptionOkButton::DinoUiOptionOkButton(DinoUiOptionMenu *owner, u32 textId)
    : DinoUiOptionMenuItem(owner, textId) {
    mMotion = new MMotion(data_ov004_0217520c.mPaths[8], mOwner->GetOam(), 2, 1);
    mMotion->Play(2, 0);
    mMotion->SetPosition(data_ov004_0217520c.mButtonPos);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    SetKeys(8);
    DinoHelpItem::Enable();
}

inline DinoUiOptionPower::DinoUiOptionPower(DinoUiOptionMenu *menu) {
    mMenu = menu;
    MSleepHook::Register();
}

// &mParallel through a non-member inline: the speed mark loop hoists it into the frame's lowest slot, as the game does.
static inline MTaskSet *OptionParallel(DinoUiOptionMenu *menu) {
    return &menu->mParallel;
}

#pragma inline_max_size(1000)
// The speed, volume and mic test items, OK and back, the top screen's help areas and the links between them all;
// starts on the saved settings.
void DinoUiOptionMenu::OnStart() {
    DinoHelpBase::Disable();
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    mParallel.AddTail(new DinoUiOptionPower(this));
    mParallel.AddTail(mUnk134 = new MTextPicture(std::string(data_ov004_021759c4), TRUE, false));
    mUnk134->SetPriority(2);
    DinoHelpDummyItem *help;
    mParallel.AddTail(help = new DinoHelpDummyItem(GetDisplay(), MRect(0x2D, 0x4F, 0xF0, 0x5E),
                                                   std::string(gDinoResource->GetText(data_0208aba8))));
    mHelps[3] = help;
    mParallel.AddTail(help = new DinoHelpDummyItem(GetDisplay(), MRect(0x2D, 0x63, 0xF0, 0x72),
                                                   std::string(gDinoResource->GetText(data_0208aba4))));
    mHelps[4] = help;
    mParallel.AddTail(help = new DinoHelpDummyItem(GetDisplay(), data_ov004_0217530c.mMicTestFrame,
                                                   std::string(gDinoResource->GetText(data_0208aba0))));
    mHelps[5] = help;
    mOam = new MSmartOam(0x40, 0, TRUE);
    mOam->mHidden = TRUE;
    for (int i = 0; i < 3; i++) {
        mSpeedMarks[i] = new MMotion(std::string(data_ov004_021772b4[i]), mOam, 2, 1);
        mSpeedMarks[i]->Play(1, 0);
        mSpeedMarks[i]->SetPosition(data_ov004_0217520c.mMarkPos);
        mSpeedMarks[i]->SetPriority(2);
        mSpeedMarks[i]->Show();
        MTaskSet *tasks = OptionParallel(this);
        tasks->AddTail(mSpeedMarks[i]);
    }
    mBgmMotion = new MMotion(std::string(data_ov004_0217520c.mPaths[13]), mOam, 0, 1);
    mBgmMotion->Play(1, 0);
    mBgmMotion->SetPosition(data_ov004_0217520c.mBgmMotionPos);
    mBgmMotion->SetPriority(2);
    mBgmMotion->Show();
    mParallel.AddTail(mBgmMotion);
    mSeMotion = new MMotion(std::string(data_ov004_0217520c.mPaths[4]), mOam, 0, 1);
    mSeMotion->Play(1, 0);
    mSeMotion->SetPosition(data_ov004_0217520c.mSeMotionPos);
    mSeMotion->SetPriority(2);
    mSeMotion->Show();
    mParallel.AddTail(mSeMotion);
    for (int i = 0; i < 3; i++) {
        DinoUiOptionSpeedMenuItem *item = new DinoUiOptionSpeedMenuItem(this, i);
        mHelps[i] = item;
        mTopItems[i] = item;
        AddItem(mTopItems[i]);
    }
    for (int i = 0; i < 11; i++) {
        mBgmItems[i] = new BGMVolumeButton(this, i, data_0208aba8);
        AddItem(mBgmItems[i]);
    }
    for (int i = 0; i < 11; i++) {
        mSeItems[i] = new SEVolumeButton(this, i, data_0208aba4);
        AddItem(mSeItems[i]);
    }
    mMicTest = new MicTestButton(this, data_0208aba4);
    AddItem(mMicTest);
    DinoUiOptionBackButton *back = new DinoUiOptionBackButton(this, data_0208ab98);
    mHelps[7] = back;
    mBackButton = back;
    AddItem(mBackButton);
    DinoUiOptionOkButton *ok = new DinoUiOptionOkButton(this, data_0208ab9c);
    mHelps[6] = ok;
    mUnk128 = ok;
    AddItem(mUnk128);
    AddEntry((u32)mHelps[0], 4, 0);
    AddEntry((u32)mHelps[0], 2, (u32)mHelps[3]);
    AddEntry((u32)mHelps[0], 3, 0);
    AddEntry((u32)mHelps[0], 1, (u32)mHelps[1]);
    AddEntry((u32)mHelps[1], 4, 0);
    AddEntry((u32)mHelps[1], 2, (u32)mHelps[3]);
    AddEntry((u32)mHelps[1], 3, (u32)mHelps[0]);
    AddEntry((u32)mHelps[1], 1, (u32)mHelps[2]);
    AddEntry((u32)mHelps[2], 4, 0);
    AddEntry((u32)mHelps[2], 2, (u32)mHelps[3]);
    AddEntry((u32)mHelps[2], 3, (u32)mHelps[1]);
    AddEntry((u32)mHelps[2], 1, 0);
    AddEntry((u32)mHelps[3], 4, (u32)mHelps[1]);
    AddEntry((u32)mHelps[3], 2, (u32)mHelps[4]);
    AddEntry((u32)mHelps[3], 3, 0);
    AddEntry((u32)mHelps[3], 1, 0);
    AddEntry((u32)mHelps[4], 4, (u32)mHelps[3]);
    AddEntry((u32)mHelps[4], 2, (u32)mHelps[5]);
    AddEntry((u32)mHelps[4], 3, 0);
    AddEntry((u32)mHelps[4], 1, 0);
    AddEntry((u32)mHelps[5], 4, (u32)mHelps[4]);
    AddEntry((u32)mHelps[5], 2, (u32)mHelps[6]);
    AddEntry((u32)mHelps[5], 3, 0);
    AddEntry((u32)mHelps[5], 1, (u32)mHelps[7]);
    AddEntry((u32)mHelps[6], 4, (u32)mHelps[5]);
    AddEntry((u32)mHelps[6], 2, 0);
    AddEntry((u32)mHelps[6], 3, 0);
    AddEntry((u32)mHelps[6], 1, (u32)mHelps[7]);
    AddEntry((u32)mHelps[7], 4, (u32)mHelps[5]);
    AddEntry((u32)mHelps[7], 2, 0);
    AddEntry((u32)mHelps[7], 3, (u32)mHelps[6]);
    AddEntry((u32)mHelps[7], 1, 0);
    mParallel.AddTail(mOam);
    mSpeed = gDinoBackup->GetUnk34_16();
    mOldSpeed = mSpeed;
    mBgmVolume = gDinoBackup->GetTargetVolume();
    mOldBgmVolume = mBgmVolume;
    mSeVolume = gDinoBackup->GetVolume();
    mOldSeVolume = mSeVolume;
    FocusItem(mTopItems[mSpeed]);
    for (int i = 0; i < 3; i++) {
        mSpeedMarks[i]->Hide();
    }
    mSpeedMarks[mSpeed]->Show();
    for (int i = 0; i < 11; i++) {
        AddLink(mSeItems[i], 2, mMicTest);
    }
    for (int i = 0; i < 11; i++) {
        AddLink(mBgmItems[i], 4, mTopItems[mSpeed]);
    }
    mBgmMotion->Play(mBgmVolume + 1, 0);
    for (int i = 0; i < 3; i++) {
        AddLink(mTopItems[i], 2, mBgmItems[mBgmVolume]);
    }
    for (int i = 0; i < 11; i++) {
        AddLink(mSeItems[i], 4, mBgmItems[mBgmVolume]);
    }
    mSeMotion->Play(mSeVolume + 1, 0);
    for (int i = 0; i < 11; i++) {
        AddLink(mBgmItems[i], 2, mSeItems[mSeVolume]);
    }
    for (int i = 0; i < 3; i++) {
        AddLink(mTopItems[i], 4, mUnk128);
    }
    AddLink(mTopItems[2], 1, NULL);
    AddLink(mTopItems[0], 3, NULL);
    AddLink(mMicTest, 4, mSeItems[mSeVolume]);
    AddLink(mMicTest, 2, mUnk128);
    AddLink(mMicTest, 1, mMicTest);
    AddLink(mMicTest, 3, mMicTest);
    AddLink(mUnk128, 2, mTopItems[mSpeed]);
    AddLink(mUnk128, 4, mMicTest);
    AddLink(mUnk128, 1, mBackButton);
    AddLink(mUnk128, 3, mBackButton);
    AddLink(mBackButton, 2, mTopItems[mSpeed]);
    AddLink(mBackButton, 4, mMicTest);
    AddLink(mBackButton, 1, mUnk128);
    AddLink(mBackButton, 3, mUnk128);
    mSerial.AddTail(mOam->CreateShowTransfer(), mUnk134->CreateShowTransfer(),
                    MCreateCallTask(this, data_ov004_0217520c.mEnable));
    mBlower = new DinoBlower2();
    mParallel.AddTail(mBlower);
    OptionKeyCheckTask *keys = new OptionKeyCheckTask(this);
    keys->Suspend();
    mParallel.AddTail(keys);
    mSerial.AddTail(MTaskRunTransfer::Create(keys));
    mUnk13C = keys;
}
#pragma inline_max_size reset

DinoUiOptionMenuItem::DinoUiOptionMenuItem(DinoUiOptionMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->Hide();
    mParallel.AddTail(mMotion);
}

template <> MTask *MCreateCallTask(DinoUiOptionMenu *obj, void (DinoHelpBase::*func)()) {
    return new MCallTask0<DinoUiOptionMenu, DinoHelpBase>(obj, func);
}
