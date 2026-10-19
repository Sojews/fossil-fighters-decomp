#ifndef GAME_DINO_UI2_PARTS_KASEKI_BUTTON_H
#define GAME_DINO_UI2_PARTS_KASEKI_BUTTON_H

#include "m/view.h"
#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "m/motion.h"
#include "game/help.h"
#include "game/dino_ui3_parts_kaseki_button.h"

class DinoUi2PartsKasekiButton : public MMenuItem, public DinoHelpItem {
public:
    DinoUi2PartsKasekiButton(MSmartOam *oam, MPoint pos, DinoUiKasekiIcon icon, u8 marked, u32 textId);
    virtual ~DinoUi2PartsKasekiButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    void Setup(MSmartOam *oam, DinoUiKasekiIcon icon);
    void SetMarked(BOOL marked);
    void SetActive(BOOL active);    // (does nothing here)
    void HideMotion();

    MPoint mPos;        // 0x94: the center
    MMotion *mMotion;   // 0x9C
    MView *mView;       // 0xA0: the focus highlight
    MView *mMark;       // 0xA4
    u8 unkA8[20];
    u32 mTextId;        // 0xBC
};

#endif
