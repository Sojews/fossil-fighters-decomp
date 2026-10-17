#ifndef GAME_DINO_CLEANING_TUTORIAL_WINDOW_H
#define GAME_DINO_CLEANING_TUTORIAL_WINDOW_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_common_tutorial_window.h"
#include "game/dino_field_camera_hook.h"

class DinoCleaningTutorialWindow : public DinoCommonTutorialWindow {
public:
    virtual ~DinoCleaningTutorialWindow();

    u32 mUnk9C;                 // 0x9C
    DinoHelpDisabler mDisabler; // 0xA0
};

#endif
