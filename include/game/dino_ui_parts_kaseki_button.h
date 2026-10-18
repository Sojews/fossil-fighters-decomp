#ifndef GAME_DINO_UI_PARTS_KASEKI_BUTTON_H
#define GAME_DINO_UI_PARTS_KASEKI_BUTTON_H

#include "m/view.h"
#include "m/menu_item.h"
#include "m/motion.h"

// What a kaseki button shows.
struct DinoKasekiButtonInfo {
    u8 icon;            // 0x00: picks the icon's motion (of 30)
    u8 color;           // 0x01: the icon's anime is 1 + color
    u8 type : 4;        // 0x02: 3 = badge; 1, 2 = level, or the badge from level 125; 5 = level, other label
    u8 unk2_4 : 4;
    u8 unk3;            // 0x03
    u16 level : 12;     // 0x04
    u16 unk4_12 : 4;
};

class DinoUiPartsKasekiButton : public MMenuItem {
public:
    DinoUiPartsKasekiButton(MSmartOam *oam, MPoint pos, DinoKasekiButtonInfo info, u8 marked);
    virtual ~DinoUiPartsKasekiButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    void Setup(MSmartOam *oam, DinoKasekiButtonInfo info);
    void SetMarked(BOOL marked);
    void HideMotion();

    MPoint mPos;        // 0x88: the center
    MMotion *mMotion;   // 0x90
    MMotion *mView;     // 0x94: the focus highlight
    MMotion *mMark;     // 0x98
    MMotion *mLevel[4]; // 0x9C: three digits, then the label
    MMotion *mBadge;    // 0xAC
};

#endif
