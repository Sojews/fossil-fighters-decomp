// NitroSDK CARD: reading the game card's ROM, a 512-byte page at a time. Page-aligned reads into ordinary memory
// go by DMA in the background; the rest the CPU reads, through a one-page cache.

#include "card_common.h"

#define CARD_ROM_PAGE_SIZE 512
#define MI_DMA_MAX_NUM 3
#define HW_CACHE_LINE_SIZE 32
#define HW_ITCM 0x01ff8000
#define HW_ITCM_END 0x02000000
#define HW_DTCM_SIZE 0x4000

#define reg_CARD_MASTERCNT (*(vu8 *) 0x040001a1)
#define reg_CARD_CNT (*(vu32 *) 0x040001a4)
#define reg_CARD_CMD0 (*(vu8 *) 0x040001a8)
#define reg_CARD_DATA (*(vu32 *) 0x04100010)

#define CARDMST_SEL_ROM 0x00
#define CARDMST_IF_ENABLE 0x40
#define CARDMST_ENABLE 0x80

#define CARD_DATA_READY 0x00800000
#define CARD_COMMAND_PAGE 0x01000000
#define CARD_COMMAND_ID 0x07000000
#define CARD_COMMAND_MASK 0x07000000
#define CARD_RESET_HI 0x20000000
#define CARD_READ_MODE 0x00000000
#define CARD_START 0x80000000
#define CARD_LATENCY1_MASK 0x00001fff

#define MROMOP_G_READ_ID 0xb8000000
#define MROMOP_G_READ_PAGE 0xb7000000

#define OS_IE_CARD_DATA (1 << 19)

typedef struct CARDRomStat {
    void (*read_func)(struct CARDRomStat *p);   // 0x000
    u32 ctrl;                                   // 0x004: the card control value for a page read
    u32 cache_page;                             // 0x008: the ROM address cache_buf holds
    u32 dummy[5];
    u8 cache_buf[CARD_ROM_PAGE_SIZE];           // 0x020
} CARDRomStat;

typedef struct {
    u8 unk00[0x60];
    u32 game_cmd_param;     // 0x60: the card control value the game's ROM header gives for normal reads
} CARDRomHeader;

void MIi_CardDmaCopy32(u32 dmaNo, const void *src, void *dest, u32 size);
u32 OS_GetDTCMAddress(void);
void IC_InvalidateRange(void *startAddr, u32 nBytes);
void CARDi_CheckPulledOutCore(u32 id);
void CARD_InitPulledOutCallback(void);
void CARDi_InitCommon(void);
void CARD_WaitRomAsync(void);

static CARDRomHeader *cardi_rom_header_addr = (CARDRomHeader *) HW_ROM_HEADER_BUF;
static CARDRomStat rom_stat ATTRIBUTE_ALIGN(32);
static u32 cardi_rom_base;

static u32 CARDi_ReadRomIDCore(void);
static void CARDi_ReadCard(CARDRomStat *p);
static void *CARDi_GetRomAccessor(void);

#pragma opt_common_subs off
static BOOL CARDi_ReadFromCache(CARDRomStat *p) {
    CARDiCommon *const c = &cardi_common;
    const u32 cur_page = (u32) (c->src & ~(CARD_ROM_PAGE_SIZE - 1));
    if (cur_page == p->cache_page) {
        const u32 mod = c->src - cur_page;
        u32 len = CARD_ROM_PAGE_SIZE - mod;
        if (len > c->len) {
            len = c->len;
        }
        MI_CpuCopy8(p->cache_buf + mod, (void *) c->dst, len);
        c->src += len;
        c->dst += len;
        c->len -= len;
    }
    return c->len > 0;
}
#pragma opt_common_subs reset

