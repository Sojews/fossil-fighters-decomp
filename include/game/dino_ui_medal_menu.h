// The medal list menu.
#ifndef GAME_DINO_UI_MEDAL_MENU_H
#define GAME_DINO_UI_MEDAL_MENU_H

#include <vector>
#include "m/menu.h"
#include "m/motion.h"
#include "game/help.h"

class DinoUiMedalMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiMedalMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u32 mUnkB4[58];                     // 0xB4
    std::vector<void *> mUnk19C;        // 0x19C
    std::vector<void *> mUnk1A8;        // 0x1A8
    std::vector<void *> mUnk1B4;        // 0x1B4
    u32 mUnk1C0[18];                    // 0x1C0
    std::vector<MMotionFile> mMotions;  // 0x208
};

#endif
