#ifndef GAME_DINO_BATTLE_VIEW_KASEKI_POINT_H
#define GAME_DINO_BATTLE_VIEW_KASEKI_POINT_H

#include "m/task.h"

class MMotion;
class MIndicator;
class MPalUnit;
class MSmartOam;
class DinoBattleView;

// 32 palette rows fading between two palettes.
class KasekiPointPalette {
public:
    KasekiPointPalette(const u16 *from, const u16 *to);
    void Init(const u16 *from, const u16 *to);
    const u16 *GetRow(int index);

    u32 unk00;
    u16 mRows[32][16];  // 0x04
};

class DinoBattleViewKasekiPoint : public MProcess {
public:
    DinoBattleViewKasekiPoint(DinoBattleView *view, MSmartOam *oam, u32 side, u32 player);
    virtual ~DinoBattleViewKasekiPoint();
    virtual void Idle();

    void UpdateAnime(u32 player);
    void SetValue(int value);
    void LoadPalette(int index);
    BOOL IsCounting();

    int mValue;                 // 0x74
    int mUnk78;                 // 0x78
    int mUnk7C;                 // 0x7C
    MIndicator *mText;          // 0x80
    MMotion *mUnk84;            // 0x84
    MSmartOam *mOam;            // 0x88
    MPalUnit *mPalBlock;       // 0x8C
    KasekiPointPalette *mPalette; // 0x90
    DinoBattleView *mView;      // 0x94
    u32 mUnk98;                 // 0x98
    u16 mUnk9C[16];             // 0x9C
};

#endif
