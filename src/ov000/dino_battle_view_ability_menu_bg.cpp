// DinoBattleViewAbilityMenuB.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_ability_menu_bg.h"
#include "types.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_view_ui_ability_menu_item.h"

DinoBattleViewAbilityMenuBG::~DinoBattleViewAbilityMenuBG() {
}

void DinoBattleViewUiAbilityMenuItem::OnDecide() {
}

MRect DinoBattleViewUiAbilityMenuItem::GetFrame() {
    return mUnk98->GetFrame();
}

extern "C" MTask *func_ov000_0218ec8c(u32 arg0, int arg1, int arg2);

void DinoBattleViewUiAbilityMenu::OnIdle() {
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnk104)->GetUnk12C(), -1, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
}
