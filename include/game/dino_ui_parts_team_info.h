#ifndef GAME_DINO_UI_PARTS_TEAM_INFO_H
#define GAME_DINO_UI_PARTS_TEAM_INFO_H

#include "m/task.h"

class DinoUiPartsTeamInfo : public MProcess {
public:
    DinoUiPartsTeamInfo();
    virtual ~DinoUiPartsTeamInfo();
    virtual void Idle();

    // Shows the saved team's members.
    void SetTeam(u32 team);

    class MSmartOam *mOam;                      // 0x74
    class DinoUiFieldMenuNumber3 *mNumbers[10]; // 0x78: two per member (level, then the other)
    class MIndicator *mTexts[5];                // 0xA0: the members' names
    class MIndicator *mName;                    // 0xB4: the team's name, for the current team
    class MIndicator *mOtherName;               // 0xB8: for the others
    u32 unkBC[2];
    class MMotion *mMotions[5];                 // 0xC4: by member
    class MParallelTaskSet *mMotionTasks;       // 0xD8
    class DinoUiPartsTeamTech *mTech;           // 0xDC
};

#endif
