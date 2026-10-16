// DinoResource, the game's resources. Only a few functions are decompiled so far: the file's statics (the data
// file paths, pooled together, and caches made at startup) aren't all here, so code that loads them can't match.

#include "game/dino_resource.h"
#include "m/seed_generator.h"

static const char *sTextPath = "/text/japanese";
static const char *sCreatureDefsPath = "/etc/creature_defs";
static const char *sAttackDefsPath = "/etc/attack_defs";
static const char *sBattleAdjustDefsPath = "/etc/btl_adjust_defs";
static const char *sBattleBgDefsPath = "/etc/btl_bg_defs";
static const char *sFlagMaxIdPath = "/etc/flag_maxid";
static const char *sKeyItemMaxIdPath = "/etc/keyitem_maxid";
static const char *sHeadMaskMaxIdPath = "/etc/headmask_maxid";
static const char *sVarMaxIdPath = "/etc/var_maxid";
static const char *sEpisodeMaxIdPath = "/etc/episode_maxid";
static const char *sEventMaxIdPath = "/etc/event_maxid";
static const char *sMapMaxIdPath = "/etc/map_maxid";
static const char *sRegionMaxIdPath = "/etc/region_maxid";
static const char *sCharaMaxIdPath = "/etc/chara_maxid";
static const char *sKasekiRockMaxIdPath = "/etc/kasekirock_maxid";
static const char *sBattleMaxIdPath = "/etc/battle_maxid";
static const char *sExcavateDefsPath = "/etc/excavate_defs";
static const char *sFont24Path = "/font/font24";
static const char *sFont16Path = "/font/font16";
static const char *sFont12Path = "/font/font12";
static u16 sUnkTable[16];

static const u32 sLanguageTextId = 1;

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

MFontSet *DinoResource::CreateFont24(int color, int shadow, int back) {
    return new MFontSet(sFont24Path, color, shadow, back);
}

MFontSet *DinoResource::CreateFont16(int color, int shadow, int back) {
    return new MFontSet(sFont16Path, color, shadow, back);
}

MFontSet *DinoResource::CreateFont12(int color, int shadow, int back) {
    return new MFontSet(sFont12Path, color, shadow, back);
}
