// DinoBackup: the save data.

#include "game/dino_backup.h"
#include <algorithm>
#include "m/archive.h"
#include "m/backup.h"
#include "m/irq.h"
#include "m/heap.h"
#include "m/lcd.h"
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

// Text IDs.
extern const u32 kBackupEntryUnknownText;
extern const u32 kBackupEntryUnk2Text;
extern const u64 kBackupRecentFrames;
extern const u32 kCreatureLevelLimits[2];

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
    MScopedLock lock(&sBackupMutex.mutex, TRUE);
    mPlayFrames++;
    mFrames = std::min(mFrames + 1, sLimits[4]);
}

// The save file cipher (RC4, it seems): the key schedule and the stream.
struct DinoBackupCipher {
    u32 i, j;
    u32 state[256];
};
extern "C" void func_0204b0ec(DinoBackupCipher *cipher, const void *key, u32 keySize);
extern "C" void func_0204b15c(DinoBackupCipher *cipher, const void *src, u32 size, void *dest);
extern "C" void MATH_CalcMD5(void *digest, const void *data, u32 size);
extern "C" const u8 data_0208ae00[16]; // the key, XORed with each block's own

// Reads a save file: 0x200-byte blocks of a 16-byte key, the MD5 of the data, and 0x1E0 bytes of data, all but
// the key encrypted. Fails if a block's data doesn't match its MD5.
BOOL DinoBackup::Read(u32 file, u32 size, void *dest) {
    if (mStorage == NULL) {
        return FALSE;
    }
    if (!mStorage->HasFile(file)) {
        return FALSE;
    }
    MDisableSoftReset();
    MDisableSleep();
    BOOL ok = FALSE;
    u8 *buf;
    u32 blocks = (size + 0x1DF) / 0x1E0;
    buf = new u8[blocks << 9];
    if (!mStorage->ReadFile(file, buf)) {
        mEnabled = FALSE;
    } else {
        for (u32 i = 0; i < blocks; i++) {
            u8 *block = buf + i * 0x200;
            u8 key[16];
            for (u32 j = 0; j < 16; j++) {
                key[j] = block[j] ^ data_0208ae00[j];
            }
            DinoBackupCipher cipher;
            func_0204b0ec(&cipher, key, 16);
            func_0204b15c(&cipher, block + 0x10, 0x1F0, block + 0x10);
            u8 digest[16];
            MATH_CalcMD5(digest, block + 0x20, 0x1E0);
            if (memcmp(digest, block + 0x10, 16) != 0) {
                goto done;
            }
            MI_CpuCopy8(block + 0x20, (u8 *) dest + i * 0x1E0, 0x1E0);
        }
        ok = TRUE;
    }
done:
    delete[] buf;
    MEnableSoftReset();
    MEnableSleep();
    return ok;
}

// Writes a save file in Read's format. Blocks whose data hasn't changed keep their old encryption.
// NONMATCHING: the end of the loop (the game makes the 0x200 step first and the source step from it).
BOOL DinoBackup::Write(u32 file, u32 size, const void *src) {
    if (mStorage == NULL) {
        return FALSE;
    }
    MDisableSoftReset();
    MDisableSleep();
    BOOL ok = FALSE;
    u8 *out;
    u8 *old;
    u32 i;
    u32 blocks = (size + 0x1DF) / 0x1E0;
    out = new u8[blocks << 9];
    old = NULL;
    if (mStorage->HasFile(file)) {
        old = new u8[blocks << 9];
        if (!mStorage->ReadFile(file, old)) {
            mEnabled = FALSE;
            goto done;
        }
    }
    for (i = 0; i < blocks; i++) {
        u8 *block = out + i * 0x200;
        u8 *oldBlock = old + i * 0x200;
        if (old != NULL) {
            MI_CpuCopy8(oldBlock, block, 0x200);
            u8 key[16];
            for (u32 j = 0; j < 16; j++) {
                key[j] = oldBlock[j] ^ data_0208ae00[j];
            }
            DinoBackupCipher cipher;
            func_0204b0ec(&cipher, key, 16);
            func_0204b15c(&cipher, oldBlock + 0x10, 0x1F0, oldBlock + 0x10);
            u8 digest[16];
            MATH_CalcMD5(digest, oldBlock + 0x20, 0x1E0);
            if (memcmp(digest, oldBlock + 0x10, 16) == 0 && memcmp(oldBlock + 0x20, src, 0x1E0) == 0) {
                goto next;
            }
        }
        {
            MI_CpuCopy8(src, block + 0x20, 0x1E0);
            MATH_CalcMD5(block + 0x10, block + 0x20, 0x1E0);
            for (u32 k = 0; k < 4; k++) {
                ((u32 *) block)[k] = gDinoResource->GetRandom()->Next();
            }
            u8 key[16];
            for (u32 j = 0; j < 16; j++) {
                key[j] = block[j] ^ data_0208ae00[j];
            }
            DinoBackupCipher cipher;
            func_0204b0ec(&cipher, key, 16);
            func_0204b15c(&cipher, block + 0x10, 0x1F0, block + 0x10);
        }
    next:
        src = (const u8 *) src + 0x1E0;
    }
    if (!mStorage->WriteFile(file, out)) {
        mEnabled = FALSE;
    } else {
        ok = TRUE;
    }
done:
    delete[] old;
    delete[] out;
    MEnableSoftReset();
    MEnableSleep();
    return ok;
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
    MScopedLock lock(&sBackupMutex.mutex, TRUE);
    return mPlayFrames;
}

