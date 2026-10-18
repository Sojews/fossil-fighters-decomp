// The game's resources: the data files it keeps loaded, the file paths it opens by name, its fonts and its random
// numbers.
#ifndef GAME_DINO_RESOURCE_H
#define GAME_DINO_RESOURCE_H

#include "m/font.h"
#include "m/resource.h"
#include "m/fx32.h"

class MRandom;

// The record at a position in a file of fixed-size records.
struct DinoRegionCenter {
    u32 unk00;
    u32 unk04;
};

// A file of strings: an offset table at offset 8 (the format shared by most data files).
struct DinoTextData {
    u32 unk00;
    u32 count;          // 0x04
    u32 tableOffset;    // 0x08
};

// The creature (vivosaur) definitions: an entry per creature, and the text IDs their strings start at.
struct DinoCreatureDefs {
    u32 unk00;
    u32 nameText;       // 0x04
    u32 text08;         // 0x08
    u32 text0C;         // 0x0C
    u32 unk10;
    u32 count;          // 0x14
    u32 tableOffset;    // 0x18
    u32 text1C;         // 0x1C
    u32 text20;         // 0x20
    u32 text24;         // 0x24
};

struct DinoCreatureDef {
    u8 unk00[0xD];
    u8 unk0D;
    u8 unk0E[0x28 - 0xE];
    u32 attackCount;        // 0x28
    u32 attackIds;          // 0x2C: offset of attackCount u32 attack ids
    u32 specialAttack;      // 0x30
    u32 unk34[2];
    u32 attackLevels;       // 0x3C: offset of the level each attack needs (u32 each)
    u32 specialAttackLevel; // 0x40
    u8 unk44[0x54 - 0x44];
    u32 unk54;          // 0x54: offset of 4 s8 values, or 0
    u32 unk58;          // 0x58: offset of 4 s8 values, or 0
    u8 unk5C[0x78 - 0x5C];
    u32 attackUnk78;        // 0x78: offset of a u8 per attack
    u8 unk7C[0x84 - 0x7C];
    u32 voice;          // 0x84: the cry, from SE 0x475
};

struct DinoAttackDefs {
    u32 unk00;
    u32 nameText;       // 0x04
    u32 text08;         // 0x08
    u32 unk0C;
    u32 tableOffset;    // 0x10
};

struct DinoAttackDef {
    u8 unk00[0xC];
    u32 cost;           // 0x0C: offset of the u16 cost (in kaseki points), 0 for none
    u32 unk10;          // 0x10: offset of a u8
    u32 unk14;          // 0x14: offset of a u8
    u32 unk18;          // 0x18
};

// The data `offset` bytes into `base`, or NULL for offset 0.
template <class T> inline const T *DinoOffsetPtr(const void *base, u32 offset) {
    return offset == 0 ? NULL : (const T *)((const u8 *)base + offset);
}
struct DinoBattleAdjustDefs {
    u8 unk000[0x14];
    MFx32 unk14;            // 0x14
    u32 unk18[2];
    MFx32 randomMin;        // 0x20: the range of the random damage factor
    MFx32 randomMax;        // 0x24
    u32 unk28[2];
    MFx32 affinityRatio[3]; // 0x30: by CompareUnkECA0's result (1, 2, other)
    u8 unk03C[0x138 - 0x3C];
    int kasekiPointX;   // 0x138
    int kasekiPointY;   // 0x13C
    u32 unk140[2];
    int statusIconX;    // 0x148
    int statusIconY;    // 0x14C
    u32 unk150;
    u32 backdropTop;    // 0x154: offset of the backdrops' top colors (DinoRgb)
    u32 unk158;
    u32 backdropBottom; // 0x15C: offset of their bottom colors
    u32 unk160[3];
    MFx32 pinchRatio;   // 0x16C: at or below this share of its life, a creature is in a pinch
};

struct DinoRgb {
    u8 r;
    u8 g;
    u8 b;
    u8 unk3;
};
struct DinoBattleBgDefs;
struct DinoExcavateRange {
    u32 min;
    u32 max;
    u32 value;
};

struct DinoExcavateLevel {
    u32 unk0;           // 0x00
    u32 minUnk;         // 0x04
    u32 unk8;           // 0x08
    MFx32 unkC;         // 0x0C
    MFx32 unk10;        // 0x10
    u8 unk14[0x2C - 0x14];
};