static void CARDi_SetRomOp(u32 cmd1, u32 cmd2) {
    while ((reg_CARD_CNT & CARD_START) != 0) {
    }
    reg_CARD_MASTERCNT = CARDMST_SEL_ROM | CARDMST_ENABLE | CARDMST_IF_ENABLE;
    {
        vu8 *p_cmd = &reg_CARD_CMD0;
        p_cmd[0] = (u8) (cmd1 >> 24);
        p_cmd[1] = (u8) (cmd1 >> 16);
        p_cmd[2] = (u8) (cmd1 >> 8);
        p_cmd[3] = (u8) (cmd1 >> 0);
        p_cmd[4] = (u8) (cmd2 >> 24);
        p_cmd[5] = (u8) (cmd2 >> 16);
        p_cmd[6] = (u8) (cmd2 >> 8);
        p_cmd[7] = (u8) (cmd2 >> 0);
    }
}

static void CARDi_SetCardDma(void) {
    CARDiCommon *const c = &cardi_common;
    CARDRomStat *const p = &rom_stat;
    MIi_CardDmaCopy32(c->dma, (const void *) &reg_CARD_DATA, (void *) c->dst, CARD_ROM_PAGE_SIZE);
    {
        u32 src = c->src;
        CARDi_SetRomOp((u32) (MROMOP_G_READ_PAGE | (src >> 8)), (u32) (src << 24));
    }
    reg_CARD_CNT = p->ctrl;
}

static inline BOOL CARDi_NextRomPage(void) {
    cardi_common.src += CARD_ROM_PAGE_SIZE;
    cardi_common.dst += CARD_ROM_PAGE_SIZE;
    cardi_common.len -= CARD_ROM_PAGE_SIZE;
    return cardi_common.len != 0;
}

static void CARDi_OnReadCard(void) {
    MI_StopDma(cardi_common.dma);
    if (!CARDi_NextRomPage()) {
        CARDiCommon *const c = &cardi_common;
        (void) OS_DisableIrqMask(OS_IE_CARD_DATA);
        (void) OS_ResetRequestIrqMask(OS_IE_CARD_DATA);
        CARDi_CheckPulledOutCore(CARDi_ReadRomIDCore());
        c->cmd->result = CARD_RESULT_SUCCESS;
        CARDi_EndTask(c, TRUE);
    } else {
        CARDi_SetCardDma();
    }
}

static inline BOOL CARDi_IsInTcm(u32 buf, u32 len) {
    const u32 i_tcm = HW_ITCM;
    const u32 i_tcm_end = HW_ITCM_END;
    const u32 d_tcm = OS_GetDTCMAddress();
    const u32 d_tcm_end = d_tcm + HW_DTCM_SIZE;
    const u32 buf_end = buf + len;
    return (i_tcm < buf_end && i_tcm_end > buf) || (d_tcm < buf_end && d_tcm_end > buf);
}

static BOOL CARDi_TryReadCardDma(CARDRomStat *p) {
    CARDiCommon *const c = &cardi_common;
    u32 dst = c->dst;
    u32 len = c->len;
    const u32 mod = dst & (HW_CACHE_LINE_SIZE - 1);
    const BOOL is_dma_available = !mod && c->dma <= MI_DMA_MAX_NUM && !CARDi_IsInTcm(dst, len);
    const BOOL is_page_aligned = is_dma_available && !((c->src | len) & (CARD_ROM_PAGE_SIZE - 1));
    const BOOL is_async = is_page_aligned && len > 0;

    p->ctrl = (u32) ((cardi_rom_header_addr->game_cmd_param & ~CARD_COMMAND_MASK) |
                     (CARD_COMMAND_PAGE | CARD_READ_MODE | CARD_START | CARD_RESET_HI));
    if (is_async) {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        IC_InvalidateRange((void *) dst, len);
        if (mod) {
            dst -= mod;
            DC_StoreRange((void *) dst, HW_CACHE_LINE_SIZE);
            DC_StoreRange((void *) (dst + len), HW_CACHE_LINE_SIZE);
            len += HW_CACHE_LINE_SIZE;
        }
        DC_InvalidateRange((void *) dst, len);
        DC_WaitWriteBufferEmpty();
        OS_SetIrqFunction(OS_IE_CARD_DATA, CARDi_OnReadCard);
        (void) OS_ResetRequestIrqMask(OS_IE_CARD_DATA);
        (void) OS_EnableIrqMask(OS_IE_CARD_DATA);
        (void) OS_RestoreInterrupts(bak_psr);
        CARDi_SetCardDma();
    }
    return is_async;
}

