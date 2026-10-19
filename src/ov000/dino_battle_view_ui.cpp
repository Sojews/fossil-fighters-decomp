// DinoBattleViewUi.

// The game built this file without common-subexpression elimination (loops reload their bounds).
#pragma opt_common_subs off

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/dino_message.h"
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
#include "game/dino_battle_view_kaseki_point_menu.h"

// The tutorial step for an action (or -1): a task to run, and whether the action is blocked.
extern "C" MTask *func_ov000_0218ec8c(u32 arg0, int arg1, BOOL *blocked);

DinoBattleViewUiSelectAreaCommon::~DinoBattleViewUiSelectAreaCommon() {
}

DinoBattleViewUiChangeMenu::~DinoBattleViewUiChangeMenu() {
}

DinoBattleViewUiAttackerMenu::~DinoBattleViewUiAttackerMenu() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoBattleViewCreatureInfo, DinoBattleViewCreatureInfo>;
template MTask *MCreateCallTask(DinoBattleViewCreatureInfo *obj, void (DinoBattleViewCreatureInfo::*func)());

DinoBattleViewUiDefenderMenu::~DinoBattleViewUiDefenderMenu() {
    for (int i = 0; i < mCount; i++) {
        DinoBattleViewCreature *creature = ((DinoBattleView *)mUnkEC)->FindCreatureByAC(mIds[i]);
        if (((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0)->GetUnkAC() == creature->GetUnkB0()) {
            creature->ClearFlag144(4);
        } else {
            creature->ClearFlag144(3);
        }
    }
}

DinoBattleViewUiSelectAllMenu::~DinoBattleViewUiSelectAllMenu() {
    for (int i = 0; i < mCount; i++) {
        DinoBattleViewCreature *creature = ((DinoBattleView *)mView)->FindCreatureByAC(mIds[i]);
        if (((DinoBattleView *)mView)->GetUnkE4(mPlayer)->GetUnkAC() == creature->GetUnkB0()) {
            creature->ClearFlag144(4);
        } else {
            creature->ClearFlag144(3);
        }
    }
}

void DinoBattleViewUiSelectAllMenu::OnDecide(MMenuItem *item) {
    DinoHelpBase::Disable();
    if (item == mOkItem) {
        DinoSoundPlaySe(0x63, 0);
        mTarget->Post(new DinoBattleViewUiDefenderMenuResult(mIds[0], TRUE));
    } else {
        DinoSoundPlaySe(0x61, 0);
        mTarget->Post(new DinoBattleViewUiDefenderMenuResult(mIds[0], FALSE));
    }
    Exit();
}

DinoBattleViewUiRootMenuItem::~DinoBattleViewUiRootMenuItem() {
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

// The view's frame, moved; off the screen while it's hidden.
// NONMATCHING: the game offsets the frame in the return slot (and needs no copy of the offset).
MRect DinoBattleViewInfoLRItem::GetFrame() {
    if (mView->IsHidden()) {
        return MRect(0x140, 0x140, 0x140, 0x140);
    }
    MPoint offset = mOffset;
    MRect frame = mView->GetFrame();
    frame.Offset(offset);
    return frame;
}

void DinoBattleViewInfoLRItem::OnDecide() {
}

// The root menu's rows (two words each).
struct DinoBattleViewUiRootRow {
    u32 unk0;
    u32 unk4;
};
extern "C" const DinoBattleViewUiRootRow data_ov000_0218fec4[];

u32 DinoBattleViewUiRootMenuItem::GetUnk4() {
    return data_ov000_0218fec4[mIndex].unk4;
}

u32 DinoBattleViewUiRootMenuItem::GetUnk0() {
    return data_ov000_0218fec4[mIndex].unk0;
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

#pragma auto_inline off
BOOL DinoBattleViewUiDefenderMenu::CanDefend(u32 id) {
    for (int i = 0; i < mCount; i++) {
        if (id == mIds[i]) {
            return TRUE;
        }
    }
    return FALSE;
}
#pragma auto_inline reset

extern const u32 data_ov000_0218fd00[]; // tutorial steps by creature slot

void DinoBattleViewUiDefenderMenu::OnDecide(MMenuItem *item) {
    DinoBattleViewUiRootMenuItem *root = dynamic_cast<DinoBattleViewUiRootMenuItem *>(item);
    if (HandlePageItem(item)) {
        return;
    }
    if (root != NULL) {
        DinoBattleViewCreature *creature = GetItemCreature(root);
        if (root->mDisabled) {
            DinoSoundPlaySe(0x62, 0);
            return;
        }
        if (creature != NULL) {
            if (CanDefend(creature->GetUnkAC())) {
                BOOL blocked = FALSE;
                MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(),
                                                  data_ov000_0218fd00[creature->GetUnkB4()], &blocked);
                if (task != NULL) {
                    mSerial.AddTail(task);
                }
                if (blocked) {
                    return;
                }
                DinoHelpBase::Disable();
                DinoSoundPlaySe(0x61, 0);
                ResetRows();
                DinoHelpBase::Disable();
                MTask *exit = MTaskExitTransfer::Create(this);
                MTask *post = mTarget->CreatePostTask(new DinoBattleViewUiDefenderMenuResult(creature->GetUnkAC(), FALSE));
                MTask *wait = MTaskSetWaitEmptyTask::Create(mUnkF8);
                MSerialTaskSet *serial = &mSerial;
                serial->AddTail(wait);
                serial->AddTail(post);
                serial->AddTail(exit);
            } else {
                DinoSoundPlaySe(0x62, 0);
            }
            return;
        }
    }
    if (item == mItem120) {
        BOOL blocked = FALSE;
        MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), 0x27, &blocked);
        if (task != NULL) {
            mSerial.AddTail(task);
        }
        if (!blocked) {
            DinoHelpBase::Disable();
            DinoSoundPlaySe(0x63, 0);
            ResetRows();
            DinoHelpBase::Disable();
            MTask *exit = MTaskExitTransfer::Create(this);
            MTask *post = mTarget->CreatePostTask(new DinoBattleViewUiDefenderMenuResult(0, TRUE));
            MTask *wait = MTaskSetWaitEmptyTask::Create(mUnkF8);
            MSerialTaskSet *serial = &mSerial;
            serial->AddTail(wait);
            serial->AddTail(post);
            serial->AddTail(exit);
        }
    }
}

