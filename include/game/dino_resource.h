// The game's resources: the data files it keeps loaded, the file paths it opens by name, its fonts and its random
// numbers.
#ifndef GAME_DINO_RESOURCE_H
#define GAME_DINO_RESOURCE_H

#include "m/font.h"
#include "m/resource.h"
#include "m/fx32.h"
#include "m/geometry.h"

class MRandom;

// The record at a position in a file of fixed-size records.
struct DinoRegionCenter {
    DinoRegionCenter() {}
    u32 unk00;
    u32 unk04;
};

// The position at a region's center.
static inline MFx32Point RegionPos(DinoRegionCenter center) {
    MFx32Point pos;
    pos.Set(MFx32((int)center.unk00), MFx32((int)center.unk04));
    return pos;
}

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

// A creature's stat: by rank, from low (rank 0) to high (rank 8) along a curve, then on towards max (rank 12).
struct DinoCreatureStatDef {
    u8 curve;   // which of the curves (1-4)
    u8 high;
    u8 low;
    u8 max;
};

struct DinoCreatureDef {
    u8 GetUnk0D() const { return unk0D; }
    u32 GetVoice() const { return voice; }

    // The hit points by rank, from rank 1 (u16 each).
    const u16 *GetHpTable() const { return (const u16 *)((const u8 *)this + hpTable); }
    // The level the attack needs.
    u32 GetAttackLevel(u32 i) const { return ((const u32 *)((const u8 *)this + attackLevels))[i]; }

    u8 unk00[0xD];
    u8 unk0D;
    u8 unk0E[0x10 - 0xE];
    DinoCreatureStatDef stats[4];   // 0x10
    u8 unk20[0x24 - 0x20];
    u32 types;              // 0x24: a bit per type
    u32 attackCount;        // 0x28
    u32 attackIds;          // 0x2C: offset of attackCount u32 attack ids
    u32 specialAttack;      // 0x30
    u32 unk34[2];
    u32 attackLevels;       // 0x3C: offset of the level each attack needs (u32 each)
    u32 specialAttackLevel; // 0x40
    u8 unk44[0x54 - 0x44];
    u32 unk54;          // 0x54: offset of 4 s8 values, or 0
    u32 unk58;          // 0x58: offset of 4 s8 values, or 0
    u8 unk5C;
    DinoCreatureStatDef stat4;      // 0x5D
    u8 unk61;
    u8 unk62[0x78 - 0x62];
    u32 attackUnk78;        // 0x78: offset of a u8 per attack
    u32 unk7C;
    u32 hpTable;        // 0x80: offset of the hit points by rank
    u32 voice;          // 0x84: the cry, from SE 0x475
    u32 unk88;          // 0x88
};

struct DinoAttackDefs {
    u32 unk00;
    u32 nameText;       // 0x04
    u32 text08;         // 0x08
    u32 unk0C;
    u32 tableOffset;    // 0x10
};

struct DinoAttackDef {
    u32 unk00;
    u32 hitCount;       // 0x04
    u32 hits;           // 0x08: offset of hitCount {u16, u16 weight}
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
    u32 unk00;
    MFx32 curveBase;        // 0x04
    MFx32 curveRange;       // 0x08
    MFx32 curveLow;         // 0x0C
    MFx32 unk10;            // 0x10: divides the damage

