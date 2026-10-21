// MArchive and MArchivePtr.

#include <map>
#include "m/archive.h"
#include "m/file.h"
#include "m/heap.h"
#include "m/irq.h"
#include "m/task.h"
#include "m/transfer.h"

// Read's buffers for compressed and filtered files, and whether they're in use.
static bool sReadBuffersBusy;
static u8 *sReadBuffer;
static u8 *sReadBuffer2;
// The open archives, by file.
static std::map<FSFileID, MArchive *> sArchives;
static MMutex sArchivesMutex;

void MInitArchive() {
    sReadBuffer = new u8[0x2004];
    sReadBuffer2 = new u8[0x2004];
}

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

MArchive *MArchive::Open(std::string path) {
    FSFileID id;
    if (!MGetFileID(path, &id)) {
        OS_Terminate();
    }
    return Get(id);
}

MArchive *MArchive::Get(FSFileID id) {
    MScopedLock lock(&sArchivesMutex.mutex, TRUE);
    MArchive *archive;
    std::map<FSFileID, MArchive *>::iterator it = sArchives.find(id);
    if (it != sArchives.end()) {
        archive = it->second;
    } else {
        archive = new MArchive(id);
        sArchives[id] = archive;
    }
    MIrqLock irq;
    archive->mRefCount++;
    return archive;
}

void MArchive::AddRef() {
    MIrqLock lock;
    mRefCount++;
}

void MArchive::Release() {
    int refs;
    {
        MIrqLock lock;
        refs = --mRefCount;
    }
    if (refs <= 0) {
        MMutexLock lock(&sArchivesMutex.mutex);
        if (mRefCount == 0) {
            sArchives.erase(mId);
            delete this;
        }
    }
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

// A packed file's header: it's stored in chunks, each passed through up to four filters in turn (0 ends the list),
// with the chunks' offsets after it (count + 1 of them).
struct MArchivePackHeader {
    u32 unk00;
    u32 size;           // 0x04: unpacked
    u32 chunkSize;      // 0x08: unpacked, a power of two
    u32 chunkCount;     // 0x0C
    u8 filters[4];      // 0x10: 1 RL, 2 LZ, 3 Huffman, 4 differences
};

// VRAM takes 16-bit writes only.
static inline BOOL MArchiveIsVram(const void *p) {
    BOOL vram = FALSE;
    if ((int) p >= 0x05000000 && (int) p < 0x08000000) {
        vram = TRUE;
    }
    return vram;
}

// NONMATCHING: ours hoists `dest + done` out of the filter loop into r7 (the game recomputes it), so `partial` spills.
void MArchive::Read(u32 index, u32 offset, void *dest, u32 size, MArchiveFilter *filter) {
    Entry &entry = mEntries[index];
    {
        MScopedLock lock(&mMutex.mutex, TRUE);
        if (mLocks[index] != 0) {
            if (filter != NULL) {
                filter->Filter((u8 *) mData[index] + offset, dest, size);
            } else {
                MCopy((u8 *) mData[index] + offset, dest, size);
            }
            delete filter;
            return;
        }
    }
    MFile file(mId);
    file.Seek(entry.offset, FS_SEEK_SET);
    MArchivePackHeader header;
    file.Read(&header, sizeof(header));
    u32 *chunks = new u32[header.chunkCount + 1];
    file.Read(chunks, (header.chunkCount + 1) * sizeof(u32));
    BOOL shared;
    {
        MIrqLock irq;
        if (!sReadBuffersBusy && header.chunkSize <= 0x2000) {
            shared = TRUE;
            sReadBuffersBusy = TRUE;
        } else {
            shared = FALSE;
        }
    }
    u8 *buffer;
    u8 *buffer2;
    if (shared) {
        buffer = sReadBuffer;
        buffer2 = sReadBuffer2;
    } else {
        buffer = new u8[header.chunkSize + 4];
        buffer2 = new u8[header.chunkSize + 4];
    }
    u32 filters = 0;
    while (header.filters[filters] != 0) {
        filters++;
    }
    u32 first = offset / header.chunkSize;
    u32 end = offset + size;
    u32 last = (end - 1) / header.chunkSize;
    u32 done = 0;
    u32 *chunk = &chunks[first];
    file.Seek(entry.offset + chunks[first], FS_SEEK_SET);
    for (u32 c = first; c <= last; c++) {
        BOOL partial = FALSE;
        u32 skip;
        if (c == first) {
            skip = offset & (header.chunkSize - 1);
            if (skip != 0) {
                partial = TRUE;
            }
        } else {
            skip = 0;
        }
        u32 length;
        if (c == last) {
            length = ((end - 1) & (header.chunkSize - 1)) + 1;
            if (c == header.chunkCount - 1) {
                partial = (partial | (header.size != end)) ? TRUE : FALSE;
            } else {
                partial = (partial | (length != header.chunkSize)) ? TRUE : FALSE;
            }
        } else {
            length = header.chunkSize;
        }
        length -= skip;
        if (filter != NULL) {
            partial = TRUE;
        }
        u32 steps = filters;
        if (partial || filters == 0) {
            steps++;
        }
        file.Read(buffer2, chunk[1] - chunk[0]);
        for (u32 i = 0; i < steps; i++) {
            u8 *src = buffer2;
            buffer2 = buffer;
            buffer = src;
            u8 *out;
            if (i == steps - 1) {
                if (partial || filters == 0) {
                    if (filter != NULL) {
                        filter->Filter(src + skip, (u8 *) dest + done, length);
                    } else {
                        MCopy(src + skip, (u8 *) dest + done, length);
                    }
                    break;
                }
                out = (u8 *) dest + done;
            } else {
                out = buffer2;
            }
            switch (header.filters[i]) {
            case 1:
                if (MArchiveIsVram(out)) {
                    MI_UncompressRL16(src, out);
                } else {
                    MI_UncompressRL8(src, out);
                }
                break;
            case 2:
                if (MArchiveIsVram(out)) {
                    MI_UncompressLZ16(src, out);
                } else {
                    MI_UncompressLZ8(src, out);
                }
                break;
            case 3:
                MI_UncompressHuffman(src, out);
                break;
            case 4:
                if (MArchiveIsVram(out)) {
                    MI_UnfilterDiff16(src, out);
                } else {
                    MI_UnfilterDiff8(src, out);
                }
                break;
            }
        }
        done += length;
        chunk++;
    }
    delete[] chunks;
    if (shared) {
        MIrqLock irq;
        sReadBuffersBusy = FALSE;
    } else {
        delete[] buffer;
        delete[] buffer2;
    }
    delete filter;
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

BOOL MArchivePtr::IsValid() {
    if (mArchive != NULL) {
        return TRUE;
    }
    return FALSE;
}

// (Walks the open archives; whatever it did with them isn't in this build.)
void MDumpArchives() {
    MMutexLock lock(&sArchivesMutex.mutex);
    for (std::map<FSFileID, MArchive *>::iterator it = sArchives.begin(); it != sArchives.end(); ++it) {
    }
}
