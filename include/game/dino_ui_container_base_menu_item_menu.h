#ifndef GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_MENU_H
#define GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_MENU_H

#include "m/archive.h"
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiContainerBaseMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiContainerBaseMenu();
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();

    u32 mUnkB4[21];             // 0xB4
    MArchivePtr mArchive;       // 0x108
    MArchiveFile mFile;         // 0x10C
    u32 mUnk118[2];             // 0x118
    std::vector<void *> mUnk120;// 0x120
    u32 mUnk12C[7];             // 0x12C
    std::vector<u32> mUnk148;   // 0x148
};

#endif
