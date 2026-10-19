// DinoBattleLive.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_live.h"
#include "types.h"
#include "game/dino_battle_model.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_message.h"
#include "game/dino_static_init.h"

extern "C" BOOL func_ov000_0218c354(void *obj);

DinoBattleLive::DinoBattleLive(DinoBattleModel *model, const char *name) : DinoCommonLive(name, 1, 1) {
    mModel = model;
    mSource = (DinoBattleLiveSource *)mModel->GetUnk10C();
    mView = static_cast<DinoBattleView *>(mSource->mView);
}

DinoBattleLive::~DinoBattleLive() {
}

void DinoBattleLive::Open() {
}

void DinoBattleLive::ShowTutorial(MDisplay *display, const char *text) {
}

void DinoBattleLive::Signal(u32 id) {
    DinoBattleViewUnkE0 *target = mView->mUnkE0;
    target->Post(new DinoBattleViewLiveMessageSignal(id));
}

void DinoBattleLive::Close() {
    mView->Post(new DinoBattleViewLiveMessageSignal(0));
}

BOOL DinoBattleLive::IsBusy() {
    if (!func_ov000_0218c354(mView->mUnkE0->mUnk138)) {
        return TRUE;
    }
    return FALSE;
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_021974ac[];
static struct DinoBattleLiveStaticInit {
    DinoBattleLiveStaticInit() {
        DINO_POOL_WORD(data_ov000_021974ac, 0x14) = 128;
        DINO_POOL_WORD(data_ov000_021974ac, 0x18) = 176;
        DINO_POOL_WORD(data_ov000_021974ac, 0x4) = 8;
        DINO_POOL_WORD(data_ov000_021974ac, 0x8) = 150;
        DINO_POOL_WORD(data_ov000_021974ac, 0xc) = 64;
        DINO_POOL_WORD(data_ov000_021974ac, 0x10) = 112;
    }
} sDinoBattleLiveStaticInit;
