// NitroSDK FS: overlays. The ROM's overlay tables give each overlay's load address, size and static
// initializers; loading reads the image (perhaps compressed) and runs the initializers, unloading runs the
// destructors of the objects the overlay registered.

#include "fs_internal.h"
#include "MWException.h"

#define OS_BOOTTYPE_DOWNLOAD_MB 2
#define DGT_HASH2_DIGEST_SIZE 20
#define FS_OVERLAY_FLAG_COMP 0x01
#define FS_OVERLAY_FLAG_AUTH 0x02

// Where the ROM header (copied to the end of main memory at boot) gives the ARM9 overlay table, then the ARM7's.
#define HW_ROM_HEADER_OVT 0x027ffe50

typedef enum {
    MI_PROCESSOR_ARM9 = 0,
    MI_PROCESSOR_ARM7 = 1
} MIProcessor;

typedef u32 FSOverlayID;
typedef void (*FSOverlayInitFunc)(void);

typedef struct {
    u32 offset;
    u32 length;
} CARDRomRegion;

typedef struct {
    u32 id;                                 // 0x00
    u8 *ram_address;                        // 0x04
    u32 ram_size;                           // 0x08
    u32 bss_size;                           // 0x0c
    FSOverlayInitFunc *sinit_init;          // 0x10
    FSOverlayInitFunc *sinit_init_end;      // 0x14
    u32 file_id;                            // 0x18
    u32 compressed : 24;                    // 0x1c
    u32 flag : 8;
} FSOverlayInfoHeader;

typedef struct {
    FSOverlayInfoHeader header;             // 0x00
    MIProcessor target;                     // 0x20
    CARDRomRegion file_pos;                 // 0x24
} FSOverlayInfo;

void IC_InvalidateRange(void *startAddr, u32 nBytes);
void DC_InvalidateRange(void *startAddr, u32 nBytes);
void MIi_UncompressBackward(void *bottom);
void DGT_Hash2CalcHmac(void *digest, const void *bin_ptr, int bin_len, const void *key_ptr, int key_len);

extern FSArchive fsi_arc_rom;
extern CARDRomRegion fsi_ovt9;
extern CARDRomRegion fsi_ovt7;

// The overlay digests, from the linker script.
extern u8 SDK_OVERLAY_DIGEST[];
extern u8 SDK_OVERLAY_DIGEST_END[];

// The default key for the digests (in .rodata, outside this file's range).
extern const u8 fsi_def_digest_key[64];

static const u8 *fsi_digest_key_ptr = fsi_def_digest_key;
static u32 fsi_digest_key_len = sizeof(fsi_def_digest_key);

u32 FS_GetOverlayImageSize(const FSOverlayInfo *p_ovi) {
    return (p_ovi->header.flag & FS_OVERLAY_FLAG_COMP) ? p_ovi->header.compressed : p_ovi->header.ram_size;
}

static inline u32 FS_GetOverlayTotalSize(const FSOverlayInfo *p_ovi) {
    return p_ovi->header.ram_size + p_ovi->header.bss_size;
}

void FS_ClearOverlayImage(FSOverlayInfo *p_ovi) {
    u8 *const argument = p_ovi->header.ram_address;
    const u32 image_size = p_ovi->header.ram_size;
    const u32 total_size = image_size + p_ovi->header.bss_size;
    IC_InvalidateRange(argument, total_size);
    DC_InvalidateRange(argument, total_size);
    MI_CpuFill8(argument + image_size, 0, total_size - image_size);
}

FSFileID FS_GetOverlayFileID(const FSOverlayInfo *p_ovi) {
    FSFileID ret;
    ret.arc = &fsi_arc_rom;
    ret.file_id = p_ovi->header.file_id;
    return ret;
}

