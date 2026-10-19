#ifndef GAME_DINO_UI3_PARTS_KASEKI_BUTTON_H
#define GAME_DINO_UI3_PARTS_KASEKI_BUTTON_H

#include "m/view.h"
#include "m/menu_item.h"
#include "m/motion.h"

class MSmartOam;

// What a fossil button shows (copied in halfwords; its first two bytes pick the motion and its anime).
struct DinoUiKasekiIcon {
    u8 unk00;       // /30: which motion
    u8 unk01;       // the anime, less one
    u16 unk02;
    u16 unk04;
};

class DinoUi3PartsKasekiButton : public MMenuItem {
public:
    DinoUi3PartsKasekiButton(MSmartOam *oam, MPoint pos, DinoUiKasekiIcon icon, u8 marked);
    virtual ~DinoUi3PartsKasekiButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    void Setup(MSmartOam *oam, DinoUiKasekiIcon icon);
    void SetMarked(BOOL marked);
    void SetActive(BOOL active);
    void HideMotion();

    MPoint mPos;        // 0x88: the center
    MMotion *mMotion;   // 0x90
    MView *mView;       // 0x94: the focus highlight
    MView *mMark;       // 0x98
    u8 unk9C[0xB0 - 0x9C];
};

#endif