void DinoBattleViewUiSelectAreaCommon::OnDecide(MMenuItem *item) {
    Exit();
}

extern "C" void func_ov000_02177590(void *owner, DinoBattleViewUiRootMenuItem *item, int arg2, int arg3);

void DinoBattleViewUiRootMenuItem::OnSelectChanged(BOOL selected) {
    Refresh();
}

#pragma auto_inline off

void DinoBattleViewUiRootMenuItem::Refresh() {
}

#pragma auto_inline on

MRect DinoFrameItem::GetFrame() {
    return mFrame;
}


void DinoBattleViewUiChangeMenu::OnIdle() {
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), -1, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
}

// Has the 3D view show the focused creature's status, and marks its type advantages against the other side.
void DinoBattleViewUiCreatureInfoMenu::OnFocus(MMenuItem *item) {
    DinoBattleViewUiSelectAreaCommon::OnFocus(item);
    if (item == mItem124 || item == mItem128 || item == mItem120) {
        return;
    }
    DinoSoundPlaySe(0x5F, 0);
    DinoBattleViewCreature *creature = GetItemCreature(item);
    ((DinoBattleView *)mUnkEC)->Post(new DinoBattleView3DWatchStatusRequest(0, creature->GetUnkAC()));
    DinoBattleViewUiRootMenuItem *root = dynamic_cast<DinoBattleViewUiRootMenuItem *>(item);
    if (root != NULL) {
        DinoBattleViewCreature *focused = GetItemCreature(root);
        DinoBattleViewPlayer *player = (DinoBattleViewPlayer *)focused->GetUnkDC();
        DinoBattleViewPlayer *opponent = player->GetOpponent();
        for (int i = 0; i < opponent->GetCreatureCount(); i++) {
            opponent->GetCreature(i)->SetAdvantage(focused);
        }
        for (int i = 0; i < player->GetCreatureCount(); i++) {
            player->GetCreature(i)->SetAdvantage(NULL);
        }
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
    Refresh();
}


void DinoBattleViewUiAttackerMenu::OnBlur(MMenuItem *item) {
    DinoBattleViewUiRootMenuItem *root = dynamic_cast<DinoBattleViewUiRootMenuItem *>(item);
    if (root != NULL) {
        GetItemCreature(root)->ClearFlag144(1);
    }
}

#pragma auto_inline off

void DinoBattleViewUiSelectAreaCommon::Nop() {
}

BOOL DinoBattleViewUiChangeMenu::HasId(u32 id) {
    for (int i = 0; i < mCount; i++) {
        if (id == mIds[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL DinoBattleViewUiAttackerMenu::HasId(u32 id) {
    for (int i = 0; i < mCount; i++) {
        if (id == mIds[i]) {
            return TRUE;
        }
    }
    return FALSE;
}
