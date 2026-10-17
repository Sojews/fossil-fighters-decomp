// DinoBackup: the save data.

#include "game/dino_backup.h"
#include <algorithm>
#include "m/archive.h"
#include "m/backup.h"
#include "m/irq.h"
#include "m/heap.h"
#include "m/seed_generator.h"
#include "m/serializer.h"
#include "m/sleep.h"
#include "m/soft_reset.h"
#include "m/task.h"
#include "game/dino_resource.h"
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

void DinoBackup::DisconnectRegion(u32 from, u32 to) {
    if (to != mRegionLinks[from]) {
        OS_Terminate();
    }
    mRegionLinks[from] = 0;
}

u16 DinoBackup::GetRegionLink(u32 from) {
    return mRegionLinks[from];
}

void DinoBackup::GetLinkedRegions(MBitVector *out) {
    u32 count = gDinoResource->GetRegionCount();
    for (u32 i = 1; i < count; i++) {
        if (mRegionLinks[i] != 0) {
            out->Set(i);
        }
    }
}

void DinoBackup::SetPlayer(const DinoPlayerData *player) {
    *mPlayer = *player;
}

void DinoBackup::GetPlayer(DinoPlayerData *player, BOOL shortName) {
    *player = *mPlayer;
    if (shortName) {
        for (u32 i = 0; i < 8; i++) {
            if (player->name[i] == 0) {
                break;
            }
            if (i >= 5) {
                player->name[i] = '.';
            }
        }
    }
}

void DinoBackup::SetChara(u32 index, const DinoCharaState *chara) {
    mCharas[index] = *chara;
}

void DinoBackup::GetChara(u32 index, DinoCharaState *chara) {
    *chara = mCharas[index];
}

void DinoBackup::SetCharaTask(u32 id, const DinoCharaTask *task) {
    if (task->unk4 == 0 && task->unk28.active == 0) {
        for (u32 i = 0; i < 60; i++) {
            if (id == mCharaTasks[i].id) {
                mCharaTasks[i].id = 0;
                return;
            }
        }
        return;
    }
    for (u32 i = 0; i < 60; i++) {
        if (id == mCharaTasks[i].id) {
            mCharaTasks[i] = *task;
            mCharaTasks[i].id = id;
            return;
        }
    }
    for (u32 i = 0; i < 60; i++) {
        if (mCharaTasks[i].id == 0) {
            mCharaTasks[i] = *task;
            mCharaTasks[i].id = id;
            return;
        }
    }
    OS_Terminate();
}

void DinoBackup::GetCharaTask(u32 id, DinoCharaTask *task) {
    DinoCharaTask *tasks = mCharaTasks;
    for (u32 i = 0; i < 60; i++) {
        if (id == tasks[i].id) {
            *task = mCharaTasks[i];
            return;
        }
    }
    MI_CpuFill8(task, 0, sizeof(DinoCharaTask));
    task->id = id;
}

void DinoBackup::SetUnk98(u32 index, const DinoBackupEntry *entry) {
    mUnk98[index] = *entry;
}

void DinoBackup::GetUnk98(u32 index, DinoBackupEntry *entry) {
    *entry = mUnk98[index];
}

void DinoBackup::SetKasekiRock(u32 index, const u32 *rock) {
    mKasekiRocks[index] = *rock;
}

void DinoBackup::GetKasekiRock(u32 index, u32 *rock) {
    *rock = mKasekiRocks[index];
}

void DinoBackup::SetRecord(u32 index, const DinoBackupRecord *record) {
    mRecords[index] = *record;
}

void DinoBackup::GetRecord(u32 index, DinoBackupRecord *record) {
    *record = mRecords[index];
}

void DinoBackup::SetTeam(u32 index, const DinoTeam *team) {
    mTeams[index] = *team;
}

void DinoBackup::GetTeam(u32 index, DinoTeam *team) {
    *team = mTeams[index];
}

// Makes sure the current team has at least one creature, taking the first one owned that isn't in it.
void DinoBackup::FillTeam() {
    DinoTeam *team = &mTeams[GetUnk34_26()];
    u32 i;
    u32 count = 0;
    for (i = 0; i < 5; i++) {
        if (team->slots[i] != 0) {
            count++;
        }
    }
    if (count >= 1) {
        return;
    }
    for (u32 id = 1; id < gDinoResource->GetCreatureCount(); id++) {
        if (mCreatures[id].parts[0].level == 0) {
            continue;
        }
        BOOL inTeam;
        for (u32 i = 0; i < 5; i++) {
            if (id == team->slots[i]) {
                inTeam = TRUE;
                goto found;
            }
        }
        inTeam = FALSE;
    found:
        if (inTeam) {
            continue;
        }
        u32 slot;
        for (slot = 0; slot < 5; slot++) {
            if (team->slots[slot] == 0) {
                goto empty;
            }
        }
        slot = -1;
    empty:
        team->slots[slot] = id;
        count = 0;
        for (u32 i = 0; i < 5; i++) {
            if (team->slots[i] != 0) {
                count++;
            }
        }
        if (count >= 1) {
            return;
        }
    }
}

