#ifndef GAME_DINO_UI_ISLAND_MAP_MENU_H
#define GAME_DINO_UI_ISLAND_MAP_MENU_H

#include "m/archive.h"
#include "m/picture.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class MSmartOam;
class MRasterWindow;
class MMotion;

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

    u32 mUnkB4;             // 0xB4
    MSmartOam *mOam;        // 0xB8
    MMenuItem *mItems[10];  // 0xBC: one per place on the map
    MRasterWindow *mWindow; // 0xE4: the spotlight on the focused place
    MMotion *mNameMotion;   // 0xE8: the focused place's name plate
    MMotion *mMarkMotion;   // 0xEC
    MTaskSet *mSpotTasks;   // 0xF0: the motions and text of the focused place
    MTaskSet *mMoveTasks;   // 0xF4: the scroll and spotlight moves
    MTask *mUnkF8;          // 0xF8
    MMenuItem *mBackButton; // 0xFC: B goes to it
    MPictureFile mMap;      // 0x100: image/all_map
    MIndexBitmap *mBitmap;  // 0x134
    MPalUnit *mPalette;    // 0x138
    MPoint mScroll;         // 0x13C: the map's offset
    u32 mUnk144;            // 0x144
    int *mResult;           // 0x148
    u32 mUnk14C;            // 0x14C
    MTaskSet *mUnk150;      // 0x150
    u32 mUnk154;            // 0x154
    u8 mSilentFocus;        // 0x158: no focus SE for the next focus change
};

#endif