    MFx32 unk14;            // 0x14
    MFx32 criticalMin;      // 0x18: the range of the critical hit factor (func_ov000_0216ea64)
    MFx32 criticalMax;      // 0x1C
    MFx32 randomMin;        // 0x20: the range of the random damage factor
    MFx32 randomMax;        // 0x24
    u32 unk28[2];
    MFx32 affinityRatio[3]; // 0x30: by CompareUnkECA0's result (1, 2, other)
    MFx32 unk3C;            // 0x3C
    MFx32 unk40;            // 0x40
    MFx32 pinchBoost;       // 0x44: per point of a pinch creature's def percentage, in tenths
    MFx32 unk48;            // 0x48
    MFx32 unk4C;            // 0x4C: the chance of the second roll (func_ov000_0216e918)
    MFx32 unk50;            // 0x50
    u32 unk54;
    // Creature layout, by slot kind (0, 2, 1, 3 in that order): where the slots are with the side's own creatures
    // set and not, slot position extras, and offsets from them.
    MPoint slotPos[4];          // 0x58
    MPoint slotPosAlt[4];       // 0x78
    MPoint slotPos98[4];        // 0x98
    MPoint slotPosB8[4];        // 0xB8
    MPoint slotOffset[4];       // 0xD8: from slotPos
    MPoint slotOffsetAlt[4];    // 0xF8: from slotPosAlt
    MPoint slotOffsetB8[4];     // 0x118: from slotPosB8
    int kasekiPointX;   // 0x138
    int kasekiPointY;   // 0x13C
    u32 unk140[2];
    int statusIconX;    // 0x148
    int statusIconY;    // 0x14C
    u32 unk150;
    u32 backdropTop;    // 0x154: offset of the backdrops' top colors (DinoRgb)
    u32 unk158;
    u32 backdropBottom; // 0x15C: offset of their bottom colors
    u32 unk160;
    u32 fieldPoints;        // 0x164: offset of the field's positions (MPoint), by slot
    u8 unk168;          // 0x168: whether the support effects apply
    u8 unk169[3];
    MFx32 pinchRatio;   // 0x16C: at or below this share of its life, a creature is in a pinch
    u8 unk170;
    u8 debugLog;        // 0x171: show the battle's debug log
    u8 unk172[2];
    u32 actionLog;      // 0x174: list the actions in the action info bar
    u32 unk178;
    u32 fixedFieldBg;       // 0x17C: show the battle's own background behind the fields (see DinoBattleBgDefs)
    int advantageArrowX0;   // 0x180: where the advantage arrow points at the first side
    int advantageArrowY0;   // 0x184
    int advantageArrowX1;   // 0x188: and at the second
    int advantageArrowY1;   // 0x18C
    u8 abilityMenuWrap; // 0x190: the ability menu's cursor wraps around
    u8 unk191;
    u8 unk192;          // 0x192: (with unk170) the 3D view shows the game-set effect
    u8 unk193[0x198 - 0x193];
    fx32 rankScaleHigh;     // 0x198
    fx32 rankScaleLow;      // 0x19C
    u8 unk1A0[0x1AC - 0x1A0];
    u32 kpUpDownPoints; // 0x1AC: offset of the KP up/down numbers' positions (MPoint), by side
    u8 unk1B0;          // 0x1B0: team attacks (attack kind 3) take the team-attack path
    u8 unk1B1[3];
    u32 wirelessUnk1B4;     // 0x1B4: the wireless battle's settings
    u32 wirelessUnk1B8;     // 0x1B8
    u32 wirelessUnk1BC;     // 0x1BC
    u32 wirelessUnk1C0;     // 0x1C0
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

// The battles the title screen's demo picks from, by how far the save has got: lists of battle ids.
struct DinoDemoBattles {
    u32 count;          // 0x00
    u32 offset;         // 0x04
    u32 lateCount;      // 0x08: with flag1a in the save
    u32 lateOffset;     // 0x0C
    u32 midCount;       // 0x10: with flag1b in the save
    u32 midOffset;      // 0x14
    u32 unk18;          // 0x18: the demo battles watched before the demo stops
};
struct DinoExcavateDefs {
    const int *GetRockKindWeights() const { return (const int *)((const u8 *)this + rockKindWeights); }
    const u32 *GetRockSet1() const { return (const u32 *)((const u8 *)this + rockSet1); }
    const u32 *GetRockSet2() const { return (const u32 *)((const u8 *)this + rockSet2); }
    // The rock area's size.
    int GetRockAreaWidthA4() const { return rockAreaAC - rockAreaA4; }
    int GetRockAreaWidthA0() const { return rockAreaA8 - rockAreaA0; }

