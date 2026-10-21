// DinoResource, the game's resources. Only a few functions are decompiled so far: the file's statics (the data
// file paths, pooled together, and caches made at startup) aren't all here, so code that loads them can't match.

#define MSL_STRING_BASICS
#pragma ipa file
#include "game/dino_resource.h"
#include "m/seed_generator.h"
#include "nitro/fs.h"
#include "nitro/mi.h"

// Reads records from a file through a 256-byte buffer.
class DinoFileCache {
public:
    DinoFileCache(const char *path) {
        mOpened = FALSE;
        mPath = path;
    }

    void Read(u32 offset, void *dest, u32 size);

    u8 mOpened;         // 0x00
    const char *mPath;  // 0x04
    FSFileID mId;       // 0x08
    u32 mBlock;         // 0x10
    u8 mBuffer[0x100];  // 0x14
};

static inline u16 Rgb(int r, int g, int b) {
    return 0x8000 | (b << 10) | (g << 5) | r;
}

// The file's path statics, in its size-sorted .data pool (two anchors: the game loads each through the one in reach).
extern "C" struct {
    const char *mUnk00;                      // 0x00
    const char *mFont16;                     // 0x04: "/font/font16"
    const char *mEtcTalkMsgMatch;            // 0x08: "/etc/talk_msg_match"
    const char *mEpisodeDir;                 // 0x0C: "/episode/%04d"
    const char *mEtcMapExcavateMatch;        // 0x10: "/etc/map_excavate_match"
    const char *mEtcRegionMapMatch;          // 0x14: "/etc/region_map_match"
    const char *mEventMaxId;                 // 0x18: "/etc/event_maxid"
    const char *mEtcRegionCenterMatch;       // 0x1C: "/etc/region_center_match"
    const char *mEpisode;                    // 0x20: "/episode/e%04d"
    const char *mCharaMaxId;                 // 0x24: "/etc/chara_maxid"
    const char *mFontFont10;                 // 0x28: "/font/font10"
    const char *mShop;                       // 0x2C: "/shop/%04d"
    const char *mFontFont24f;                // 0x30: "/font/font24f"
    const char *mEtcMapSceneMatch;           // 0x34: "/etc/map_scene_match"
    const char *mMessage;                    // 0x38: "/msg/msg_%04d"
    const char *mBattleMaxId;                // 0x3C: "/etc/battle_maxid"
    const char *mSenryuDefs;                 // 0x40: "/etc/senryu_defs"
    const char *mChara;                      // 0x44: "/chara/%04d"
    const char *mKasekiDefs;                 // 0x48: "/etc/kaseki_defs"
    const char *mKasekiRockMaxId;            // 0x4C: "/etc/kasekirock_maxid"
    const char *mDance;                      // 0x50: "/dance/%04d"
    const char *mMapG;                       // 0x54: "/map/g/%04d"
    const char *mEpisodeMaxId;               // 0x58: "/etc/episode_maxid"
    const char *mFont24;                     // 0x5C: "/font/font24"
    const char *mMapE;                       // 0x60: "/map/e/%04d"
    const char *mMapMaxId;                   // 0x64: "/etc/map_maxid"
    const char *mMapM;                       // 0x68: "/map/m/%04d"
    const char *mBattleCreatureMotion;       // 0x6C: "/motion/battle_creature/creature_%03d.bin"
    const char *mCreatureDefs;               // 0x70: "/etc/creature_defs"
    const char *mFlagMaxId;                  // 0x74: "/etc/flag_maxid"
    const char *mMapC;                       // 0x78: "/map/c/%04d"
    const char *mMuseumDefs;                 // 0x7C: "/etc/museum_defs"
} data_020b9fc4;
extern "C" struct {
    const char *mExcavateDefs;               // 0x00: "/etc/excavate_defs"
    const char *mKeyItemMaxId;               // 0x04: "/etc/keyitem_maxid"
    const char *mBattle;                     // 0x08: "/battle/%04d"
    const char *mBattleAdjustDefs;           // 0x0C: "/etc/btl_adjust_defs"
    const char *mBattleBgDefs;               // 0x10: "/etc/btl_bg_defs"
    const char *mText;                       // 0x14: "/text/japanese"
    const char *mFont12;                     // 0x18: "/font/font12"
    const char *mEtcMapBgmMatch;             // 0x1C: "/etc/map_bgm_match"
    const char *mRegionMaxId;                // 0x20: "/etc/region_maxid"
    const char *mVarMaxId;                   // 0x24: "/etc/var_maxid"
    const char *mHeadMaskMaxId;              // 0x28: "/etc/headmask_maxid"
    const char *mHeadMaskDefs;               // 0x2C: "/etc/headmask_defs"
    const char *mMapR;                       // 0x30: "/map/r/%04d"
    const char *mAttackDefs;                 // 0x34: "/etc/attack_defs"
    const char *mFontFont8;                  // 0x38: "/font/font8"
    const char *mKeyItemDefs;                // 0x3C: "/etc/keyitem_defs"
    const char *mEtcBattleAttackType;        // 0x40: "/etc/battle_attack_type"
} data_020ba044;
#define sUnk00Path (data_020b9fc4.mUnk00)
#define sFont16Path (data_020b9fc4.mFont16)
#define sEtcTalkMsgMatchPath (data_020b9fc4.mEtcTalkMsgMatch)
#define sEpisodeDirPath (data_020b9fc4.mEpisodeDir)
#define sEtcMapExcavateMatchPath (data_020b9fc4.mEtcMapExcavateMatch)
#define sEtcRegionMapMatchPath (data_020b9fc4.mEtcRegionMapMatch)
#define sEventMaxIdPath (data_020b9fc4.mEventMaxId)
#define sEtcRegionCenterMatchPath (data_020b9fc4.mEtcRegionCenterMatch)
#define sEpisodePath (data_020b9fc4.mEpisode)
#define sCharaMaxIdPath (data_020b9fc4.mCharaMaxId)
#define sFontFont10Path (data_020b9fc4.mFontFont10)
#define sShopPath (data_020b9fc4.mShop)
#define sFontFont24fPath (data_020b9fc4.mFontFont24f)
#define sEtcMapSceneMatchPath (data_020b9fc4.mEtcMapSceneMatch)
#define sMessagePath (data_020b9fc4.mMessage)
#define sBattleMaxIdPath (data_020b9fc4.mBattleMaxId)
#define sSenryuDefsPath (data_020b9fc4.mSenryuDefs)
#define sCharaPath (data_020b9fc4.mChara)
#define sKasekiDefsPath (data_020b9fc4.mKasekiDefs)
#define sKasekiRockMaxIdPath (data_020b9fc4.mKasekiRockMaxId)
#define sDancePath (data_020b9fc4.mDance)
#define sMapGPath (data_020b9fc4.mMapG)
#define sEpisodeMaxIdPath (data_020b9fc4.mEpisodeMaxId)
#define sFont24Path (data_020b9fc4.mFont24)
#define sMapEPath (data_020b9fc4.mMapE)
#define sMapMaxIdPath (data_020b9fc4.mMapMaxId)
#define sMapMPath (data_020b9fc4.mMapM)
#define sBattleCreatureMotionPath (data_020b9fc4.mBattleCreatureMotion)
#define sCreatureDefsPath (data_020b9fc4.mCreatureDefs)
#define sFlagMaxIdPath (data_020b9fc4.mFlagMaxId)
#define sMapCPath (data_020b9fc4.mMapC)
#define sMuseumDefsPath (data_020b9fc4.mMuseumDefs)
#define sExcavateDefsPath (data_020ba044.mExcavateDefs)
#define sKeyItemMaxIdPath (data_020ba044.mKeyItemMaxId)
#define sBattlePath (data_020ba044.mBattle)
#define sBattleAdjustDefsPath (data_020ba044.mBattleAdjustDefs)
#define sBattleBgDefsPath (data_020ba044.mBattleBgDefs)
#define sTextPath (data_020ba044.mText)
#define sFont12Path (data_020ba044.mFont12)
#define sEtcMapBgmMatchPath (data_020ba044.mEtcMapBgmMatch)
#define sRegionMaxIdPath (data_020ba044.mRegionMaxId)
#define sVarMaxIdPath (data_020ba044.mVarMaxId)
#define sHeadMaskMaxIdPath (data_020ba044.mHeadMaskMaxId)
#define sHeadMaskDefsPath (data_020ba044.mHeadMaskDefs)
#define sMapRPath (data_020ba044.mMapR)
#define sAttackDefsPath (data_020ba044.mAttackDefs)
#define sFontFont8Path (data_020ba044.mFontFont8)
#define sKeyItemDefsPath (data_020ba044.mKeyItemDefs)
#define sEtcBattleAttackTypePath (data_020ba044.mEtcBattleAttackType)

