#ifndef GAME_DINO_UI_MUSEUM_INFORMATION_MENU_H
#define GAME_DINO_UI_MUSEUM_INFORMATION_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MSmartOam;
class DinoUiMuseum3DModel;
class MIndicator;
class MConsole;

class DinoUiMuseumInformationMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    struct Info;
    DinoUiMuseumInformationMenu(Info *info, u32 *page, DinoUiMuseum3DModel *model);
    virtual ~DinoUiMuseumInformationMenu();
    virtual void OnStart();
    virtual void OnIdle();
    void OnBackDecided();
    void PageRight();
    void PageLeft();
    void PlayVoice();
    void Refresh();

    MSmartOam *mOam;    // 0xB4: the items' sprites
    u8 unkB8[0xCC - 0xB8];
    MIndicator *mNumber;        // 0xCC
    MView *mTypes[6];           // 0xD0: one shown, for the creature's element
    MIndicator *mName;          // 0xE8
    MConsole *mDescription;     // 0xEC
    struct Info {
        u32 *mCreatures;    // 0x00: one per page
        u32 mPageCount;     // 0x04
    } *mInfo;           // 0xF0
    u32 *mPage;         // 0xF4
    u32 mStartPage;     // 0xF8
    DinoUiMuseum3DModel *mModel;    // 0xFC
};

#endif
