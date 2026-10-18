// NitroSDK file system (declarations only).
#ifndef NITRO_FS_H
#define NITRO_FS_H

#include "types.h"
#include "nitro/os.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FS_ARCHIVE_NAME_LEN_MAX 3
#define FS_ENTRY_LONGNAME_MAX 128

typedef struct FSArchive FSArchive;
typedef struct FSFile FSFile;

typedef enum {
    FS_RESULT_SUCCESS = 0,
    FS_RESULT_FAILURE,
    FS_RESULT_BUSY,
    FS_RESULT_CANCELED,
    FS_RESULT_UNSUPPORTED,
    FS_RESULT_ERROR,
    FS_RESULT_PROC_ASYNC,
    FS_RESULT_PROC_DEFAULT,
    FS_RESULT_PROC_UNKNOWN
} FSResult;

typedef enum {
    FS_COMMAND_READFILE = 0,
    FS_COMMAND_WRITEFILE,
    FS_COMMAND_SEEKDIR,
    FS_COMMAND_READDIR,
    FS_COMMAND_FINDPATH,
    FS_COMMAND_GETPATH,
    FS_COMMAND_OPENFILEFAST,
    FS_COMMAND_OPENFILEDIRECT,
    FS_COMMAND_CLOSEFILE,
    FS_COMMAND_MAX,
    FS_COMMAND_ACTIVATE = FS_COMMAND_MAX,
    FS_COMMAND_IDLE,
    FS_COMMAND_SUSPEND,
    FS_COMMAND_RESUME,
    FS_COMMAND_INVALID = 14
} FSCommandType;

typedef FSResult (*FS_ARCHIVE_READ_FUNC)(FSArchive *p_arc, void *dst, u32 pos, u32 size);
typedef FSResult (*FS_ARCHIVE_WRITE_FUNC)(FSArchive *p_arc, const void *src, u32 pos, u32 size);
typedef FSResult (*FS_ARCHIVE_PROC_FUNC)(FSFile *p_file, FSCommandType command);

typedef struct FSFileLink {
    FSFile *prev;   // 0x0
    FSFile *next;   // 0x4
} FSFileLink;

struct FSArchive {
    union {
        char ptr[FS_ARCHIVE_NAME_LEN_MAX + 1];
        u32 pack;
    } name;                             // 0x00
    FSArchive *next;                    // 0x04
    FSArchive *prev;                    // 0x08
    OSThreadQueue sync_q;               // 0x0c
    OSThreadQueue stat_q;               // 0x14
    u32 flag;                           // 0x1c
    FSFileLink list;                    // 0x20: the files with commands waiting
    u32 base;                           // 0x28
    u32 fat;                            // 0x2c
    u32 fat_size;                       // 0x30
    u32 fnt;                            // 0x34
    u32 fnt_size;                       // 0x38
    u32 fat_bak;                        // 0x3c
    u32 fnt_bak;                        // 0x40
    void *load_mem;                     // 0x44
    FS_ARCHIVE_READ_FUNC read_func;     // 0x48
    FS_ARCHIVE_WRITE_FUNC write_func;   // 0x4c
    FS_ARCHIVE_READ_FUNC table_func;    // 0x50
    FS_ARCHIVE_PROC_FUNC proc;          // 0x54
    u32 proc_flag;                      // 0x58
};

typedef struct {
    FSArchive *arc;     // 0x0
    u32 file_id;        // 0x4
} FSFileID;

typedef struct {
    FSArchive *arc;     // 0x0
    u16 own_id;         // 0x4
    u16 index;          // 0x6
    u32 pos;            // 0x8
} FSDirPos;

typedef struct {
    union {
        FSFileID file_id;
        FSDirPos dir_id;
    } id;                               // 0x00
    u32 is_directory;                   // 0x0c
    u32 name_len;                       // 0x10
    char name[FS_ENTRY_LONGNAME_MAX];   // 0x14
} FSDirEntry;

struct FSFile {
    FSFileLink link;            // 0x00
    FSArchive *arc;             // 0x08
    u32 stat;                   // 0x0c
    FSCommandType command;      // 0x10
    FSResult error;             // 0x14
    OSThreadQueue queue[1];     // 0x18
    union {
        struct {
            u32 own_id;         // 0x20
            u32 top;            // 0x24
            u32 bottom;         // 0x28
            u32 pos;            // 0x2c
        } file;
        struct {
            FSDirPos pos;       // 0x20
            u32 parent;         // 0x2c
        } dir;
    } prop;
    union {
        struct {
            void *dst;
            u32 len_org;
            u32 len;
        } readfile;
        struct {
            const void *src;
            u32 len_org;
            u32 len;
        } writefile;
        struct {
            FSDirPos pos;
        } seekdir;
        struct {
            FSDirEntry *p_entry;
            BOOL skip_string;
        } readdir;
        struct {
            FSDirPos pos;
            const char *path;
            BOOL find_directory;
            union {
                FSFileID *file;
                FSDirPos *dir;
            } result;
        } findpath;
        struct {
            u8 *buf;
            u32 buf_len;
            u16 total_len;
            u16 dir_id;
        } getpath;
        struct {
            FSFileID id;
        } openfilefast;
        struct {
            u32 top;
            u32 bottom;
            u32 index;
        } openfiledirect;
        struct {
            u32 reserved;
        } closefile;
    } arg;                      // 0x30
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
void FS_CancelFile(FSFile *p_file);
BOOL FS_SeekFile(FSFile *p_file, s32 offset, FSSeekFileMode origin);
s32 FS_ReadFile(FSFile *p_file, void *dst, s32 len);
s32 FS_GetPathLength(FSFile *p_file);
BOOL FS_GetPathName(FSFile *p_file, char *buf, u32 len);

#define FS_OVERLAY_FLAG_COMP 0x01

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

BOOL FS_LoadOverlayInfo(FSOverlayInfo *p_ovi, MIProcessor target, FSOverlayID id);
FSFileID FS_GetOverlayFileID(const FSOverlayInfo *p_ovi);
void FS_ClearOverlayImage(FSOverlayInfo *p_ovi);
void FS_StartOverlay(FSOverlayInfo *p_ovi);
BOOL FS_UnloadOverlay(MIProcessor target, FSOverlayID id);

#ifdef __cplusplus
}
#endif

#endif
