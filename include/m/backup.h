// Save memory (the cartridge's backup chip), read and written through the CARD library.
#ifndef M_BACKUP_H
#define M_BACKUP_H

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

#endif
