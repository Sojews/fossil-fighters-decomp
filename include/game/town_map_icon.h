#ifndef GAME_TOWN_MAP_ICON_H
#define GAME_TOWN_MAP_ICON_H

#include "m/task.h"
#include "m/motion.h"

class MMotion;

class MSmartOam;

class TownMapIcon : public MProcess {
public:
    inline TownMapIcon(MSmartOam *oam);
    virtual ~TownMapIcon();
    virtual void Idle();

    void SetPosition(MPoint pos) {
        for (int i = 0; i < 2; i++) {
            mMotions[i]->SetPosition(pos);
        }
    }
    void Show() {
        for (int i = 0; i < 2; i++) {
            mMotions[i]->Show();
        }
    }
    void Hide() {
        for (int i = 0; i < 2; i++) {
            mMotions[i]->Hide();
        }
    }

    MMotion *mMotions[2];   // 0x74
};

#endif