u64 DinoBackup::GetUnk160() {
    return mUnk160;
}

void DinoBackup::MarkUnk160() {
    MScopedLock lock(&sBackupMutex.mutex, TRUE);
    mUnk160 = mPlayFrames;
}

u64 DinoBackup::GetUnk168() {
    return mUnk168;
}

void DinoBackup::MarkUnk168() {
    MScopedLock lock(&sBackupMutex.mutex, TRUE);
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

// The order the entries are sorted into (std::stable_sort's comparison, not decompiled yet).
struct DinoBackupEntryOrder {};
extern "C" void func_020187c4(DinoBackupEntry *first, DinoBackupEntry *last, DinoBackupEntryOrder order);

// Moves the used entries in front of the free ones, then sorts them.
void DinoBackup::CompactEntries(DinoBackupEntry *entries, u32 count) {
again:
    s32 hole = -1;
    for (u32 i = 0; i < count; i++) {
        if (entries[i].id != 0) {
            if (hole != -1) {
                std::swap(entries[i], entries[hole]);
                goto again;
            }
        } else if (hole == -1) {
            hole = i;
        }
    }
    func_020187c4(entries, entries + count, DinoBackupEntryOrder());
}

u32 DinoBackup::GetUnk98Capacity() {
    return mPlayer->containerNum * 8;
}

u32 DinoBackup::CountUsedUnk98() {
    s32 free = FindFreeUnk98();
    if (free == -1) {
        free = GetUnk98Capacity();
    }
    return free;
}

u32 DinoBackup::CountFreeUnk98() {
    return GetUnk98Capacity() - CountUsedUnk98();
}

BOOL DinoBackup::IsUnk98Empty() {
    return AreEntriesEmpty(mUnk98, GetUnk98Capacity());
}

BOOL DinoBackup::IsUnk98Full() {
    return AreEntriesFull(mUnk98, GetUnk98Capacity());
}

s32 DinoBackup::FindFreeUnk98() {
    return FindFreeEntry(mUnk98, GetUnk98Capacity());
}

void DinoBackup::CompactUnk98() {
    CompactEntries(mUnk98, GetUnk98Capacity());
}

std::string DinoBackup::GetEntryName(const DinoBackupEntry *entry) {
    if (!entry->active) {
        if (entry->unk10_0 == 1) {
            return std::string(gDinoResource->GetText(kBackupEntryUnknownText));
        }
        if (entry->unk10_0 == 0 && entry->type == 1) {
            return std::string(gDinoResource->GetText(kBackupEntryUnk2Text));
        }
        if (!entry->unk10_3 && !((DinoKasekiRock *) &mKasekiRocks[entry->id])->found) {
            return std::string(gDinoResource->GetText(kBackupEntryUnknownText));
        }
    }
    switch (entry->type) {
    case 0:
        if (entry->id >= 0x1a9) {
            return std::string(gDinoResource->GetCreatureText08((entry->id - 1) / 4 + 1));
        }
        return gDinoResource->GetKasekiRockName(entry->id);
    case 1: {
        MResource<DinoKasekiDefs> defs = gDinoResource->LoadKasekiDefs();
        const DinoKasekiDefs *data = defs.Get();
        u32 id = entry->id;
        return std::string(gDinoResource->GetText(
            ((const DinoKasekiDef *) ((const u8 *) data + ((const u32 *) ((const u8 *) data + data->tableOffset))[id]))
                ->nameText));
    }
    case 2: {
        MResource<DinoKasekiDefs> defs = gDinoResource->LoadKasekiDefs();
        const DinoKasekiDefs *data = defs.Get();
        u32 id = entry->id;
        return std::string(gDinoResource->GetText(
            ((const DinoKasekiDef *) ((const u8 *) data + ((const u32 *) ((const u8 *) data + data->tableOffset))[id]))
                ->nameText));
    }
    }
    return std::string("");
}

u32 DinoBackup::CountUnk9C() {
    u32 count = 0;
    for (u32 i = 0; i < 64; i++) {
        if (mUnk9C[i].id != 0) {
            count++;
        }
    }
    return count;
}

void DinoBackup::SetUnk9C(u32 index, const DinoBackupEntry *entry) {
    mUnk9C[index] = *entry;
}

void DinoBackup::GetUnk9C(u32 index, DinoBackupEntry *entry) {
    *entry = mUnk9C[index];
}

BOOL DinoBackup::IsUnk9CEmpty() {
    return AreEntriesEmpty(mUnk9C, 64);
}

s32 DinoBackup::FindFreeUnk9C() {
    return FindFreeEntry(mUnk9C, 64);
}

BOOL DinoBackup::IsEntryUnrevealed(const DinoBackupEntry *entry) {
    BOOL result = TRUE;
    BOOL a = FALSE;
    BOOL b = FALSE;
    if (entry->type == 1 && !entry->active) {
        b = TRUE;
    }
    if (b && entry->unk10_0 != 1) {
        a = TRUE;
    }
    if (!a) {
        if (entry->type != 0 || entry->unk10_0 == 1 ||
            !((DinoKasekiRock *) &mKasekiRocks[entry->id])->found) {
            result = FALSE;
        }
    }
    return result;
}

BOOL DinoBackup::HasInactiveUnk98() {
    for (u32 i = 0; i < 64; i++) {
        if (mUnk98[i].id != 0 && !mUnk98[i].active) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL DinoBackup::HasUnrevealedUnk98() {
    for (u32 i = 0; i < 64; i++) {
        if (mUnk98[i].id != 0 && IsEntryUnrevealed(&mUnk98[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

const DinoExcavateLevel *DinoBackup::GetExcavateLevel() {
    MResource<DinoExcavateDefs> defs = gDinoResource->GetExcavateDefs();
    for (s32 i = defs.Get()->levelCount - 1; i >= 0; i--) {
        const DinoExcavateDefs *data = defs.Get();
        const DinoExcavateLevel *levels = (const DinoExcavateLevel *) ((const u8 *) data + data->levelOffset);
        if (levels[i].minUnk <= mPlayer->unk34_0) {
            data = defs.Get();
            return &((const DinoExcavateLevel *) ((const u8 *) data + data->levelOffset))[i];
        }
    }
    return NULL;
}

u32 DinoBackup::GetExcavateLevelUnk0() {
    return GetExcavateLevel()->unk0;
}

s32 DinoBackup::GetExcavateLevelAverage() {
    const DinoExcavateLevel *level = GetExcavateLevel();
    MFx32 b = level->unk10;
    MFx32 a = level->unkC;
    return MFx32ToInt((a + b) / MFx32(2));
}

s32 DinoBackup::GetExcavateLevelRemaining() {
    return GetExcavateLevel()->unk8 - mPlayer->unk34_0;
}

u32 DinoBackup::GetMaxExcavateLevelUnk0() {
    MResource<DinoExcavateDefs> defs = gDinoResource->GetExcavateDefs();
    u32 max = 0;
    for (s32 i = defs.Get()->levelCount - 1; i >= 0; i--) {
        const DinoExcavateDefs *data = defs.Get();
        u32 value = ((const DinoExcavateLevel *) ((const u8 *) data + data->levelOffset))[i].unk0;
        max = std::max(max, value);
    }
    return max;
}

void DinoBackup::SetUnk138(u32 index, const DinoBackupEntry28 *entry) {
    mUnk138[index] = *entry;
}

void DinoBackup::GetUnk138(u32 index, DinoBackupEntry28 *entry) {
    *entry = mUnk138[index];
}

BOOL DinoBackup::CanAddUnk138(const DinoBackupEntry *entry, BOOL checkDuplicate, BOOL checkCount, u32 maxCount) {
    if (checkDuplicate) {
        for (u32 i = 0; i < 20; i++) {
            if (mUnk138[i].used && entry->id == mUnk138[i].id) {
                return FALSE;
            }
        }
    }
    if (checkCount) {
        u32 count = 0;
        for (u32 i = 0; i < 20; i++) {
            if (mUnk138[i].used && mUnk138[i].flag) {
                count++;
            }
        }
        if (count >= maxCount) {
            return FALSE;
        }
    }
    return TRUE;
}

void DinoBackup::SetSystemVar(u32 index, s32 value) {
    switch (index) {
    case 1:
        mPlayer->horiderLevel = value;
        break;
    case 2:
        mPlayer->containerNum = value;
        break;
    case 3:
        mPlayer->money = std::max(sLimits[2], std::min(sLimits[5], value));
        break;
    case 4:
        mPlayer->unk14_0 = value;
        break;
    case 5:
        mPlayer->license = value;
        break;
    case 6:
        mPlayer->unk14_14 = value;
        break;
    case 7:
        mPlayer->rank = value;
        break;
    case 14:
        mPlayer->unk1c_23 = value;
        break;
    case 15:
        mPlayer->unk20_0 = value;
        break;
    case 16:
        mPlayer->unk20_8 = value;
        break;
    case 28:
        mPlayer->unk20_16 = value;
        break;
    case 29:
        mPlayer->unk24 = value;
        break;
    case 39:
        mPlayer->unk30_17 = value;
        break;
    case 17:
        mPlayer->unk28_0 = value;
        break;
    case 18:
        mPlayer->unk28_8 = value;
        break;
    case 19:
        mPlayer->unk2c_0 = std::max(sLimits[13], std::min(sLimits[3], value));
        break;
    case 25: {
        u32 v = value;
        mPlayer->unk34_0 = std::min(v, (u32) sLimits[9]);
        break;
    }
    case 30:
        mPlayer->unk34_28 = value;
        break;
    case 31:
        mPlayer->unk34_30 = value;
        break;
    case 32:
        mPlayer->unk38_0 = value;
        break;
    case 26: {
        MScopedLock lock(&sBackupMutex.mutex, TRUE);
        mFrames = value;
        break;
    }
    case 43:
        mPlayer->unk38_4 = value;
        break;
    case 44:
        mUnk1B4 = value;
        break;
    case 45:
        mUnk1B8 = value;
        break;
    case 46:
        mPlayer->unk30_0 = value;
        break;
    }
}

s32 DinoBackup::GetSystemVar(u32 index) {
    switch (index) {
    case 1:
        return mPlayer->horiderLevel;
    case 2:
        return mPlayer->containerNum;
    case 3:
        return mPlayer->money;
    case 4:
        return mPlayer->unk14_0;
    case 5:
        return mPlayer->license;
    case 6:
        return mPlayer->unk14_14;
    case 7:
        return mPlayer->rank;
    case 13:
        return mPlayer->unk1c_7;
    case 14:
        return mPlayer->unk1c_23;
    case 15:
        return mPlayer->unk20_0;
    case 16:
        return mPlayer->unk20_8;
    case 28:
        return mPlayer->unk20_16;
    case 29:
        return mPlayer->unk24;
    case 39:
        return mPlayer->unk30_17;
    case 17:
        return mPlayer->unk28_0;
    case 18:
        return mPlayer->unk28_8;
    case 19:
        return mPlayer->unk2c_0;
    case 20:
        return CountUnk9C();
    case 21:
        return GetExcavateLevelUnk0();
    case 22:
        return GetExcavateLevelAverage();
    case 23:
        return GetExcavateLevelRemaining();
    case 24:
        return CountCreatureParts();
    case 25:
        return mPlayer->unk34_0;
    case 30:
        return mPlayer->unk34_28;
    case 31:
        return mPlayer->unk34_30;
    case 32:
        return mPlayer->unk38_0;
    case 26: {
        MScopedLock lock(&sBackupMutex.mutex, TRUE);
        return mFrames;
    }
    case 43:
        return mPlayer->unk38_4;
    case 8: {
        u32 count = 0;
        for (u32 i = 0; i < 64; i++) {
            if (mUnk98[i].id != 0 && !mUnk98[i].active) {
                count++;
            }
        }
        return count;
    }
    case 9: {
        u32 count = 0;
        for (u32 i = 0; i < 64; i++) {
            if (mUnk98[i].id != 0 && !mUnk98[i].active && mUnk98[i].unk10_0 >= 2) {
                count++;
            }
        }
        return count;
    }
    case 10: {
        u32 count = 0;
        for (u32 i = 0; i < 64; i++) {
            if (mUnk98[i].id != 0 && !mUnk98[i].active && mUnk98[i].unk10_0 == 1) {
                count++;
            }
        }
        return count;
    }
    case 11: {
        u32 count = 0;
        for (u32 i = 0; i < 64; i++) {
            if (mUnk98[i].id != 0 && mUnk98[i].active && mUnk98[i].type == 1) {
                count++;
            }
        }
        return count;
    }
    case 41: {
        u32 count = 0;
        for (u32 i = 0; i < 64; i++) {
            if (mUnk98[i].id != 0 && !mUnk98[i].active && mUnk98[i].unk10_0 == 0 && mUnk98[i].type == 1) {
                count++;
            }
        }
        return count;
    }
    case 12: {
        u32 count = 0;
        for (u32 i = 0; i < 64; i++) {
            if (mUnk98[i].id != 0 && mUnk98[i].active && mUnk98[i].type == 2) {
                count++;
            }
        }
        return count;
    }
    case 27: {
        u32 count = 0;
        for (u32 id = 1; id < gDinoResource->GetCreatureCount(); id++) {
            if (mCreatures[id].parts[0].level != 0) {
                count++;
            }
        }
        return count;
    }
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38: {
        u32 count = 0;
        DinoTeam team;
        GetTeam(gDinoBackup->GetUnk34_26(), &team);
        for (u32 i = 0; i < 5; i++) {
            if (team.slots[i] != 0 && index - 32 == gDinoResource->GetCreatureDef(team.slots[i])->unk0D) {
                count++;
            }
        }
        return count;
    }
    case 40: {
        u32 count = 0;
        DinoTeam team;
        GetTeam(gDinoBackup->GetUnk34_26(), &team);
        for (u32 i = 0; i < 5; i++) {
            if (team.slots[i] != 0) {
                count++;
            }
        }
        return count;
    }
    case 42:
        return CountUsedUnk14C();
    case 44:
        return mUnk1B4;
    case 45:
        return mUnk1B8;
    case 46:
        return mPlayer->unk30_0;
    }
    return 0;
}

void DinoBackup::SetSystemFlag(u32 id, BOOL value) {
    switch (id) {
    default:
        (*mSystemFlags)[id] = value;
        break;
    case 0x18:
        gLcd->SetSwapped(value);
        break;
    case 0x2f:
        mPlayer->unk38_20 = value;
        break;
    }
}

static inline u32 GetCreatureTotal(const DinoCreatureState *creature) {
    return creature->unk0_8 + (creature->parts[0].level + creature->parts[1].level + creature->parts[2].level +
                               creature->parts[3].level);
}

BOOL DinoBackup::GetSystemFlag(u32 id) {
    switch (id) {
    default:
        return mSystemFlags->Test(id);
    case 3:
        return IsUnk98Empty();
    case 4:
        return IsUnk98Full();
    case 0x18:
        return gLcd->IsSwapped();
    case 0x23:
        for (u32 i = 1; i <= 100; i++) {
            if (mCreatures[i].parts[0].level == 0) {
                return FALSE;
            }
        }
        return TRUE;
    case 0x24: {
        for (u32 i = 1; i <= 100; i++) {
            u32 total;
            DinoCreatureState *creature = &mCreatures[i];
            u32 p0 = creature->parts[0].level;
            if (p0 == 0) {
                total = 0;
            } else {
                u32 sum = creature->unk0_8 + (p0 + creature->parts[1].level + creature->parts[2].level +
                                              creature->parts[3].level);
                total = std::min(kCreatureLevelLimits[0] * 11 + 50, sum);
            }
            if (total < 600) {
                return FALSE;
            }
        }
        return TRUE;
    }
    case 0x29:
        if (IsUnk148Empty() && GetPlayFrames() - GetUnk160() <= kBackupRecentFrames) {
            return TRUE;
        }
        return FALSE;
    case 0x2a:
        for (u32 i = 0; i < 64; i++) {
            DinoBackupEntry *entry = &mUnk98[i];
            if (entry->id != 0 && entry->unk10_0 != 1 &&
                (entry->unk10_3 || ((DinoKasekiRock *) &mKasekiRocks[entry->id])->found)) {
                return FALSE;
            }
        }
        return TRUE;
    case 0x2b:
        for (u32 i = 0; i < 42; i++) {
            if (mRecords[i].unk78) {
                return FALSE;
            }
        }
        return TRUE;
    case 0x2f:
        if (mPlayer->unk38_20) {
            return TRUE;
        }
        return FALSE;
    }
}

void DinoBackup::SetPlayerUnk58(const DinoPlayerUnk58 *value) {
    *mPlayer->GetUnk58() = *value;
}

void DinoBackup::GetPlayerUnk58(DinoPlayerUnk58 *value) {
    *value = *mPlayer->GetUnk58();
}

void DinoBackup::MakeProfile(DinoPlayerProfile *profile) {
    InitProfile(profile);
    profile->unk7A = 0;
    for (u32 i = 0; i < 8; i++) {
        profile->name[i] = mPlayer->name[i];
    }
}
