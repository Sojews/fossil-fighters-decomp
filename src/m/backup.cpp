// Save memory devices.

#pragma ipa file

#include <string.h>
#include "m/backup.h"
#include "nitro/mi.h"
#include <mem.h>

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

// Writes through whole pages: each page the range touches is read, patched and written back if it changed.
// (The game's code is a do-while behind an if, with the last page's number kept in a variable.)
BOOL MBackupPageDevice::Write(u32 offset, const void *buf, u32 size) {
    u8 *pageBuf;
    u32 lastPage;
    u32 first;
    u32 last;
    u32 len;
    u32 end;
    first = offset / mPageSize;
    end = offset + size;
    last = (mPageSize + end - 1) / mPageSize;
    u32 page = first;
    if (page != last) {
        lastPage = last - 1;
        do {
            u32 start = page == first ? offset % mPageSize : 0;
            u32 stop = page == lastPage ? (mPageSize + end - 1) % mPageSize + 1 : mPageSize;
            pageBuf = mPageBuf;
            len = stop - start;
            if (!ReadPages(page * mPageSize, pageBuf, mPageSize)) {
                return FALSE;
            }
            if (memcmp(buf, pageBuf + start, len) != 0) {
                MI_CpuCopy8(buf, pageBuf + start, len);
                if (!WritePages(page * mPageSize, mPageBuf, mPageSize)) {
                    return FALSE;
                }
            }
            buf = (const u8 *) buf + len;
            page++;
        } while (page != last);
    }
    return TRUE;
}

