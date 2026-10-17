// DinoATMSample, DinoFieldMenuSample: debug launchers that set up save data and open the ATM / the field menu.

#include "game/dino_atm_sample.h"
#include "game/dino_field_menu_sample.h"
#include "game/dino_ui_atm.h"
#include "game/dino_ui_field_menu.h"
#include "game/dino_backup.h"
#include <stdlib.h>

DinoATMSample::~DinoATMSample() {
}

DinoFieldMenuSample::~DinoFieldMenuSample() {
}

DinoATMSample::DinoATMSample() : MProcess(NULL) {
    DinoCreatureState state;
    DinoTeam team;
    int i;
    u32 slot;

    for (i = 1; i <= 0x56; i++) {
        gDinoBackup->GetCreature(i, &state);
        state.index = i;
        state.parts[0].level = i / 2 + 50;
        state.parts[1].level = rand() % 100;
        state.parts[2].level = rand() % 100;
        state.parts[3].level = rand() % 100;
        gDinoBackup->SetCreature(i, &state);
    }
    for (i = 0x6B; i <= 0x6F; i++) {
        gDinoBackup->GetCreature(i, &state);
        state.index = i;
        state.parts[0].level = 600;
        state.parts[1].level = 0;
        state.parts[2].level = 0;
        state.parts[3].level = 0;
        gDinoBackup->SetCreature(i, &state);
    }
    gDinoBackup->GetTeam(0, &team);
    team.slots[0] = 1;
    team.slots[1] = 2;
    team.slots[2] = 3;
    gDinoBackup->SetTeam(0, &team);
    slot = gDinoBackup->GetUnk34_26();
    if (slot < 1 || slot > 3) {
        slot = 1;
        gDinoBackup->SetUnk34_26(1);
    }
    gDinoBackup->SetTeam(slot, &team);
}

void DinoATMSample::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *atm = new DinoUiATM(0);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(atm);
    serial->AddTail(exit);
}

DinoFieldMenuSample::DinoFieldMenuSample() : MProcess(NULL) {
    int i;

    for (i = 1; i <= 12; i++) {
        gDinoBackup->SetKeyItem(i, TRUE);
    }
}

void DinoFieldMenuSample::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiFieldMenu(0);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}
