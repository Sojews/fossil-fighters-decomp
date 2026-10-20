// DinoCleaningBlower: blowing the dust off with the microphone, or by tapping L/R.

#include "game/dino_cleaning_blower.h"
#include "game/dino_blower.h"
#include "game/dino_cleaning.h"
#include "game/dino_common_live.h"
#include "game/dino_sound.h"
#include "m/input.h"
#include "game/dino_static_init.h"

extern "C" void func_ov002_0215fdec(DinoCleaningKasekiRock *rock);

DinoCleaningBlower::DinoCleaningBlower(DinoCleaning *cleaning) : MProcess(NULL) {
    u32 i;

    mCleaning = cleaning;
    for (i = 0; i < 25; i++) {
        mPressed[i] = FALSE;
    }
    mIndex = 0;
    mBlower = new DinoBlower();
    mParallel.AddTail(mBlower);
}

DinoCleaningBlower::~DinoCleaningBlower() {
}

void DinoCleaningBlower::Idle() {
    MInput *input = mCleaning->GetUnkDC();
    u32 count;
    u32 i;
    BOOL blowing;
    BOOL tapping;

    mPressed[mIndex] = (input->mButtons & (input->mPrevButtons ^ input->mButtons) & 0x300) ? TRUE : FALSE;
    mIndex = (mIndex + 1) % 25;
    count = 0;
    for (i = 0; i < 25; i++) {
        count += mPressed[i];
    }
    blowing = mBlower->IsBlowing();
    tapping = count >= 2 ? TRUE : FALSE;
    if (tapping) {
        mCleaning->GetUnkE4()->SetUnkAC(20);
        DinoSoundFunc_0201feec(0x2C);
    }
    if (blowing) {
        mCleaning->GetUnkE4()->SetUnkAC(19);
    }
    if ((blowing || tapping) && mCleaning->GetKasekiRock()) {
        func_ov002_0215fdec(mCleaning->GetKasekiRock());
    }
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov002_02164da4[];
#pragma always_inline on
static struct DinoCleaningBlowerStaticInit {
    DinoCleaningBlowerStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_02164da4, 0x0, 0x38, 0x18, 0x10, 0x48, 0x58, 0x60, 0x68);
    }
} sDinoCleaningBlowerStaticInit;
