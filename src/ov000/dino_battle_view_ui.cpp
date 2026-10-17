// DinoBattleViewUi.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/dino_battle_view_ui_change_menu.h"
#include "types.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_view_creature.h"
#include "game/dino_battle_view_ui_attacker_menu.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_battle_view_ui_defender_menu.h"
#include "game/dino_battle_view_ui_select_all.h"
#include "game/dino_battle_view.h"
#include "game/dino_battle_view_creature_info.h"

DinoBattleViewUiChangeMenu::~DinoBattleViewUiChangeMenu() {
}

DinoBattleViewUiAttackerMenu::~DinoBattleViewUiAttackerMenu() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoBattleViewCreatureInfo, DinoBattleViewCreatureInfo>;
template MTask *MCreateCallTask(DinoBattleViewCreatureInfo *obj, void (DinoBattleViewCreatureInfo::*func)());

DinoBattleViewUiDefenderMenu::~DinoBattleViewUiDefenderMenu() {
}

DinoBattleViewUiSelectAllMenu::~DinoBattleViewUiSelectAllMenu() {
}

DinoBattleViewUiRootMenuItem::~DinoBattleViewUiRootMenuItem() {
}

DinoBattleViewUiSelectAreaCommon::~DinoBattleViewUiSelectAreaCommon() {
}

DinoBattleViewUiCreatureInfoMenu::~DinoBattleViewUiCreatureInfoMenu() {
}

DinoBattleViewInfoLRItem::~DinoBattleViewInfoLRItem() {
}

DinoBattleViewCancelMenuItem::~DinoBattleViewCancelMenuItem() {
}

DinoBattleHelpCreatureInfo::~DinoBattleHelpCreatureInfo() {
}

DinoFrameItem::~DinoFrameItem() {
}

void DinoBattleHelpBase::Idle() {
}

void DinoBattleViewCancelMenuItem::OnDecide() {
}

void DinoBattleViewInfoLRItem::OnDecide() {
}

void DinoBattleViewUiRootMenuItem::OnDecide() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.

MRect DinoBattleViewCancelMenuItem::GetFrame() {
    return MRect(0, 0, 256, 192);
}

void DinoBattleViewUiDefenderMenu::OnFocus(MMenuItem *item) {
    DinoBattleViewUiSelectAreaCommon::OnFocus(item);
    DinoSoundPlaySe(0x5F, 0);
}

void DinoBattleViewUiSelectAreaCommon::OnDecide(MMenuItem *item) {
    Exit();
}

extern "C" void func_ov000_02177804(DinoBattleViewUiRootMenuItem *item, ...);
extern "C" void func_ov000_02177590(void *owner, DinoBattleViewUiRootMenuItem *item, int arg2, int arg3);

void DinoBattleViewUiRootMenuItem::OnSelectChanged(BOOL selected) {
    func_ov000_02177804(this, selected);
}

MRect DinoFrameItem::GetFrame() {
    return mFrame;
}

extern "C" MTask *func_ov000_0218ec8c(u32 arg0, int arg1, int arg2);

void DinoBattleViewUiChangeMenu::OnIdle() {
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), -1, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
}

void DinoBattleViewUiCreatureInfoMenu::OnIdle() {
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), -1, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
}

void DinoBattleViewUiRootMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        func_ov000_02177590(mOwner, this, 8, -2);
    }
    func_ov000_02177804(this);
}

extern "C" DinoBattleViewCreature *func_ov000_02177644(DinoBattleViewUiAttackerMenu *menu);

void DinoBattleViewUiAttackerMenu::OnBlur(MMenuItem *item) {
    if (dynamic_cast<DinoBattleViewUiRootMenuItem *>(item) != NULL) {
        func_ov000_02177644(this)->ClearFlag144(1);
    }
}
