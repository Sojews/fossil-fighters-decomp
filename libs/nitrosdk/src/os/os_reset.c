// NitroSDK OS: software reset. The ARM7 is told first; the rest happens in OSi_DoResetSystem (in ITCM).

#include "nitro/os.h"
#include "nitro/section.h"
#include "nitro/pxi.h"

#define PXI_FIFO_TAG_OS 12
#define OS_PXI_COMMAND_MASK 0x7f00
#define OS_PXI_COMMAND_SHIFT 8
#define OS_PXI_COMMAND_RESET 0x10

#define OS_BOOTTYPE_DOWNLOAD_MB 2
#define OS_IE_FIFO_RECV (1 << 18)
#define HW_RESET_PARAMETER_BUF 0x027ffc20

s32 OS_GetLockID(void);
void CARD_LockRom(u16 lock_id);
void MI_StopDma(u32 dmaNo);
void OSi_DoResetSystem(void);

volatile u16 OSi_IsResetOccurred;
static u16 OSi_IsInitReset;

static void OSi_CommonCallback(int tag, u32 data, BOOL err);
static void OSi_SendToPxi(u16 data);

#pragma thumb off
void OS_InitReset(void) {
    if (OSi_IsInitReset) {
        return;
    }
    OSi_IsInitReset = TRUE;
    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_OS, PXI_PROC_ARM7)) {
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_OS, OSi_CommonCallback);
}

static void OSi_CommonCallback(int tag, u32 data, BOOL err) {
#pragma unused(tag, err)
    u16 command = (u16) ((data & OS_PXI_COMMAND_MASK) >> OS_PXI_COMMAND_SHIFT);
    if (command == OS_PXI_COMMAND_RESET) {
        OSi_IsResetOccurred = TRUE;
    } else {
        OS_Terminate();
    }
}

static void OSi_SendToPxi(u16 data) {
    u32 pxi_send_data = ((u32) data) << 8;
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_OS, pxi_send_data, FALSE) != 0) {
    }
}

void OS_ResetSystem(u32 parameter) {
    if (*(u16 *) 0x027ffc40 == OS_BOOTTYPE_DOWNLOAD_MB) {
        OS_Terminate();
    }
    CARD_LockRom((u16) OS_GetLockID());
    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);
    (void) OS_SetIrqMask(OS_IE_FIFO_RECV);
    (void) OS_ResetRequestIrqMask((u32) ~0);
    *(u32 *) HW_RESET_PARAMETER_BUF = parameter;
    OSi_SendToPxi(OS_PXI_COMMAND_RESET);
    OSi_DoResetSystem();
}
#pragma thumb reset

// The reset itself, run from ITCM since main memory gets reloaded: the ROM's ARM9 and ARM7 binaries are read again
// and the game is booted afresh.
#define HW_ROM_HEADER_BUF 0x027ffe00
#define HW_ROM_BASE_OFFSET_BUF 0x027ffc2c
#define REG_CARD_MASTERCNT (*(vu8 *) 0x040001a1)
#define REG_CARD_CNT (*(vu32 *) 0x040001a4)
#define REG_CARD_CMD ((vu8 *) 0x040001a8)
#define REG_CARD_DATA (*(vu32 *) 0x04100010)

void DC_StoreAll(void);
void DC_InvalidateAll(void);
void IC_InvalidateAll(void);
void DC_WaitWriteBufferEmpty(void);
static void OSi_DoBoot(void);
static void OSi_CpuClear32(register u32 data, register void *destp, register u32 size);
static void OSi_ReloadRomData(void);
static void OSi_ReadCardRom32(u32 src, void *dst, int len);

#pragma section ITCM begin
#pragma thumb off
void OSi_DoResetSystem(void) {
    while (!OSi_IsResetOccurred) {
    }
    *(vu16 *) 0x04000208 = 0;
    OSi_ReloadRomData();
    OSi_DoBoot();
}

