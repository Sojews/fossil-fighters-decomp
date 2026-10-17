// DinoBackup: the save data. It holds the game's state (flags, variables, the player, creatures, ...) in
// serializers, and reads and writes it to the backup memory through a file table.
#ifndef GAME_DINO_BACKUP_H
#define GAME_DINO_BACKUP_H

#include <string>
#include "m/looper.h"
#include "m/bit_vector.h"
#include "nitro/os.h"

class MBackupDevice;
class MBackupStorage;
class MSerializer;

// A variable named by an event script: its kind, then its index.
struct DinoEventVar {
    u32 type;
    u32 index;
};

// The player and settings, packed in bits.
struct DinoExcavateLevel;

struct DinoPlayerData {
    u16 name[9];                // 0x00
    // 0x14
    u32 unk14_0 : 6;            // system var 4
    u32 rank : 4;               // system var 7
    u32 horiderLevel : 4;       // system var 1
    u32 unk14_14 : 2;           // system var 6
    u32 license : 2;            // system var 5
    u32 unk14_18 : 14;
    s32 money : 32;             // 0x18: system var 3
    // 0x1C
    u32 containerNum : 4;       // system var 2
    u32 radarMode : 2;
    u32 townMapMode : 1;
    u32 unk1c_7 : 16;
    u32 unk1c_23 : 8;           // system var 14
    u32 unk1c_31 : 1;
    // 0x20
    u32 unk20_0 : 8;            // system var 15
    u32 unk20_8 : 8;            // system var 16
    u32 unk20_16 : 8;           // system var 28
    u32 unk20_24 : 8;
    s32 unk24 : 32;             // 0x24: system var 29
    // 0x28
    u32 unk28_0 : 8;            // system var 17
    u32 unk28_8 : 8;            // system var 18
    u32 unk28_16 : 16;
    // 0x2C
    u32 unk2c_0 : 17;           // system var 19
    u32 unk2c_17 : 15;
    // 0x30
    u32 unk30_0 : 17;           // system var 46
    u32 unk30_17 : 5;           // system var 39
    u32 unk30_22 : 10;
    // 0x34
    u32 unk34_0 : 16;           // system var 25
    u32 unk34_16 : 2;
    u32 volume : 4;
    u32 targetVolume : 4;
    u32 unk34_26 : 2;
    u32 unk34_28 : 2;           // system var 30
    u32 unk34_30 : 2;           // system var 31
    // 0x38
    u32 unk38_0 : 2;            // system var 32
    u32 unk38_2 : 2;
    u32 unk38_4 : 16;           // system var 43
    u32 unk38_20 : 1;           // system flag 0x2F
    u32 unk38_21 : 11;
    u32 unk3c[4];               // 0x3C
    struct {
        u32 a;
        u32 b;
        u32 c;
    } unk4c;                    // 0x4C
    u32 unk58;                  // 0x58
    u32 unk5c;                  // 0x5C
    u32 unk60;                  // 0x60
    u32 unk64;                  // 0x64
    u32 unk68;                  // 0x68
};

// What the file select screen shows of a save.
struct DinoSaveSummary {
    u16 name[9];        // 0x00
    // 0x14
    u32 valid : 1;
    u32 rank : 4;
    u32 horiderLevel : 4;
    u32 unk9 : 16;
    u32 unk14_14 : 2;
    u32 flag19 : 1;
    u32 flag1a : 1;
    u32 flag1b : 1;
    u32 flag2f : 1;
    u32 unk31 : 1;
    u64 playFrames;     // 0x18
};

struct DinoCharaState {
    DinoCharaState() {}
    u8 unk0;    // 0x00
    u32 bodyModel : 8;  // 0x04
    u32 headModel : 8;
    u32 unk4_16 : 16;
    u32 unk8;   // 0x08
    u32 unkC;   // 0x0C
    u32 unk10;  // 0x10
};

