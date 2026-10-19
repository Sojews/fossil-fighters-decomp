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
class DinoUiMuseumInformationMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    struct Info;
    DinoUiMuseumInformationMenu(Info *info, u32 *page, u32 unkFC);
    virtual ~DinoUiMuseumInformationMenu();
    virtual void OnStart();
    virtual void OnIdle();
    void OnBackDecided();
    void PageRight();
    void PageLeft();
    void PlayVoice();

    MSmartOam *mOam;    // 0xB4: the items' sprites
    u8 unkB8[0x38];
    struct Info {
        u32 *mCreatures;    // 0x00: one per page
        u32 mPageCount;     // 0x04
    } *mInfo;           // 0xF0
    u32 *mPage;         // 0xF4
    u32 mStartPage;     // 0xF8
    u32 mUnkFC;         // 0xFC
};

#endif