static BOOL FSi_LoadOverlayInfoCore(FSOverlayInfo *p_ovi, MIProcessor target, FSOverlayID id, FSArchive *p_arc,
                                    u32 offset_arm9, u32 len_arm9, u32 offset_arm7, u32 len_arm7) {
    u32 offset;
    u32 len;
    u32 pos;

    if (target == MI_PROCESSOR_ARM9) {
        offset = offset_arm9;
        len = len_arm9;
    } else {
        offset = offset_arm7;
        len = len_arm7;
    }
    pos = (u32) id * sizeof(FSOverlayInfoHeader);
    if (pos >= len) {
        return FALSE;
    }
    {
        FSFile file;
        FS_InitFile(&file);
        if (!FS_OpenFileDirect(&file, p_arc, offset + pos, offset + len, (u32) ~0)) {
            return FALSE;
        }
        if (FS_ReadFile(&file, p_ovi, sizeof(FSOverlayInfoHeader)) != sizeof(FSOverlayInfoHeader)) {
            (void) FS_CloseFile(&file);
            return FALSE;
        }
        (void) FS_CloseFile(&file);
        p_ovi->target = target;
        if (!FS_OpenFileFast(&file, FS_GetOverlayFileID(p_ovi))) {
            return FALSE;
        }
        p_ovi->file_pos.offset = file.prop.file.top;
        p_ovi->file_pos.length = file.prop.file.bottom - file.prop.file.top;
        (void) FS_CloseFile(&file);
        return TRUE;
    }
}

BOOL FS_LoadOverlayInfo(FSOverlayInfo *p_ovi, MIProcessor target, FSOverlayID id) {
    CARDRomRegion *const pr = (target == MI_PROCESSOR_ARM9) ? &fsi_ovt9 : &fsi_ovt7;
    if (pr->offset) {
        const u32 pos = (u32) id * sizeof(FSOverlayInfoHeader);
        if (pos >= pr->length) {
            return FALSE;
        }
        MI_CpuCopy8((const void *) (pr->offset + pos), p_ovi, sizeof(FSOverlayInfoHeader));
        p_ovi->target = target;
        {
            FSFile file;
            FS_InitFile(&file);
            if (!FS_OpenFileFast(&file, FS_GetOverlayFileID(p_ovi))) {
                return FALSE;
            }
            p_ovi->file_pos.offset = file.prop.file.top;
            p_ovi->file_pos.length = file.prop.file.bottom - file.prop.file.top;
            (void) FS_CloseFile(&file);
            return TRUE;
        }
    } else {
        const u8 *p = (const u8 *) HW_ROM_HEADER_OVT;
        const CARDRomRegion *p_ovt9 = (const CARDRomRegion *) p;
        const CARDRomRegion *p_ovt7 = (const CARDRomRegion *) (p + 8);
        return FSi_LoadOverlayInfoCore(p_ovi, target, id, &fsi_arc_rom, p_ovt9->offset, p_ovt9->length,
                                       p_ovt7->offset, p_ovt7->length);
    }
}

BOOL FS_LoadOverlayImage(FSOverlayInfo *p_ovi) {
    FSFile file;
    FS_InitFile(&file);
    if (!FS_OpenFileFast(&file, FS_GetOverlayFileID(p_ovi))) {
        return FALSE;
    }
    {
        const s32 size = (s32) FS_GetOverlayImageSize(p_ovi);
        FS_ClearOverlayImage(p_ovi);
        if (FS_ReadFile(&file, p_ovi->header.ram_address, size) != size) {
            (void) FS_CloseFile(&file);
            return FALSE;
        }
    }
    (void) FS_CloseFile(&file);
    return TRUE;
}

static BOOL FSi_CompareDigest(const u8 *spec_digest, void *src, int len) {
    int i;
    u8 digest[DGT_HASH2_DIGEST_SIZE];
    u8 digest_key[64];

    MI_CpuFill8(digest, 0, sizeof(digest));
    MI_CpuCopy8(fsi_digest_key_ptr, digest_key, fsi_digest_key_len);
    DGT_Hash2CalcHmac(digest, src, len, digest_key, (int) fsi_digest_key_len);
    for (i = 0; i < sizeof(digest); i += sizeof(u32)) {
        if (*(const u32 *) (digest + i) != *(const u32 *) (spec_digest + i)) {
            break;
        }
    }
    return i == sizeof(digest);
}