static asm void OSi_DoBoot(void) {
    mov r12, #0x04000000
    str r12, [r12, #0x208]
    ldr r1, =SDK_AUTOLOAD_DTCM_START
    add r1, r1, #0x3fc0
    add r1, r1, #0x3c
    mov r0, #0
    str r0, [r1, #0]
    ldr r1, =0x04000180
@1:
    ldrh r0, [r1]
    and r0, r0, #0xf
    cmp r0, #1
    bne @1
    mov r0, #0x100
    strh r0, [r1]
    mov r0, #0
    ldr r3, =0x027ffd9c
    ldr r4, [r3]
    ldr r1, =0x027ffd80
    mov r2, #0x80
    bl OSi_CpuClear32
    str r4, [r3]
    ldr r1, =0x027fff80
    mov r2, #0x18
    bl OSi_CpuClear32
    ldr r1, =0x027fff98
    strh r0, [r1]
    ldr r1, =0x027fff9c
    mov r2, #0x64
    bl OSi_CpuClear32
    ldr r1, =0x04000180
@2:
    ldrh r0, [r1]
    and r0, r0, #0xf
    cmp r0, #1
    beq @2
    mov r0, #0
    strh r0, [r1]
    ldr r3, =0x027ffe00
    ldr r12, [r3, #0x24]
    mov lr, r12
    ldr r11, =0x027fff80
    ldmia r11, {r0-r10}
    mov r11, #0
    bx r12
}

static asm void OSi_CpuClear32(register u32 data, register void *destp, register u32 size) {
    add r12, r1, r2
@1:
    cmp r1, r12
    stmltia r1!, {r0}
    blt @1
    bx lr
}

static void OSi_ReloadRomData(void) {
    u32 header = HW_ROM_HEADER_BUF;
    const u32 rom_base = *(u32 *) HW_ROM_BASE_OFFSET_BUF;
    if (rom_base >= 0x8000) {
        OSi_ReadCardRom32(rom_base, (void *) header, 0x160);
    }
    {
        u32 src_arm9 = *(u32 *) (header + 0x20);
        u32 dst_arm9 = *(u32 *) (header + 0x28);
        u32 len_arm9 = *(u32 *) (header + 0x2c);
        u32 src_arm7 = *(u32 *) (header + 0x30);
        u32 dst_arm7 = *(u32 *) (header + 0x38);
        u32 len_arm7 = *(u32 *) (header + 0x3c);
        {
            OSIntrMode bak_psr = OS_DisableInterrupts();
            DC_StoreAll();
            DC_InvalidateAll();
            (void) OS_RestoreInterrupts(bak_psr);
        }
        IC_InvalidateAll();
        DC_WaitWriteBufferEmpty();
        src_arm9 += rom_base;
        if (src_arm9 < 0x8000) {
            u32 diff = 0x8000 - src_arm9;
            src_arm9 = 0x8000;
            dst_arm9 += diff;
            len_arm9 -= diff;
        }
        OSi_ReadCardRom32(src_arm9, (void *) dst_arm9, (int) len_arm9);
        OSi_ReadCardRom32(src_arm7 + rom_base, (void *) dst_arm7, (int) len_arm7);
    }
}

static void OSi_ReadCardRom32(u32 src, void *dst, int len) {
    vu32 *const hdr_GAME_BUF = (vu32 *) (HW_ROM_HEADER_BUF + 0x60);
    const u32 ctrl_start = (u32) ((*hdr_GAME_BUF & ~0x07000000) | (0x01000000 | 0x80000000 | 0x20000000));
    int pos = -(int) (src & (512 - 1));
    while ((REG_CARD_CNT & 0x80000000) != 0) {
    }
    REG_CARD_MASTERCNT = 0x80;
    for (src = (u32) (src + pos); pos < len; src += 512) {
        REG_CARD_CMD[0] = 0xb7;
        REG_CARD_CMD[1] = (u8) (src >> 24);
        REG_CARD_CMD[2] = (u8) (src >> 16);
        REG_CARD_CMD[3] = (u8) (src >> 8);
        REG_CARD_CMD[4] = (u8) (src >> 0);
        REG_CARD_CMD[5] = 0;
        REG_CARD_CMD[6] = 0;
        REG_CARD_CMD[7] = 0;
        REG_CARD_CNT = ctrl_start;
        for (;;) {
            u32 ctrl = REG_CARD_CNT;
            if ((ctrl & 0x00800000) != 0) {
                const u32 data = REG_CARD_DATA;
                if ((pos >= 0) && (pos < len)) {
                    *(u32 *) ((u32) dst + pos) = data;
                }
                pos += sizeof(u32);
            }
            if (!(ctrl & 0x80000000)) {
                break;
            }
        }
    }
}
#pragma thumb reset
#pragma section ITCM end
