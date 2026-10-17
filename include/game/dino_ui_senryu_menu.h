#ifndef GAME_DINO_UI_SENRYU_MENU_H
#define GAME_DINO_UI_SENRYU_MENU_H

#include <string>
#include "m/menu_item.h"
#include "m/font.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiSenryuMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiSenryuMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u8 unkB4[0x38];
    MView *mCursor;     // 0xEC
    u8 unkF0[0x30];
    MFont *mFont120;    // 0x120
    MFont *mFont124;    // 0x124
    MFont *mFont128;    // 0x128
    u32 unk12C;
    u32 mPage;          // 0x130
    u8 unk134[9];
    u8 mSilentFocus;    // 0x13D
    std::wstring mUnk140[126];  // 0x140
    std::wstring mUnk728[42];   // 0x728
};

#endif
