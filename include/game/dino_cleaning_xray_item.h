#ifndef GAME_DINO_CLEANING_XRAY_ITEM_H
#define GAME_DINO_CLEANING_XRAY_ITEM_H

#include "m/motion.h"
#include "game/help.h"
#include "m/menu_item.h"
#include "game/dino_cleaning.h"
#include "game/dino_cleaning_kaseki_rock.h"

class DinoCleaningXrayItem : public DinoHelpItem, public MMenuItem {
public:
    virtual ~DinoCleaningXrayItem();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
    virtual MRect GetFrame();
    virtual void OnEnableChanged(BOOL enabled);
    virtual void OnDecide();

    DinoCleaning *mCleaning; // 0x94
    MMotion *mMotion; // 0x98
};

#endif
