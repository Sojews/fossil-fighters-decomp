#ifndef GAME_DINO_UI_ISLAND_MAP_MENU_H
#define GAME_DINO_UI_ISLAND_MAP_MENU_H

#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoUiIslandMapMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiIslandMapMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u32 mUnkB4[19];         // 0xB4
    MArchivePtr mArchive;   // 0x100
    MArchiveFile mFile104;  // 0x104
    MArchiveFile mFile110;  // 0x110
    MArchiveFile mFile11C;  // 0x11C
    MArchiveFile mFile128;  // 0x128
    MIndexBitmap *mBitmap;  // 0x134
    MPalBlock *mPalette;    // 0x138
};

#endif
