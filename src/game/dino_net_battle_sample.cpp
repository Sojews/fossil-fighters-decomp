// DinoNetBattleSample.
// Only the destructors are decompiled so far.

#include "game/dino_backup.h"
#include "game/dino_net_battle_sample.h"
#include "game/dino_ui_team_select.h"
#include "m/overlay.h"

DinoNetBattleSample::~DinoNetBattleSample() {
}

void DinoNetBattleSample::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoNetBattleSample::*Unk02034130_04)();
    u8 unk0C[0x14 - 0xC];
    void (DinoNetBattleSample::*Start_14)();
    void (DinoNetBattleSample::*StateTeamSelect_1C)();
} data_020bd0d8;
// L"テスト%02d"
extern "C" const wchar_t data_020bd14c[];
extern "C" int swprintf(wchar_t *s, size_t n, const wchar_t *format, ...);

M_EXTERN_OVERLAY(ov003);
M_EXTERN_OVERLAY(ov007);

DinoNetBattleSample::DinoNetBattleSample() : MProcess(NULL) {
    mState = data_020bd0d8.Start_14;
}

void DinoNetBattleSample::StateTeamSelect() {
    mTeam = 0;
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov007));
    mSerial.AddTail(new DinoUiTeamSelect(&mTeam, 0, 0));
    mState = data_020bd0d8.Unk02034130_04;
}

// Debug: every creature at level 50, and two test teams.
void DinoNetBattleSample::StateSetup() {
    int i;
    for (i = 1; i <= 100; i++) {
        DinoCreatureState creature;
        gDinoBackup->GetCreature(i, &creature);
        creature.index = i;
        creature.parts[0].level = 50;
        gDinoBackup->SetCreature(i, &creature);
    }
    for (i = 1; i <= 2; i++) {
        DinoTeam team;
        gDinoBackup->GetTeam(i, &team);
        swprintf((wchar_t *)team.name, 16, data_020bd14c, i);
        team.slots[0] = i * 10 + 1;
        team.slots[1] = i * 10 + 2;
        team.slots[2] = i * 10 + 3;
        if (i == 2) {
            team.slots[3] = i * 10 + 4;
        }
        gDinoBackup->SetTeam(i, &team);
    }
    mState = data_020bd0d8.StateTeamSelect_1C;
}
