// NitroSDK CARD: what the library's files share. Requests that need the ARM7 (the backup chip sits on its
// side) go through a task thread; the shared state is one structure.
#ifndef CARD_COMMON_H
#define CARD_COMMON_H

#define ATTRIBUTE_ALIGN(n) __attribute__((aligned(n)))

#include "nitro/os.h"
#include "nitro/mi.h"
#include "nitro/pxi.h"

#define PXI_FIFO_TAG_FS 11

#define OS_LOCK_ID_ERROR (-3)
#define OS_BOOTTYPE_DOWNLOAD_MB 2

#define HW_ROM_HEADER_BUF 0x027ffe00
#define HW_CARD_ROM_HEADER 0x027ffa80
#define HW_CARD_ROM_HEADER_SIZE 0x160

static inline BOOL CARDi_IsDownloadBoot(void) {
    return *(u16 *) 0x027ffc40 == OS_BOOTTYPE_DOWNLOAD_MB;
}

typedef enum {
    CARD_RESULT_SUCCESS = 0,
    CARD_RESULT_FAILURE,
    CARD_RESULT_INVALID_PARAM,
    CARD_RESULT_UNSUPPORTED,
    CARD_RESULT_TIMEOUT,
    CARD_RESULT_ERROR,
    CARD_RESULT_NO_RESPONSE,
    CARD_RESULT_CANCELED
} CARDResult;

typedef enum {
    CARD_TARGET_NONE = 0,
    CARD_TARGET_ROM,
    CARD_TARGET_BACKUP
} CARDTargetMode;

typedef enum {
    CARD_REQ_INIT = 0,
    CARD_REQ_ACK,
    CARD_REQ_IDENTIFY,
    CARD_REQ_READ_ID,
    CARD_REQ_READ_ROM,
    CARD_REQ_WRITE_ROM,
    CARD_REQ_READ_BACKUP,
    CARD_REQ_WRITE_BACKUP,
    CARD_REQ_PROGRAM_BACKUP,
    CARD_REQ_VERIFY_BACKUP,
    CARD_REQ_ERASE_PAGE_BACKUP,
    CARD_REQ_ERASE_SECTOR_BACKUP,
    CARD_REQ_ERASE_CHIP_BACKUP,
    CARD_REQ_READ_STATUS,
    CARD_REQ_WRITE_STATUS,
    CARD_REQ_ERASE_SUBSECTOR_BACKUP,
    CARD_REQ_MAX
} CARDRequest;

typedef enum {
    CARD_REQUEST_MODE_RECV = 0,
    CARD_REQUEST_MODE_SEND,
    CARD_REQUEST_MODE_SEND_VERIFY,
    CARD_REQUEST_MODE_SPECIAL
} CARDRequestMode;

#define CARD_STAT_INIT (1 << 0)
#define CARD_STAT_INIT_CMD (1 << 1)
#define CARD_STAT_BUSY (1 << 2)
#define CARD_STAT_TASK (1 << 3)
#define CARD_STAT_RECV (1 << 4)
#define CARD_STAT_REQ (1 << 5)
#define CARD_STAT_CANCEL (1 << 6)

#define CARD_THREAD_PRIORITY_DEFAULT 4

typedef void (*MIDmaCallback)(void *arg);

// The capabilities of the backup chip.
typedef struct CARDBackupSpec {
    u32 total_size;             // 0x00
    u32 sect_size;              // 0x04
    u32 page_size;              // 0x08
    u32 addr_width;             // 0x0c
    u32 program_page;           // 0x10
    u32 write_page;             // 0x14
    u32 write_page_total;       // 0x18
    u32 erase_chip;             // 0x1c
    u32 erase_chip_total;       // 0x20
    u32 erase_sector;           // 0x24
    u32 erase_sector_total;     // 0x28
    u32 erase_page;             // 0x2c
    u8 initial_status;          // 0x30
    u8 padding1[3];
    u32 caps;                   // 0x34
    u8 padding2[16];
} CARDBackupSpec;

