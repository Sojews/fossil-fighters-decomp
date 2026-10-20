#ifndef GAME_DINO_UI_TOP_MENU_H
#define GAME_DINO_UI_TOP_MENU_H

#include "m/menu.h"
#include "game/help.h"

// The title screen's top menu.
class MSmartOam;

class DinoUiTopMenuItem;
class DinoUiTopMenuItem2;
class DinoUiTopMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiTopMenu(int *result);
    virtual ~DinoUiTopMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    void OnBackDecided();
    void OnSingleDecided();
    void OnMultiDecided();
    void OnPassDecided();
    void OnOptionDecided();
    MSmartOam *GetOam();

    u8 unkB3[1];     // members start in MMenu's tail padding
    MSmartOam *mOam;    // 0xB4: the items' sprites
    DinoUiTopMenuItem *mSingle;     // 0xB8
    DinoUiTopMenuItem *mMulti;      // 0xBC
    DinoUiTopMenuItem2 *mPass;      // 0xC0
    DinoUiTopMenuItem2 *mOption;    // 0xC4
    int *mResult;    // 0xC8: which button was chosen
    u8 mSilentFocus; // 0xCC
};

#endif