// The order here gives the game's layout (see tools/static_order.py).
// NONMATCHING (__sinit): the game constructs the table, the mutex, then the caches region map, region center, talk,
// excavate, bgm, scene; declaring them in that order gives that code but breaks the layout the readers need.
// static_order: begin
DinoResource *gDinoResource;
static MMutex sMutex;
static DinoFileCache sTalkMessageMatch(sEtcTalkMsgMatchPath);
static DinoFileCache sMapSceneMatch(sEtcMapSceneMatchPath);
static DinoFileCache sMapBgmMatch(sEtcMapBgmMatchPath);
static DinoFileCache sRegionCenterMatch(sEtcRegionCenterMatchPath);
static DinoFileCache sMapExcavateMatch(sEtcMapExcavateMatchPath);
static DinoFileCache sRegionMapMatch(sEtcRegionMapMatchPath);
static const u16 sUnkTable[14] = {0, Rgb(0, 0, 0), Rgb(0, 18, 0), Rgb(30, 2, 2), Rgb(22, 11, 0), Rgb(9, 15, 27), Rgb(31, 12, 25), Rgb(26, 0, 12), Rgb(10, 15, 23), Rgb(31, 18, 0), Rgb(31, 31, 31), Rgb(16, 16, 16), Rgb(31, 18, 20), 0};
// static_order: end

