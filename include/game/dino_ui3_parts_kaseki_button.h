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
    u8 kind : 4;    // 3 = badge; 1, 2 and 5 show the value (125 and up: badge)
    u8 unk02 : 4;
    u8 unk03;
    u16 value : 12;
    u16 unk04 : 4;
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
    MMotion *mView;     // 0x94: the focus highlight
    MMotion *mMark;     // 0x98
    MMotion *mDigits[4]; // 0x9C: the value's three digits, and its unit
    MMotion *mBadge;    // 0xAC
};

#endif
