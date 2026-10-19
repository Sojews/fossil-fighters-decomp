#ifndef GAME_INFO_PANEL_WINDOW_H
#define GAME_INFO_PANEL_WINDOW_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MTextPicture;
class MPictureFile;
class MSmartOam;

class InfoPanelWindow : public MProcess {
public:
    InfoPanelWindow(MPictureFile *file, MSmartOam *oam);
    virtual ~InfoPanelWindow();
    virtual void Idle();

    MTextPicture *mPicture;     // 0x74
    MSmartOam *mOam;            // 0x78
    u32 mUnk7C;                 // 0x7C
};

class DinoBattleView;
class MIndicator;

// A number that pops up over a creature, bounces and goes. kind: 0 the value's own color (up or down), 3 rises
// without bouncing, others that color.
class DinoBattleViewPopNumber : public MProcess {
public:
    DinoBattleViewPopNumber(DinoBattleView *view, MSmartOam *oam, MPoint pos, int value, int arg4, int kind);
    virtual ~DinoBattleViewPopNumber();
    virtual void Idle();

    static int GetCount();          // the numbers still popping
    static MTask *CreateWaitTask(); // waits for them

    MIndicator *mIndicator;     // 0x74
};

#endif