inline void DinoFileCache::Read(u32 offset, void *dest, u32 size) {
    if (!mOpened) {
        FS_ConvertPathToFileID(&mId, mPath);
        mBlock = -1;
        mOpened = TRUE;
    }
    MScopedLock lock(&sMutex.mutex, TRUE);
    if (offset >> 8 != mBlock) {
        mBlock = offset >> 8;
        FSFile file;
        FS_InitFile(&file);
        FS_OpenFileFast(&file, mId);
        FS_SeekFile(&file, mBlock << 8, FS_SEEK_SET);
        FS_ReadFile(&file, mBuffer, 0x100);
        FS_CloseFile(&file);
    }
    MI_CpuCopy8(mBuffer + (offset & 0xFF), dest, size);
}


extern "C" const u32 sLanguageTextId;  // 1 (the game reads it from .rodata)

// Text IDs defined elsewhere.
extern const u32 gKasekiRockNameText;
extern const u32 gKasekiRockSizeText;
extern const u32 gKasekiRockNameSuffixText;



void DinoResource::Create() {
    gDinoResource = new DinoResource();
}

// (Out of line, unlike the rest of MArchivePtr: every caller calls this copy.)
MArchiveFile MArchivePtr::GetFile(u32 index) const {
    return MArchiveFile(mArchive, index);
}

