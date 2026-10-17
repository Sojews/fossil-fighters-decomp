// DinoATMSample, DinoFieldMenuSample.
// Only the destructors are decompiled so far.

#include "game/dino_atm_sample.h"
#include "game/dino_field_menu_sample.h"
#include "game/dino_ui_atm.h"
#include "game/dino_ui_field_menu.h"

DinoATMSample::~DinoATMSample() {
}

DinoFieldMenuSample::~DinoFieldMenuSample() {
}

void DinoATMSample::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *atm = new DinoUiATM(0);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(atm);
    serial->AddTail(exit);
}

void DinoFieldMenuSample::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiFieldMenu(0);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}