// As Write, through Unk24.
BOOL MBackupPageDevice::Unk14(u32 offset, const void *buf, u32 size) {
    u8 *pageBuf;
    u32 lastPage;
    u32 first;
    u32 last;
    u32 len;
    u32 end;
    first = offset / mPageSize;
    end = offset + size;
    last = (mPageSize + end - 1) / mPageSize;
    u32 page = first;
    if (page != last) {
        lastPage = last - 1;
        do {
            u32 start = page == first ? offset % mPageSize : 0;
            u32 stop = page == lastPage ? (mPageSize + end - 1) % mPageSize + 1 : mPageSize;
            pageBuf = mPageBuf;
            len = stop - start;
            if (!ReadPages(page * mPageSize, pageBuf, mPageSize)) {
                return FALSE;
            }
            if (memcmp(buf, pageBuf + start, len) != 0) {
                MI_CpuCopy8(buf, pageBuf + start, len);
                if (!Unk24(page * mPageSize, mPageBuf, mPageSize)) {
                    return FALSE;
                }
            }
            buf = (const u8 *) buf + len;
            page++;
        } while (page != last);
    }
    return TRUE;
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

MBackupStorage::MBackupStorage(MBackupDevice *device) : mDevice(device) {
    mMountTried = FALSE;
    mMounted = FALSE;
    mFormatted = FALSE;
}

MBackupStorage::~MBackupStorage() {
    delete mDevice;
}

// NONMATCHING since the file went to #pragma ipa file: the inlined resize's tag temporary takes the other byte slot.
void MBackupStorage::AddFile(u32 size, u32 copies) {
    File file;
    file.size = size;
    file.current = 0;
    file.serial = 0;
    file.copies.resize(copies);
    file.damaged = FALSE;
    mFiles.push_back(file);
}

// (Emitted before its caller, as in the game's order.)
template void std::__vec_deleter<MBackupStorage::Region, std::allocator<MBackupStorage::Region> >::sub_push_back(
    const MBackupStorage::Region &x, Metrowerks::int2type<true>);

u32 MBackupStorage::AllocRegion(u32 size) {
    Region region;
    region.offset = mRegions.size() == 0 ? 0 : mDevice->GetPageSize() * ((mRegions.back().offset + mRegions.back().size + mDevice->GetPageSize() - 1) / mDevice->GetPageSize());
    region.size = size;
    u32 index = mRegions.size();
    mRegions.push_back(region);
    return index;
}

// The storage's vector members, in the game's order.
template std::__vec_deleter<MBackupStorage::Region, std::allocator<MBackupStorage::Region> >::~__vec_deleter();
template void std::__vec_deleter<MBackupStorage::File, std::allocator<MBackupStorage::File> >::sub_push_back(
    const MBackupStorage::File &x, Metrowerks::int2type<false>);
template std::__vec_deleter<MBackupStorage::File, std::allocator<MBackupStorage::File> >::~__vec_deleter();
template void std::__vec_deleter<MBackupStorage::Copy, std::allocator<MBackupStorage::Copy> >::append(
    size_t n, Metrowerks::int2type<true>);
template std::__vec_deleter<MBackupStorage::Copy, std::allocator<MBackupStorage::Copy> >::~__vec_deleter();

void MBackupStorage::Mount() {
    mMountTried = TRUE;
    if (!mDevice->IsValid()) {
        return;
    }
    MATHi_CRC32InitTableRev(&mCrcTable, 0xEDB88320);
    mHeaderSize = mFiles.size() * 4 + 4;
    mHeader = new u8[mHeaderSize];
    mHeaderWords = (u32 *) mHeader;
    mHeaderWords[0] = 'MBKU';
    for (u32 i = 0; i < mFiles.size(); i++) {
        mHeaderWords[i + 1] = mFiles[i].size;
    }
    AllocRegion(mHeaderSize);
    for (u32 i = 0; i < mFiles.size(); i++) {
        File &file = mFiles[i];
        file.current = 0;
        file.serial = 0;
        for (u32 j = 0; j < file.copies.size(); j++) {
            Copy &copy = file.copies[j];
            copy.serial = 0;
            copy.crc = 0;
            copy.headerRegion = AllocRegion(8);
            copy.dataRegion = AllocRegion(file.size);
        }
    }
    u8 *header = new u8[mHeaderSize];
    if (!ReadRegion(0, header) || memcmp(mHeader, header, mHeaderSize) != 0) {
        delete[] header;
        Format();
        return;
    }
    delete[] header;
    for (u32 i = 0; i < mFiles.size(); i++) {
        File &file = mFiles[i];
        u32 newestSerial = 0;
        u32 newest = 0;
        u8 *data = new u8[file.size];
        for (u32 j = 0; j < file.copies.size(); j++) {
            Copy &copy = file.copies[j];
            if (!ReadRegion(copy.headerRegion, &copy)) {
                return;
            }
            if (copy.serial != 0) {
                if (!ReadRegion(copy.dataRegion, data)) {
                    delete[] data;
                    return;
                }
                u32 crc = 0xFFFFFFFF;
                MATHi_CRC32UpdateRev(&mCrcTable, &crc, &copy, 4);
                MATHi_CRC32UpdateRev(&mCrcTable, &crc, data, file.size);
                if (copy.crc != ~crc) {
                    file.damaged = TRUE;
                    copy.serial = 0;
                    if (!WriteRegion(copy.headerRegion, &copy)) {
                        delete[] data;
                        return;
                    }
                } else if (newestSerial < copy.serial) {
                    newestSerial = copy.serial;
                    newest = j;
                }
            }
        }
        file.serial = newestSerial;
        file.current = newest;
        delete[] data;
    }
    mMounted = TRUE;
    PageRound(0);   // (a leftover of a debug check)
}

u8 MBackupStorage::IsMounted() {
    return mMounted;
}

BOOL MBackupStorage::ReadRegion(u32 region, void *buf) {
    Region &r = mRegions[region];
    return mDevice->Read(r.offset, buf, r.size);
}

BOOL MBackupStorage::WriteRegion(u32 region, const void *buf) {
    Region &r = mRegions[region];
    return mDevice->Unk14(r.offset, buf, r.size);
}

u8 MBackupStorage::IsFileDamaged(u32 file) {
    return mFiles[file].damaged;
}

BOOL MBackupStorage::HasFile(u32 file) {
    if (mFiles[file].serial != 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL MBackupStorage::ReadFile(u32 file, void *buf) {
    File &f = mFiles[file];
    if (f.serial == 0) {
        return FALSE;
    }
    return ReadRegion(f.copies[f.current].dataRegion, buf);
}

BOOL MBackupStorage::WriteFile(u32 file, const void *buf) {
    File *files = &mFiles[0];
    File &f = files[file];
    f.serial++;
    f.current = (f.current + 1) % f.copies.size();
    Copy &copy = f.copies[f.current];
    copy.serial = f.serial;
    u32 crc = 0xFFFFFFFF;
    MATHi_CRC32UpdateRev(&mCrcTable, &crc, &copy, 4);
    MATHi_CRC32UpdateRev(&mCrcTable, &crc, buf, files[file].size);
    copy.crc = ~crc;
    if (WriteRegion(copy.dataRegion, buf) && WriteRegion(copy.headerRegion, &copy)) {
        return TRUE;
    }
    return FALSE;
}

#pragma opt_common_subs off
void MBackupStorage::Format() {
    mMounted = FALSE;
    mFormatted = FALSE;
    for (u32 i = 0; i < mFiles.size(); i++) {
        File &file = mFiles[i];
        file.current = 0;
        file.serial = 0;
        for (u32 j = 0; j < file.copies.size(); j++) {
            Copy &copy = file.copies[j];
            copy.serial = 0;
            copy.crc = 0;
        }
    }
    for (u32 i = 0; i < mFiles.size(); i++) {
        File &file = mFiles[i];
        for (u32 j = 0; j < file.copies.size(); j++) {
            Copy &copy = file.copies[j];
            if (!WriteRegion(copy.headerRegion, &copy)) {
                return;
            }
        }
    }
    if (!WriteRegion(0, mHeader)) {
        return;
    }
    mMounted = TRUE;
    mFormatted = TRUE;
    PageRound(0);   // (a leftover of a debug check)
}
#pragma opt_common_subs reset
