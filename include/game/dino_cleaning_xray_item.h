#ifndef GAME_DINO_CLEANING_XRAY_ITEM_H
#define GAME_DINO_CLEANING_XRAY_ITEM_H

#include <string>
#include "m/motion.h"
#include "game/help.h"
#include "m/menu_item.h"
#include "game/dino_cleaning.h"
#include "game/dino_cleaning_kaseki_rock.h"

extern "C" u32 data_ov002_02165418[]; // the file's pool; words 1-5 are the tools' motions

class DinoCleaningXrayItem : public DinoHelpItem, public MMenuItem {
public:
    DinoCleaningXrayItem(DinoCleaning *cleaning, MSmartOam *oam) : mCleaning(cleaning) {
        mMotion = new MMotion(std::string(((const char **)data_ov002_02165418)[2]), oam, 0, 1);
        mMotion->SetPriority(1);
        mMotion->SetAnchor(3);
        mMotion->SetPosition(0x80, 0x60);
        mMotion->Play(2, 0);
        mMotion->Show();
        mParallel.AddTail(mMotion);
        SetNoFocus(TRUE);
    }
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
