// DinoBackup: the save data. It holds the game's state (flags, variables, the player, creatures, ...) in
// serializers, and reads and writes it to the backup memory through a file table.
#ifndef GAME_DINO_BACKUP_H
#define GAME_DINO_BACKUP_H

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
struct DinoPlayerData {
    u16 name[9];                // 0x00
    u8 unk12[2];
    // 0x14
    u32 unk14_0 : 6;            // system var 4
    u32 rank : 4;               // system var 7
    u32 horiderLevel : 4;       // system var 1
    u32 unk14_14 : 2;           // system var 6
    u32 license : 2;            // system var 5
    u32 unk14_18 : 14;
    s32 money;                  // 0x18: system var 3
    // 0x1C
    u32 containerNum : 4;       // system var 2
    u32 unk1c_4 : 2;
    u32 unk1c_6 : 1;
    u32 unk1c_7 : 16;
    u32 unk1c_23 : 8;           // system var 14
    u32 unk1c_31 : 1;
    // 0x20
    u32 unk20_0 : 8;            // system var 15
    u32 unk20_8 : 8;            // system var 16
    u32 unk20_16 : 8;           // system var 28
    u32 unk20_24 : 8;
    s32 unk24;                  // 0x24: system var 29
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
    u8 unk3c[0x6C - 0x3C];
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
    u8 data[0x14];
};

struct DinoBackupEntry {
    DinoBackupEntry() {}
    u8 data[0x14];
};

struct DinoBackupEntry28 {
    DinoBackupEntry28() {}
    u8 data[0x28];
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
    u8 *mUnk94;                     // 0x094
    DinoBackupEntry *mUnk98;        // 0x098
    DinoBackupEntry *mUnk9C;        // 0x09C
    u32 *mKasekiRocks;              // 0x0A0
    u8 mUnkA4[0x84];                // 0x0A4
    u8 *mUnk128;                    // 0x128
    u8 *mUnk12C;                    // 0x12C
    u8 *mCreatures;                 // 0x130
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
