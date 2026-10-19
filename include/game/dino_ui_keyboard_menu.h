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
    DinoUiKeyboardMenu(int mode, u32 arg, int flag);
    virtual ~DinoUiKeyboardMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    MTask *CreateSetFocusTask(MMenuItem *item);
    MTask *CreatePageChangeTask(u32 page);

    u8 unkB4[0x13C - 0xB4];
    u32 mUnk13C;                // 0x13C
    u32 unk140;
    u32 mUnk144;                // 0x144
    u8 unk148[0x188 - 0x148];
    int mMode;                  // 0x188
    u32 mArg;                   // 0x18C
    u32 mUnk190;                // 0x190
    u8 unk194[0x19C - 0x194];
    std::wstring mUnk19C;       // 0x19C
    std::wstring mUnk1A8;       // 0x1A8
    std::wstring mUnk1B4[3];    // 0x1B4
    std::wstring mUnk1D8;       // 0x1D8
    u8 unk1E4[0x1F5 - 0x1E4];
    u8 mFlag;                   // 0x1F5
    u8 unk1F6[2];
};

#endif