struct DinoBackupEntry {
    DinoBackupEntry() {}
    u32 unk0;               // 0x00
    u32 unk4;               // 0x04
    u32 unk8;               // 0x08
    // 0x0C
    u32 id : 10;
    u32 type : 2;
    u32 unkC_12 : 1;
    u32 active : 1;
    u32 unkC_14 : 18;
    // 0x10
    u32 unk10_0 : 3;
    u32 unk10_3 : 1;
    u32 unk10_4 : 28;
};

struct DinoCharaWatch {
    u32 w[5];
};

// A character's saved task.
struct DinoCharaTask {
    u32 id;             // 0x00: 0 = free
    u8 unk4;            // 0x04: 0 = none, 1 = follow, 2 = wander, 3 = patrol
    union {
        u32 w[8];
        struct {
            u16 target;
        } fellow;
        struct {
            u32 w[4];
        } wander;
        struct {
            u32 w[3];
            u32 count;
            u16 points[8];
        } patrol;
    } unk8;             // 0x08
    struct {
        u8 active;      // watching
        u8 pad[3];
        u32 w[5];
    } unk28;            // 0x28
};

struct DinoBackupRecord {
    u8 data[0x65];      // 0x00
    u16 name[9];        // 0x66
    u8 unk78;           // 0x78
    u8 unk79;           // 0x79
    u8 unk7a;           // 0x7A
    u64 time;           // 0x7C
};

// A team: a name and up to five creatures.
struct DinoTeam {
    u16 name[17];       // 0x00
    u8 slots[5];        // 0x22
};

struct DinoCreaturePart {
    u32 level : 12;
    u32 rest : 20;
};

struct DinoCreatureState {
    // 0x00
    u32 index : 8;
    u32 unk0_8 : 10;
    u32 unk0_18 : 14;
    DinoCreaturePart parts[4];  // 0x04
    u32 unk14;                  // 0x14
};

struct DinoKasekiRock {
    u32 unk0 : 10;
    u32 found : 1;
    u32 unk11 : 21;
};

// Where the entry tables' new entries come from.
struct DinoEntrySource {
    u32 type;   // 0: a given id, 1: a found kaseki rock at random, 2: any rock at random
    u32 id;
};

struct DinoBackupEntry28 {
    DinoBackupEntry28() {}
    u8 used;            // 0x00
    u8 unk1;            // 0x01
    struct {
        u32 a;
        u32 b;
    } unk4;             // 0x04
    u32 unkC;           // 0x0C
    u32 unk10;          // 0x10
    u32 unk14;          // 0x14
    u32 unk18;          // 0x18
    u32 unk1C;          // 0x1C
    // 0x20
    u32 id : 10;
    u32 unk20_10 : 22;
    // 0x24
    u32 unk24_0 : 4;
    u32 flag : 1;
    u32 unk24_5 : 27;
};

class DinoBackup : public MLooperVSyncHook {
public:
    static void Create(u32 extraSize, BOOL useBackup);

    DinoBackup(u32 extraSize, BOOL useBackup);
    virtual ~DinoBackup() {}
    virtual void OnVSync();

