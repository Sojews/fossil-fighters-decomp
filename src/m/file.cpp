// MFile, the path-to-file-ID cache, archives mounted from memory, and overlay loading.

#define MSL_STRING_DEFINITIONS
#include <algorithm>
#include <map>
#include "m/file.h"
#include "m/archive.h"
#include "m/task.h"
#include "nnsys/fnd/archive.h"

// A NARC image in memory mounted as a file system archive, kept while anything has it mounted.
class MFileArchive {
public:
    MFileArchive(const char *name, const void *data);
    virtual ~MFileArchive() {
        if (!NNS_FndUnmountArchive(&mArchive)) {
            OS_Terminate();
        }
    }

    int mRefCount;          // 0x04
    NNSFndArchive mArchive; // 0x08
};

namespace std {
// (The game's map<std::string, ...> has no unused word in its header.)
template <class V> struct __tree_header_padded<pair<const string, V> > {
    static const bool value = false;
};
}  // namespace std

// The last few ROM paths looked up, most recent first.
struct MPathCacheEntry {
    MPathCacheEntry() {
        path.reserve(0x400);
        id.arc = NULL;
        id.file_id = 0;
    }

    std::string path;   // 0x00
    FSFileID id;        // 0x0C
};

inline void swap(MPathCacheEntry &a, MPathCacheEntry &b) {
    std::swap(a.id, b.id);
    a.path.swap(b.path);
}

// The most bytes read at once (0xFFFFFFFF), in the TU's .data pool.
extern "C" struct {
    u32 unk00[2];
    u32 mMaxReadSize;   // 0x08
} data_020c0a10;
#define sMaxReadSize (data_020c0a10.mMaxReadSize)
FSArchive *gRomArchive;
static MMutex sPathCacheMutex;
static MPathCacheEntry sPathCache[10];
static std::string sRomPrefix("rom:/");
static std::map<std::string, MFileArchive *> sMountedArchives;
static MMutex sMountMutex;

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

// Makes a path absolute in the ROM archive ("rom:/..."), unless it names an archive already.
static std::string MMakeRomPath(std::string path) {
    MScopedLock lock(&sPathCacheMutex.mutex, TRUE);
    if (path.find(":", 0) != std::string::npos) {
        return path;
    }
    if (path[0] == '/') {
        return sRomPrefix.substr(0, 4) + path;
    }
    return sRomPrefix + path;
}

BOOL MGetFileID(std::string path, FSFileID *id) {
    MScopedLock lock(&sPathCacheMutex.mutex, TRUE);
    path = MMakeRomPath(path);
    for (u32 i = 0; i < 10; i++) {
        if (path == sPathCache[i].path) {
            *id = sPathCache[i].id;
            for (u32 j = i; j != 0; j--) {
                swap(sPathCache[j], sPathCache[j - 1]);
            }
            return TRUE;
        }
    }
    BOOL found = FS_ConvertPathToFileID(id, path.c_str()) ? TRUE : FALSE;
    if (found && id->arc == gRomArchive) {
        for (u32 j = 9; j != 0; j--) {
            swap(sPathCache[j], sPathCache[j - 1]);
        }
        sPathCache[0].path = path;
        sPathCache[0].id.arc = id->arc;
        sPathCache[0].id.file_id = id->file_id;
    }
    return found;
}

static std::string MReadFilePath(FSFileID id) {
    FSFile file;
    FS_InitFile(&file);
    FS_OpenFileFast(&file, id);
    s32 length = FS_GetPathLength(&file);
    char *buf = new char[length];
    FS_GetPathName(&file, buf, length);
    std::string path(buf, length);
    delete[] buf;
    FS_CloseFile(&file);
    return path;
}

std::string MGetFilePath(const FSFileID id) {
    MScopedLock lock(&sPathCacheMutex.mutex, TRUE);
    if (id.arc != gRomArchive) {
        return MReadFilePath(id);
    }
    for (u32 i = 0; i < 10; i++) {
        if (id.file_id == sPathCache[i].id.file_id) {
            std::string path = sPathCache[i].path;
            for (u32 j = i; j != 0; j--) {
                swap(sPathCache[j], sPathCache[j - 1]);
            }
            return path;
        }
    }
    std::string path = MReadFilePath(id);
    for (u32 j = 9; j != 0; j--) {
        swap(sPathCache[j], sPathCache[j - 1]);
    }
    sPathCache[0].path = path;
    sPathCache[0].id.arc = id.arc;
    sPathCache[0].id.file_id = id.file_id;
    return path;
}

BOOL MFileExists(std::string path) {
    FSFileID id;
    return MGetFileID(path, &id);
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

std::string MFile::GetDirectory() {
    std::string path = GetPath();
    u32 pos = path.rfind('/');
    if (pos != std::string::npos) {
        return path.substr(0, pos + 1);
    }
    return "";
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

MFileArchive::MFileArchive(const char *name, const void *data) {
    mRefCount = 0;
    if (!NNS_FndMountArchive(&mArchive, name, (void *) data)) {
        OS_Terminate();
    }
}

void MMountArchive(const char *name, const void *data) {
    MScopedLock lock(&sMountMutex.mutex, TRUE);
    std::map<std::string, MFileArchive *>::iterator it;
    it = sMountedArchives.find(name);
    if (it != sMountedArchives.end()) {
        it->second->mRefCount++;
    } else {
        MFileArchive *archive = new MFileArchive(name, data);
        archive->mRefCount++;
        sMountedArchives[name] = archive;
    }
}

void MUnmountArchive(const char *name) {
    MScopedLock lock(&sMountMutex.mutex, TRUE);
    std::map<std::string, MFileArchive *>::iterator it;
    it = sMountedArchives.find(name);
    if (it != sMountedArchives.end()) {
        MFileArchive *archive = it->second;
        if (--archive->mRefCount == 0) {
            delete archive;
            sMountedArchives.erase(name);
        }
    } else {
        OS_Terminate();
    }
}

extern "C" {
// (Copies of the SDK's, reading through MReadFile.)
static u32 FSi_GetOverlayBinarySize(const FSOverlayInfo *p_ovi) {
    return (p_ovi->header.flag & FS_OVERLAY_FLAG_COMP) ? p_ovi->header.compressed : p_ovi->header.ram_size;
}
}

static BOOL MLoadOverlayImage(FSOverlayInfo *p_ovi) {
    FSFile file;
    FS_InitFile(&file);
    if (!FS_OpenFileFast(&file, FS_GetOverlayFileID(p_ovi))) {
        return FALSE;
    }
    u32 size = FSi_GetOverlayBinarySize(p_ovi);
    FS_ClearOverlayImage(p_ovi);
    if (MReadFile(&file, p_ovi->header.ram_address, size) != size) {
        FS_CloseFile(&file);
        return FALSE;
    }
    FS_CloseFile(&file);
    return TRUE;
}

BOOL MLoadOverlay(u32 target, u32 id) {
    FSOverlayInfo info;
    if (!FS_LoadOverlayInfo(&info, (MIProcessor) target, id) || !MLoadOverlayImage(&info)) {
        return FALSE;
    }
    FS_StartOverlay(&info);
    return TRUE;
}

BOOL MUnloadOverlay(u32 target, u32 id) {
    if (FS_UnloadOverlay((MIProcessor) target, id)) {
        return TRUE;
    }
    return FALSE;
}
