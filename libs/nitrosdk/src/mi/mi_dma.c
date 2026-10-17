// NitroSDK MI: DMA transfers (copies and fills, synchronous, asynchronous and H-blank).

#include "nitro/mi.h"
#include "nitro/os.h"

typedef void (*MIDmaCallback)(void *arg);

#define REG_DMA0SAD_ADDR 0x040000b0
#define REG_DMA0_CLR_DATA_ADDR 0x040000e0
#define MI_DMA_ENABLE 0x80000000

#define MI_DMA_TIMING_MASK 0x38000000
#define MI_DMA_TIMING_V_BLANK 0x08000000
#define MI_DMA_TIMING_H_BLANK 0x10000000
#define MI_DMA_SRC_INC 0
#define MI_DMA_SRC_DEC 0x00800000

#define MI_CNT_CLEAR32(size) (0x85000000 | ((size) >> 2))
#define MI_CNT_CLEAR32_IF(size) (0xc5000000 | ((size) >> 2))
#define MI_CNT_CLEAR16(size) (0x81000000 | ((size) >> 1))
#define MI_CNT_COPY32(size) (0x84000000 | ((size) >> 2))
#define MI_CNT_COPY32_IF(size) (0xc4000000 | ((size) >> 2))
#define MI_CNT_COPY16(size) (0x80000000 | ((size) >> 1))
#define MI_CNT_HBCOPY32(size) (0x96600000 | ((size) >> 2))
#define MI_CNT_HBCOPY16(size) (0x92600000 | ((size) >> 1))

void MIi_DmaSetParams(u32 dmaNo, u32 src, u32 dest, u32 ctrl);
void MIi_DmaSetParams_noInt(u32 dmaNo, u32 src, u32 dest, u32 ctrl);
void MIi_DmaSetParams_wait(u32 dmaNo, u32 src, u32 dest, u32 ctrl);
void MIi_DmaSetParams_wait_noInt(u32 dmaNo, u32 src, u32 dest, u32 ctrl);
void OSi_EnterDmaCallback(u32 dmaNo, MIDmaCallback callback, void *arg);
void MI_WaitDma(u32 dmaNo);
void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 dir);

#define MIi_WAIT_DMA(dmaCntp) \
    while (*(dmaCntp) & MI_DMA_ENABLE) { \
    }

void MI_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size) {
    vu32 *dmaCntp;
    if (size == 0) {
        return;
    }
    dmaCntp = &((vu32 *) REG_DMA0SAD_ADDR)[dmaNo * 3 + 2];
    MIi_WAIT_DMA(dmaCntp);
    {
        OSIntrMode enabled = OS_DisableInterrupts();
        vu32 *p = (vu32 *) (REG_DMA0_CLR_DATA_ADDR + dmaNo * 4);
        *p = data;
        MIi_DmaSetParams_wait_noInt(dmaNo, (u32) p, (u32) dest, MI_CNT_CLEAR32(size));
        OS_RestoreInterrupts(enabled);
    }
    MIi_WAIT_DMA(dmaCntp);
}

void MI_DmaCopy32(u32 dmaNo, const void *src, void *dest, u32 size) {
    vu32 *dmaCntp;
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, MI_DMA_SRC_INC);
    if (size == 0) {
        return;
    }
    dmaCntp = &((vu32 *) REG_DMA0SAD_ADDR)[dmaNo * 3 + 2];
    MIi_WAIT_DMA(dmaCntp);
    MIi_DmaSetParams_wait(dmaNo, (u32) src, (u32) dest, MI_CNT_COPY32(size));
    MIi_WAIT_DMA(dmaCntp);
}

void MI_DmaFill16(u32 dmaNo, void *dest, u16 data, u32 size) {
    vu32 *dmaCntp;
    if (size == 0) {
        return;
    }
    dmaCntp = &((vu32 *) REG_DMA0SAD_ADDR)[dmaNo * 3 + 2];
    MIi_WAIT_DMA(dmaCntp);
    {
        OSIntrMode enabled = OS_DisableInterrupts();
        vu32 *p = (vu32 *) (REG_DMA0_CLR_DATA_ADDR + dmaNo * 4);
        *p = data;
        MIi_DmaSetParams_wait_noInt(dmaNo, (u32) p, (u32) dest, MI_CNT_CLEAR16(size));
        OS_RestoreInterrupts(enabled);
    }
    MIi_WAIT_DMA(dmaCntp);
}

void MI_DmaCopy16(u32 dmaNo, const void *src, void *dest, u32 size) {
    vu32 *dmaCntp;
    if (size == 0) {
        return;
    }
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, MI_DMA_SRC_INC);
    dmaCntp = &((vu32 *) REG_DMA0SAD_ADDR)[dmaNo * 3 + 2];
    MIi_WAIT_DMA(dmaCntp);
    MIi_DmaSetParams_wait(dmaNo, (u32) src, (u32) dest, MI_CNT_COPY16(size));
    MIi_WAIT_DMA(dmaCntp);
}

void MI_DmaFill32Async(u32 dmaNo, void *dest, u32 data, u32 size, MIDmaCallback callback, void *arg) {
    if (size == 0) {
        if (callback != NULL) {
            callback(arg);
        }
        return;
    }
    MI_WaitDma(dmaNo);
    if (callback != NULL) {
        OSi_EnterDmaCallback(dmaNo, callback, arg);
        {
            OSIntrMode enabled = OS_DisableInterrupts();
            vu32 *p = (vu32 *) (REG_DMA0_CLR_DATA_ADDR + dmaNo * 4);
            *p = data;
            MIi_DmaSetParams_noInt(dmaNo, (u32) p, (u32) dest, MI_CNT_CLEAR32_IF(size));
            OS_RestoreInterrupts(enabled);
        }
    } else {
        OSIntrMode enabled = OS_DisableInterrupts();
        vu32 *p = (vu32 *) (REG_DMA0_CLR_DATA_ADDR + dmaNo * 4);
        *p = data;
        MIi_DmaSetParams_noInt(dmaNo, (u32) p, (u32) dest, MI_CNT_CLEAR32(size));
        OS_RestoreInterrupts(enabled);
    }
}