// NONMATCHING: see above.
DinoResource::DinoResource() {
    mRandom = new MRandom();
    mText = MResource<DinoTextData>(sTextPath);
    MSetDefaultEncoding(2);
    MSetUnkText(GetText(sLanguageTextId));
    mCreatureDefs = MResource<DinoCreatureDefs>(sCreatureDefsPath);
    mAttackDefs = MResource<DinoAttackDefs>(sAttackDefsPath);
    mBattleAdjustDefs = MResource<DinoBattleAdjustDefs>(sBattleAdjustDefsPath);
    mBattleBgDefs = MResource<DinoBattleBgDefs>(sBattleBgDefsPath);
    mFlagCount = MResource<DinoMaxId>(sFlagMaxIdPath).Get()->count;
    mKeyItemCount = MResource<DinoMaxId>(sKeyItemMaxIdPath).Get()->count;
    mHeadMaskCount = MResource<DinoMaxId>(sHeadMaskMaxIdPath).Get()->count;
    mVarCount = MResource<DinoMaxId>(sVarMaxIdPath).Get()->count;
    mEpisodeCount = MResource<DinoMaxId>(sEpisodeMaxIdPath).Get()->count;
    mEventCount = MResource<DinoMaxId>(sEventMaxIdPath).Get()->count;
    mMapCount = MResource<DinoMaxId>(sMapMaxIdPath).Get()->count;
    mRegionCount = MResource<DinoMaxId>(sRegionMaxIdPath).Get()->count;
    mCharaCount = MResource<DinoMaxId>(sCharaMaxIdPath).Get()->count;
    mKasekiRockCount = MResource<DinoMaxId>(sKasekiRockMaxIdPath).Get()->count;
    mBattleCount = MResource<DinoMaxId>(sBattleMaxIdPath).Get()->count;
    mExcavateDefs = MResource<DinoExcavateDefs>(sExcavateDefsPath);
}

MRandom *DinoResource::GetRandom() {
    return mRandom;
}

const char *DinoResource::GetText(u32 id) {
    const DinoTextData *data = mText.Get();
    return (const char *)data + ((const u32 *)((const u8 *)data + data->tableOffset))[id];
}

const u16 *DinoResource::GetUnkTable() {
    return sUnkTable;
}

u32 DinoResource::GetCreatureCount() {
    return mCreatureDefs.Get()->count;
}

const DinoCreatureDef *DinoResource::GetCreatureDef(u32 id) {
    const DinoCreatureDefs *defs = mCreatureDefs.Get();
    return (const DinoCreatureDef *)((const u8 *)defs + ((const u32 *)((const u8 *)defs + defs->tableOffset))[id]);
}

const DinoAttackDef *DinoResource::GetAttackDef(u32 id) {
    const DinoAttackDefs *defs = mAttackDefs.Get();
    return (const DinoAttackDef *)((const u8 *)defs + ((const u32 *)((const u8 *)defs + defs->tableOffset))[id]);
}

std::string DinoResource::GetBattleCreatureMotionPath(int id) {
    char path[256];
    OS_SPrintf(path, sBattleCreatureMotionPath, id);
    return std::string(path);
}

const DinoBattleAdjustDefs *DinoResource::GetBattleAdjustDefs() {
    return mBattleAdjustDefs.Get();
}

const DinoBattleBgDefs *DinoResource::GetBattleBgDefs() {
    return mBattleBgDefs.Get();
}

const char *DinoResource::GetAttackName(u32 id) {
    return GetText(id + mAttackDefs.Get()->nameText);
}

const char *DinoResource::GetCreatureText1C(u32 id) {
    return GetText(id + mCreatureDefs.Get()->text1C);
}

const char *DinoResource::GetCreatureName(u32 id) {
    return GetText(id + mCreatureDefs.Get()->nameText);
}

const char *DinoResource::GetCreatureText08(u32 id) {
    return GetText(id + mCreatureDefs.Get()->text08);
}

const char *DinoResource::GetCreatureText24(u32 id) {
    return GetText(id + mCreatureDefs.Get()->text24);
}

const char *DinoResource::GetCreatureText20(u32 id) {
    return GetText(id + mCreatureDefs.Get()->text20);
}

const char *DinoResource::GetCreatureText0C(u32 id) {
    return GetText(id + mCreatureDefs.Get()->text0C);
}

const char *DinoResource::GetAttackText08(u32 id) {
    return GetText(id + mAttackDefs.Get()->text08);
}

u32 DinoResource::GetFlagCount() {
    return mFlagCount;
}

u32 DinoResource::GetKeyItemCount() {
    return mKeyItemCount;
}

u32 DinoResource::GetHeadMaskCount() {
    return mHeadMaskCount;
}

u32 DinoResource::GetVarCount() {
    return mVarCount;
}

u32 DinoResource::GetEpisodeCount() {
    return mEpisodeCount;
}

