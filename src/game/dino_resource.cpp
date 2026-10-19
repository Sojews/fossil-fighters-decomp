// DinoResource, the game's resources. Only a few functions are decompiled so far: the file's statics (the data
// file paths, pooled together, and caches made at startup) aren't all here, so code that loads them can't match.

#define MSL_STRING_BASICS
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

// The order here gives the game's layout (see tools/static_order.py).
// static_order: begin
static const char *sEtcBattleAttackTypePath = "/etc/battle_attack_type";
static const char *sKeyItemDefsPath = "/etc/keyitem_defs";
static const char *sFontFont8Path = "/font/font8";
static const char *sAttackDefsPath = "/etc/attack_defs";
static const char *sMapRPath = "/map/r/%04d";
static const char *sHeadMaskDefsPath = "/etc/headmask_defs";
static const char *sHeadMaskMaxIdPath = "/etc/headmask_maxid";
static const char *sVarMaxIdPath = "/etc/var_maxid";
static const char *sRegionMaxIdPath = "/etc/region_maxid";
static const char *sFont12Path = "/font/font12";
static const char *sMapMaxIdPath = "/etc/map_maxid";
static const char *sTextPath = "/text/japanese";
static const char *sBattleBgDefsPath = "/etc/btl_bg_defs";
static const char *sBattleAdjustDefsPath = "/etc/btl_adjust_defs";
static const char *sEtcMapBgmMatchPath = "/etc/map_bgm_match";
static const char *sBattlePath = "/battle/%04d";
static const char *sKeyItemMaxIdPath = "/etc/keyitem_maxid";
static const char *sExcavateDefsPath = "/etc/excavate_defs";
static const char *sMuseumDefsPath = "/etc/museum_defs";
static const char *sMapCPath = "/map/c/%04d";
static const char *sFlagMaxIdPath = "/etc/flag_maxid";
static const char *sCreatureDefsPath = "/etc/creature_defs";
static const char *sBattleCreatureMotionPath = "/motion/battle_creature/creature_%03d.bin";
static const char *sMapMPath = "/map/m/%04d";
static const char *sMapEPath = "/map/e/%04d";
static const char *sFont24Path = "/font/font24";
static const char *sMapGPath = "/map/g/%04d";
static const char *sEpisodeMaxIdPath = "/etc/episode_maxid";
static const char *sDancePath = "/dance/%04d";
static const char *sKasekiRockMaxIdPath = "/etc/kasekirock_maxid";
static const char *sEtcMapSceneMatchPath = "/etc/map_scene_match";
static const char *sEpisodePath = "/episode/e%04d";
static const char *sCharaPath = "/chara/%04d";
static const char *sKasekiDefsPath = "/etc/kaseki_defs";
static const char *sSenryuDefsPath = "/etc/senryu_defs";
static const char *sBattleMaxIdPath = "/etc/battle_maxid";
static const char *sMessagePath = "/msg/msg_%04d";
static const char *sFontFont24fPath = "/font/font24f";
static const char *sFontFont10Path = "/font/font10";
static const char *sCharaMaxIdPath = "/etc/chara_maxid";
static const char *sShopPath = "/shop/%04d";
static const char *sEtcRegionCenterMatchPath = "/etc/region_center_match";
static const char *sEventMaxIdPath = "/etc/event_maxid";
static const char *sEtcRegionMapMatchPath = "/etc/region_map_match";
static const char *sEtcMapExcavateMatchPath = "/etc/map_excavate_match";
static const char *sEpisodeDirPath = "/episode/%04d";
static const char *sEtcTalkMsgMatchPath = "/etc/talk_msg_match";
static const char *sFont16Path = "/font/font16";
static const char *sUnk00Path = 0;
static const u16 sUnkTable[14] = {0, Rgb(0, 0, 0), Rgb(0, 18, 0), Rgb(30, 2, 2), Rgb(22, 11, 0), Rgb(9, 15, 27), Rgb(31, 12, 25), Rgb(26, 0, 12), Rgb(10, 15, 23), Rgb(31, 18, 0), Rgb(31, 31, 31), Rgb(16, 16, 16), Rgb(31, 18, 20), 0};
DinoResource *gDinoResource;
static OSMutex sMutex;
static DinoFileCache sMapBgmMatch(sEtcMapBgmMatchPath);
static DinoFileCache sMapSceneMatch(sEtcMapSceneMatchPath);
static DinoFileCache sRegionMapMatch(sEtcRegionMapMatchPath);
static DinoFileCache sRegionCenterMatch(sEtcRegionCenterMatchPath);
static DinoFileCache sTalkMessageMatch(sEtcTalkMsgMatchPath);
static DinoFileCache sMapExcavateMatch(sEtcMapExcavateMatchPath);
// static_order: end

inline void DinoFileCache::Read(u32 offset, void *dest, u32 size) {
    if (!mOpened) {
        FS_ConvertPathToFileID(&mId, mPath);
        mBlock = -1;
        mOpened = TRUE;
    }
    MScopedLock lock(&sMutex, TRUE);
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


static const u32 sLanguageTextId = 1;

// Text IDs defined elsewhere.
extern const u32 gKasekiRockNameText;
extern const u32 gKasekiRockSizeText;
extern const u32 gKasekiRockNameSuffixText;



void DinoResource::Create() {
    gDinoResource = new DinoResource();
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
    u32 sub = (id - 1) & 3;
    u32 group = ((id - 1) >> 2) + 1;
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