void MI_DmaCopy32Async(u32 dmaNo, const void *src, void *dest, u32 size, MIDmaCallback callback, void *arg) {
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, MI_DMA_SRC_INC);
    if (size == 0) {
        if (callback != NULL) {
            callback(arg);
        }
        return;
    }
    MI_WaitDma(dmaNo);
    if (callback != NULL) {
        OSi_EnterDmaCallback(dmaNo, callback, arg);
        MIi_DmaSetParams(dmaNo, (u32) src, (u32) dest, MI_CNT_COPY32_IF(size));
    } else {
        MIi_DmaSetParams(dmaNo, (u32) src, (u32) dest, MI_CNT_COPY32(size));
    }
}

BOOL MI_IsDmaBusy(u32 dmaNo) {
    vu32 *dmaCntp = &((vu32 *) REG_DMA0SAD_ADDR)[dmaNo * 3 + 2];
    return (BOOL) ((*dmaCntp & MI_DMA_ENABLE) >> 31);
}

void MI_WaitDma(u32 dmaNo) {
    OSIntrMode enabled = OS_DisableInterrupts();
    vu32 *dmaCntp = &((vu32 *) REG_DMA0SAD_ADDR)[dmaNo * 3 + 2];
    MIi_WAIT_DMA(dmaCntp);
    if (dmaNo == 0) {
        vu32 *p = (vu32 *) (REG_DMA0SAD_ADDR + dmaNo * 12);
        p[0] = 0;
        p[1] = 0;
        p[2] = 0x81400001;
    }
    OS_RestoreInterrupts(enabled);
}

void MI_StopDma(u32 dmaNo) {
    OSIntrMode enabled = OS_DisableInterrupts();
    vu16 *dmaCntHp = &((vu16 *) REG_DMA0SAD_ADDR)[dmaNo * 6 + 5];
    *dmaCntHp &= ~((MI_DMA_TIMING_MASK | 0x02000000) >> 16);
    *dmaCntHp &= ~(MI_DMA_ENABLE >> 16);
    (void) *dmaCntHp;
    (void) *dmaCntHp;
    if (dmaNo == 0) {
        vu32 *p = (vu32 *) (REG_DMA0SAD_ADDR + dmaNo * 12);
        p[0] = 0;
        p[1] = 0;
        p[2] = 0x81400001;
    }
    OS_RestoreInterrupts(enabled);
}

void MIi_CheckAnotherAutoDMA(u32 dmaNo, u32 dmaType) {
    int i;
    for (i = 0; i < 3; i++) {
        u32 dmaCnt;
        u32 timing;
        if (i == dmaNo) {
            continue;
        }
        dmaCnt = *(vu32 *) (REG_DMA0SAD_ADDR + 8 + i * 12);
        if (!(dmaCnt & MI_DMA_ENABLE)) {
            continue;
        }
        timing = dmaCnt & MI_DMA_TIMING_MASK;
        if (timing == dmaType) {
            continue;
        }
        if (timing == MI_DMA_TIMING_V_BLANK && dmaType == MI_DMA_TIMING_H_BLANK) {
            continue;
        }
        if (timing == MI_DMA_TIMING_H_BLANK && dmaType == MI_DMA_TIMING_V_BLANK) {
            continue;
        }
        if (timing == 0x18000000 || timing == 0x20000000 || timing == 0x28000000 || timing == 0x30000000 ||
            timing == 0x38000000 || timing == MI_DMA_TIMING_V_BLANK || timing == MI_DMA_TIMING_H_BLANK) {
            OS_Terminate();
        }
    }
}

void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 dir) {
    if (dmaNo == 0) {
        u32 addStart = src & 0xff000000;
        u32 addEnd;
        switch (dir) {
        case MI_DMA_SRC_INC:
            src += size;
            break;
        case MI_DMA_SRC_DEC:
            src -= size;
            break;
        }
        addEnd = src & 0xff000000;
        if (addStart == 0x04000000 || addStart >= 0x08000000 || addEnd == 0x04000000 || addEnd >= 0x08000000) {
            OS_Terminate();
        }
    }
}

void MI_HBlankDmaCopy32(u32 dmaNo, const void *src, void *dest, u32 size) {
    MIi_CheckAnotherAutoDMA(dmaNo, MI_DMA_TIMING_H_BLANK);
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, MI_DMA_SRC_INC);
    if (size == 0) {
        return;
    }
    MI_WaitDma(dmaNo);
    MIi_DmaSetParams(dmaNo, (u32) src, (u32) dest, MI_CNT_HBCOPY32(size));
}

void MI_HBlankDmaCopy16(u32 dmaNo, const void *src, void *dest, u32 size) {
    MIi_CheckAnotherAutoDMA(dmaNo, MI_DMA_TIMING_H_BLANK);
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, MI_DMA_SRC_INC);
    if (size == 0) {
        return;
    }
    MI_WaitDma(dmaNo);
    MIi_DmaSetParams(dmaNo, (u32) src, (u32) dest, MI_CNT_HBCOPY16(size));
}
