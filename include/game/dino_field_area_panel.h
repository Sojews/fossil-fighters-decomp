#ifndef GAME_DINO_FIELD_AREA_PANEL_H
#define GAME_DINO_FIELD_AREA_PANEL_H

#include "m/task.h"

class MMotion;
class MIndicator;
class MFont;

// The panel that slides in with the area's name when the player enters it.
class DinoFieldAreaPanel : public MProcess {
public:
    DinoFieldAreaPanel();
    virtual ~DinoFieldAreaPanel();
    virtual void Idle();
    // Notes the area, and slides its name in and out if `show` (area 0 hides the panel).
    void SetMode(int area, int show);

    int mArea;              // 0x74
    MMotion *mFrame;        // 0x78
    MIndicator *mName;      // 0x7C
    MSerialTaskSet *mTasks; // 0x80
    MFont *mFont;           // 0x84
};

#endif