u32 DinoResource::GetEventCount() {
    return mEventCount;
}

u32 DinoResource::GetMapCount() {
    return mMapCount;
}

u32 DinoResource::GetRegionCount() {
    return mRegionCount;
}

u32 DinoResource::GetCharaCount() {
    return mCharaCount;
}

u32 DinoResource::GetKasekiRockCount() {
    return mKasekiRockCount;
}

u32 DinoResource::GetBattleCount() {
    return mBattleCount;
}

MFontSet *DinoResource::CreateFont24(int color, int shadow, int back) {
    return new MFontSet(sFont24Path, color, shadow, back);
}

MFontSet *DinoResource::CreateFont16(int color, int shadow, int back) {
    return new MFontSet(sFont16Path, color, shadow, back);
}

MFontSet *DinoResource::CreateFont12(int color, int shadow, int back) {
    return new MFontSet(sFont12Path, color, shadow, back);
}

MResource<DinoMapMData> DinoResource::LoadMapM(int id) {
    char path[256];
    OS_SPrintf(path, sMapMPath, id);
    return MResource<DinoMapMData>(path);
}

MResource<DinoMapData> DinoResource::LoadMapR(int id) {
    char path[256];
    OS_SPrintf(path, sMapRPath, id);
    return MResource<DinoMapData>(path);
}

MResource<DinoMapData> DinoResource::LoadMapC(int id) {
    char path[256];
    OS_SPrintf(path, sMapCPath, id);
    return MResource<DinoMapData>(path);
}

MResource<DinoMapData> DinoResource::LoadMapE(int id) {
    char path[256];
    OS_SPrintf(path, sMapEPath, id);
    return MResource<DinoMapData>(path);
}

MResource<DinoMapData> DinoResource::LoadMapG(int id) {
    char path[256];
    OS_SPrintf(path, sMapGPath, id);
    return MResource<DinoMapData>(path);
}

MResource<DinoBattleData> DinoResource::LoadBattle(int id) {
    char path[256];
    OS_SPrintf(path, sBattlePath, id);
    return MResource<DinoBattleData>(path);
}

MResource<DinoShopData> DinoResource::LoadShop(int id) {
    char path[256];
    OS_SPrintf(path, sShopPath, id);
    return MResource<DinoShopData>(path);
}

MResource<DinoDanceData> DinoResource::LoadDance(int id) {
    char path[256];
    OS_SPrintf(path, sDancePath, id);
    return MResource<DinoDanceData>(path);
}

MResource<DinoCharaData> DinoResource::LoadChara(int id) {
    char path[256];
    OS_SPrintf(path, sCharaPath, id);
    return MResource<DinoCharaData>(path);
}

MResource<DinoMessageData> DinoResource::LoadMessage(int id) {
    char path[256];
    OS_SPrintf(path, sMessagePath, id);
    return MResource<DinoMessageData>(path);
}

MResource<DinoEpisodeData> DinoResource::LoadEpisode(int id) {
    char path[256];
    OS_SPrintf(path, sEpisodePath, id);
    return MResource<DinoEpisodeData>(path);
}

MResource<DinoKeyItemDefs> DinoResource::LoadKeyItemDefs() {
    return MResource<DinoKeyItemDefs>(sKeyItemDefsPath);
}

MResource<DinoHeadMaskDefs> DinoResource::LoadHeadMaskDefs() {
    return MResource<DinoHeadMaskDefs>(sHeadMaskDefsPath);
}

MResource<DinoMuseumDefs> DinoResource::LoadMuseumDefs() {
    return MResource<DinoMuseumDefs>(sMuseumDefsPath);
}

MResource<DinoKasekiDefs> DinoResource::LoadKasekiDefs() {
    return MResource<DinoKasekiDefs>(sKasekiDefsPath);
}

MResource<DinoSenryuDefs> DinoResource::LoadSenryuDefs() {
    return MResource<DinoSenryuDefs>(sSenryuDefsPath);
}

MResource<DinoExcavateDefs> DinoResource::GetExcavateDefs() {
    return mExcavateDefs;
}

