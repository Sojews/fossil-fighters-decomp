#ifndef GAME_BATTLE_SELECT_H
#define GAME_BATTLE_SELECT_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "m/seed_generator.h"
#include <vector>

// A debug process that plays battles automatically.
class DebugAutoBattle : public MProcess {
public:
    DebugAutoBattle();
    virtual ~DebugAutoBattle();
    virtual void Idle();

    MRandom *mRandom;           // 0x74
    std::vector<u32> mUnk78;    // 0x78
};

class MInput;
class MIndicator;

class BattleSelect : public MProcess {
public:
    BattleSelect(u32 unk74);
    virtual ~BattleSelect();
    virtual void Idle();

    u32 mUnk74;                 // 0x74
    int mUnk78;                 // 0x78: the battle number
    MInput *mInput;             // 0x7C
    MIndicator *mIndicator;     // 0x80
};

class UseCreatureSelect : public MProcess {
public:
    UseCreatureSelect(void *params, u32 unk7C, u32 unk80, u8 *cancelled);
    virtual ~UseCreatureSelect();
    virtual void Idle();

    class MIndicator *CreateIndicator(class MSmartOam *oam, class MFont *font, MPoint size, MPoint pos);
    void CreateTexts(class MSmartOam *oam, class MFont *font);
    void ShowCreature(u32 creature);

    // Builds the screen: the two sides' creature rows (name, level, id), the cursor and the first creature's
    // attacks.
    void Start();
    // Handles the keys: up/down pick a row, left/right (and X/Y by ten) change its creature, R1/L1 its level, R/L
    // its unk5C value; start writes the rows into the battle's parameters, B cancels.
    void Select();

    void (UseCreatureSelect::*mState)(); // 0x74
    u32 mUnk7C;             // 0x7C: both sides (six rows, else three)
    u32 mUnk80;             // 0x80: by side, rows that show their unk5C value
    int mCount;             // 0x84: the rows
    u32 mUnk88;             // 0x88: the row the cursor is on
    u32 mIds[6];            // 0x8C: by row, the creature
    u32 mUnkA4[6];          // 0xA4: by row, the side's unk5C value
    u32 mLevels[6];         // 0xBC: by row
    class MIndicator *mHeader;      // 0xD4
    class MIndicator *mRows[6];     // 0xD8
    class MIndicator *mCursor;      // 0xF0
    class MInputHub *mInputHub;     // 0xF4
    u32 unkF8;
    class MIndicator *mTitle;       // 0xFC
    u32 unk100;
    class MIndicator *mNames[5];    // 0x104
    class MIndicator *mValues[5];   // 0x118
    void *mParams;          // 0x12C
    u8 *mCancelled;         // 0x130
};

class PlayerLevelSelect : public MProcess {
public:
    PlayerLevelSelect(u32 *level, BOOL unk7C, u32 unk80);
    virtual ~PlayerLevelSelect();
    virtual void Idle();

    // Builds the menu (title, rows, cursor, tutorial and commentary lines) and takes the level range from the KP defs.
    void Start();
    // Handles the menu's keys: up/down pick a row, left/right change it, L/R step the tutorial or commentary, start
    // writes the choices into the battle's parameters.
    void StateSelect();

    void (PlayerLevelSelect::*mState)(); // 0x74
    int mUnk7C;             // 0x7C: the last level row (0 or 1)
    u32 mUnk80;             // 0x80
    int mValues[2];         // 0x84: the sides' levels (indexed by row, the rows past them reach mCounts)
    int mCounts[2];         // 0x8C: rows 2 and 3 (1 to 10)
    u32 unk94[2];
    int mRow;               // 0x9C: 0 to 3
    int mMin;               // 0xA0: the levels' range
    int mMax;               // 0xA4
    int mUnkA8;             // 0xA8: the tutorial (0 for none)
    int mUnkAC;             // 0xAC: the commentary (0 for none)
    class MIndicator *mTexts[4];    // 0xB0: by row
    class MView *mCursor;           // 0xC0
    class MIndicator *mTutorialText;    // 0xC4
    class MIndicator *mLiveText;        // 0xC8
    class MInputHub *mInputHub;         // 0xCC
    struct DinoBattleParams *mLevel;    // 0xD0
};

class IncValTask : public MTask {
public:
    virtual ~IncValTask();
    virtual void Execute();

    int *mValue;    // 0x10
};

class TimeCountTask : public MProcess {
public:
    virtual ~TimeCountTask();
    virtual void Idle();
};

#endif
