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

class DinoUiMuseumInformationMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiMuseumInformationMenu();
    virtual void OnStart();
    virtual void OnIdle();
    void OnBackDecided();
    void PageRight();
    void PageLeft();
};

#endif
