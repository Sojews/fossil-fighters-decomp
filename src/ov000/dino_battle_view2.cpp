// DinoBattleView2.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/input.h"
#include "m/console.h"
#include "game/dino_battle_debug_log.h"
#include "game/dino_resource.h"
#include "m/obj.h"
#include "game/dino_sound.h"
#include "game/dino_battle_view2.h"
#include "game/dino_message.h"
#include "types.h"
#include "game/dino_battle_view_class.h"
#include "game/wait_action_info_bar_task.h"
#include "m/smart_oam.h"
#include "game/creature_motion.h"
#include "game/dino_battle_view_creature.h"

AnyInputTimerWait::~AnyInputTimerWait() {
}

DinoBattleViewUiRootSelect::~DinoBattleViewUiRootSelect() {
}

DinoBattleViewTurnEndDialog::~DinoBattleViewTurnEndDialog() {
}

DinoBattleViewGivingUpDialog::~DinoBattleViewGivingUpDialog() {
}

DinoBattleViewGivingUpMenu::~DinoBattleViewGivingUpMenu() {
}

MotionDisableHelp::~MotionDisableHelp() {
}

DinoBattleViewCreatureAttackIcon::~DinoBattleViewCreatureAttackIcon() {
}

DinoBattleViewUiRootSelectItem::~DinoBattleViewUiRootSelectItem() {
}

MenuExitTask::~MenuExitTask() {
}

WaitEnableRootMenu::~WaitEnableRootMenu() {
}

DinoBattleViewAbilityIcon::~DinoBattleViewAbilityIcon() {
    GetDisplay()->mObj->mAfn->Free(mAffine);
}

void DinoBattleViewUiRootSelectItem::OnDecide() {
}

// NONMATCHING: the frame copy loads the offset's y before x (register order only).
MRect DinoBattleViewCreatureAttackIcon::GetFrame() {
    CreatureMotion *motion = (CreatureMotion *) mCreature->mMotion;
    MSmartOam *oam = motion->mOam;
    MPoint offset(-oam->mOffset.x, -oam->mOffset.y);
    return motion->GetFrame().Moved(offset);
}

void DinoBattleViewCreatureAttackIcon::OnFocusChanged(BOOL value) {
}

void DinoBattleViewCreatureAttackIcon::OnDecide() {
}

MRect DinoBattleViewUiRootSelectItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoBattleViewUiRootSelectItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoBattleViewUiRootSelectItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoBattleViewUiRootSelectItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoBattleViewGivingUpMenu::Idle() {
    (this->*mState)();
}

// After the dialog: if the player gave up, tells the root menu (5) and exits.
void DinoBattleViewGivingUpMenu::StateDone() {
    if (mGaveUp) {
        *mResult = TRUE;
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *post = mView->CreatePostTask(new DinoBattleViewUiRootMenuResult(5, 0, 0));
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(post);
        serial->AddTail(exit);
    } else {
        Exit();
    }
}


void WaitEnableRootMenu::Execute() {
    if (((DinoBattleView *)mMenu)->GetUnk13C() <= 0) {
        Exit();
    }
}

void DinoBattleViewUiRootSelectItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(focused ? mFocusAnime : mBlurAnime, 0);
}

void MenuExitTask::Execute() {
    if (*mFlag) {
        mMenu->Exit();
    }
    Exit();
}

extern "C" MTask *func_ov000_0218ec8c(u32 arg0, int arg1, BOOL *proceed);

extern "C" const u32 data_0208a3a0, data_0208a3a4, data_0208a444;

// NONMATCHING: 99.9%, the game tests the flag with the branch the other way round (beq to the 1).
DinoBattleViewTurnEndDialog::DinoBattleViewTurnEndDialog(u32 view, MMessageObserver *target, BOOL focusNo)
    : DinoCommonYesNoMenu(std::string(gDinoResource->GetText(data_0208a444)), NULL, data_0208a3a4, data_0208a3a0,
                          !focusNo),
      mTarget(target), mUnkE0(view) {
}

