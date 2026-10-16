// MFile. Not decompiled yet: the path-to-file-ID cache and path functions (std::string).

#include "m/file.h"

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
    return mFile.bottom - mFile.top;
}

s32 MFile::Read(void *dest, s32 size) {
    return FS_ReadFile(&mFile, dest, size);
}
