// MFile. Not decompiled yet: the path-to-file-ID cache and path functions (std::string).

#include <algorithm>
#include "m/file.h"

static u32 sMaxReadSize = 0xFFFFFFFF;
FSArchive *gRomArchive;

s32 MReadFile(FSFile *file, void *dest, u32 size) {
    s32 total = 0;
    u32 wanted = size;
    while (size != 0) {
        s32 chunk = std::min(sMaxReadSize, size);
        s32 read = FS_ReadFile(file, dest, chunk);
        total += read;
        size -= read;
        dest = (u8 *) dest + read;
        if (read < chunk) {
            break;
        }
    }
    if (wanted != total) {
        total = -1;
    }
    return total;
}

void MInitFile() {
    FS_Init((u32) -1);
    u32 size = FS_TryLoadTable(NULL, 0);
    FS_TryLoadTable(new u8[size], size);
    gRomArchive = FS_FindArchive("rom", 3);
}

void MSetMaxReadSize(u32 size) {
    sMaxReadSize = size;
}

void MFile::Init() {
    FS_InitFile(&mFile);
    mResult = FALSE;
    mOpened = FALSE;
}

MFile::MFile() {
    Init();
}

MFile::MFile(const std::string &path) {
    Init();
    Open(path);
}

MFile::MFile(FSFileID id) {
    Init();
    Open(id);
}

MFile::~MFile() {
    Close();
}

BOOL MFile::Open(std::string path) {
    FSFileID id;
    return MGetFileID(path, &id) && Open(id);
}

BOOL MFile::Open(FSFileID id) {
    mId = id;
    mResult = FS_OpenFileFast(&mFile, id) ? TRUE : FALSE;
    if (mResult) {
        mOpened = TRUE;
    }
    return mResult;
}

BOOL MFile::Close() {
    if (!mOpened) {
        return mResult;
    }
    mResult = FS_CloseFile(&mFile) ? TRUE : FALSE;
    if (mResult) {
        mOpened = FALSE;
    }
    return mResult;
}

std::string MFile::GetPath() {
    return MGetFilePath(mId);
}

BOOL MFile::IsUnk10() {
    if ((mFile.stat & 0x10) ? TRUE : FALSE) {
        return TRUE;
    }
    return FALSE;
}

BOOL MFile::Seek(s32 offset, FSSeekFileMode origin) {
    mResult = FS_SeekFile(&mFile, offset, origin) ? TRUE : FALSE;
    return mResult;
}

u32 MFile::GetLength() {
    return mFile.prop.file.bottom - mFile.prop.file.top;
}

s32 MFile::Read(void *dest, s32 size) {
    return MReadFile(&mFile, dest, size);
}
