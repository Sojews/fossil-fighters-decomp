// DinoUiFieldMenu.
// Only the destructors are decompiled so far.

#include "m/obj.h"
#include "m/bg.h"
#include "m/display.h"
#include "m/lcd.h"
#include "game/dino_ui_item_item_button_menu.h"
#include "game/dino_ui_container_menu_item_menu.h"
#include "game/dino_ui_parts_tab_icon_menu_item_menu.h"
#include "game/dino_ui_field_menu.h"

DinoUiFieldMenu::~DinoUiFieldMenu() {
}

void DinoUiFieldMenu::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x8];
    void (DinoUiFieldMenu::*Unk02171cd8_08)();
    void (DinoUiFieldMenu::*Start_10)();
    u8 unk18[0x30 - 0x18];
    void (DinoUiFieldMenu::*StateMenu_30)();
} data_ov006_02190da0;

DinoUiFieldMenu::DinoUiFieldMenu(u32 arg) : MProcess(NULL), mUnk7C(arg) {
    mState = data_ov006_02190da0.Start_10;
}

void DinoUiFieldMenu::StateStart() {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(4, 0, 4, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(TRUE);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gDinoBackup->GetTeam(gDinoBackup->GetUnk34_26(), &mTeam);
    gDinoBackup->GetTeam(gDinoBackup->GetUnk34_26(), &mTeam2);
    mHasCreature = mTeam.slots[0] != 0 ? TRUE : FALSE;
    mState = data_ov006_02190da0.StateMenu_30;
}

// The tab chosen last: status, containers or items.
void DinoUiFieldMenu::StateMenu() {
    mChoice = 99;
    switch (gDinoBackup->GetSystemVar(0x2C)) {
    case 0:
        mSerial.AddTail(new DinoUiStatusMenu(&mChoice, &mTeam, &mTeam2));
        break;
    case 1:
        mSerial.AddTail(new DinoUiContainerMenu(&mChoice, &mTeam, &mTeam2));
        break;
    case 2:
        mSerial.AddTail(new DinoUiItemMenu(&mChoice, &mTeam, &mTeam2));
        break;
    }
    mState = data_ov006_02190da0.Unk02171cd8_08;
}
