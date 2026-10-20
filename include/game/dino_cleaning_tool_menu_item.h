#ifndef GAME_DINO_CLEANING_TOOL_MENU_ITEM_H
#define GAME_DINO_CLEANING_TOOL_MENU_ITEM_H

#include "game/dino_resource.h"
#include "game/help.h"
#include "m/menu_item.h"
#include <string>
#include "m/motion.h"
#include "game/dino_se_motion.h"

class DinoCleaning;

class DinoCleaningToolMenuItem : public DinoHelpItem, public MMenuItem {
public:
    DinoCleaningToolMenuItem(DinoCleaning *cleaning, MSmartOam *oam, const char *motion, u32 *selected,
                             const u32 &textId, u32 tool)
        : mCleaning(cleaning), mTextId(textId), mSelected(selected), mTool(tool) {
        mMotion = new DinoSeMotion(std::string(motion), oam, 0, 1);
        ((DinoSeMotion *)mMotion)->AddSe(0x42);
        mMotion->Play(1, 0);
        mMotion->SetPriority(1);
        mMotion->SetAnchor(3);
        mMotion->SetPosition(0x80, 0x60);
        mMotion->Show();
        mParallel.AddTail(mMotion);
    }
    virtual ~DinoCleaningToolMenuItem();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
    virtual MRect GetFrame();
    virtual void OnDecide();
    virtual void OnEnableChanged(BOOL enabled);
    virtual void OnFocusChanged(BOOL focused);

    DinoCleaning *mCleaning;    // 0x94
    u32 mTextId;        // 0x98
    u32 *mSelected;     // 0x9C: gets the tool when it's focused
    u32 mTool;          // 0xA0: 0-3
    MMotion *mMotion;   // 0xA4
};

#endif
