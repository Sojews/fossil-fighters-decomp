#ifndef GAME_DINO_UI2_PARTS_KASEKI_BUTTON_H
#define GAME_DINO_UI2_PARTS_KASEKI_BUTTON_H

#include "m/view.h"
#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "m/motion.h"
#include "game/help.h"
#include "game/dino_ui3_parts_kaseki_button.h"
#include "game/dino_ui_parts_kaseki_button.h"
#include "m/menu.h"
#include <vector>

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

// What the container entry's button shows (as DinoGetKasekiButtonInfo, but an uncleaned fossil shows as type 4
// unless the entry's unk10_3 is set).
DinoKasekiButtonInfo DinoUi2GetKasekiButtonInfo(const DinoBackupEntry *entry);
// What an empty slot's button shows.
DinoKasekiButtonInfo DinoUi2GetEmptyKasekiInfo();
// Links the items as a grid four wide: down to the next row (from the last row, to below), and left and right
// wrapping around each row.
void DinoUi2LinkKasekiGrid(MMenu *menu, std::vector<MMenuItem *> &items, MMenuItem *below);

#endif