const void *DinoResource::GetExcavate78() {
    const DinoExcavateDefs *defs = mExcavateDefs.Get();
    if (defs->offset78 == 0) {
        return NULL;
    }
    return (const u8 *)defs + defs->offset78;
}

const void *DinoResource::GetExcavate7C() {
    const DinoExcavateDefs *defs = mExcavateDefs.Get();
    if (defs->offset7C == 0) {
        return NULL;
    }
    return (const u8 *)defs + defs->offset7C;
}

const void *DinoResource::GetExcavate80() {
    const DinoExcavateDefs *defs = mExcavateDefs.Get();
    if (defs->offset80 == 0) {
        return NULL;
    }
    return (const u8 *)defs + defs->offset80;
}

static const u32 sCreatureUnk0DValues[6] = {0, 2, 4, 5, 3, 6};

u32 DinoResource::GetCreatureUnk0D(u32 id) {
    return sCreatureUnk0DValues[GetCreatureDef(id)->unk0D];
}

std::string DinoResource::GetKasekiName(u32 id) {
    MResource<DinoKasekiDefs> defs = LoadKasekiDefs();
    const DinoKasekiDefs *data = defs.Get();
    const DinoKasekiDef *def =
        (const DinoKasekiDef *)((const u8 *)data + ((const u32 *)((const u8 *)data + data->tableOffset))[id]);
    return std::string(GetText(def->nameText));
}

MResource<DinoBattleAttackType> DinoResource::LoadBattleAttackType() {
    return MResource<DinoBattleAttackType>(sEtcBattleAttackTypePath);
}

DinoRegionCenter DinoResource::GetRegionCenter(u32 id) {
    DinoRegionCenter center;
    sRegionCenterMatch.Read(id * sizeof(DinoRegionCenter), &center, sizeof(DinoRegionCenter));
    return center;
}

u16 DinoResource::GetRegionMapMatch(u32 id) {
    u16 value;
    sRegionMapMatch.Read(id * sizeof(u16), &value, sizeof(u16));
    return value;
}

u16 DinoResource::GetTalkMessageMatch(u32 id) {
    u16 value;
    sTalkMessageMatch.Read(id * sizeof(u16), &value, sizeof(u16));
    return value;
}

u16 DinoResource::GetMapExcavateMatch(u32 id) {
    u16 value;
    sMapExcavateMatch.Read(id * sizeof(u16), &value, sizeof(u16));
    return value;
}

u16 DinoResource::GetMapBgmMatch(u32 id) {
    u16 value;
    sMapBgmMatch.Read(id * sizeof(u16), &value, sizeof(u16));
    return value;
}

u16 DinoResource::GetMapSceneMatch(u32 id) {
    u16 value;
    sMapSceneMatch.Read(id * sizeof(u16), &value, sizeof(u16));
    return value;
}

std::string DinoResource::GetKasekiRockName(u32 id) {
    // (The game keeps both in memory, reading them at each use.)
    volatile u32 group = ((id - 1) >> 2) + 1;
    volatile u32 sub = (id - 1) & 3;
    if (group == 0 || group >= mKasekiRockCount) {
        return std::string("");
    }
    std::string name;
    name += GetCreatureText08(group);
    name += " ";
    name += GetText(gKasekiRockNameText);
    name += GetText(gKasekiRockSizeText + sub);
    name += GetText(gKasekiRockNameSuffixText);
    return name;
}

void DinoResource::SetUnk90(u32 a, u32 b) {
    mUnk90 = a;
    mUnk94 = b;
}

void DinoResource::GetUnk90(u32 *a, u32 *b) {
    *a = mUnk90;
    *b = mUnk94;
}

u32 DinoResource::FindExcavateRange(u32 value) {
    for (u32 i = 0; i < mExcavateDefs.Get()->rangeCount; i++) {
        const DinoExcavateDefs *defs = mExcavateDefs.Get();
        const DinoExcavateRange *range = (const DinoExcavateRange *)((const u8 *)defs + defs->rangeOffset) + i;
        if (range->min <= value && value <= range->max) {
            return range->value;
        }
    }
    return 1;
}

// String members the game emits in this file.
template std::string &std::string::operator+=(const char *s);
