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

    MMotion *mRowGuide;     // 0x78: a line across at the drill's height
    MMotion *mColumnGuide;  // 0x7C: a line down at the drill's x
    MMotion *mMark;         // 0x80: at the drill
    u8 mUnk84;              // 0x84
    u8 unk85[0x87C - 0x85];
    u8 mUnk87C;             // 0x87C
};

#endif
