// DinoUiMultiplayMenu: the multiplay menu's code (its destructors and vtables are in dino_ui_multiplay_menu_item.cpp).

#include "game/dino_ui_multiplay_menu_item_menu.h"
#include "game/dino_ui_multiplay_menu_item.h"
#include "game/dino_ui_multiplay_menu_item2.h"
#include "m/motion.h"
#include "m/smart_oam.h"
#include "game/dino_guide_message.h"

#pragma auto_inline off

// The file's pooled data: the sprites' paths and places, and the help's enable function.
extern "C" struct {
    u8 unk00[4];
    const char *markPath04;
    const char *organizationTopPath08;
    const char *backPath0C;
    const char *organizationPath10;
    const char *wirelessPath14;
    const char *titlePath18;
    MPoint wirelessPos1C;
    u8 unk24[0x34 - 0x24];
    MPoint markPos34;
    u8 unk3C[0x7C - 0x3C];
    MPoint backPos7C;
    u8 unk84[0x9C - 0x84];
    MPoint organizationPos9C;
} data_ov004_02173b70;
// Text ids.
extern "C" const u32 data_0208a4dc;
extern "C" const u32 data_0208a4e0;
extern "C" const u32 data_0208a4e4;
extern "C" const u32 data_0208a908;
// (MCreateCallTask is only declared here: the game's copy follows the file.)
template <class T, class U> MTask *MCreateCallTask(T *obj, void (U::*func)());

DinoUiMultiplayMenu::DinoUiMultiplayMenu(int *result) : MMenu(NULL, 0), mResult(result), mSilentFocus(TRUE) {
    SetDeferStart(TRUE);
}

inline DinoUiMultiplayWirelessButton::DinoUiMultiplayWirelessButton(DinoUiMultiplayMenu *owner)
    : DinoUiMultiplayMenuItem(owner, data_ov004_02173b70.wirelessPath14, data_ov004_02173b70.wirelessPos1C,
                              data_0208a4e4) {
}

inline DinoUiMultiplayOrganizationButton::DinoUiMultiplayOrganizationButton(DinoUiMultiplayMenu *owner)
    : DinoUiMultiplayMenuItem2(owner, data_ov004_02173b70.organizationTopPath08, data_ov004_02173b70.organizationPath10,
                               data_ov004_02173b70.organizationPos9C, data_0208a4e0) {
}

inline DinoUiMultiplayMenuBackButton::DinoUiMultiplayMenuBackButton(DinoUiMultiplayMenu *owner)
    : DinoUiMultiplayMenuItem(owner, data_ov004_02173b70.backPath0C, data_ov004_02173b70.backPos7C, data_0208a4dc) {
    mMotion->Play(2, 0);
    SetKeys(2);
    SetMode(4);
}

void DinoUiMultiplayMenu::OnStart() {
    Disable();
    mOam = new MSmartOam(0x41, 0, TRUE);
    mOam->mHidden = TRUE;
    mSerial.AddTail(mOam->CreateShowTransfer());
    mWireless = new DinoUiMultiplayWirelessButton(this);
    AddItem(mWireless);
    mOrganization = new DinoUiMultiplayOrganizationButton(this);
    AddItem(mOrganization);
    DinoUiMultiplayMenuBackButton *back = new DinoUiMultiplayMenuBackButton(this);
    AddItem(back);
    SetLinkFallback(1);
    AddLink(mWireless, 1, NULL);
    AddLink(mOrganization, 1, NULL);
    AddLink(mOrganization, 2, back);
    AddLink(back, 2, mWireless);
    AddLink(mWireless, 4, back);
    AddLink(back, 4, mOrganization);
    AddEntry((u32)(DinoHelpItem *)back, 4, (u32)(DinoHelpItem *)mOrganization);
    MMotion *mark = new MMotion(std::string(data_ov004_02173b70.markPath04), mOam, 2, 1);
    mark->Play(1, 0);
    mark->SetPosition(data_ov004_02173b70.markPos34);
    mark->Show();
    mParallel.AddTail(mark);
    MMotion *title = new MMotion(std::string(data_ov004_02173b70.titlePath18), mOam, 2, 1);
    title->Play(1, 0);
    title->SetPosition(data_ov004_02173b70.markPos34);
    title->Show();
    mParallel.AddTail(title);
    mParallel.AddTail(mOam);
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208a908))));
    if (*mResult == 2) {
        FocusItem(mOrganization);
    } else {
        FocusItem(mWireless);
    }
    *mResult = 0;
    mSerial.AddTail(MCreateCallTask(this, &DinoHelpBase::Enable));
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
