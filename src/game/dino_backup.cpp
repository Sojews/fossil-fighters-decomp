// DinoBackup: the save data.

#include "game/dino_backup.h"
#include <algorithm>
#include "m/archive.h"
#include "m/backup.h"
#include "m/irq.h"
#include "m/heap.h"
#include "m/serializer.h"
#include "m/sleep.h"
#include "m/soft_reset.h"
#include "m/task.h"
#include "game/dino_sound.h"
#include "nitro/mi.h"

DinoBackup *gDinoBackup;
static MMutex sBackupMutex;

static const s32 sLimits[] = {
    0,  -4, 0, 99999, 5184000, 99999999, -4, -4, -4, 9999, -4, 100, 8, 0, -4,
};

#define BACKUP_BLOCK_DATA 480   // what's left of a 512-byte block after the header

void DinoBackup::Create(u32 extraSize, BOOL useBackup) {
    DinoBackup *backup = new DinoBackup(extraSize, useBackup);
    MIrqLock lock;
    gDinoBackup = backup;
}

BOOL DinoBackup::IsBackupPresent() {
    if (!mEnabled) {
        return FALSE;
    }
    if (mDevice->IsValid() == FALSE) {
        return FALSE;
    }
    u8 byte;
    return mDevice->Read(0, &byte, 1);
}

u8 DinoBackup::TakeLoadFailed() {
    u8 failed = mLoadFailed;
    mLoadFailed = FALSE;
    return failed;
}

void DinoBackup::OnVSync() {
    MMutexLock lock(&sBackupMutex.mutex);
    mPlayFrames++;
    mFrames = std::min(mFrames + 1, sLimits[4]);
}

u32 DinoBackup::GetSlot() {
    return mSlot;
}

void DinoBackup::SetSlot(u32 slot) {
    mSlot = slot;
}

void DinoBackup::SetSummary(u32 slot, const DinoSaveSummary *summary) {
    mSummaries[slot] = *summary;
}

void DinoBackup::GetSummary(u32 slot, DinoSaveSummary *summary) {
    *summary = mSummaries[slot];
}

void DinoBackup::MakeSummary(DinoSaveSummary *summary) {
    MScopedLock lock(&sBackupMutex.mutex, TRUE);
    summary->valid = TRUE;
    MI_CpuCopy8(mPlayer, summary, sizeof(summary->name));
    summary->rank = mPlayer->rank;
    summary->horiderLevel = mPlayer->horiderLevel;
    summary->unk14_14 = mPlayer->unk14_14;
    summary->playFrames = (u64) mPlayFrames;
    summary->flag19 = GetSystemFlag(0x19);
    summary->flag1a = GetSystemFlag(0x1a);
    summary->flag1b = GetSystemFlag(0x1b);
    summary->flag2f = GetSystemFlag(0x2f);
    summary->unk9 = mUnk180;
}

void DinoBackup::SetUnk180(s32 value) {
    mUnk180 = value;
}

s32 DinoBackup::GetUnk180() {
    return mUnk180;
}

void DinoBackup::ApplyVolume() {
    DinoSoundSetTargetVolume(mPlayer->targetVolume * 127 / 10);
    DinoSoundSetVolume(mPlayer->volume * 127 / 10);
}

void DinoBackup::WriteHeader() {
    if (mStorage == NULL) {
        return;
    }
    MDisableSoftReset();
    MDisableSleep();
    u8 *buf = new u8[(mHeaderSize + BACKUP_BLOCK_DATA - 1) / BACKUP_BLOCK_DATA * BACKUP_BLOCK_DATA];
    MI_CpuFill8(buf, 0, (mHeaderSize + BACKUP_BLOCK_DATA - 1) / BACKUP_BLOCK_DATA * BACKUP_BLOCK_DATA);
    mHeaderSerializer->Write(buf);
    Write(0, mHeaderSize, buf);
    delete[] buf;
    MEnableSoftReset();
    MEnableSleep();
}

void DinoBackup::WriteSlotRaw() {
    if (mStorage == NULL) {
        return;
    }
    MDisableSoftReset();
    MDisableSleep();
    u8 *buf = new u8[(mDataSize + BACKUP_BLOCK_DATA - 1) / BACKUP_BLOCK_DATA * BACKUP_BLOCK_DATA];
    MI_CpuFill8(buf, 0, (mDataSize + BACKUP_BLOCK_DATA - 1) / BACKUP_BLOCK_DATA * BACKUP_BLOCK_DATA);
    {
        MScopedLock lock(&sBackupMutex.mutex, TRUE);
        mSerializer->Write(buf);
    }
    Write(mSlot + 1, mDataSize, buf);
    Write(mSlot + 1, mDataSize, buf);
    delete[] buf;
    MEnableSoftReset();
    MEnableSleep();
}

void DinoBackup::Save() {
    if (mStorage == NULL) {
        return;
    }
    MDisableSoftReset();
    MDisableSleep();
    u8 *buf = new u8[(mDataSize + BACKUP_BLOCK_DATA - 1) / BACKUP_BLOCK_DATA * BACKUP_BLOCK_DATA];
    MI_CpuFill8(buf, 0, (mDataSize + BACKUP_BLOCK_DATA - 1) / BACKUP_BLOCK_DATA * BACKUP_BLOCK_DATA);
    {
        MScopedLock lock(&sBackupMutex.mutex, TRUE);
        mSerializer->Write(buf);
        MakeSummary(&mSummaries[mSlot]);
    }
    Write(mSlot + 1, mDataSize, buf);
    delete[] buf;
    WriteHeader();
    MEnableSoftReset();
    MEnableSleep();
}

