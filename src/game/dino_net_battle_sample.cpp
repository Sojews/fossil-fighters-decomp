// DinoNetBattleSample.

#include "game/dino_backup.h"
#include "game/dino_net_battle_sample.h"
#include "game/dino_ui_team_select.h"
#include "m/overlay.h"
#include "game/dino_resource.h"
#include "game/dino_ui_order_select.h"
#include "nitro/mi.h"

DinoNetBattleSample::~DinoNetBattleSample() {
}

void DinoNetBattleSample::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoNetBattleSample::*StateOrderSelect_04)();
    void (DinoNetBattleSample::*StateEnd_0C)();
    void (DinoNetBattleSample::*Start_14)();
    void (DinoNetBattleSample::*StateTeamSelect_1C)();
} data_020bd0d8;
// L"テスト%02d"
extern "C" const wchar_t data_020bd14c[];
extern "C" int swprintf(wchar_t *s, size_t n, const wchar_t *format, ...);

M_EXTERN_OVERLAY(ov003);
M_EXTERN_OVERLAY(ov006);
M_EXTERN_OVERLAY(ov007);

void DinoNetBattleSample::StateEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov003));
    MUnloadOverlay(0, M_OVERLAY_ID(ov006));
    Exit();
}

DinoNetBattleSample::DinoNetBattleSample() : MProcess(NULL) {
    mState = data_020bd0d8.Start_14;
}

void DinoNetBattleSample::StateTeamSelect() {
    mTeam = 0;
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov007));
    mSerial.AddTail(new DinoUiTeamSelect(&mTeam, 0, 0));
    mState = data_020bd0d8.StateOrderSelect_04;
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

extern "C" const u32 data_0208a430; // the opponent's name
extern "C" const u32 data_0208a0d4; // the levels the parts get

void DinoNetBattleSample::StateOrderSelect() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov003));
    MUnloadOverlay(0, M_OVERLAY_ID(ov007));
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov006));
    DinoUiOrderSelectInfo info;
    MI_CpuFill8(&info, 0, sizeof(info));
    info.name = gDinoResource->GetText(data_0208a430);
    for (int i = 0; i < 4; i++) {
        DinoCreatureState *creature = &info.creatures[i];
        creature->index = i + 1;
        int n = 12 - i;
        if (n > 12) {
            n = 12;
        }
        for (u32 j = 0; j < 4; j++) {
            creature->parts[j].level = 0;
            creature->parts[j].unk12 = 0;
        }
        switch (n) {
        case 12:
            creature->parts[3].level += data_0208a0d4;
        case 11:
            creature->parts[3].level += data_0208a0d4;
        case 10:
            creature->parts[3].level += data_0208a0d4;
        case 9:
            creature->parts[2].level += data_0208a0d4;
        case 8:
            creature->parts[2].level += data_0208a0d4;
        case 7:
            creature->parts[2].level += data_0208a0d4;
        case 6:
            creature->parts[1].level += data_0208a0d4;
        case 5:
            creature->parts[1].level += data_0208a0d4;
        case 4:
            creature->parts[1].level += data_0208a0d4;
        case 3:
            creature->parts[0].level += data_0208a0d4;
        case 2:
            creature->parts[0].level += data_0208a0d4;
        case 1:
            creature->parts[0].level += 50;
        }
    }
    mSerial.AddTail(new DinoUiOrderSelect(mTeam, info, 1, 0, 0, 0));
    mState = data_020bd0d8.StateEnd_0C;
}
