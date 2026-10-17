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
    void OnBackDecided();
    void CheckKeys();
    void OnSpotDecided();
    void SetSilentFocus(BOOL silent);
    BOOL IsSilentFocus();

    u32 mUnkB4[2];          // 0xB4
    MMenuItem *mItems[10];  // 0xBC: one per place on the map
    u32 mUnkE4[7];          // 0xE4
    MArchivePtr mArchive;   // 0x100
    MArchiveFile mFile104;  // 0x104
    MArchiveFile mFile110;  // 0x110
    MArchiveFile mFile11C;  // 0x11C
    MArchiveFile mFile128;  // 0x128
    MIndexBitmap *mBitmap;  // 0x134
    MPalBlock *mPalette;    // 0x138
    u32 mUnk13C[3];         // 0x13C
    int *mResult;           // 0x148
    u32 mUnk14C[3];         // 0x14C
    u8 mSilentFocus;        // 0x158: no focus SE for the next focus change
};

#endif
