// NitroSDK FS: the "rom" archive, the game card's own file system. Its reads go through the CARD library.

#include "fs_internal.h"

#define OS_BOOTTYPE_DOWNLOAD_MB 2
#define FS_ARCHIVE_PROC_WRITEFILE (1 << FS_COMMAND_WRITEFILE)
#define FS_ARCHIVE_PROC_ALL (~0)

typedef struct {
    u32 offset;
    u32 length;
} CARDRomRegion;

// Where the ROM header (copied to the end of main memory at boot) gives the file tables: the name table, then
// the allocation table.
#define HW_ROM_HEADER_FNT 0x027ffe40

s32 OS_GetLockID(void);
void CARD_Init(void);
BOOL CARD_IsPulledOut(void);
void CARD_LockRom(u16 lock_id);
void CARD_UnlockRom(u16 lock_id);
void CARDi_ReadRom(u32 dma, const void *src, void *dst, u32 len, void (*callback)(void *), void *arg,
                   BOOL is_async);
void FS_SetArchiveProc(FSArchive *p_arc, FS_ARCHIVE_PROC_FUNC proc, u32 flags);
void FS_NotifyArchiveAsyncEnd(FSArchive *p_arc, FSResult ret);
u32 FS_LoadArchiveTables(FSArchive *p_arc, void *p_mem, u32 max_size);

FSArchive fsi_arc_rom;
CARDRomRegion fsi_ovt7;
CARDRomRegion fsi_ovt9;
static u32 fsi_default_dma_no;
static s32 fsi_card_lock_id;

static void FSi_OnRomReadDone(void *p_arc) {
    FS_NotifyArchiveAsyncEnd((FSArchive *) p_arc, CARD_IsPulledOut() ? FS_RESULT_ERROR : FS_RESULT_SUCCESS);
}

static FSResult FSi_ReadRomCallback(FSArchive *p_arc, void *dst, u32 src, u32 len) {
    CARDi_ReadRom(fsi_default_dma_no, (const void *) src, dst, len, FSi_OnRomReadDone, p_arc, TRUE);
    return FS_RESULT_PROC_ASYNC;
}

static FSResult FSi_WriteDummyCallback(FSArchive *p_arc, const void *src, u32 dst, u32 len) {
#pragma unused(p_arc, src, dst, len)
    return FS_RESULT_FAILURE;
}

static FSResult FSi_RomArchiveProc(FSFile *p_file, FSCommandType command) {
#pragma unused(p_file)
    switch (command) {
    case FS_COMMAND_ACTIVATE:
        CARD_LockRom((u16) fsi_card_lock_id);
        return FS_RESULT_SUCCESS;
    case FS_COMMAND_IDLE:
        CARD_UnlockRom((u16) fsi_card_lock_id);
        return FS_RESULT_SUCCESS;
    case FS_COMMAND_WRITEFILE:
        return FS_RESULT_UNSUPPORTED;
    default:
        return FS_RESULT_PROC_UNKNOWN;
    }
}

static FSResult FSi_ReadDummyCallback(FSArchive *p_arc, void *dst, u32 src, u32 len) {
#pragma unused(p_arc, dst, src, len)
    return FS_RESULT_FAILURE;
}

static FSResult FSi_EmptyArchiveProc(FSFile *p_file, FSCommandType command) {
#pragma unused(p_file, command)
    return FS_RESULT_UNSUPPORTED;
}

void FSi_InitRom(u32 default_dma_no) {
    fsi_default_dma_no = default_dma_no;
    fsi_card_lock_id = OS_GetLockID();
    fsi_ovt9.offset = 0;
    fsi_ovt9.length = 0;
    fsi_ovt7.offset = 0;
    fsi_ovt7.length = 0;
    CARD_Init();
    FS_InitArchive(&fsi_arc_rom);
    (void) FS_RegisterArchiveName(&fsi_arc_rom, "rom", 3);
    if (*(u16 *) 0x027ffc40 == OS_BOOTTYPE_DOWNLOAD_MB) {
        fsi_ovt9.offset = (u32) ~0;
        fsi_ovt9.length = 0;
        fsi_ovt7.offset = (u32) ~0;
        fsi_ovt7.length = 0;
        FS_SetArchiveProc(&fsi_arc_rom, FSi_EmptyArchiveProc, (u32) FS_ARCHIVE_PROC_ALL);
        (void) FS_LoadArchive(&fsi_arc_rom, 0x00000000, 0, 0, 0, 0, FSi_ReadDummyCallback, FSi_WriteDummyCallback);
    } else {
        const u8 *p = (const u8 *) HW_ROM_HEADER_FNT;
        const CARDRomRegion *fnt = (const CARDRomRegion *) p;
        const CARDRomRegion *fat = (const CARDRomRegion *) (p + 8);
        FS_SetArchiveProc(&fsi_arc_rom, FSi_RomArchiveProc,
                          FS_ARCHIVE_PROC_WRITEFILE | FS_ARCHIVE_PROC_ACTIVATE | FS_ARCHIVE_PROC_IDLE);
        if (fnt->offset == 0xffffffff || fnt->offset == 0x00000000 || fat->offset == 0xffffffff ||
            fat->offset == 0x00000000) {
            // (A warning in debug builds: the ROM has no file system.)
        } else {
            (void) FS_LoadArchive(&fsi_arc_rom, 0x00000000, fat->offset, fat->length, fnt->offset, fnt->length,
                                  FSi_ReadRomCallback, FSi_WriteDummyCallback);
        }
    }
}

u32 FS_TryLoadTable(void *p_mem, u32 size) {
    return FS_LoadArchiveTables(&fsi_arc_rom, p_mem, size);
}
