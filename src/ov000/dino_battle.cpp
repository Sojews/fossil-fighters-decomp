// DinoBattle, DinoBattleAutoDemo, DinoBattleDebugSelect.
// Only the destructors are decompiled so far.

#include "game/battle_select.h"
#include "game/dino_battle.h"
#include "m/obj.h"
#include "m/bg.h"
#include "m/soft_reset.h"
#include "m/input.h"
#include "m/smart_oam.h"
#include "m/motion.h"
#include "game/dino_resource.h"
#include "game/dino_backup.h"
#include "game/dino_sound.h"
#include "m/blend.h"
#include "m/bright.h"
#include "m/display.h"
#include "m/lcd.h"
#include "game/dino_static_init.h"

DinoBattle::~DinoBattle() {
    delete mUnk208;
}

DinoBattleAutoDemo::~DinoBattleAutoDemo() {
    delete mUnk7C;
}

DinoBattleDebugSelect::~DinoBattleDebugSelect() {
}

void DinoBattleAutoDemo::Idle() {
    (this->*mState)();
}

void DinoBattle::SetUnk1DC(u8 value) {
    if (mUnk1DC) {
        *mUnk1DC = value;
    }
}

void DinoBattle::SetUnk1E0(u8 value) {
    if (mUnk1E0) {
        *mUnk1E0 = value;
    }
}

u32 DinoBattle::GetUnk1F4(int i) {
    return mUnk1F4[i];
}

u32 DinoBattle::GetUnk1FC() {
    return mUnk1FC;
}

MMessageObserver *DinoBattle::GetUnk200() {
    return mUnk200;
}

void DinoBattle::Idle() {
    (this->*mState)();
}

void DinoBattleDebugSelect::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    const char *mDemoPath;  // 0x04: the "demo" sign
    void (DinoBattleAutoDemo::*StateEnd_08)(); // 0x08
    u8 unk10[0x18 - 0x10];
    void (DinoBattleAutoDemo::*StateFadeOut_18)(); // 0x18
    void (DinoBattleDebugSelect::*StateLevelSelect_20)(); // 0x20
    void (DinoBattleDebugSelect::*Unk0215a05c_28)();
    u8 unk30[0x38 - 0x30];
    void (DinoBattleAutoDemo::*StateWatch_38)(); // 0x38
    void (DinoBattleAutoDemo::*StateEnd_40)(); // 0x40
    void (DinoBattleAutoDemo::*StateEnd_48)(); // 0x48
    u8 unk50[0x68 - 0x50];
    void (DinoBattle::*State0215a540_68)(); // 0x68
} data_ov000_021911ac;
extern "C" struct {
    u8 unk00[0x10];
    void (DinoBattleAutoDemo::*Start_10)();
} data_ov000_0219122c;

// Clears both screens and the music and opens the creature select.
void DinoBattleDebugSelect::StateCreatureSelect() {
    BOOL unk = mParams.unk00 != 2;
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mMain->mBright->Reset();
    gLcd->mSub->mBright->Reset();
    gLcd->mMain->mBlend->Reset();
    gLcd->mSub->mBlend->Reset();
    DinoSoundStopBgm(-1, 0);
    mSerial.AddTail(new UseCreatureSelect(&mParams, unk, 2, &mCancelled));
    mState = data_ov000_021911ac.StateLevelSelect_20;
}

void DinoBattleDebugSelect::StateLevelSelect() {
    if (mCancelled) {
        Exit();
        return;
    }
    BOOL unk = mParams.unk00 != 2;
    mSerial.AddTail(new PlayerLevelSelect(&mParams.unk00, unk, 2));
    mState = data_ov000_021911ac.Unk0215a05c_28;
}

DinoBattleAutoDemo::DinoBattleAutoDemo(MTask *unk7C, u8 *result) : MProcess(NULL), mUnk7C(unk7C), mResult(result) {
    mState = data_ov000_0219122c.Start_10;
}