void DinoBackup::SetCreature(u32 index, const DinoCreatureState *creature) {
    mCreatures[index] = *creature;
}

void DinoBackup::GetCreature(u32 index, DinoCreatureState *creature) {
    *creature = mCreatures[index];
}

static inline u32 CountLooseParts(const DinoCreatureState *creature) {
    if (creature->parts[0].level != 0) {
        return 0;
    }
    u32 count = 0;
    for (u32 i = 1; i < 4; i++) {
        if (creature->parts[i].level != 0) {
            count++;
        }
    }
    return count;
}

u32 DinoBackup::CountCreatureParts() {
    u32 total = 0;
    for (u32 id = 1; id < gDinoResource->GetCreatureCount(); id++) {
        total += CountLooseParts(&mCreatures[id]);
    }
    return total;
}

void DinoBackup::SetUnk134(u32 index, const DinoBackupEntry *entry) {
    mUnk134[index] = *entry;
}

void DinoBackup::GetUnk134(u32 index, DinoBackupEntry *entry) {
    *entry = mUnk134[index];
}

void DinoBackup::SetUnk144(s32 value) {
    mUnk144 = value;
}

s32 DinoBackup::GetUnk144() {
    return mUnk144;
}

void DinoBackup::SetUnk148(u32 index, const DinoBackupEntry *entry) {
    mUnk148[index] = *entry;
}

void DinoBackup::GetUnk148(u32 index, DinoBackupEntry *entry) {
    *entry = mUnk148[index];
}

void DinoBackup::ClearUnk148() {
    ClearEntries(mUnk148, 8);
}

BOOL DinoBackup::IsUnk148Empty() {
    return AreEntriesEmpty(mUnk148, 8);
}

BOOL DinoBackup::IsUnk148Full() {
    return AreEntriesFull(mUnk148, 8);
}

s32 DinoBackup::FindFreeUnk148() {
    return FindFreeEntry(mUnk148, 8);
}

void DinoBackup::CompactUnk148() {
    CompactEntries(mUnk148, 8);
}

void DinoBackup::AddUnk148(const DinoEntrySource *source) {
    if (IsUnk148Full()) {
        return;
    }
    DinoBackupEntry entry;
    MI_CpuFill8(&entry, 0, sizeof(entry));
    switch (source->type) {
    case 0:
        entry.id = source->id;
        break;
    case 2:
        entry.id = gDinoResource->GetRandom()->Next(400) + 1;
        break;
    case 1: {
        entry.id = 1;
        MRandomRef random(gDinoResource->GetRandom());
        for (u32 id = 1; id <= 400; id++) {
            if (((DinoKasekiRock *) &mKasekiRocks[id])->found && random.Roll()) {
                entry.id = id;
            }
        }
        break;
    }
    }
    entry.unkC_12 = 0;
    entry.type = 0;
    if (mPlayer->license < 1) {
        entry.unk10_0 = 0;
    } else {
        entry.unk10_0 = gDinoResource->GetCreatureUnk0D((entry.id - 1) / 4 + 1);
    }
    entry.unk10_3 = 1;
    mUnk148[FindFreeUnk148()] = entry;
}

void DinoBackup::SetUnk14C(u32 index, const DinoBackupEntry *entry) {
    mUnk14C[index] = *entry;
}

void DinoBackup::GetUnk14C(u32 index, DinoBackupEntry *entry) {
    *entry = mUnk14C[index];
}

u32 DinoBackup::CountUsedUnk14C() {
    s32 free = FindFreeUnk14C();
    if (free == -1) {
        free = 8;
    }
    return free;
}

u32 DinoBackup::CountFreeUnk14C() {
    return 8 - CountUsedUnk14C();
}

BOOL DinoBackup::IsUnk14CEmpty() {
    return AreEntriesEmpty(mUnk14C, 8);
}

BOOL DinoBackup::IsUnk14CFull() {
    return AreEntriesFull(mUnk14C, 8);
}

s32 DinoBackup::FindFreeUnk14C() {
    return FindFreeEntry(mUnk14C, 8);
}

void DinoBackup::CompactUnk14C() {
    CompactEntries(mUnk14C, 8);
}

void DinoBackup::ClearEntries(DinoBackupEntry *entries, u32 count) {
    MI_CpuFill8(entries, 0, count * sizeof(DinoBackupEntry));
}

BOOL DinoBackup::AreEntriesEmpty(DinoBackupEntry *entries, u32 count) {
    for (u32 i = 0; i < count; i++) {
        if (entries[i].id != 0) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL DinoBackup::AreEntriesFull(DinoBackupEntry *entries, u32 count) {
    for (u32 i = 0; i < count; i++) {
        if (entries[i].id == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

s32 DinoBackup::FindFreeEntry(DinoBackupEntry *entries, u32 count) {
    for (u32 i = 0; i < count; i++) {
        if (entries[i].id == 0) {
            return i;
        }
    }
    return -1;
}
