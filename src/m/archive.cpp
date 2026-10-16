// MArchive and MArchivePtr. Not decompiled yet: the parts that use the table of open archives (a std::map),
// and Read.

#include "m/archive.h"
#include "m/file.h"
#include "m/irq.h"
#include "m/task.h"

MArchive::MArchive(FSFileID id) : mId(id) {
    MFile file(mId);
    file.Read(&mUnk0C, 8);
    mEntries = new Entry[mCount];
    file.Read(mEntries, mCount * sizeof(Entry));
    mData = new void *[mCount];
    mLocks = new int[mCount];
    mRefCount = 0;
    for (u32 i = 0; i < mCount; i++) {
        mLocks[i] = 0;
    }
}

MArchive::~MArchive() {
    delete[] mEntries;
    delete[] mData;
    delete[] mLocks;
}

void MArchive::AddRef() {
    MIrqLock lock;
    mRefCount++;
}

// Loads the file if it isn't loaded.
void MArchive::Lock(u32 index) {
    MScopedLock lock(&mMutex.mutex, TRUE);
    if (mLocks[index] == 0) {
        u32 size = mEntries[index].size;
        mData[index] = new u8[size];
        Read(index, 0, mData[index], size, NULL);
    }
    MIrqLock irq;
    mLocks[index]++;
}

void MArchive::AddLock(u32 index) {
    MIrqLock lock;
    mLocks[index]++;
}

void MArchive::Unlock(u32 index) {
    int locks;
    {
        MIrqLock lock;
        mLocks[index]--;
        locks = mLocks[index];
    }
    if (locks <= 0) {
        MScopedLock lock(&mMutex.mutex, TRUE);
        if (mLocks[index] == 0) {
            delete[] (u8 *)mData[index];
        }
    }
}

void *MArchive::GetData(u32 index) {
    return mData[index];
}

MArchivePtr::MArchivePtr() {
    mArchive = NULL;
}

MArchivePtr::MArchivePtr(std::string path) {
    mArchive = MArchive::Open(path);
}

MArchivePtr::MArchivePtr(FSFileID id) {
    mArchive = MArchive::Get(id);
}

MArchivePtr::MArchivePtr(const MArchivePtr &other) {
    mArchive = other.mArchive;
    if (mArchive != NULL) {
        mArchive->AddRef();
    }
}

MArchivePtr::~MArchivePtr() {
    if (mArchive != NULL) {
        mArchive->Release();
    }
}

MArchivePtr &MArchivePtr::operator=(const MArchivePtr &other) {
    if (&other == this) {
        return *this;
    }
    if (other.mArchive != NULL) {
        other.mArchive->AddRef();
    }
    if (mArchive != NULL) {
        mArchive->Release();
    }
    mArchive = other.mArchive;
    return *this;
}

BOOL MArchivePtr::IsValid() {
    if (mArchive != NULL) {
        return TRUE;
    }
    return FALSE;
}

MArchiveFile MArchivePtr::GetFile(const char *path, u32 index) const {
    return Open(path).GetFile(index);
}

MArchivePtr MArchivePtr::Open(const char *path) const {
    if (path[0] == '\0') {
        return *this;
    }
    if (mArchive->mDirectory.empty()) {
        MFile file(mArchive->mId);
        mArchive->mDirectory = file.GetDirectory();
    }
    return MArchivePtr(mArchive->mDirectory + path);
}
