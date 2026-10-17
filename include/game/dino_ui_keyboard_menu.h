#ifndef GAME_DINO_UI_KEYBOARD_MENU_H
#define GAME_DINO_UI_KEYBOARD_MENU_H

#include <string>
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

    u8 unkB4[0xE8];
    std::wstring mUnk19C;       // 0x19C
    std::wstring mUnk1A8;       // 0x1A8
    std::wstring mUnk1B4[3];    // 0x1B4
    std::wstring mUnk1D8;       // 0x1D8
};

#endif