    u8 unk00[0xC];
    fx32 radarRange;    // 0x0C: how far the radar reaches
    fx32 unk10;         // 0x10: distances the radar compares squared
    fx32 unk14;         // 0x14
    fx32 unk18;         // 0x18
    u16 unk1C;          // 0x1C
    u16 unk1E;          // 0x1E
    int superRockChance;    // 0x20: percent of rocks that are super rocks (license 2)
    u32 emptyChance;    // 0x24: a dig spot stays empty when MRandom::Next() is at most this
    u8 unk28[0x30 - 0x28];
    u32 rockKindWeights;    // 0x30: offset of the weights of the three rock kinds
    u8 unk34[0x40 - 0x34];
    u32 rockSet1Count;  // 0x40: the rocks of kind 1
    u32 rockSet1;       // 0x44: (u32 each)
    u32 rockSet2Count;  // 0x48: the rocks of kind 2
    u32 rockSet2;       // 0x4C
    u8 unk50[0x5C - 0x50];
    u32 manpus;         // 0x5C: offset of the offsets of the manpu (emotion bubble) entries
    u8 unk60[4];
    u32 picts;          // 0x64: offset of the offsets of the event picture entries
    u32 levelCount;     // 0x68
    u32 levelOffset;    // 0x6C: DinoExcavateLevel entries
    u8 unk70[0x78 - 0x70];
    u32 offset78;       // 0x78
    u32 offset7C;       // 0x7C
    u32 offset80;       // 0x80
    u32 soundOffset;    // 0x84: the u8 passed to DinoSoundInit
    u32 exchangeCount;  // 0x88: how many fossils an exchange can give
    u32 exchangeOffset; // 0x8C: the fossil ids an exchange can give (u32 each)
    u32 exchangeWait;   // 0x90: frames between wireless exchanges
    u32 exchangeRetryWait;  // 0x94: frames before a found partner is taken again
    u8 unk98[0x9C - 0x98];
    u32 kasekiRockMap;  // 0x9C: the map the kaseki rocks are on
    u32 rockAreaA0;     // 0xA0: the area new field rocks are placed in
    u32 rockAreaA4;     // 0xA4
    u32 rockAreaA8;     // 0xA8
    u32 rockAreaAC;     // 0xAC
    u8 unkB0[0xB8 - 0xB0];
    u32 demoOffset;     // 0xB8: DinoDemoBattles
    u32 rangeCount;     // 0xBC
    u32 rangeOffset;    // 0xC0: DinoExcavateRange entries
};
// A map's settings.
// A town map a map can show: the first one with no flag, or a later one whose flag is set.
struct DinoMapMTownMap {
    u32 id;
    u32 flag;
};

struct DinoMapMData {
    const DinoMapMTownMap *GetTownMaps() const { return (const DinoMapMTownMap *)((const u8 *)this + townMaps); }
    MPoint GetTownMapOffset() const { return MPoint(townMapX, townMapY); }

    u8 unk00[0x18];
    u32 townMapCount;   // 0x18
    u32 townMaps;       // 0x1C: offset of the DinoMapMTownMap entries
    u8 townMapCentered; // 0x20
    u8 unk21[3];
    int townMapX;       // 0x24
    int townMapY;       // 0x28
    MFx32 townMapScale;     // 0x2C
    u8 unk30[4];
    s32 area;           // 0x34: the area the map is in (for the name panel)
    u8 unk38[0x50 - 0x38];
    const struct DinoMapDigArea *GetDigArea(u32 i) const {
        return (const DinoMapDigArea *)((const u8 *)this + ((const u32 *)((const u8 *)this + digAreas))[i]);
    }

    u32 digAreaCount;   // 0x50
    u32 digAreas;       // 0x54: offset of the offsets of the DinoMapDigArea entries
};
// A map's dig area (in its M file): which cells are in it and the fossils its rocks hold.
struct DinoMapDigArea {
    const u32 *GetFossil(u32 i) const {
        return (const u32 *)((const u8 *)this + ((const u32 *)((const u8 *)this + fossils))[i]);
    }

