// DinoUiMultiplayMenu: the multiplay menu's code (its destructors and vtables are in dino_ui_multiplay_menu_item.cpp).

#include "game/dino_ui_multiplay_menu_item_menu.h"
#include "game/dino_ui_multiplay_menu_item.h"
#include "game/dino_ui_multiplay_menu_item2.h"
#include "m/motion.h"

#pragma auto_inline off

DinoUiMultiplayMenu::DinoUiMultiplayMenu(int *result) : MMenu(NULL, 0), mResult(result), mSilentFocus(TRUE) {
    SetDeferStart(TRUE);
}

DinoUiMultiplayMenuItem::DinoUiMultiplayMenuItem(DinoUiMultiplayMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

// Two motions: the item, and one shown over it.
DinoUiMultiplayMenuItem2::DinoUiMultiplayMenuItem2(DinoUiMultiplayMenu *owner, const std::string &topPath,
                                                   const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    mMotion2 = new MMotion(std::string(topPath), mOwner->GetOam(), 2, 1);
    mMotion2->Play(1, 0);
    mMotion2->SetPosition(pos);
    mMotion2->Show();
    mParallel.AddTail(mMotion2);
}
