// Save memory devices.

#include "m/backup.h"

enum {
    CARD_REQ_READ_BACKUP = 6,
    CARD_REQ_WRITE_BACKUP = 7,
    CARD_REQ_VERIFY_BACKUP = 9,
};

MBackupDevice::MBackupDevice(CARDBackupType type) {
    mLockId = OS_GetLockID();
    MBackupLock lock(mLockId);
    mValid = CARD_IdentifyBackup(type) ? TRUE : FALSE;
}

MBackupDevice::~MBackupDevice() {
    OS_ReleaseLockID(mLockId);
}

BOOL MBackupDevice::IsValid() {
    return mValid;
}

u32 MBackupDevice::GetPageSize() {
    return CARD_GetBackupPageSize();
}

u32 MBackupDevice::GetTotalSize() {
    return CARD_GetBackupTotalSize();
}

MBackupPageDevice::MBackupPageDevice(CARDBackupType type) : MBackupDevice(type) {
    mPageSize = GetPageSize();
    mPageCount = GetTotalSize() / mPageSize;
    mPageBuf = new u8[GetPageSize()];
}

MBackupPageDevice::~MBackupPageDevice() {
    delete[] mPageBuf;
}

BOOL MBackupPageDevice::Read(u32 offset, void *buf, u32 size) {
    return ReadPages(offset, buf, size);
}

BOOL MBackupPageDevice::Unk10(u32 offset, void *buf, u32 size) {
    return Unk24(offset, buf, size);
}

MBackupFlashDevice::MBackupFlashDevice(CARDBackupType type) : MBackupPageDevice(type) {
}

BOOL MBackupFlashDevice::ReadPages(u32 offset, void *buf, u32 size) {
    BOOL ok;

    {
        MBackupLock lock(mLockId);
        ok = CARDi_RequestStreamCommand(offset, (u32)buf, size, NULL, NULL, FALSE, CARD_REQ_READ_BACKUP, 1, 0);
    }
    if (ok) {
        return TRUE;
    }
    return FALSE;
}

BOOL MBackupFlashDevice::WritePages(u32 offset, const void *buf, u32 size) {
    BOOL ok;

    {
        MBackupLock lock(mLockId);
        ok = CARDi_RequestStreamCommand((u32)buf, offset, size, NULL, NULL, FALSE, CARD_REQ_WRITE_BACKUP, 10, 1);
    }
    if (ok) {
        return TRUE;
    }
    return FALSE;
}

BOOL MBackupFlashDevice::VerifyPages(u32 offset, const void *buf, u32 size) {
    BOOL ok;

    {
        MBackupLock lock(mLockId);
        ok = CARDi_RequestStreamCommand((u32)buf, offset, size, NULL, NULL, FALSE, CARD_REQ_VERIFY_BACKUP, 1, 1);
    }
    if (ok) {
        return TRUE;
    }
    return FALSE;
}

BOOL MBackupFlashDevice::Unk24(u32 offset, const void *buf, u32 size) {
    BOOL ok;

    {
        MBackupLock lock(mLockId);
        ok = CARDi_RequestStreamCommand((u32)buf, offset, size, NULL, NULL, FALSE, CARD_REQ_WRITE_BACKUP, 10, 2);
    }
    if (ok) {
        return TRUE;
    }
    return FALSE;
}

MBackupFlash2MBitsDevice::MBackupFlash2MBitsDevice() : MBackupFlashDevice(0x1202) {
}
