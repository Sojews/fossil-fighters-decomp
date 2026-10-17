#ifndef GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_MENU_H
#define GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_MENU_H

#include "m/blend.h"
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUi2ContainerBaseMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUi2ContainerBaseMenu();

    u32 mUnkB4[22];                 // 0xB4
    std::vector<void *> mUnk10C;    // 0x10C
    u32 mUnk118[7];                 // 0x118
    std::vector<unsigned short> mUnk134; // 0x134: (the element type is a guess, distinct from other overlays')
};

class DinoUi2CannonShootMenu : public DinoUi2ContainerBaseMenu {
public:
    virtual ~DinoUi2CannonShootMenu();
};

#endif
