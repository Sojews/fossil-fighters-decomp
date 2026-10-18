// DinoBattleView2.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/obj.h"
#include "game/dino_sound.h"
#include "game/dino_battle_view2.h"
#include "game/dino_message.h"
#include "types.h"
#include "game/dino_battle_view_class.h"
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

extern "C" MTask *func_ov000_0218ec8c(u32 arg0, int arg1, int arg2);

void DinoBattleViewTurnEndDialog::OnStart() {
    DinoCommonYesNoMenu::OnStart();
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkE0)->GetUnk12C(), 0x20, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
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