struct DinoExcavateDefs {
    u8 unk00[0x68];
    u32 levelCount;     // 0x68
    u32 levelOffset;    // 0x6C: DinoExcavateLevel entries
    u8 unk70[0x78 - 0x70];
    u32 offset78;       // 0x78
    u32 offset7C;       // 0x7C
    u32 offset80;       // 0x80
    u8 unk84[0x38];
    u32 rangeCount;     // 0xBC
    u32 rangeOffset;    // 0xC0: DinoExcavateRange entries
};
struct DinoMapMData;
struct DinoMapData;
struct DinoBattleData;
// A shop's stock: entries always sold, then entries picked from at random.
struct DinoShopData {
    u32 unk00;
    u32 fixedCount;     // 0x04
    u32 fixedOffset;    // 0x08: DinoEntrySource[fixedCount]
    u32 randomCount;    // 0x0C
    u32 randomOffset;   // 0x10: DinoEntrySource[randomCount]
};
struct DinoDanceData;
struct DinoCharaData;
struct DinoMessageData;
struct DinoEpisodeData;
struct DinoKeyItemDefs;
// The head masks (the player's alternative heads).
struct DinoHeadMaskDefs {
    u32 unk00;
    u32 count;          // 0x04
    u32 tableOffset;    // 0x08: offsets of the masks
};
struct DinoHeadMaskDef {
    u32 unk00;
    u32 price;          // 0x04
    u32 modelName;      // 0x08: offset of the head model's name, from the mask
    u32 moveSe;         // 0x0C: the player's footstep sounds with it on
    u32 moveSe2;        // 0x10
};
struct DinoMuseumDefs;
// The fossil (kaseki) definitions.
struct DinoKasekiDefs {
    u32 unk00;
    u32 count;          // 0x04
    u32 tableOffset;    // 0x08
};

struct DinoKasekiDef {
    u8 unk00[0x18];
    u32 price;          // 0x18
    u32 sellPrice;      // 0x1C
    u32 unk20;          // 0x20: the selling price of one still in its rock?
    u32 unk24;          // 0x24

    u32 nameText;       // 0x28
};
struct DinoSenryuDefs;
struct DinoBattleAttackType;

// A file holding one number: how many of something there are.
struct DinoMaxId {
    u32 unk00;
    u32 count;  // 0x04
};

class DinoResource {
public:
    DinoResource();

    static void Create();

    MRandom *GetRandom();
    const char *GetText(u32 id);
    const u16 *GetUnkTable();
    u32 GetCreatureCount();
    const DinoCreatureDef *GetCreatureDef(u32 id);
    const DinoAttackDef *GetAttackDef(u32 id);
    std::string GetBattleCreatureMotionPath(int id);
    const DinoBattleAdjustDefs *GetBattleAdjustDefs();
    const DinoBattleBgDefs *GetBattleBgDefs();
    const char *GetAttackName(u32 id);
    const char *GetCreatureText1C(u32 id);
    const char *GetCreatureName(u32 id);
    const char *GetCreatureText08(u32 id);
    const char *GetCreatureText24(u32 id);
    const char *GetCreatureText20(u32 id);
    const char *GetCreatureText0C(u32 id);
    const char *GetAttackText08(u32 id);
    u32 GetFlagCount();
    u32 GetKeyItemCount();
    u32 GetHeadMaskCount();
    u32 GetVarCount();
    u32 GetEpisodeCount();
    u32 GetEventCount();
    u32 GetMapCount();
    u32 GetRegionCount();
    u32 GetCharaCount();
    u32 GetKasekiRockCount();
    u32 GetBattleCount();
    MResource<DinoMapMData> LoadMapM(int id);
    MResource<DinoMapData> LoadMapR(int id);
    MResource<DinoMapData> LoadMapC(int id);
    MResource<DinoMapData> LoadMapE(int id);
    MResource<DinoMapData> LoadMapG(int id);
    MResource<DinoBattleData> LoadBattle(int id);
    MResource<DinoShopData> LoadShop(int id);
    MResource<DinoDanceData> LoadDance(int id);
    MResource<DinoCharaData> LoadChara(int id);
    MResource<DinoMessageData> LoadMessage(int id);
    MResource<DinoEpisodeData> LoadEpisode(int id);
    MResource<DinoKeyItemDefs> LoadKeyItemDefs();
    MResource<DinoHeadMaskDefs> LoadHeadMaskDefs();
    MResource<DinoMuseumDefs> LoadMuseumDefs();
    MResource<DinoKasekiDefs> LoadKasekiDefs();
    MResource<DinoSenryuDefs> LoadSenryuDefs();
    MResource<DinoExcavateDefs> GetExcavateDefs();
    const void *GetExcavate78();
    const void *GetExcavate7C();
    const void *GetExcavate80();
    u32 GetCreatureUnk0D(u32 id);
    std::string GetKasekiName(u32 id);
    std::string GetKasekiRockName(u32 id);
    void SetUnk90(u32 a, u32 b);
    void GetUnk90(u32 *a, u32 *b);
    u32 FindExcavateRange(u32 value);
    MResource<DinoBattleAttackType> LoadBattleAttackType();
    DinoRegionCenter GetRegionCenter(u32 id);
    u16 GetRegionMapMatch(u32 id);
    u16 GetTalkMessageMatch(u32 id);
    u16 GetMapExcavateMatch(u32 id);
    u16 GetMapBgmMatch(u32 id);
    u16 GetMapSceneMatch(u32 id);
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
