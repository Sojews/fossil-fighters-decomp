// NitroSDK file system (declarations only, with the FSFile fields the game reads).
#ifndef NITRO_FS_H
#define NITRO_FS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FSArchive FSArchive;

typedef struct {
    FSArchive *arc;
    u32 file_id;
} FSFileID;

typedef struct {
    u8 unk00[0xC];
    u32 stat;           // 0x0C
    u8 unk10[0x14];
    u32 top;            // 0x24
    u32 bottom;         // 0x28
    u8 unk2C[0x1C];
} FSFile;

typedef enum {
    FS_SEEK_SET,
    FS_SEEK_CUR,
    FS_SEEK_END
} FSSeekFileMode;

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
