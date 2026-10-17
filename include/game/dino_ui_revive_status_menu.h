#ifndef GAME_DINO_UI_REVIVE_STATUS_MENU_H
#define GAME_DINO_UI_REVIVE_STATUS_MENU_H

#include <string>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiReviveStatusParts : public MProcess {
public:
    virtual ~DinoUiReviveStatusParts();
    virtual void Idle();

    u32 mUnk74[5];      // 0x74
    std::string mText;  // 0x88
};

#endif
