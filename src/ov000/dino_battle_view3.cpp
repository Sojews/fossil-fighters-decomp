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
#include "m/smart_oam.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view_class.h"

// The TU's pooled statics: the panel's motion paths and pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u32 unk00;
    const char *panelPaths[2];  // 0x04: by side
    const char *unk0CPath;      // 0x0C
    const char *digitPath;      // 0x10
    void (DinoBattleViewDecideFirstAttackTeam::*StateEnd_14)(); // 0x14
    u8 unk1C[0x34 - 0x1C];
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

// Where each panel slides in from and to (by its place), and the drop's start (the place's end point).
struct DinoLPPanelSlide {
    MPoint from;
    MPoint to;
};
extern "C" const DinoLPPanelSlide data_ov000_02197368[];
extern "C" u32 data_ov000_02197370[];
extern "C" const s32 data_ov000_02197374[];
extern "C" const u32 data_0208a174, data_0208a178, data_0208a17c, data_0208a184, data_0208a188, data_0208a1bc;
extern "C" MTask *func_ov000_0218d504(DinoBattleViewLPCountPanel *panel, MPoint from, MPoint to, int frames, int ease);

// NONMATCHING: the drop's end point stores its y before its x in the game.
void DinoBattleViewDecideFirstAttackTeam::StateStart() {
    u32 place;
    MTask *drop;
    mOam = new MSmartOam(20, 0, 1);
    MParallelTaskSet *slides = new MParallelTaskSet();
    mParallel.AddTail(slides);
    MParallelTaskSet *parallel = &mParallel;
    for (int i = 0; i < 2; i++) {
        place = ((DinoBattleView *)mUnk88)->GetUnkE4(i)->GetUnkAC();
        mPanels[i] = new DinoBattleViewLPCountPanel(mOam, place, (&mUnk90)[i], mUnk8C == i);
        parallel->AddTail(mPanels[i]);
        slides->AddTail(func_ov000_0218d504(mPanels[i], data_ov000_02197368[place].from, data_ov000_02197368[place].to,
                                            data_0208a1bc, data_0208a188));
    }
    mParallel.AddTail(mOam);
    GetSerial()->AddTail(DinoSoundCreatePlaySeTask(0x38B, 0), new MTimer(data_0208a1bc));
    MParallelTaskSet *counts = new MParallelTaskSet(TRUE);
    counts->AddTail(func_ov000_0218d318(mPanels[0], data_0208a184), func_ov000_0218d318(mPanels[1], data_0208a184));
    int loser = 1 - mUnk8C;
    place = ((DinoBattleView *)mUnk88)->GetUnkE4(loser)->GetUnkAC();
    drop = func_ov000_0218d504(mPanels[loser], *(const MPoint *)&data_ov000_02197370[place * 4],
                               MPoint((s32)data_ov000_02197370[place * 4], data_ov000_02197374[place * 4] + 128),
                               data_0208a178, data_0208a174);
    GetSerial()->AddTail(counts, new MTimer(data_0208a17c), drop);
    mState = data_ov000_02197300.StateEnd_14;
}

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
