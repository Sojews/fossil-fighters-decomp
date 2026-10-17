// The parts of the NitroSDK MIC (microphone) library the game code uses (declarations only).
#ifndef NITRO_MIC_H
#define NITRO_MIC_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MIC_SAMPLING_TYPE_8BIT = 0,
    MIC_SAMPLING_TYPE_12BIT,
    MIC_SAMPLING_TYPE_SIGNED_8BIT,
    MIC_SAMPLING_TYPE_SIGNED_12BIT,
    MIC_SAMPLING_TYPE_12BIT_FILTER_OFF,
    MIC_SAMPLING_TYPE_SIGNED_12BIT_FILTER_OFF
} MICSamplingType;

#define MIC_SAMPLING_RATE_16K 0x105D

typedef void (*MICCallback)(int result, void *arg);

typedef struct {
    MICSamplingType type;       // 0x00
    void *buffer;               // 0x04
    u32 size;                   // 0x08
    u32 rate;                   // 0x0C
    BOOL loop_enable;           // 0x10
    MICCallback full_callback;  // 0x14
    void *full_arg;             // 0x18
} MICAutoParam;

void MIC_Init(void);
int MIC_StartAutoSampling(const MICAutoParam *param);
int MIC_StopAutoSampling(void);

#ifdef __cplusplus
}
#endif

#endif
