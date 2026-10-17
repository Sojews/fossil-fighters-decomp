// Files in the ROM file system.
#ifndef M_FILE_H
#define M_FILE_H

#include <string>
#include "nitro/fs.h"

// Reads from a file in pieces no bigger than the largest read the file system is set to do at once.
s32 MReadFile(FSFile *file, void *dest, u32 size);
void MInitFile();
void MSetMaxReadSize(u32 size);

extern FSArchive *gRomArchive;

class MFile {
public:
    MFile();
    MFile(const std::string &path);
    MFile(FSFileID id);
    virtual ~MFile();

    void Init();
    BOOL Open(std::string path);
    BOOL Open(FSFileID id);
    BOOL Close();
    std::string GetPath();
    std::string GetDirectory();
    BOOL IsUnk10();
    BOOL Seek(s32 offset, FSSeekFileMode origin);
    u32 GetLength();
    s32 Read(void *dest, s32 size);

    FSFile mFile;       // 0x04
    FSFileID mId;       // 0x4C
    u8 mResult;         // 0x54: whether the last operation succeeded
    u8 mOpened;         // 0x55
};

// Looks up a path's file ID (caching the last few).
BOOL MGetFileID(std::string path, FSFileID *id);
std::string MGetFilePath(FSFileID id);

// Mounts an archive image in memory under a name (reference counted), and unmounts it.
void MMountArchive(const char *name, const void *data);
void MUnmountArchive(const char *name);

#endif
