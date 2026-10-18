#ifndef GAME_DINO_UI_PARTS_TEAM_TECH_H
#define GAME_DINO_UI_PARTS_TEAM_TECH_H

#include "m/motion.h"
#include "m/smart_oam.h"
#include "m/task.h"

// The team's three technique icons.
class DinoUiPartsTeamTech : public MProcess {
public:
    DinoUiPartsTeamTech(const MPoint *positions);
    virtual ~DinoUiPartsTeamTech();
    virtual void Idle();
    void Show();
    void Hide();
    void Raise(u32 index);
    void Lower();

    MSmartOam *mOam;            // 0x74
    MMotion *mIcons[3];         // 0x78
    MParallelTaskSet *mIconTasks; // 0x84
    MPoint mPositions[3];       // 0x88
};

#endif