void DinoBattleViewTurnEndDialog::OnStart() {
    DinoCommonYesNoMenu::OnStart();
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkE0)->GetUnk12C(), 0x20, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
}

AnyInputTimerWait::AnyInputTimerWait(int frames) : MProcess(NULL), mTimer(frames) {
    MSingleInputHub *hub = new MSingleInputHub(1);
    mParallel.AddTail(hub);
    mInput = hub->GetInput(0);
}

void AnyInputTimerWait::Idle() {
    if (mInput->GetPressed() & 0x1003) {
        mTimer = 0;
    }
    if (mTimer-- == 0) {
        Exit();
    }
}

void DinoBattleViewTurnEndDialog::OnFocus(MMenuItem *item) {
    DinoHelpItem *help = dynamic_cast<DinoHelpItem *>(item);
    if (help != NULL) {
        SetSelectedItem(help);
    }
    DinoSoundPlaySe(0x5E, 0);
}

void DinoBattleViewTurnEndDialog::OnDecide(MMenuItem *item) {
    DinoCommonYesNoMenu::OnDecide(item);
    DinoCommonYesNoMenuItem *yesNo = static_cast<DinoCommonYesNoMenuItem *>(item);
    BOOL proceed = TRUE;
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkE0)->GetUnk12C(), yesNo->IsYes() ? 0xC : 0xD, &proceed);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
    if (proceed) {
        if (yesNo->IsYes()) {
            DinoSoundPlaySe(0x61, 0);
            mSerial.AddTail(mTarget->CreatePostTask(new DinoBattleViewUiTurnEndResult(TRUE)));
        } else {
            DinoSoundPlaySe(0x63, 0);
            mSerial.AddTail(mTarget->CreatePostTask(new DinoBattleViewUiTurnEndResult(FALSE)));
        }
        mSerial.AddTail(MTaskExitTransfer::Create(this));
    }
}

void DinoBattleViewGivingUpDialog::OnFocus(MMenuItem *item) {
    DinoHelpItem *help = dynamic_cast<DinoHelpItem *>(item);
    if (help != NULL) {
        SetSelectedItem(help);
    }
    DinoSoundPlaySe(0x5E, 0);
}

void DinoBattleViewGivingUpDialog::OnDecide(MMenuItem *item) {
    DinoCommonYesNoMenu::OnDecide(item);
    DinoCommonYesNoMenuItem *yesNo = static_cast<DinoCommonYesNoMenuItem *>(item);
    DinoSoundPlaySe(yesNo->IsYes() ? 0x61 : 0x63, 0);
    *mGivingUp = yesNo->IsYes();
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

// The tutorial's hook first; otherwise START offers to give up.
void DinoBattleViewUiRootSelect::OnIdle() {
    MTask *task = func_ov000_0218ec8c(mView->GetUnk12C(), -1, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
        return;
    }
    if ((GetInput()->GetPressed() & 8) && ((const u8 *)mView->GetUnk12C())[0xD] == 0) {
        mGivingUp = FALSE;
        MenuExitTask *exit = new MenuExitTask(this, &mGivingUp);
        DinoBattleViewGivingUpMenu *menu = new DinoBattleViewGivingUpMenu(mObserver, &mGivingUp);
        MSerialTaskSet *serial = GetSerial();
        serial->AddTail(menu);
        serial->AddTail(exit);
    }
}

void DinoBattleViewUiRootSelect::OnFocus(MMenuItem *item) {
    DinoHelpItem *help = dynamic_cast<DinoHelpItem *>(item);
    if (help != NULL) {
        SetSelectedItem(help);
    }
    if (mSilentFocus) {
        mSilentFocus = FALSE;
        DinoSoundPlaySe(0x5D, 0);
        return;
    }
    DinoSoundPlaySe(0x5E, 0);
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0xC];
    void (DinoBattleViewGivingUpMenu::*StateDone_0C)();
    void (DinoBattleViewUiRootSelect::*StateIdle_14)();
    u8 unk1C[0x4C - 0x1C];
    void (DinoBattleViewGivingUpMenu::*StateAsk_4C)();
    u8 unk54[0x5C - 0x54];
    void (DinoBattleViewUiRootSelect::*StateBegin_5C)();
    u8 unk64[0x6C - 0x64];
    void (DinoBattleViewGivingUpMenu::*StateAnswered_6C)();
} data_ov000_0219569c;

