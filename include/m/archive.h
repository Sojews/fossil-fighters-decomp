// Archives: files loaded from the ROM file system, read in pieces through an optional filter.
#ifndef M_ARCHIVE_H
#define M_ARCHIVE_H

#include <string>
#include "nitro/os.h"
#include "nitro/fs.h"

class MMutex {
public:
    MMutex() { OS_InitMutex(&mutex); }

    OSMutex mutex;
};

// Transforms data as it's read.
class MArchiveFilter {
public:
    virtual ~MArchiveFilter() {}
    virtual void Filter(const void *src, void *dest, u32 size) = 0;
};

// A ROM file holding files of its own (a count, then offset and size pairs). Archives are kept open, one per
// file, while anything refers to them; each file is loaded into memory while it's locked.
inline bool operator<(const FSFileID &a, const FSFileID &b) {
    return a.arc < b.arc ? true : a.arc > b.arc ? false : a.file_id < b.file_id;
}

class MArchive {
public:
    struct Entry {
        u32 offset;
        u32 size;
    };

    MArchive(FSFileID id);
    ~MArchive();

    static MArchive *Open(std::string path);
    static MArchive *Get(FSFileID id);

    void AddRef();
    void Release();
    void Lock(u32 index);
    void AddLock(u32 index);
    void Unlock(u32 index);
    void *GetData(u32 index);
    // Reads part of a file into dest, through filter if there is one (which is deleted afterwards).
    void Read(u32 index, u32 offset, void *dest, u32 size, MArchiveFilter *filter);

    FSFileID mId;           // 0x00
    int mRefCount;          // 0x08
    u32 mUnk0C;             // 0x0C
    u32 mCount;             // 0x10
    Entry *mEntries;        // 0x14
    void **mData;           // 0x18
    int *mLocks;            // 0x1C
    MMutex mMutex;          // 0x20
    std::string mDirectory; // 0x38: the directory of the files it names, filled in when first asked for
};

// A file in an archive, held loaded while locked.
class MArchiveFile {
public:
    MArchiveFile() {
        archive = NULL;
        locked = FALSE;
    }
    MArchiveFile(MArchive *a, u32 i) {
        archive = a;
        index = i;
        locked = FALSE;
        if (archive != NULL) {
            archive->AddRef();
            if (locked) {
                archive->Lock(index);
            }
        }
    }
    MArchiveFile(const MArchiveFile &other) {
        archive = other.archive;
        index = other.index;
        locked = other.locked;
        if (archive != NULL) {
            archive->AddRef();
            if (locked) {
                archive->AddLock(index);
            }
        }
    }
    ~MArchiveFile() {
        if (archive != NULL) {
            if (locked) {
                archive->Unlock(index);
            }
            archive->Release();
        }
    }
    MArchiveFile &operator=(const MArchiveFile &other) {
        if (&other != this) {
            if (archive != NULL) {
                if (locked) {
                    archive->Unlock(index);
                }
                archive->Release();
            }
            archive = other.archive;
            index = other.index;
            locked = other.locked;
            if (archive != NULL) {
                archive->AddRef();
                if (locked) {
                    archive->AddLock(index);
                }
            }
        }
        return *this;
    }
    void Lock() {
        if (archive != NULL && !locked) {
            locked = TRUE;
            archive->Lock(index);
        }
    }
    void *GetData() { return archive->GetData(index); }

    MArchive *archive;
    u32 index;
    u8 locked;
};


// A counted reference to an archive.
class MArchivePtr {
public:
    MArchivePtr();
    MArchivePtr(std::string path);
    MArchivePtr(FSFileID id);
    MArchivePtr(const MArchivePtr &other);
    ~MArchivePtr();
    MArchivePtr &operator=(const MArchivePtr &other);
    BOOL IsValid();
    MArchiveFile GetFile(u32 index) const { return MArchiveFile(mArchive, index); }
    // The archive at a path relative to this one's directory (this one, if the path is empty).
    MArchivePtr Open(const char *path) const;
    MArchiveFile GetFile(const char *path, u32 index) const;

    MArchive *mArchive;
};

#endif
