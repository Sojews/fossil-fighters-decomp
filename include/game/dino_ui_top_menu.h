#ifndef GAME_DINO_UI_TOP_MENU_H
#define GAME_DINO_UI_TOP_MENU_H

#include "m/menu.h"
#include "game/help.h"

// The title screen's top menu.
class DinoUiTopMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiTopMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    void OnBackDecided();
    void OnSingleDecided();
    void OnMultiDecided();
    void OnPassDecided();
    void OnOptionDecided();
    u32 GetUnkB4();

    u8 unkB3[1];     // members start in MMenu's tail padding
    u32 mUnkB4;      // 0xB4
    u8 unkB8[0x10];
    int *mResult;    // 0xC8: which button was chosen
    u8 mSilentFocus; // 0xCC
};

#endif