// "Give up the battle?" (a text id).
extern "C" const u32 data_0208a3c4;

DinoBattleViewGivingUpMenu::DinoBattleViewGivingUpMenu(MMessageObserver *view, u8 *result)
    : MProcess(NULL), mView(view), mResult(result) {
    mState = data_ov000_0219569c.StateAsk_4C;
}

void DinoBattleViewGivingUpMenu::StateAsk() {
    *mResult = FALSE;
    mSerial.AddTail(new DinoBattleViewGivingUpDialog(gDinoResource->GetText(data_0208a3c4), &mGaveUp));
    mState = data_ov000_0219569c.StateAnswered_6C;
}

// Asks again to be sure.
void DinoBattleViewGivingUpMenu::StateAnswered() {
    if (mGaveUp) {
        mSerial.AddTail(new DinoBattleViewGivingUpDialog(gDinoResource->GetText(data_0208a3c4), &mGaveUp));
        mState = data_ov000_0219569c.StateDone_0C;
        return;
    }
    Exit();
}

#pragma auto_inline off

// R shows the next page of the log (when the log is on).
void DinoBattleDebugLog::Idle() {
    if (gDinoResource->GetBattleAdjustDefs()->debugLog) {
        if (mInputHub->GetInput(0)->mButtons & 0x400) {
            Clear();
        }
    }
}

// Adds a line to the log (when the log is on).
void DinoBattleDebugLog::Print(const char *text) {
    if (gDinoResource->GetBattleAdjustDefs()->debugLog) {
        mConsole->Print(std::string(text) + "\n");
    }
}

void DinoBattleDebugLog::Clear() {
    if (gDinoResource->GetBattleAdjustDefs()->debugLog) {
        mConsole->ClearText();
        mConsole->Clear();
    }
}

#pragma auto_inline off

extern "C" const u32 data_0208a39c, data_0208a3a0;

DinoBattleViewGivingUpDialog::DinoBattleViewGivingUpDialog(const char *text, u8 *givingUp)
    : DinoCommonYesNoMenu(std::string(text), NULL, data_0208a39c, data_0208a3a0, 1), mGivingUp(givingUp) {
}

void DinoBattleViewUiRootSelect::StateBegin() {
    mState = data_ov000_0219569c.StateIdle_14;
}

void DinoBattleViewUiRootSelect::StateIdle() {
}

DinoBattleViewUiRootSelect::DinoBattleViewUiRootSelect(DinoBattleView *view, MMessageObserver *observer, u32 unkC0,
                                                       u32 unkC4, u32 unkBC, u32 unkC8, u32 unkCC)
    : MMenu(NULL, 0), mUnkBC(unkBC), mUnkC0(unkC0), mUnkC4(unkC4), mUnkC8(unkC8), mUnkCC(unkCC), mUnkD0(0),
      mSilentFocus(TRUE), mView(view), mObserver(observer) {
    DinoHelpBase::Disable();
    WaitEnableRootMenu *enable = new WaitEnableRootMenu(mView);
    MTask *bar = new WaitActionInfoBarTask((DinoBattleViewActionInfoBar *)mView->GetUnk104());
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(bar);
    serial->AddTail(enable);
    mState = data_ov000_0219569c.StateBegin_5C;
}