    BOOL IsBackupPresent();
    u8 TakeLoadFailed();
    BOOL Read(u32 file, u32 size, void *buf);
    BOOL Write(u32 file, u32 size, const void *buf);
    u32 GetSlot();
    void SetSlot(u32 slot);
    void SetSummary(u32 slot, const DinoSaveSummary *summary);
    void GetSummary(u32 slot, DinoSaveSummary *summary);
    void MakeSummary(DinoSaveSummary *summary);
    void SetUnk180(s32 value);
    s32 GetUnk180();
    void ApplyVolume();
    void Reset(BOOL applyVolume);
    void WriteHeader();
    void WriteSlotRaw();
    void Save();
    void Load();
    u8 IsLoaded();
    u8 *GetUnk140();
    u64 GetPlayFrames();
    u64 GetUnk160();
    void MarkUnk160();
    u64 GetUnk168();
    void MarkUnk168();
    void SetUnk34_16(u32 value);
    u32 GetUnk34_16();
    void SetVolume(u32 volume);
    u32 GetVolume();
    void SetTargetVolume(u32 volume);
    u32 GetTargetVolume();
    void SetUnk34_26(u32 value);
    u32 GetUnk34_26();
    void SetSystemFlag(u32 id, BOOL value);
    BOOL GetSystemFlag(u32 id);
    void SetFlag(u32 id, BOOL value);
    BOOL GetFlag(u32 id);
    void SetKeyItem(u32 id, BOOL value);
    BOOL GetKeyItem(u32 id);
    void SetHeadMask(u32 id, BOOL value);
    BOOL GetHeadMask(u32 id);
    void SetLocalVar(u32 index, s32 value);
    s32 GetLocalVar(u32 index);
    void SetSystemVar(u32 index, s32 value);
    s32 GetSystemVar(u32 index);
    void SetVar(DinoEventVar var, s32 value);
    s32 GetVar(DinoEventVar var);
    void SetVarFlag(DinoEventVar var, BOOL value);
    BOOL GetVarFlag(DinoEventVar var);
    BOOL GetVarClear(DinoEventVar var);
    void SetEventClear(u32 id, BOOL value);
    BOOL GetEventClear(u32 id);
    void SetEpisodeClear(u32 id, BOOL value);
    BOOL GetEpisodeClear(u32 id);
    void SetEpisodeSeen(u32 id, BOOL value);
    BOOL GetEpisodeSeen(u32 id);
    void ConnectRegion(u32 from, u32 to);
    void DisconnectRegion(u32 from, u32 to);
    u16 GetRegionLink(u32 from);
    void GetLinkedRegions(MBitVector *out);
    void SetPlayer(const DinoPlayerData *player);
    void GetPlayer(DinoPlayerData *player, BOOL shortName);
    void SetChara(u32 index, const DinoCharaState *chara);
    void GetChara(u32 index, DinoCharaState *chara);
    void SetCharaTask(u32 id, const DinoCharaTask *task);
    void GetCharaTask(u32 id, DinoCharaTask *task);
    void SetUnk98(u32 index, const DinoBackupEntry *entry);
    void GetUnk98(u32 index, DinoBackupEntry *entry);
    void SetKasekiRock(u32 index, const u32 *rock);
    void GetKasekiRock(u32 index, u32 *rock);
    void SetRecord(u32 index, const DinoBackupRecord *record);
    void GetRecord(u32 index, DinoBackupRecord *record);
    void SetTeam(u32 index, const DinoTeam *team);
    void GetTeam(u32 index, DinoTeam *team);
    void FillTeam();
    void SetCreature(u32 index, const DinoCreatureState *creature);
    void GetCreature(u32 index, DinoCreatureState *creature);
    u32 CountCreatureParts();
    void SetUnk134(u32 index, const DinoBackupEntry *entry);
    void GetUnk134(u32 index, DinoBackupEntry *entry);
    void SetUnk144(s32 value);
    s32 GetUnk144();
    void SetUnk148(u32 index, const DinoBackupEntry *entry);
    void GetUnk148(u32 index, DinoBackupEntry *entry);
    void ClearUnk148();
    BOOL IsUnk148Empty();
    BOOL IsUnk148Full();
    s32 FindFreeUnk148();
    void CompactUnk148();
    void AddUnk148(const DinoEntrySource *source);
    void SetUnk14C(u32 index, const DinoBackupEntry *entry);
    void GetUnk14C(u32 index, DinoBackupEntry *entry);
    u32 CountUsedUnk14C();
    u32 CountFreeUnk14C();
    BOOL IsUnk14CEmpty();
    BOOL IsUnk14CFull();
    s32 FindFreeUnk14C();
    void CompactUnk14C();
    void ClearEntries(DinoBackupEntry *entries, u32 count);
    BOOL AreEntriesEmpty(DinoBackupEntry *entries, u32 count);
    BOOL AreEntriesFull(DinoBackupEntry *entries, u32 count);
    s32 FindFreeEntry(DinoBackupEntry *entries, u32 count);
    void CompactEntries(DinoBackupEntry *entries, u32 count);
    u32 GetUnk98Capacity();
    u32 CountUsedUnk98();
    u32 CountFreeUnk98();
    BOOL IsUnk98Empty();
    BOOL IsUnk98Full();
    s32 FindFreeUnk98();
    void CompactUnk98();
    std::string GetEntryName(const DinoBackupEntry *entry);
    u32 CountUnk9C();
    void SetUnk9C(u32 index, const DinoBackupEntry *entry);
    void GetUnk9C(u32 index, DinoBackupEntry *entry);
    BOOL IsUnk9CEmpty();
    s32 FindFreeUnk9C();
    BOOL IsEntryUnrevealed(const DinoBackupEntry *entry);
    BOOL HasInactiveUnk98();
    BOOL HasUnrevealedUnk98();
    const DinoExcavateLevel *GetExcavateLevel();
    u32 GetExcavateLevelUnk0();
    s32 GetExcavateLevelAverage();
    s32 GetExcavateLevelRemaining();
    u32 GetMaxExcavateLevelUnk0();
    void SetUnk138(u32 index, const DinoBackupEntry28 *entry);
    void GetUnk138(u32 index, DinoBackupEntry28 *entry);
    BOOL CanAddUnk138(const DinoBackupEntry *entry, BOOL checkDuplicate, BOOL checkCount, u32 maxCount);

