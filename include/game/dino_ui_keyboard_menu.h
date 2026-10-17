#ifndef GAME_DINO_UI_KEYBOARD_MENU_H
#define GAME_DINO_UI_KEYBOARD_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiKeyboardMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiKeyboardMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

#endif
