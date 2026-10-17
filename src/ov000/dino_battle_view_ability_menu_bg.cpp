// DinoBattleViewAbilityMenuB.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_ability_menu_bg.h"
#include "types.h"
#include "m/motion.h"
#include "m/bg.h"
#include "m/text.h"
#include "m/pal.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_view_ui_ability_menu_item.h"

extern "C" int data_ov000_021978dc;    // live ability menu BGs

DinoBattleViewAbilityMenuBG::~DinoBattleViewAbilityMenuBG() {
    if (mUnk10D4) {
        mBg->DeleteText(mText);
        mBg->DeleteChr(mChr);
        mBg->mPal->Free(mPalBlock);
    }
    data_ov000_021978dc--;
}

void DinoBattleViewUiAbilityMenuItem::OnDecide() {
}

MRect DinoBattleViewUiAbilityMenuItem::GetFrame() {
    return mUnk98->GetFrame();
}

#pragma auto_inline off

AttackMenuBarMotion::AttackMenuBarMotion(void *owner, MSmartOam *oam, u32 flags) : MProcess(NULL) {
    mOwner = owner;
    mFlags = flags;
    bool flag1 = (flags & 0x10000) ? true : false;
    bool flag2 = (flags & 0x20000) ? true : false;
    Init(oam, (u16)flags, flag1, flag2, false, false);
}

AttackMenuBarMotion::AttackMenuBarMotion(void *owner, MSmartOam *oam, u32 attack, bool flag1, bool flag2, bool flag3, bool flag4)
    : MProcess(NULL) {
    mOwner = owner;
    mFlags = (u16)attack | (flag1 ? 0x10000 : 0) | (flag2 ? 0x20000 : 0);
    Init(oam, attack, flag1, flag2, flag3, flag4);
}

void AttackMenuBarMotion::SetPriority(int priority) {
    mMotion->SetPriority(priority);
    mUnk80->SetPriority(priority);
    mUnk7C->SetPriority(priority);
    if (mUnk78 != NULL) {
        mUnk78->SetPriority(priority);
    }
}

MPoint AttackMenuBarMotion::GetPosition() {
    return mMotion->GetPosition();
}

void AttackMenuBarMotion::Hide() {
    mMotion->Hide();
    mUnk80->Hide();
    mUnk7C->Hide();
    if (mUnk78 != NULL) {
        mUnk78->Hide();
    }
}

void AttackMenuBarMotion::Show() {
    mMotion->Show();
    mUnk80->Show();
    mUnk7C->Show();
    if (mUnk78 != NULL) {
        mUnk78->Show();
    }
}

MTask *AttackMenuBarMotion::CreateShowTask() {
    return new ShowTask(this);
}

void AttackMenuBarMotion::Play(int anime) {
    mMotion->Play(anime, 0);
}

MRect AttackMenuBarMotion::GetFrame() {
    return mMotion->GetFrame();
}

MPoint DinoBattleViewUiAbilityMenuItem::GetBarPosition() {
    return mUnk98->GetPosition();
}

u32 DinoBattleViewUiAbilityMenuItem::GetUnk8C() {
    return mUnk8C;
}

u8 DinoBattleViewUiAbilityMenuItem::GetUnk94() {
    return mUnk94;
}

u32 DinoBattleViewUiAbilityMenuItem::GetUnk90() {
    return mUnk90;
}

AttackMenuBarMotion *DinoBattleViewUiAbilityMenuItem::GetBar() {
    return mUnk98;
}

MRect DinoBattleViewUiAbilityMenuItem::GetRect() {
    return GetFrame();
}

#pragma auto_inline on

extern "C" MTask *func_ov000_0218ec8c(u32 arg0, int arg1, int arg2);

void DinoBattleViewUiAbilityMenu::OnIdle() {
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnk104)->GetUnk12C(), -1, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
}