void DinoBackup::Load() {
    if (mStorage == NULL) {
        return;
    }
    Reset(FALSE);
    if (!mStorage->HasFile(mSlot + 1)) {
        return;
    }
    MDisableSoftReset();
    MDisableSleep();
    u8 *buf = new u8[(mDataSize + BACKUP_BLOCK_DATA - 1) / BACKUP_BLOCK_DATA * BACKUP_BLOCK_DATA];
    mLoaded = Read(mSlot + 1, mDataSize, buf);
    if (mLoaded) {
        MScopedLock lock(&sBackupMutex.mutex, TRUE);
        mSerializer->Read(buf);
    }
    delete[] buf;
    ApplyVolume();
    MEnableSoftReset();
    MEnableSleep();
}

u8 DinoBackup::IsLoaded() {
    return mLoaded;
}

u8 *DinoBackup::GetUnk140() {
    return mUnk140;
}

u64 DinoBackup::GetPlayFrames() {
    MMutexLock lock(&sBackupMutex.mutex);
    return mPlayFrames;
}

u64 DinoBackup::GetUnk160() {
    return mUnk160;
}

void DinoBackup::MarkUnk160() {
    MMutexLock lock(&sBackupMutex.mutex);
    mUnk160 = mPlayFrames;
}

u64 DinoBackup::GetUnk168() {
    return mUnk168;
}

void DinoBackup::MarkUnk168() {
    MMutexLock lock(&sBackupMutex.mutex);
    mUnk168 = mPlayFrames;
}

void DinoBackup::SetUnk34_16(u32 value) {
    mPlayer->unk34_16 = value;
}

u32 DinoBackup::GetUnk34_16() {
    return mPlayer->unk34_16;
}

void DinoBackup::SetVolume(u32 volume) {
    mPlayer->volume = volume;
    ApplyVolume();
}

u32 DinoBackup::GetVolume() {
    return mPlayer->volume;
}

void DinoBackup::SetTargetVolume(u32 volume) {
    mPlayer->targetVolume = volume;
    ApplyVolume();
}

u32 DinoBackup::GetTargetVolume() {
    return mPlayer->targetVolume;
}

void DinoBackup::SetUnk34_26(u32 value) {
    mPlayer->unk34_26 = value;
}

u32 DinoBackup::GetUnk34_26() {
    return mPlayer->unk34_26;
}

void DinoBackup::SetFlag(u32 id, BOOL value) {
    (*mFlags)[id] = value;
}

BOOL DinoBackup::GetFlag(u32 id) {
    return mFlags->Test(id);
}

void DinoBackup::SetKeyItem(u32 id, BOOL value) {
    (*mKeyItems)[id] = value;
}

BOOL DinoBackup::GetKeyItem(u32 id) {
    return mKeyItems->Test(id);
}

void DinoBackup::SetHeadMask(u32 id, BOOL value) {
    (*mHeadMasks)[id] = value;
}

BOOL DinoBackup::GetHeadMask(u32 id) {
    return mHeadMasks->Test(id);
}

void DinoBackup::SetLocalVar(u32 index, s32 value) {
    mVars[index] = value;
}

s32 DinoBackup::GetLocalVar(u32 index) {
    return mVars[index];
}

void DinoBackup::SetVar(DinoEventVar var, s32 value) {
    switch (var.type) {
    case 8:
        SetLocalVar(var.index, value);
        break;
    case 9:
        SetSystemVar(var.index, value);
        break;
    }
}

s32 DinoBackup::GetVar(DinoEventVar var) {
    switch (var.type) {
    default:
        return 0;
    case 8:
        return GetLocalVar(var.index);
    case 9:
        return GetSystemVar(var.index);
    }
}

void DinoBackup::SetVarFlag(DinoEventVar var, BOOL value) {
    switch (var.type) {
    case 5:
        SetFlag(var.index, value);
        break;
    case 10:
        SetSystemFlag(var.index, value);
        break;
    case 6:
        SetKeyItem(var.index, value);
        break;
    case 7:
        SetHeadMask(var.index, value);
        break;
    }
}

BOOL DinoBackup::GetVarFlag(DinoEventVar var) {
    switch (var.type) {
    default:
        return FALSE;
    case 5:
        return GetFlag(var.index);
    case 10:
        return GetSystemFlag(var.index);
    case 6:
        return GetKeyItem(var.index);
    case 7:
        return GetHeadMask(var.index);
    }
}

BOOL DinoBackup::GetVarClear(DinoEventVar var) {
    switch (var.type) {
    case 2:
        return GetEventClear(var.index);
    case 3:
        return GetEpisodeClear(var.index);
    }
    return FALSE;
}

void DinoBackup::SetEventClear(u32 id, BOOL value) {
    (*mEvents)[id] = value;
}

BOOL DinoBackup::GetEventClear(u32 id) {
    return mEvents->Test(id);
}

void DinoBackup::SetEpisodeClear(u32 id, BOOL value) {
    (*mEpisodes)[id] = value;
}

BOOL DinoBackup::GetEpisodeClear(u32 id) {
    return mEpisodes->Test(id);
}

void DinoBackup::SetEpisodeSeen(u32 id, BOOL value) {
    (*mEpisodesSeen)[id] = value;
}

BOOL DinoBackup::GetEpisodeSeen(u32 id) {
    return mEpisodesSeen->Test(id);
}

void DinoBackup::ConnectRegion(u32 from, u32 to) {
    if (mRegionLinks[from] != 0) {
        OS_Terminate();
    }
    mRegionLinks[from] = to;
}