// The arguments of a request, which the ARM7 reads.
typedef struct CARDiCommandArg {
    CARDResult result;          // 0x00
    u32 type;                   // 0x04: the backup type
    u32 id;                     // 0x08
    u32 src;                    // 0x0c
    u32 dst;                    // 0x10
    u32 len;                    // 0x14
    CARDBackupSpec spec;        // 0x18
} CARDiCommandArg;

typedef struct CARDiCommon CARDiCommon;
typedef void (*CARDiTaskFunc)(CARDiCommon *p);

struct CARDiCommon {
    CARDiCommandArg *cmd;       // 0x000
    u32 command;                // 0x004
    volatile s32 lock_owner;    // 0x008
    volatile s32 lock_ref;      // 0x00c
    OSThreadQueue lock_queue[1];    // 0x010
    CARDTargetMode lock_target; // 0x018
    u32 src;                    // 0x01c
    u32 dst;                    // 0x020
    u32 len;                    // 0x024
    u32 dma;                    // 0x028
    CARDRequest req_type;       // 0x02c
    int req_retry;              // 0x030
    CARDRequestMode req_mode;   // 0x034
    MIDmaCallback callback;     // 0x038
    void *callback_arg;         // 0x03c
    CARDiTaskFunc task_func;    // 0x040
    OSThread thread[1];         // 0x044
    OSThread *cur_th;           // 0x104
    u32 priority;               // 0x108
    OSThreadQueue busy_q[1];    // 0x10c
    volatile u32 flag;          // 0x114
    u8 dummy[8];                // 0x118
    u8 backup_cache_page_buf[256];  // 0x120
};

extern CARDiCommon cardi_common;

void CARDi_SetTask(CARDiTaskFunc task);
void CARDi_LockResource(s32 owner, CARDTargetMode target);
void CARDi_UnlockResource(s32 owner, CARDTargetMode target);
BOOL CARDi_WaitAsync(void);
BOOL CARD_IsEnabled(void);
void CARD_CheckEnabled(void);
void CARD_Enable(BOOL enable);
void CARDi_OnFifoRecv(int tag, u32 data, BOOL err);
void CARDi_TaskThread(void *arg);
BOOL CARDi_Request(CARDiCommon *p, int req_type, int retry_count);
u32 CARD_GetBackupSectorSize(void);
void CARDi_ReadRom(u32 dma, const void *src, void *dst, u32 len, MIDmaCallback callback, void *arg, BOOL is_async);

s32 OS_LockCard(u16 lockID);
s32 OS_UnlockCard(u16 lockID);
void OSi_ReferSymbol(void *symbol);
void DC_InvalidateRange(void *startAddr, u32 nBytes);
void DC_WaitWriteBufferEmpty(void);
void MI_CpuFill8(void *dest, u8 data, u32 size);

// Waits out a request in progress and claims the library for a new one.
static inline void CARDi_WaitTask(CARDiCommon *p, MIDmaCallback callback, void *callback_arg) {
    OSIntrMode bak_psr = OS_DisableInterrupts();
    while ((p->flag & CARD_STAT_BUSY) != 0) {
        OS_SleepThread(p->busy_q);
    }
    p->flag |= CARD_STAT_BUSY;
    p->callback = callback;
    p->callback_arg = callback_arg;
    (void) OS_RestoreInterrupts(bak_psr);
}

// Ends a request: wakes whoever waits for the library, then runs the request's callback.
static inline void CARDi_EndTask(CARDiCommon *p, BOOL is_own_task) {
    const MIDmaCallback func = p->callback;
    void *const arg = p->callback_arg;
    {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        p->flag &= ~(CARD_STAT_BUSY | CARD_STAT_TASK | CARD_STAT_CANCEL);
        OS_WakeupThread(p->busy_q);
        if ((p->flag & CARD_STAT_RECV) != 0) {
            OS_WakeupThreadDirect(p->thread);
        }
        (void) OS_RestoreInterrupts(bak_psr);
    }
    if (is_own_task && func) {
        (*func) (arg);
    }
}

#endif
