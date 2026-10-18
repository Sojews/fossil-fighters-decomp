// DinoBattleView, EnableRootSelectTask.

#include "types.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/enable_root_select_task.h"

#pragma auto_inline off

MFont *DinoBattleView::GetUnk11C() {
    return mUnk11C;
}

MFont *DinoBattleView::GetUnk118() {
    return mUnk118;
}

MFont *DinoBattleView::GetUnk120() {
    return mUnk120;
}

MFont *DinoBattleView::GetUnk124() {
    return mUnk124;
}

DinoBattleView::~DinoBattleView() {
    mUnk118->Release();
    mUnk11C->Release();
    mUnk120->Release();
    mUnk124->Release();
    delete mUnk100;
}

const void *DinoBattleView::GetUnk11CPalette() {
    return mUnk11C->GetPalette();
}

DinoBattleViewPlayer *DinoBattleView::GetPlayerByAC(u32 id) {
    if (id == mUnkE4[0]->GetUnkAC()) {
        return mUnkE4[0];
    }
    return mUnkE4[1];
}

int DinoBattleView::GetUnk140() {
    if (GetUnk138()) {
        return -1;
    }
    return mUnk140;
}

DinoBattleViewPlayer *DinoBattleView::GetUnkE4(int index) {
    return mUnkE4[index];
}

u8 DinoBattleView::GetUnk138() {
    return mUnk138;
}

void DinoBattleView::SetUnk128(u32 value) {
    mUnk128 = value;
}

void DinoBattleView::SetUnk12C(u32 value) {
    mUnk12C = value;
}

u32 DinoBattleView::GetUnk12C() {
    return mUnk12C;
}

u32 DinoBattleView::GetUnk130() {
    return mUnk130;
}

u32 DinoBattleView::GetUnk134() {
    return mUnk134;
}

u32 DinoBattleView::GetUnkEC() {
    return mUnkEC;
}

void DinoBattleView::Idle() {
    Dispatch();
}

void DinoBattleView::OnMessage(MMessage *msg) {
    mSubject.Notify(msg);
}

DinoBattleViewCreature *DinoBattleView::FindCreatureByAC(u32 id) {
    for (int i = 0; i < 2; i++) {
        DinoBattleViewCreature *creature = mUnkE4[i]->FindCreatureByAC(id);
        if (creature != NULL) {
            return creature;
        }
    }
    return NULL;
}

u32 DinoBattleView::GetUnk104() {
    return mUnk104;
}

u32 DinoBattleView::GetUnk108() {
    return mUnk108;
}

void DinoBattleView::IncUnk13C() {
    mUnk13C++;
}

void DinoBattleView::DecUnk13C() {
    mUnk13C--;
}

int DinoBattleView::GetUnk13C() {
    return mUnk13C;
}

EnableRootSelectTask::~EnableRootSelectTask() {
}

void EnableRootSelectTask::Execute() {
    ((DinoBattleView *)mUnk10)->DecUnk13C();
    Exit();
}