    u32 unk00;
    u32 cellArea;       // 0x04: the cells with this area value belong to it
    u32 flag;           // 0x08: the flag it needs (0 none)
    u32 rockCount;      // 0x0C
    u8 unk10[0x28 - 0x10];
    u32 fossilCount;    // 0x28
    u32 fossils;        // 0x2C: offset of the offsets of {u32 id, int weight}
};
// A map's cells (its E file): each cell's dig area.
struct DinoMapData {
    u8 GetCell(u32 i) const {
        const u8 *p = (const u8 *)this + cells;
        return p[i];
    }

    u32 unk00;
    u32 width;          // 0x04: cells per row
    u32 unk08;
    u32 cellCount;      // 0x0C
    u32 cells;          // 0x10: offset of the cells' u8 area values
};
// A battle's settings.
// A battle side's creature.
struct DinoBattleCreatureData {
    u32 id;             // 0x00
    s32 level;          // 0x04: 1-12
    u32 unk08;          // 0x08
};
// One side of a battle (the counts are of the u32s at the offsets after them).
struct DinoBattleSideData {
    const DinoBattleCreatureData *GetCreatures() const {
        return (const DinoBattleCreatureData *)((const u8 *)this + creatureOffset);
    }
    const DinoBattleCreatureData &GetCreature(u32 i) const { return GetCreatures()[i]; }
    const u32 *GetUnk14() const { return (const u32 *)((const u8 *)this + unk14); }
    const u32 *GetUnk1C() const { return (const u32 *)((const u8 *)this + unk1C); }
    const u32 *GetUnk24() const { return (const u32 *)((const u8 *)this + unk24); }
    const u32 *GetUnk2C() const { return (const u32 *)((const u8 *)this + unk2C); }

    u32 creatureCount;  // 0x00
    u32 creatureOffset; // 0x04
    u32 nameText;       // 0x08: 0 for the player's own name
    u32 unk0C;          // 0x0C
    u32 unk10;          // 0x10
    u32 unk14;          // 0x14
    u32 unk18;          // 0x18
    u32 unk1C;          // 0x1C
    u32 unk20;          // 0x20
    u32 unk24;          // 0x24
    u32 unk28;          // 0x28
    u32 unk2C;          // 0x2C
    u32 unk30;          // 0x30
    u8 unk34;           // 0x34
};
// A rock a battle can give, and its weight.
struct DinoBattleRock {
    u32 weight;         // 0x00: out of 100
    u32 id;             // 0x04
};
struct DinoBattleData {
    const DinoBattleRock *GetRocks() const { return (const DinoBattleRock *)((const u8 *)this + rockOffset); }
    const DinoBattleRock &GetRock(u32 i) const { return GetRocks()[i]; }
    // The creatures the rule names.
    const u32 *GetUnk40() const { return (const u32 *)((const u8 *)this + unk40); }
    u32 GetUnk44() const { return unk44; }
    const DinoBattleSideData *GetPlayer() const {
        return playerOffset == 0 ? NULL : (const DinoBattleSideData *)((const u8 *)this + playerOffset);
    }
    const DinoBattleSideData *GetEnemy() const {
        return enemyOffset == 0 ? NULL : (const DinoBattleSideData *)((const u8 *)this + enemyOffset);
    }

