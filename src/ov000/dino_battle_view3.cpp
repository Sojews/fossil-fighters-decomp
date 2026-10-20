// DinoBattleView3.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view3.h"
#include "types.h"
#include "game/dino_static_init.h"
#include "m/motion.h"
#include <string>
#include "game/dino_sound.h"
#include "nitro/os.h"

// The TU's pooled statics: the panel's motion paths and pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u32 unk00;
    const char *panelPaths[2];  // 0x04: by side
    const char *unk0CPath;      // 0x0C
    const char *digitPath;      // 0x10
    u8 unk14[0x34 - 0x14];
    void (DinoBattleViewDecideFirstAttackTeam::*Start_34)();
} data_ov000_02197300;

#pragma auto_inline off
DinoBattleViewLPCountPanel::DinoBattleViewLPCountPanel(MSmartOam *oam, u32 side, u32 unk98, bool unk9C)
    : MProcess(NULL) {
    mSide = side;
    mUnk98 = unk98;
    mUnk9C = unk9C;
    mPanel = new MMotion(std::string(mSide == 0 ? data_ov000_02197300.panelPaths[0] : data_ov000_02197300.panelPaths[1]),
                         oam, 0, 1);
    mPanel->Show();
    mPanel->Play(1, 0);
    mParallel.AddTail(mPanel);
    for (int i = 0; i < 4; i++) {
        mDigits[i] = new MMotion(std::string(data_ov000_02197300.digitPath), oam, 0, 1);
        mDigits[i]->Show();
        mParallel.AddTail(mDigits[i]);
    }
    mUnk80 = new MMotion(std::string(data_ov000_02197300.unk0CPath), oam, 0, 1);
    mParallel.AddTail(mUnk80);
}

extern "C" const char data_ov000_021974a8[];   // the count's format
extern "C" const int data_0208a180;            // frames the count stays up

// (Takes the frames by reference: the game reads them after the timer's vtable is set.)
static inline MTimer *NewTimer(const int &frames) {
    return new MTimer(frames);
}

// Shows the count digit by digit from the last, then (with mUnk9C) plays the panel's finish.
extern "C" MTask *func_ov000_0218d318(DinoBattleViewLPCountPanel *panel, int delay) {
    MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
    tasks->AddTail(new MTimer(delay));
    char text[8];
    OS_SPrintf(text, data_ov000_021974a8, panel->mUnk98);
    for (int i = 3; i >= 0; i--) {
        if (text[i] != ' ') {
            tasks->AddTail(panel->mDigits[i]->CreatePlayTransfer(text[i] - '0' + 1, 0));
            if (!panel->mUnk9C) {
                tasks->AddTail(DinoSoundCreatePlaySeTask(100, 0));
            }
        }
        tasks->AddTail(new MTimer(4));
    }
    tasks->AddTail(NewTimer(data_0208a180));
    if (panel->mUnk9C) {
        panel->mUnk80->Show();
        MTask *play;
        MTask *timer;
        MTask *se;
        MTask *wait;
        wait = panel->mPanel->CreateWaitStopTask();
        se = DinoSoundCreatePlaySeTask(0x65, 0);
        timer = new MTimer(8);
        play = panel->mPanel->CreatePlayTransfer(2, 0);
        tasks->AddTail(panel->mUnk80->CreatePlayTransfer(3, 0));
        tasks->AddTail(play);
        tasks->AddTail(timer);
        tasks->AddTail(se);
        tasks->AddTail(wait);
    }
    return tasks;
}
#pragma auto_inline on

DinoBattleViewLPCountPanel::~DinoBattleViewLPCountPanel() {
}

DinoBattleViewDecideFirstAttackTeam::~DinoBattleViewDecideFirstAttackTeam() {
}

void DinoBattleViewLPCountPanel::Idle() {
}

void DinoBattleViewDecideFirstAttackTeam::Idle() {
    (this->*mState)();
}


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
