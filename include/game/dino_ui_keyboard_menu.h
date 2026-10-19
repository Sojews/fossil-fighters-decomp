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

class MTextPicture;

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
    void SetPage(int page);
    // Keeps the name (a default one if it's empty) and fades out.
    void OnBackDecided();

    u8 unkB4[0xE8 - 0xB4];
    MMenuItem *mUnkE8;          // 0xE8: focused after a page change
    u8 unkEC[0x13C - 0xEC];
    MTextPicture *mPagePicture; // 0x13C
    u32 unk140;
    u32 mUnk144;                // 0x144
    u8 unk148[0x158 - 0x148];
    MTask *mUnk158;             // 0x158
    u8 unk15C[0x188 - 0x15C];
    int mMode;                  // 0x188
    u32 mArg;                   // 0x18C
    int mPage;                  // 0x190
    MProcess *mUnk194;          // 0x194: waited on before exiting (its +0x84 flag is set first)
    u8 unk198[0x19C - 0x198];
    std::wstring mUnk19C;       // 0x19C
    std::wstring mUnk1A8;       // 0x1A8
    std::wstring mUnk1B4[3];    // 0x1B4
    std::wstring mUnk1D8;       // 0x1D8
    u8 unk1E4[0x1F4 - 0x1E4];
    u8 mUnk1F4;                 // 0x1F4
    u8 mFlag;                   // 0x1F5
    u8 unk1F6[2];
};

#endif
