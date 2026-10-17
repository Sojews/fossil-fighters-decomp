// NitroSDK file system (declarations only, with the FSFile fields the game reads).
#ifndef NITRO_FS_H
#define NITRO_FS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FSArchive FSArchive;

typedef struct FSFile FSFile;
typedef int (*FS_ARCHIVE_READ_FUNC)(FSArchive *p_arc, void *dst, u32 pos, u32 size);
typedef int (*FS_ARCHIVE_WRITE_FUNC)(FSArchive *p_arc, const void *src, u32 pos, u32 size);

struct FSArchive {
    u8 unk00[0x5C];
};

typedef struct {
    FSArchive *arc;
    u32 file_id;
} FSFileID;

struct FSFile {
    u8 unk00[0xC];
    u32 stat;           // 0x0C
    u8 unk10[0x14];
    u32 top;            // 0x24
    u32 bottom;         // 0x28
    u8 unk2C[0x1C];
};

typedef enum {
    FS_SEEK_SET,
    FS_SEEK_CUR,
    FS_SEEK_END
} FSSeekFileMode;

void FS_Init(u32 default_dma_no);
u32 FS_TryLoadTable(void *mem, u32 size);
FSArchive *FS_FindArchive(const char *name, int name_len);
void FS_InitArchive(FSArchive *p_arc);
BOOL FS_RegisterArchiveName(FSArchive *p_arc, const char *name, u32 name_len);
void FS_ReleaseArchiveName(FSArchive *p_arc);
BOOL FS_LoadArchive(FSArchive *p_arc, u32 base, u32 fat, u32 fat_size, u32 fnt, u32 fnt_size,
                    FS_ARCHIVE_READ_FUNC read_func, FS_ARCHIVE_WRITE_FUNC write_func);
BOOL FS_UnloadArchive(FSArchive *p_arc);
void FS_InitFile(FSFile *p_file);
BOOL FS_ConvertPathToFileID(FSFileID *p_file_id, const char *path);
BOOL FS_OpenFileFast(FSFile *p_file, FSFileID file_id);
BOOL FS_CloseFile(FSFile *p_file);
BOOL FS_SeekFile(FSFile *p_file, s32 offset, FSSeekFileMode origin);
s32 FS_ReadFile(FSFile *p_file, void *dst, s32 len);

#ifdef __cplusplus
}
#endif

#endif
