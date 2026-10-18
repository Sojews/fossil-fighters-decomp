#ifndef GAME_DINO_UI3_PARTS_KASEKI_BUTTON_H
#define GAME_DINO_UI3_PARTS_KASEKI_BUTTON_H

#include "m/view.h"
#include "m/menu_item.h"
#include "m/motion.h"

class DinoUi3PartsKasekiButton : public MMenuItem {
public:
    virtual ~DinoUi3PartsKasekiButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    void SetMarked(BOOL marked);
    void SetActive(BOOL active);
    void HideMotion();

    MPoint mPos;        // 0x88: the center
    MMotion *mMotion;   // 0x90
    MView *mView;       // 0x94: the focus highlight
    MView *mMark;       // 0x98
};

#endif