    MBackupDevice *mDevice;         // 0x00C
    MBackupStorage *mStorage;       // 0x010
    MSerializer *mSerializer;       // 0x014: the game data
    u8 mLoaded;                     // 0x018
    u8 unk19[0x64 - 0x19];
    MBitVector *mSystemFlags;       // 0x064
    MBitVector *mFlags;             // 0x068
    MBitVector *mKeyItems;          // 0x06C
    MBitVector *mHeadMasks;         // 0x070
    MBitVector *mEvents;            // 0x074: cleared events
    MBitVector *mEpisodes;          // 0x078: cleared episodes
    MBitVector *mEpisodesSeen;      // 0x07C
    s32 *mVars;                     // 0x080
    u16 *mRegionLinks;              // 0x084
    u32 unk88;
    DinoPlayerData *mPlayer;        // 0x08C
    DinoCharaState *mCharas;        // 0x090
    DinoCharaTask *mCharaTasks;     // 0x094
    DinoBackupEntry *mUnk98;        // 0x098
    DinoBackupEntry *mUnk9C;        // 0x09C
    u32 *mKasekiRocks;              // 0x0A0
    u8 mUnkA4[0x84];                // 0x0A4
    DinoBackupRecord *mRecords;     // 0x128
    DinoTeam *mTeams;               // 0x12C
    DinoCreatureState *mCreatures;  // 0x130
    DinoBackupEntry *mUnk134;       // 0x134
    DinoBackupEntry28 *mUnk138;     // 0x138
    u32 mUnk140Size;                // 0x13C
    u8 *mUnk140;                    // 0x140
    s32 mUnk144;                    // 0x144
    DinoBackupEntry *mUnk148;       // 0x148
    DinoBackupEntry *mUnk14C;       // 0x14C
    u8 *mUnk150;                    // 0x150
    u64 mPlayFrames;                // 0x154
    s32 mFrames;                    // 0x15C
    u64 mUnk160;                    // 0x160
    u64 mUnk168;                    // 0x168
    u8 *mUnk170;                    // 0x170
    MSerializer *mHeaderSerializer; // 0x174
    u32 mSlot;                      // 0x178
    DinoSaveSummary *mSummaries;    // 0x17C
    s32 mUnk180;                    // 0x180
    u32 mHeaderSize;                // 0x184
    u32 mDataSize;                  // 0x188
    u8 mEnabled;                    // 0x18C
    s32 mUnk190[3];                 // 0x190
    s32 mUnk19C[3];                 // 0x19C
    s32 mUnk1A8[3];                 // 0x1A8
    u32 mUnk1B4;                    // 0x1B4: system var 44
    u32 mUnk1B8;                    // 0x1B8: system var 45
    u8 mLoadFailed;                 // 0x1BC
};

extern DinoBackup *gDinoBackup;

#endif
