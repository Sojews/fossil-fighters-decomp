#ifndef GAME_DINO_UI_ISLAND_MAP_MENU_H
#define GAME_DINO_UI_ISLAND_MAP_MENU_H

#include "m/archive.h"
#include "m/picture.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoUiIslandMapMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiIslandMapMenu(int *result, u32 arg);
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
    u32 mUnkE4;             // 0xE4
    u32 mUnkE8;             // 0xE8
    u32 mUnkEC[4];          // 0xEC
    MMenuItem *mBackButton; // 0xFC: B goes to it
    MPictureFile mMap;      // 0x100: image/all_map
    MIndexBitmap *mBitmap;  // 0x134
    MPalBlock *mPalette;    // 0x138
    u32 mUnk13C[2];         // 0x13C
    u32 mUnk144;            // 0x144
    int *mResult;           // 0x148
    u32 mUnk14C[3];         // 0x14C
    u8 mSilentFocus;        // 0x158: no focus SE for the next focus change
};

#endif
