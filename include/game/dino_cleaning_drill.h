#ifndef GAME_DINO_CLEANING_DRILL_H
#define GAME_DINO_CLEANING_DRILL_H

#include "m/motion.h"
#include "game/dino_cleaning_tool.h"

class DinoCleaningDrill : public DinoCleaningTool {
public:
    DinoCleaningDrill(DinoCleaning *cleaning, u8 arg1);
    virtual ~DinoCleaningDrill();
    virtual void Unk18();
    virtual void Unk1C();
    virtual void Unk20();

    void LoadGuides();
    void ShowGuides();
    void HideGuides();
    void SetGuidePos(MPoint pos);
    // Marks the cell for drilling, with its neighbours no deeper than it (with three or more neighbours at least
    // as deep, the deepest of them sets the depth).
    void MarkCell(int x, int y);

    MMotion *mRowGuide;     // 0x78: a line across at the drill's height
    MMotion *mColumnGuide;  // 0x7C: a line down at the drill's x
    MMotion *mMark;         // 0x80: at the drill
    u8 mTouching;           // 0x84: the stylus was down last frame
    MPoint mPrevTouch;      // 0x88
    MPoint mPrevCell;       // 0x90
    u8 mMarks[36][28];      // 0x98: the cells to drill
    u8 mPrevMarks[36][28];  // 0x488: last frame's
    u32 mUnk878;            // 0x878
    u8 mUnk87C;             // 0x87C: the stronger drill
};

#endif
