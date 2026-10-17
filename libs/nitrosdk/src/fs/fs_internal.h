// NitroSDK FS: what the library's files share.
#ifndef FS_INTERNAL_H
#define FS_INTERNAL_H

#include "nitro/fs.h"
#include "nitro/mi.h"

#define FS_FILE_STATUS_BUSY 0x00000001
#define FS_FILE_STATUS_CANCEL 0x00000002
#define FS_FILE_STATUS_SYNC 0x00000004
#define FS_FILE_STATUS_ASYNC 0x00000008
#define FS_FILE_STATUS_IS_FILE 0x00000010
#define FS_FILE_STATUS_IS_DIR 0x00000020
#define FS_FILE_STATUS_OPERATING 0x00000040

#define FS_ARCHIVE_FLAG_REGISTER 0x00000001
#define FS_ARCHIVE_FLAG_LOADED 0x00000002
#define FS_ARCHIVE_FLAG_TABLE_LOAD 0x00000004
#define FS_ARCHIVE_FLAG_SUSPEND 0x00000008
#define FS_ARCHIVE_FLAG_RUNNING 0x00000010
#define FS_ARCHIVE_FLAG_CANCELING 0x00000020
#define FS_ARCHIVE_FLAG_SUSPENDING 0x00000040
#define FS_ARCHIVE_FLAG_UNLOADING 0x00000080
#define FS_ARCHIVE_FLAG_IS_ASYNC 0x00000100
#define FS_ARCHIVE_FLAG_IS_SYNC 0x00000200

#define FS_INVALID_FILE_ID 0x10000

// One entry of an archive's file allocation table.
typedef struct {
    u32 top;
    u32 bottom;
} FSArchiveFAT;

// One entry of an archive's directory table.
typedef struct {
    u32 start;      // 0x0: where the directory's entries start
    u16 index;      // 0x4: the ID of its first file
    u16 parent;     // 0x6
} FSArchiveFNT;

// A read from an archive's tables.
typedef struct {
    FSArchive *arc;
    u32 pos;
} FSiSyncReadParam;


static inline BOOL FS_IsBusy(volatile const FSFile *p_file) {
    return (p_file->stat & FS_FILE_STATUS_BUSY) ? TRUE : FALSE;
}

static inline BOOL FS_IsCanceling(volatile const FSFile *p_file) {
    return (p_file->stat & FS_FILE_STATUS_CANCEL) ? TRUE : FALSE;
}

static inline BOOL FS_IsFileSyncMode(volatile const FSFile *p_file) {
    return (p_file->stat & FS_FILE_STATUS_SYNC) ? TRUE : FALSE;
}

static inline BOOL FS_IsFile(volatile const FSFile *p_file) {
    return (p_file->stat & FS_FILE_STATUS_IS_FILE) ? TRUE : FALSE;
}

static inline BOOL FS_IsDir(volatile const FSFile *p_file) {
    return (p_file->stat & FS_FILE_STATUS_IS_DIR) ? TRUE : FALSE;
}

static inline BOOL FSi_IsArchiveSync(volatile const FSArchive *p_arc) {
    return (p_arc->flag & FS_ARCHIVE_FLAG_IS_SYNC) ? TRUE : FALSE;
}

#define FS_ARCHIVE_PROC_SYNC 0x000001fc
#define FS_ARCHIVE_PROC_ACTIVATE 0x00000200
#define FS_ARCHIVE_PROC_IDLE 0x00000400

static inline BOOL FS_IsSucceeded(volatile const FSFile *p_file) {
    return (p_file->error == FS_RESULT_SUCCESS) ? TRUE : FALSE;
}

static inline BOOL FS_IsArchiveLoaded(volatile const FSArchive *p_arc) {
    return (p_arc->flag & FS_ARCHIVE_FLAG_LOADED) ? TRUE : FALSE;
}

static inline BOOL FS_IsArchiveTableLoaded(volatile const FSArchive *p_arc) {
    return (p_arc->flag & FS_ARCHIVE_FLAG_TABLE_LOAD) ? TRUE : FALSE;
}

static inline BOOL FS_IsArchiveSuspended(volatile const FSArchive *p_arc) {
    return (p_arc->flag & FS_ARCHIVE_FLAG_SUSPEND) ? TRUE : FALSE;
}

static inline BOOL FSi_IsArchiveRunning(volatile const FSArchive *p_arc) {
    return (p_arc->flag & FS_ARCHIVE_FLAG_RUNNING) ? TRUE : FALSE;
}

static inline BOOL FSi_IsArchiveCanceling(volatile const FSArchive *p_arc) {
    return (p_arc->flag & FS_ARCHIVE_FLAG_CANCELING) ? TRUE : FALSE;
}

static inline BOOL FS_IsArchiveSuspending(volatile const FSArchive *p_arc) {
    return (p_arc->flag & FS_ARCHIVE_FLAG_SUSPENDING) ? TRUE : FALSE;
}

static inline BOOL FSi_IsArchiveAsync(volatile const FSArchive *p_arc) {
    return (p_arc->flag & FS_ARCHIVE_FLAG_IS_ASYNC) ? TRUE : FALSE;
}

static inline u32 FS_GetArchiveNameLength(const FSArchive *p_arc) {
    return (p_arc->name.pack <= 0x000000ff) ? 1 : (p_arc->name.pack <= 0x0000ff00) ? 2 : 3;
}

void FSi_ReleaseCommand(FSFile *p_file, FSResult ret);
FSResult FSi_TranslateCommand(FSFile *p_file, FSCommandType command);
int FSi_StrNICmp(const char *str1, const char *str2, u32 len);
BOOL FSi_SendCommand(FSFile *p_file, FSCommandType command);
BOOL FS_SuspendArchive(FSArchive *p_arc);
BOOL FS_ResumeArchive(FSArchive *p_arc);
BOOL FSi_ExecuteSyncCommand(FSFile *p_com);
void FSi_InitRom(u32 default_dma_no);
BOOL FS_WaitAsync(FSFile *p_file);
BOOL FS_OpenFileDirect(FSFile *p_file, FSArchive *p_arc, u32 image_top, u32 image_bottom, u32 file_index);

extern FSDirPos current_dir_pos;

#endif