static void CARDi_ReadCard(CARDRomStat *p) {
    CARDiCommon *const c = &cardi_common;
    for (;;) {
        u32 len = CARD_ROM_PAGE_SIZE;
        u32 src = (u32) (c->src & ~(len - 1));
        u32 dst;
        if (src != c->src || (c->dst & 3) != 0 || c->len < len) {
            dst = (u32) p->cache_buf;
            p->cache_page = src;
        } else {
            dst = c->dst;
        }
        CARDi_SetRomOp((u32) (MROMOP_G_READ_PAGE | (src >> 8)), (u32) (src << 24));
        reg_CARD_CNT = p->ctrl;
        {
            u32 pos = 0;
            for (;;) {
                const u32 ctrl = reg_CARD_CNT;
                if ((ctrl & CARD_DATA_READY) != 0) {
                    u32 data = reg_CARD_DATA;
                    if (pos < len) {
                        ((u32 *) dst)[pos++] = data;
                    }
                }
                if (!(ctrl & CARD_START)) {
                    break;
                }
            }
        }
        if (dst == c->dst) {
            c->src += len;
            c->dst += len;
            c->len -= len;
            if (!c->len) {
                break;
            }
        } else if (!CARDi_ReadFromCache(p)) {
            break;
        }
    }
}

static u32 CARDi_ReadRomIDCore(void) {
    CARDi_SetRomOp(MROMOP_G_READ_ID, 0);
    reg_CARD_CNT = (u32) (((cardi_rom_header_addr->game_cmd_param & ~CARD_COMMAND_MASK) |
                           (CARD_COMMAND_ID | CARD_READ_MODE | CARD_START | CARD_RESET_HI)) &
                          ~CARD_LATENCY1_MASK);
    while (!(reg_CARD_CNT & CARD_DATA_READY)) {
    }
    return reg_CARD_DATA;
}

static void CARDi_ReadRomSyncCore(CARDiCommon *arg) {
#pragma unused(arg)
    CARDRomStat *const p = &rom_stat;
    if (CARDi_ReadFromCache(p)) {
        (*p->read_func)(p);
    }
    {
        CARDiCommon *const c = &cardi_common;
        CARDi_CheckPulledOutCore(CARDi_ReadRomIDCore());
        c->cmd->result = CARD_RESULT_SUCCESS;
        CARDi_EndTask(c, TRUE);
    }
}

void CARDi_ReadRom(u32 dma, const void *src, void *dst, u32 len, MIDmaCallback callback, void *arg, BOOL is_async) {
    CARDiCommon *const c = &cardi_common;
    CARDRomStat *const p = &rom_stat;

    CARD_CheckEnabled();
    CARDi_WaitTask(c, callback, arg);
    c->dma = dma;
    c->src = (u32) src + cardi_rom_base;
    c->dst = (u32) dst;
    c->len = len;
    if (dma <= MI_DMA_MAX_NUM) {
        MI_StopDma(dma);
    }
    if (CARDi_TryReadCardDma(p)) {
        if (!is_async) {
            CARD_WaitRomAsync();
        }
    } else if (is_async) {
        CARDi_SetTask(CARDi_ReadRomSyncCore);
    } else {
        c->cur_th = OS_GetCurrentThread();
        CARDi_ReadRomSyncCore(c);
    }
}

void CARD_Init(void) {
    CARDiCommon *const p = &cardi_common;
    if (!p->flag) {
        p->flag = CARD_STAT_INIT;
        p->src = p->dst = p->len = 0;
        p->dma = (u32) ~0;
        p->callback = NULL;
        p->callback_arg = NULL;
        cardi_rom_base = 0;
        CARDi_InitCommon();
        rom_stat.read_func = (void (*)(CARDRomStat *)) CARDi_GetRomAccessor();
        CARD_InitPulledOutCallback();
    }
}

void CARD_WaitRomAsync(void) {
    (void) CARDi_WaitAsync();
}

static void *CARDi_GetRomAccessor(void) {
    return (void *) CARDi_ReadCard;
}
