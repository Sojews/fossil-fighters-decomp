// DinoBlower and DinoBlower2: the microphone blower.

#include "game/dino_blower.h"
#include <algorithm>
#include <stdlib.h>
#include "nitro/mi.h"
#include "nitro/mic.h"
#include "nitro/pm.h"
#include "nitro/pxi.h"

DinoBlower::DinoBlower() : MLooperPreLoopHook(FALSE, TRUE) {
    MI_CpuFill8(mBufferSpace, 0, sizeof(mBufferSpace));
    mBuffer = (s8 *) (((u32) mBufferSpace + 31) & ~31);
    MICAutoParam param;
    param.type = MIC_SAMPLING_TYPE_SIGNED_8BIT;
    param.buffer = mBuffer;
    param.size = DINO_BLOWER_SAMPLES;
    param.rate = MIC_SAMPLING_RATE_16K;
    param.loop_enable = TRUE;
    param.full_callback = NULL;
    param.full_arg = NULL;
    PXI_Init();
    PM_Init();
    PM_SetAmp(1);
    PM_SetAmpGain(0);
    MIC_Init();
    MIC_StartAutoSampling(&param);
    mEnergy = 0;
    mBlowing = FALSE;
    mSettleFrames = 180;
    MSleepHook::Register();
}

DinoBlower::~DinoBlower() {
    MSleepHook::Unregister();
    MIC_StopAutoSampling();
    PM_SetAmp(0);
    DC_FlushAll();
}

u8 DinoBlower::IsBlowing() {
    return mBlowing;
}

void DinoBlower::OnSleep() {
}

void DinoBlower::OnWake() {
    mSettleFrames = 180;
}

void DinoBlower::OnPreLoop() {
    if (mSettleFrames != 0) {
        mSettleFrames--;
    }
}

void DinoBlower::Execute() {
    int sum = 0;
    if (mSettleFrames == 0) {
        s8 *p = mBuffer;
        for (u32 i = 0; i < DINO_BLOWER_SAMPLES; i++) {
            u32 level = abs(*p);
            if (level > 27) {
                sum += level;
            }
            p++;
        }
    }
    mEnergy -= mEnergy >> 3;
    int energy = labs(sum);
    if (energy >= 10000) {
        mEnergy += energy;
    }
    mBlowing = mEnergy >= 10000;
    if (mBlowing) {
        mEnergy -= 10000;
    }
}

DinoBlower2::DinoBlower2() : MLooperPreLoopHook(FALSE, FALSE) {
    MI_CpuFill8(mBufferSpace, 0, sizeof(mBufferSpace));
    mBuffer = (s8 *) (((u32) mBufferSpace + 31) & ~31);
    MICAutoParam param;
    param.type = MIC_SAMPLING_TYPE_SIGNED_8BIT;
    param.buffer = mBuffer;
    param.size = DINO_BLOWER_SAMPLES;
    param.rate = MIC_SAMPLING_RATE_16K;
    param.loop_enable = TRUE;
    param.full_callback = NULL;
    param.full_arg = NULL;
    PXI_Init();
    PM_Init();
    PM_SetAmp(1);
    PM_SetAmpGain(0);
    MIC_Init();
    MIC_StartAutoSampling(&param);
    mStrength = 0;
    mSettleFrames = 180;
    MSleepHook::Register();
    MLooperPreLoopHook::Enable();
}

DinoBlower2::~DinoBlower2() {
    MSleepHook::Unregister();
    MIC_StopAutoSampling();
    PM_SetAmp(0);
    DC_FlushAll();
}

u32 DinoBlower2::GetStrength() {
    return mStrength;
}

void DinoBlower2::OnSleep() {
}

void DinoBlower2::OnWake() {
    mSettleFrames = 180;
}

void DinoBlower2::OnPreLoop() {
    if (mSettleFrames != 0) {
        mSettleFrames--;
    }
}

void DinoBlower2::Execute() {
    if (mSettleFrames != 0) {
        mStrength = 0;
        return;
    }
    u32 peak = 0;
    for (u32 i = 0; i < DINO_BLOWER_SAMPLES; i++) {
        u32 level = abs(mBuffer[i]);
        if (level > 27) {
            peak = std::max(level, peak);
        }
    }
    if (peak > 87) {
        mStrength = 4;
    } else if (peak > 57) {
        mStrength = 3;
    } else if (peak > 42) {
        mStrength = 2;
    } else if (peak > 27) {
        mStrength = 1;
    } else {
        mStrength = 0;
    }
}
