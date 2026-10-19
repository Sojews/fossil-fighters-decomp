// DinoUiReviveStatusMenu.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_revive_status_menu.h"
#include "m/resource.h"
#include "game/dino_resource.h"
#include "game/dino_event_talk_window.h"
#include "types.h"
#include "game/dino_backup.h"
#include "m/lcd.h"
#include "m/bright.h"
#include "m/obj.h"
#include "m/bg.h"
#include "m/display.h"
#include "game/dino_sound.h"
#include "m/call_task.h"
#include "game/dino_ui_rv_information.h"

#pragma auto_inline off
DinoUiReviveStatusMenu::DinoUiReviveStatusMenu(u32 id, u32 *result, u8 flag)
    : MMenu(gLcd->mMain, NULL, 0), mResult(result), mUnk118(0), mUnk11C(0), mUnk120(0xFF), mFlag(flag) {
    mUnk122 = 1;
    gDinoBackup->GetCreature(id, &mCreature);
}

DinoUiReviveStatusMenu::DinoUiReviveStatusMenu(const DinoCreatureState &creature, u32 *result, u8 flag)
    : MMenu(gLcd->mMain, NULL, 0), mCreature(creature), mResult(result), mUnk118(0), mUnk11C(0), mUnk120(0xFF),
      mFlag(flag) {
    mUnk122 = 1;
}

DinoUiReviveStatusMenu::DinoUiReviveStatusMenu(const DinoCreatureState &creature, const DinoCreatureState &creature2,
                                               u8 flag)
    : MMenu(gLcd->mMain, NULL, 0), mCreature(creature), mCreature2(creature2), mResult(NULL), mUnk118(1), mUnk11C(0),
      mUnk120(0xFF), mFlag(flag) {
    mUnk122 = 0;
}

DinoUiReviveStatusMenu::DinoUiReviveStatusMenu(const DinoCreatureState &creature, const DinoCreatureState &creature2,
                                               u32 *result, u8 flag)
    : MMenu(gLcd->mMain, NULL, 0), mCreature(creature), mCreature2(creature2), mResult(result), mUnk118(1),
      mUnk11C(0), mUnk120(0xFF), mFlag(flag) {
    mUnk122 = 1;
}
#pragma auto_inline reset

DinoUiReviveStatusMenu::~DinoUiReviveStatusMenu() {
}

DinoUiReviveStatusLoop::~DinoUiReviveStatusLoop() {
}

DinoUiReviveStatusLoopCleaning::~DinoUiReviveStatusLoopCleaning() {
}

DinoUiReviveStatusLoopBattle::DinoUiReviveStatusLoopBattle(const DinoEventReviveCreature *creature,
                                                           const DinoEventReviveCreature *creature2)
    : MProcess(NULL), mCreature(*creature), mCreature2(*creature2) {
}

DinoUiReviveStatusLoopBattle::~DinoUiReviveStatusLoopBattle() {
}

DinoUiReviveStatusMenuItem::~DinoUiReviveStatusMenuItem() {
}

DinoUiReviveStatusBackButton::~DinoUiReviveStatusBackButton() {
}

MRect DinoUiReviveStatusMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiReviveStatusMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiReviveStatusMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiReviveStatusBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

const char *DinoUiReviveStatusMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiReviveStatusBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiReviveStatusMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 4; i++) {
        if (item == mItems[i]) {
            SetTab(i);
        }
    }
}

// Where the cursor sits on each status item.
extern MPoint data_020b5890[4];

void DinoUiReviveStatusMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (int i = 0; i < 4; i++) {
        if (item == mItems[i]) {
            frame.MoveTo(data_020b5890[i]);
            mCursor->SetMode(2);
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    mCursor->Show();
}

// DinoUiReviveStatusMenu::OnStart (not decompiled yet) re-enables the help through a call task.
#pragma auto_inline off
template MTask *MCreateCallTask(DinoUiReviveStatusMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiReviveStatusMenu *obj, void (DinoUiReviveStatusMenu::*func)(u8), u8 a1);
#pragma auto_inline reset

#pragma auto_inline off

DinoUiReviveStatusMenuItem::DinoUiReviveStatusMenuItem(DinoUiReviveStatusMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->Show();
    mMotion->SetPriority(2);
    mParallel.AddTail(mMotion);
}

DinoUiReviveStatusLoop::DinoUiReviveStatusLoop(u32 a, u32 b) : MProcess(NULL) {
    mUnk74 = a;
    mUnk78 = b;
}

void DinoUiReviveStatusLoop::Idle() {
    gLcd->mSub->ResetHooks();
    gLcd->mMain->ResetHooks();
    gLcd->mSub->Setup(5, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mMain->Setup(5, 0, 4, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->GetBottomDisplay()->mBright->Reset();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiReviveStatusMenu(mUnk74, (u32 *)mUnk78, 0);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}

DinoUiReviveStatusLoopCleaning::DinoUiReviveStatusLoopCleaning(const DinoEventReviveCreature *creature,
                                                               const DinoEventReviveCreature *creature2, u32 *done,
                                                               u8 flag)
    : MProcess(NULL), mCreature(*creature), mCreature2(*creature2), mDone(done), mFlag(flag), mStep(0) {
}

void DinoUiReviveStatusLoopBattle::Idle() {
    gLcd->mSub->ResetHooks();
    gLcd->mMain->ResetHooks();
    gLcd->mSub->Setup(5, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mMain->Setup(5, 0, 4, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->mBg->SetMode(3);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->GetBottomDisplay()->mBright->Reset();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiReviveStatusMenu(mCreature, mCreature2, 1);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}

void DinoUiReviveStatusLoopCleaning::Idle() {
    gLcd->mSub->ResetHooks();
    gLcd->mMain->ResetHooks();
    gLcd->mSub->Setup(5, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mMain->Setup(5, 0, 4, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->mBg->SetMode(3);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    if (mCreature.parts[0].level == 0) {
        mSerial.AddTail(new DinoUiReviveStatusMenu(mCreature2, mDone, 0));
    } else {
        mSerial.AddTail(new DinoUiReviveStatusMenu(mCreature, mCreature2, mDone, mFlag));
    }
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

#pragma auto_inline off

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoHelpBase::*Enable_00)();
} data_ov007_02191eb0;

extern "C" struct {
    void (DinoUiReviveStatusMenu::*SetPage_00)(u8);
} data_ov007_02191e80;
extern "C" void func_ov007_02189170(DinoUiReviveStatusMenu *menu);

extern const int data_02089a78, data_02089a7c;

void DinoUiReviveStatusMenu::FadeIn() {
    mUnkE0 = new MParallelTaskSet;
    mParallel.AddTail(mUnkE0);
    if (mFlag) {
        MBright *bright = gLcd->GetBottomDisplay()->mBright;
        MTask *fadeBottom = bright->CreateFadeInFromWhite(data_02089a7c, data_02089a78);
        MTask *fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeInFromWhite(data_02089a7c, data_02089a78);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        mUnkE0->AddTail(fades);
    } else {
        MBright *bright = gLcd->GetBottomDisplay()->mBright;
        MTask *fadeBottom = bright->CreateFadeInFromBlack(data_02089a7c, data_02089a78);
        MTask *fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeInFromBlack(data_02089a7c, data_02089a78);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        mUnkE0->AddTail(fades);
    }
    mSerial.AddTail(new MTimer(data_02089a7c));
}

extern const int data_02089a6c, data_02089a70;

// NONMATCHING: the two fade constants swap registers (as DinoUiKeyboardMenu::OnBackDecided).
void DinoUiReviveStatusMenu::Leave() {
    if (mUnk122) {
        mSerial.AddTail(DinoSoundCreateStopBgmAllTask(30));
    }
    if (mFlag) {
        MTask *exit = CreateExitTransfer();
        MBright *bright = gLcd->GetBottomDisplay()->mBright;
        MTask *fadeBottom = bright->CreateFadeOutToWhite(data_02089a70, data_02089a6c);
        MTask *fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToWhite(data_02089a70, data_02089a6c);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fades);
        serial->AddTail(exit);
    } else {
        MTask *exit = CreateExitTransfer();
        MBright *bright = gLcd->GetBottomDisplay()->mBright;
        MTask *fadeBottom = bright->CreateFadeOutToBlack(data_02089a70, data_02089a6c);
        MTask *fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(data_02089a70, data_02089a6c);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fades);
        serial->AddTail(exit);
    }
}

// Turns to the tab after a frame, and keys the tabs either side of it to L and R.
void DinoUiReviveStatusMenu::SetTab(u8 tab) {
    if (tab > 3 || mUnk11C == tab) {
        return;
    }
    mUnk11C = tab;
    DinoSoundPlaySe(0x94, 0);
    MTask *setPage = MCreateCallTask(this, data_ov007_02191e80.SetPage_00, tab);
    MTask *timer = new MTimer(1);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(setPage);
    Disable();
    mItems[tab % 4]->SetKeys(0);
    mItems[(tab + 1) % 4]->SetKeys(0x400);
    mItems[(tab + 2) % 4]->SetKeys(0);
    mItems[(tab + 3) % 4]->SetKeys(0x800);
    func_ov007_02189170(this);
}

// Turns the information to the page; help comes back once it has.
void DinoUiReviveStatusMenu::SetPage(u8 page) {
    mInfo->SetPage(page);
    GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial),
                         MCreateCallTask(this, data_ov007_02191eb0.Enable_00));
    Disable();
}

extern const int data_02089a74;
struct DinoMessageData;
// The revive messages' file, then (at +0x14) what goes before the second answer.
extern "C" const char data_ov007_02192300[];
extern const u32 data_0208a49c, data_0208a4a0, data_0208a4a4, data_0208a4a8;

// The message's text.
static inline const char *GetMessageText(const DinoMessageData *messages, u32 id) {
    const u8 *data = (const u8 *)messages;
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[id];
    return (const char *)entry + ((const u32 *)entry)[1];
}

// Leaves; with a result to give and rocks still to clean, first asks whether to keep reviving.
void DinoUiReviveStatusMenu::OnBackDecided() {
    Disable();
    MTask *wait = MTaskSetWaitEmptyTask::Create(mUnkE0);
    MTask *timer = new MTimer(data_02089a74);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(wait);
    if (mResult != NULL) {
        if (gDinoBackup->HasInactiveUnk98()) {
            gLcd->mMain->mBg->SetMode(0);
            *mResult = 0;
            MResource<DinoMessageData> messages((std::string(data_ov007_02192300)));
            DinoEventTalkWindow *talk = new DinoEventTalkWindow(
                gCurrentDisplay, std::string(GetMessageText(messages.Get(), 17)), (int)mOam, 0);
            talk->SetYesNo((u32)mResult, MPoint(0xFE, 0x8E), 3, std::string(gDinoResource->GetText(data_0208a4a8)),
                           std::string(data_ov007_02192300 + 0x14) + gDinoResource->GetText(data_0208a4a4),
                           std::string(gDinoResource->GetText(data_0208a4a0)),
                           std::string(gDinoResource->GetText(data_0208a49c)));
            mSerial.AddTail(talk);
            Disable();
        } else {
            *mResult = 1;
        }
    }
    Leave();
}
