// The game's resources: the data files it keeps loaded, the file paths it opens by name, its fonts and its random
// numbers.
#ifndef GAME_DINO_RESOURCE_H
#define GAME_DINO_RESOURCE_H

#include "m/font.h"
#include "m/resource.h"

class MRandom;

// A file of strings: an offset table at offset 8 (the format shared by most data files).
struct DinoTextData {
    u32 unk00;
    u32 count;          // 0x04
    u32 tableOffset;    // 0x08
};

struct DinoCreatureDefs;
struct DinoAttackDefs;
struct DinoBattleAdjustDefs;
struct DinoBattleBgDefs;
struct DinoExcavateDefs;

// A file holding one number: how many of something there are.
struct DinoMaxId {
    u32 unk00;
    u32 count;  // 0x04
};

class DinoResource {
public:
    DinoResource();

    MRandom *GetRandom();
    const char *GetText(u32 id);
    static const u16 *GetUnkTable();
    MFontSet *CreateFont24(int color, int shadow, int back);
    MFontSet *CreateFont16(int color, int shadow, int back);
    MFontSet *CreateFont12(int color, int shadow, int back);

    MRandom *mRandom;                                   // 0x00
    MResource<DinoTextData> mText;                      // 0x04
    MResource<DinoCreatureDefs> mCreatureDefs;          // 0x14
    MResource<DinoAttackDefs> mAttackDefs;              // 0x24
    MResource<DinoBattleAdjustDefs> mBattleAdjustDefs;  // 0x34
    MResource<DinoBattleBgDefs> mBattleBgDefs;          // 0x44
    u32 mFlagCount;                                     // 0x54
    u32 mKeyItemCount;                                  // 0x58
    u32 mHeadMaskCount;                                 // 0x5C
    u32 mVarCount;                                      // 0x60
    u32 mEpisodeCount;                                  // 0x64
    u32 mEventCount;                                    // 0x68
    u32 mMapCount;                                      // 0x6C
    u32 mRegionCount;                                   // 0x70
    u32 mCharaCount;                                    // 0x74
    u32 mKasekiRockCount;                               // 0x78
    u32 mBattleCount;                                   // 0x7C
    MResource<DinoExcavateDefs> mExcavateDefs;          // 0x80
    u32 mUnk90;
    u32 mUnk94;
};

extern DinoResource *gDinoResource;

// The text decoding defaults.
void MSetDefaultEncoding(int encoding);
void MSetUnkText(std::string text);

#endif