void FS_StartOverlay(FSOverlayInfo *p_ovi) {
    u32 rare_size = FS_GetOverlayImageSize(p_ovi);
    if (*(u16 *) 0x027ffc40 == OS_BOOTTYPE_DOWNLOAD_MB) {
        BOOL ret = FALSE;
        if (p_ovi->header.flag & FS_OVERLAY_FLAG_AUTH) {
            const u32 odt_max = (u32) ((SDK_OVERLAY_DIGEST_END - SDK_OVERLAY_DIGEST) / DGT_HASH2_DIGEST_SIZE);
            if (p_ovi->header.id < odt_max) {
                const u8 *spec_digest = SDK_OVERLAY_DIGEST + DGT_HASH2_DIGEST_SIZE * p_ovi->header.id;
                ret = FSi_CompareDigest(spec_digest, p_ovi->header.ram_address, (int) rare_size);
            }
        }
        if (!ret) {
            MI_CpuFill8(p_ovi->header.ram_address, 0, rare_size);
            OS_Terminate();
            return;
        }
    }
    if (p_ovi->header.flag & FS_OVERLAY_FLAG_COMP) {
        MIi_UncompressBackward(p_ovi->header.ram_address + rare_size);
    }
    DC_FlushRange(p_ovi->header.ram_address, p_ovi->header.ram_size);
    {
        FSOverlayInitFunc *p = p_ovi->header.sinit_init;
        FSOverlayInitFunc *q = p_ovi->header.sinit_init_end;
        for (; p < q; ++p) {
            if (*p) {
                (**p)();
            }
        }
    }
}

void FS_EndOverlay(FSOverlayInfo *p_ovi) {
    for (;;) {
        DestructorChain *head = NULL;
        DestructorChain *tail = NULL;
        const u32 region_top = (u32) p_ovi->header.ram_address;
        const u32 region_bottom = region_top + FS_GetOverlayTotalSize(p_ovi);
        {
            OSIntrMode bak_psr = OS_DisableInterrupts();
            DestructorChain *prev = NULL;
            DestructorChain *base = __global_destructor_chain;
            DestructorChain *p = base;
            while (p) {
                DestructorChain *next = p->next;
                const u32 dtor = (u32) p->dtor;
                const u32 obj = (u32) p->object;
                if ((obj == 0 && dtor >= region_top && dtor < region_bottom) ||
                    (obj >= region_top && obj < region_bottom)) {
                    if (!tail) {
                        head = p;
                    } else {
                        tail->next = p;
                    }
                    if (base == p) {
                        __global_destructor_chain = base = next;
                    }
                    tail = p;
                    p->next = NULL;
                    if (prev) {
                        prev->next = next;
                    }
                } else {
                    prev = p;
                }
                p = next;
            }
            (void) OS_RestoreInterrupts(bak_psr);
        }
        if (!head) {
            break;
        }
        do {
            DestructorChain *next = head->next;
            if (head->dtor) {
                (*head->dtor)(head->object);
            }
            head = next;
        } while (head);
    }
}

BOOL FS_UnloadOverlayImage(FSOverlayInfo *p_ovi) {
    FS_EndOverlay(p_ovi);
    return TRUE;
}

BOOL FS_LoadOverlay(MIProcessor target, FSOverlayID id) {
    FSOverlayInfo ovi;
    if (!FS_LoadOverlayInfo(&ovi, target, id) || !FS_LoadOverlayImage(&ovi)) {
        return FALSE;
    }
    FS_StartOverlay(&ovi);
    return TRUE;
}

BOOL FS_UnloadOverlay(MIProcessor target, FSOverlayID id) {
    FSOverlayInfo ovi;
    if (!FS_LoadOverlayInfo(&ovi, target, id) || !FS_UnloadOverlayImage(&ovi)) {
        return FALSE;
    }
    return TRUE;
}