    u32 unk00;
    u32 playerOffset;   // 0x04: 0 for the player's team
    u32 enemyOffset;    // 0x08
    u32 unk0C;          // 0x0C
    u32 scene;          // 0x10
    u8 unk14;           // 0x14
    u8 unk15;           // 0x15
    u8 unk16;           // 0x16
    u32 unk18;          // 0x18
    u32 unk1C;          // 0x1C: a talk text
    u32 unk20;          // 0x20: a talk text
    u32 tutorialId;     // 0x24
    u32 liveName;       // 0x28
    u32 unk2C;          // 0x2C
    u32 unk30;          // 0x30
    u32 rockCount;      // 0x34: the rocks the battle can give instead
    u32 rockOffset;     // 0x38: DinoBattleRock[rockCount]
    u32 unk3C;          // 0x3C: 1-3
    u32 unk40;          // 0x40: offset of unk3C creature ids
    u32 unk44;          // 0x44: 1-5, or 0 for none
    u32 unk48;          // 0x48: 1-3
    u32 unk4C;          // 0x4C
    u32 unk50;          // 0x50
    s32 unk54;          // 0x54: a percentage, or -1
    u8 unk58;           // 0x58
};
// A shop's stock: entries always sold, then entries picked from at random.
struct DinoShopData {
    u32 unk00;
    u32 fixedCount;     // 0x04
    u32 fixedOffset;    // 0x08: DinoEntrySource[fixedCount]
    u32 randomCount;    // 0x0C
    u32 randomOffset;   // 0x10: DinoEntrySource[randomCount]
};
struct DinoDanceData;
// A field character's data file (only what the radar reads so far).
struct DinoCharaData {
    u8 unk00[0x28];
    u32 unk28;          // 0x28: 0: the radar marks the character red
};
struct DinoMessageData;
struct DinoEpisodeData;
// The key items' definitions.
struct DinoKeyItemDefs {
    u32 unk00;
    u32 count;          // 0x04
    u32 tableOffset;    // 0x08: DinoKeyItemDef[count]
};
struct DinoKeyItemDef {
    u16 nameText;       // 0x00
    u16 descText;       // 0x02
    u32 unk04;          // 0x04
};
// The head masks (the player's alternative heads).
struct DinoHeadMaskDefs {
    u32 unk00;
    u32 count;          // 0x04
    u32 tableOffset;    // 0x08: offsets of the masks
};
struct DinoHeadMaskDef {
    u16 nameText;       // 0x00
    u16 descText;       // 0x02
    u32 price;          // 0x04
    u32 modelName;      // 0x08: offset of the head model's name, from the mask
    u32 moveSe;         // 0x0C: the player's footstep sounds with it on
    u32 moveSe2;        // 0x10
};
// The museum's descriptions of the creatures.
struct DinoMuseumDef;
struct DinoMuseumDefs {
    const DinoMuseumDef *GetDef(u32 id) const;

    u32 unk00;
    u32 count;          // 0x04
    u32 tableOffset;    // 0x08: of the DinoMuseumDef, one per creature
};

struct DinoMuseumDef {
    u32 unk00;
    u32 text;           // 0x04
    u16 unk08;          // 0x08
    u16 unk0A;          // 0x0A
    u32 unk0C;          // 0x0C
};
inline const DinoMuseumDef *DinoMuseumDefs::GetDef(u32 id) const {
    return (const DinoMuseumDef *)((const u8 *)this + tableOffset) + id;
}
// The fossil (kaseki) definitions.
struct DinoKasekiDef;
struct DinoKasekiDefs {
    const DinoKasekiDef *GetDef(u32 id) const {
        return (const DinoKasekiDef *)((const u8 *)this + ((const u32 *)((const u8 *)this + tableOffset))[id]);
    }

    u32 unk00;
    u32 count;          // 0x04
    u32 tableOffset;    // 0x08: of an offset of each def
};

struct DinoKasekiDef {
    u32 unk00;
    u8 unk04;           // 0x04: nonzero for fossils that can't be cleaned
    u8 unk05[0x0C - 0x05];
    u32 icon;           // 0x0C: its button's icon
    u8 unk10[0x18 - 0x10];
    u32 price;          // 0x18
    u32 sellPrice;      // 0x1C
    u32 unk20;          // 0x20: the selling price of one still in its rock?
    u32 unk24;          // 0x24

    u32 nameText;       // 0x28
    u8 unk2C[0x38 - 0x2C];
    u32 unk38;          // 0x38

    u8 IsUncleanable() const { return unk04; }
    u32 GetUnk38() const { return unk38; }
};
// The senryu (poems) sample's data.
struct DinoSenryuDefs {
    u32 unk00;
    u32 count;          // 0x04
};
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
