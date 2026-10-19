// DinoBattleView3.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view3.h"
#include "types.h"
#include "game/dino_static_init.h"

DinoBattleViewLPCountPanel::~DinoBattleViewLPCountPanel() {
}

DinoBattleViewDecideFirstAttackTeam::~DinoBattleViewDecideFirstAttackTeam() {
}

void DinoBattleViewLPCountPanel::Idle() {
}

void DinoBattleViewDecideFirstAttackTeam::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x34];
    void (DinoBattleViewDecideFirstAttackTeam::*Start_34)();
} data_ov000_02197300;

DinoBattleViewDecideFirstAttackTeam::DinoBattleViewDecideFirstAttackTeam(u32 unk88, u32 unk8C, u32 unk90, u32 unk94)
    : MProcess(NULL), mUnk88(unk88), mUnk8C(unk8C), mUnk90(unk90), mUnk94(unk94) {
    mState = data_ov000_02197300.Start_34;
}

#pragma auto_inline off

extern "C" const u32 data_0208a170;     // frames the result stays up

void DinoBattleViewDecideFirstAttackTeam::StateEnd() {
    GetSerial()->AddTail(new MTimer(data_0208a170), MTaskExitTransfer::Create(this));
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_02197370[];
extern "C" u32 data_ov000_02197378[];
extern "C" u32 data_ov000_02197380[];
static struct DinoBattleView3StaticInit {
    DinoBattleView3StaticInit() {
        DINO_POOL_WORD(data_ov000_02197300, 0x3c) = 128;
        DINO_POOL_WORD(data_ov000_02197300, 0x40) = 176;
        DINO_POOL_WORD(data_ov000_02197300, 0x2c) = 8;
        DINO_POOL_WORD(data_ov000_02197300, 0x30) = 150;
        DINO_POOL_WORD(data_ov000_02197300, 0x1c) = 64;
        DINO_POOL_WORD(data_ov000_02197300, 0x20) = 112;
        DINO_POOL_WORD(data_ov000_02197300, 0x68) = -64;
        DINO_POOL_WORD(data_ov000_02197300, 0x6c) = 96;
        DINO_POOL_WORD(data_ov000_02197370, 0x0) = 72;
        DINO_POOL_WORD(data_ov000_02197370, 0x4) = 96;
        DINO_POOL_WORD(data_ov000_02197378, 0x0) = 320;
        DINO_POOL_WORD(data_ov000_02197378, 0x4) = 96;
        DINO_POOL_WORD(data_ov000_02197380, 0x0) = 184;
        DINO_POOL_WORD(data_ov000_02197380, 0x4) = 96;
    }
} sDinoBattleView3StaticInit;
