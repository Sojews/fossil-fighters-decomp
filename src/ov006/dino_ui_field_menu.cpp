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
#include "game/dino_ui_rv_status_menu_item_menu.h"
#include "m/bright.h"
#include "game/dino_static_init.h"

DinoUiFieldMenu::~DinoUiFieldMenu() {
}

void DinoUiFieldMenu::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x8];
    void (DinoUiFieldMenu::*StateChoose_08)();
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
    mState = data_ov006_02190da0.StateChoose_08;
}

// Opens what the menu chose: the status, container or item menu, or a creature's status (4-8, coming back to the
// status menu); leaving saves the team (or, with an empty team, insists on the status menu).
void DinoUiFieldMenu::StateChoose() {
    switch (mChoice) {
    case 0:
        if (mTeam.slots[0] != 0) {
            gDinoBackup->SetTeam(gDinoBackup->GetUnk34_26(), &mTeam);
        } else if (mHasCreature) {
            mChoice = 98;
            mSerial.AddTail(new DinoUiStatusMenu(&mChoice, &mTeam, &mTeam2));
            gDinoBackup->SetSystemVar(0x2C, 0);
            return;
        }
        gLcd->GetTopDisplay()->mBright->Reset();
        gLcd->GetBottomDisplay()->mBright->Reset();
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        break;
    case 1:
        mSerial.AddTail(new DinoUiStatusMenu(&mChoice, &mTeam, &mTeam2));
        gDinoBackup->SetSystemVar(0x2C, 0);
        break;
    case 2:
        mSerial.AddTail(new DinoUiContainerMenu(&mChoice, &mTeam, &mTeam2));
        gDinoBackup->SetSystemVar(0x2C, 1);
        break;
    case 3:
        mSerial.AddTail(new DinoUiItemMenu(&mChoice, &mTeam, &mTeam2));
        gDinoBackup->SetSystemVar(0x2C, 2);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        mSerial.AddTail(new DinoUiRvStatusMenu(mChoice - 4, &mTeam));
        mChoice = 1;
        break;
    case 9:
    default:
        Exit();
        break;
    }
}

// The file's static initialization (see dino_static_init.h).
static struct DinoUiFieldMenuStaticInit {
    DinoUiFieldMenuStaticInit() {
        DINO_POOL_WORD(data_ov006_02190da0, 0x20) = 128;
        DINO_POOL_WORD(data_ov006_02190da0, 0x24) = 176;
        DINO_POOL_WORD(data_ov006_02190da0, 0x28) = 8;
        DINO_POOL_WORD(data_ov006_02190da0, 0x2c) = 150;
        DINO_POOL_WORD(data_ov006_02190da0, 0x0) = 64;
        DINO_POOL_WORD(data_ov006_02190da0, 0x4) = 112;
    }
} sDinoUiFieldMenuStaticInit;
