// Save memory (the cartridge's backup chip), read and written through the CARD library.
#ifndef M_BACKUP_H
#define M_BACKUP_H

#include <vector>
#include "types.h"

extern "C" {
typedef u32 CARDBackupType;

void CARD_LockBackup(u16 lockId);
void CARD_UnlockBackup(u16 lockId);
BOOL CARD_IdentifyBackup(CARDBackupType type);
u32 CARD_GetBackupPageSize(void);
u32 CARD_GetBackupTotalSize(void);
BOOL CARDi_RequestStreamCommand(u32 src, u32 dst, u32 len, void *callback, void *arg, BOOL async, int request,
                                int retries, int mode);
s32 OS_GetLockID(void);
void OS_ReleaseLockID(u16 lockId);

typedef u32 MATHCRC32Table[256];
void MATHi_CRC32InitTableRev(MATHCRC32Table *table, u32 poly);
void MATHi_CRC32UpdateRev(const MATHCRC32Table *table, u32 *context, const void *input, u32 length);
}

// Locks the backup chip for the rest of the scope.
class MBackupLock {
public:
    MBackupLock(u16 lockId) {
        mLockId = lockId;
        CARD_LockBackup(mLockId);
    }
    ~MBackupLock() { CARD_UnlockBackup(mLockId); }

    u16 mLockId;
};

class MBackupDevice {
public:
    MBackupDevice(CARDBackupType type);
    virtual ~MBackupDevice();
    virtual BOOL Read(u32 offset, void *buf, u32 size) = 0;
    virtual BOOL Write(u32 offset, const void *buf, u32 size) = 0;
    virtual BOOL Unk10(u32 offset, void *buf, u32 size) = 0;
    virtual BOOL Unk14(u32 offset, const void *buf, u32 size) = 0;

    BOOL IsValid();
    u32 GetPageSize();
    u32 GetTotalSize();

    u8 mValid;      // 0x04: the chip is the expected type
    u16 mLockId;    // 0x06
};

// A chip written a page at a time.
class MBackupPageDevice : public MBackupDevice {
public:
    MBackupPageDevice(CARDBackupType type);
    virtual ~MBackupPageDevice();
    virtual BOOL Read(u32 offset, void *buf, u32 size);
    virtual BOOL Write(u32 offset, const void *buf, u32 size);
    virtual BOOL Unk10(u32 offset, void *buf, u32 size);
    virtual BOOL Unk14(u32 offset, const void *buf, u32 size);
    virtual BOOL ReadPages(u32 offset, void *buf, u32 size) = 0;
    virtual BOOL WritePages(u32 offset, const void *buf, u32 size) = 0;
    virtual BOOL VerifyPages(u32 offset, const void *buf, u32 size) = 0;
    virtual BOOL Unk24(u32 offset, const void *buf, u32 size) = 0;

    u32 mPageSize;      // 0x08
    u32 mPageCount;     // 0x0C
    u8 *mPageBuf;       // 0x10
};

class MBackupFlashDevice : public MBackupPageDevice {
public:
    MBackupFlashDevice(CARDBackupType type);
    virtual BOOL ReadPages(u32 offset, void *buf, u32 size);
    virtual BOOL WritePages(u32 offset, const void *buf, u32 size);
    virtual BOOL VerifyPages(u32 offset, const void *buf, u32 size);
    virtual BOOL Unk24(u32 offset, const void *buf, u32 size);
};

class MBackupFlash2MBitsDevice : public MBackupFlashDevice {
public:
    MBackupFlash2MBitsDevice();
};

// Files kept in backup memory, each with several copies checked by CRC; the newest good copy is read.
class MBackupStorage {
public:
    // Where one copy of a file is.
    struct Copy {
        u32 serial;         // 0x00: 0: never written
        u32 crc;            // 0x04: of the serial and the data
        u32 headerRegion;   // 0x08: holds serial and crc
        u32 dataRegion;     // 0x0C
    };
    struct File {
        u32 size;                   // 0x00
        u32 current;                // 0x04: the copy written last
        u32 serial;                 // 0x08: of that copy; 0: the file has never been written
        std::vector<Copy> copies;   // 0x0C
        u8 damaged;                 // 0x18: a copy failed its CRC when mounting
    };
    struct Region {
        u32 size;   // 0x00
        u32 offset; // 0x04
    };

    MBackupStorage(MBackupDevice *device);
    ~MBackupStorage();
    void AddFile(u32 size, u32 copies);
    u32 AllocRegion(u32 size);
    void Mount();
    u8 IsMounted();
    BOOL ReadRegion(u32 region, void *buf);
    BOOL WriteRegion(u32 region, const void *buf);
    u8 IsFileDamaged(u32 file);
    BOOL HasFile(u32 file);
    BOOL ReadFile(u32 file, void *buf);
    BOOL WriteFile(u32 file, const void *buf);
    void Format();

    // Rounds up to whole pages.
    u32 PageRound(u32 size) {
        return mDevice->GetPageSize() * ((size + mDevice->GetPageSize() - 1) / mDevice->GetPageSize());
    }

    MBackupDevice *mDevice;         // 0x00
    u8 mMountTried;                 // 0x04
    u8 mMounted;                    // 0x05
    u8 mFormatted;                  // 0x06
    u32 mHeaderSize;                // 0x08
    u8 *mHeader;                    // 0x0C: a magic number and each file's size, kept in region 0
    u32 *mHeaderWords;              // 0x10
    std::vector<File> mFiles;       // 0x14
    std::vector<Region> mRegions;   // 0x20
    MATHCRC32Table mCrcTable;       // 0x2C
};

namespace std {
// (A file's copy copies its vector of copies.)
template <> struct __vec_push_tag<MBackupStorage::File> {
    enum { value = 1 };
};
}

#endif
