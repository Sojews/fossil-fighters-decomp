// The parts of the NitroSDK touch panel library the game code uses (declarations only).
#ifndef NITRO_TP_H
#define NITRO_TP_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TPData {
    u16 x;
    u16 y;
    u16 touch;
    u16 validity;
} TPData;

typedef struct TPCalibrateParam {
    s16 x0;
    s16 y0;
    s16 xDotSize;
    s16 yDotSize;
} TPCalibrateParam;

void TP_Init(void);
BOOL TP_GetUserInfo(TPCalibrateParam *param);
void TP_SetCalibrateParam(const TPCalibrateParam *param);
void TP_RequestSamplingAsync(void);
u32 TP_WaitCalibratedResult(TPData *result);
void TP_RequestAutoSamplingStartAsync(u16 vcount, u16 frequency, TPData samplingBufs[], u16 bufSize);
void TP_RequestAutoSamplingStopAsync(void);
u32 TP_WaitBusy(u32 commandFlags);
u32 TP_CheckError(u32 commandFlags);
void TP_GetLatestCalibratedPointInAuto(TPData *result);

#ifdef __cplusplus
}
#endif

#endif