// Starts the demo battle with a fresh save, under a "demo" sign, listening for a skip.
void DinoBattleAutoDemo::StateSetup() {
    gDinoBackup->Reset(FALSE);
    gDinoBackup->SetSystemFlag(0x28, TRUE);
    if (mUnk7C->mState == MTask::STATE_RUN) {
        mUnk7C->Execute();
    }
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, FALSE);
    player.unk1c_7 = 0;
    gDinoBackup->SetPlayer(&player);
    MSmartOam *oam = new MSmartOam(gLcd->GetBottomDisplay(), 1, 1, 0);
    MMotion *sign = new MMotion(data_ov000_021911ac.mDemoPath, oam, 0, 1);
    sign->SetPosition(0x80, 0x60);
    sign->GetUnit()->SetMode(1);
    sign->Play(1, 0);
    sign->Show();
    MParallelTaskSet *parallel = &mParallel;
    parallel->AddTail(sign);
    parallel->AddTail(oam);
    MSingleInputHub *hub = new MSingleInputHub(1);
    hub->EnableTouch();
    mInput = hub->GetInput(0);
    mParallel.AddTail(hub);
    mState = data_ov000_021911ac.StateWatch_38;
}

// Runs the battle; a key or touch ends the demo, as does watching enough of them.
void DinoBattleAutoDemo::StateWatch() {
    if (mUnk7C->mState == MTask::STATE_RUN) {
        mUnk7C->Execute();
    }
    if (mUnk7C->mState == MTask::STATE_EXIT) {
        Exit();
        return;
    }
    if (mInput->GetPressed() & 0x100B) {
        mState = data_ov000_021911ac.StateEnd_48;
        return;
    }
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, FALSE);
    BOOL done;
    {
        MResource<DinoExcavateDefs> defs = gDinoResource->GetExcavateDefs();
        const DinoExcavateDefs *data = defs.Get();
        const DinoDemoBattles *demos = DinoOffsetPtr<DinoDemoBattles>(data, data->demoOffset);
        done = player.unk1c_7 >= demos->unk18 + 1;
    }
    if (done) {
        mState = data_ov000_021911ac.StateEnd_08;
    }
}

// Once no screen is mid-fade, fades the music and both screens out to white.
void DinoBattleAutoDemo::StateEnd() {
    if (mUnk7C->mState == MTask::STATE_RUN) {
        mUnk7C->Execute();
    }
    if (mUnk7C->mState == MTask::STATE_EXIT) {
        Exit();
        return;
    }
    if ((gLcd->GetTopDisplay()->mBright->GetMode() == 0 || gLcd->GetTopDisplay()->mBright->GetFactor() == 0) &&
        (gLcd->GetBottomDisplay()->mBright->GetMode() == 0 || gLcd->GetBottomDisplay()->mBright->GetFactor() == 0)) {
        DinoSoundStopBgmAll(30);
        DinoSoundFunc_0201feb4(30);
        MTask *fadeTop;
        MTask *fadeBottom;
        fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToWhite(30, 0);
        fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToWhite(30, 0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        mSerial.AddTail(fades);
        mState = data_ov000_021911ac.StateFadeOut_18;
    }
}

// Restarts the game, telling the title screen the demo ran.
void DinoBattleAutoDemo::StateFadeOut() {
    MSoftReset((u32)mResult + 2);
}

// Sets both screens up for the battle, the touch screen at the bottom.
void DinoBattle::StateSetupDisplays() {
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mMain->Setup(1, 3, 2, 1);
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->VSyncNow();
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->mSub->mObj->Show();
    gLcd->SetBottomDisplay(gLcd->mSub);
    SetDisplay(gCurrentDisplay = gLcd->GetBottomDisplay());
    mState = data_ov000_021911ac.State0215a540_68;
}

// The file's static initialization (see dino_static_init.h).
static struct DinoBattleStaticInit {
    DinoBattleStaticInit() {
        DINO_POOL_WORD(data_ov000_0219122c, 0x8) = 128;
        DINO_POOL_WORD(data_ov000_0219122c, 0xc) = 176;
        DINO_POOL_WORD(data_ov000_021911ac, 0x40) = 8;
        DINO_POOL_WORD(data_ov000_021911ac, 0x44) = 150;
        DINO_POOL_WORD(data_ov000_021911ac, 0x30) = 64;
        DINO_POOL_WORD(data_ov000_021911ac, 0x34) = 112;
    }
} sDinoBattleStaticInit;

// Resets to the title (nothing in the game calls it).
extern "C" void func_ov000_0215a888() {
    MSoftReset(1);
}
