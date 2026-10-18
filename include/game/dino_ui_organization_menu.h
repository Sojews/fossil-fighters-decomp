// The team organization menu.
#ifndef GAME_DINO_UI_ORGANIZATION_MENU_H
#define GAME_DINO_UI_ORGANIZATION_MENU_H

#include "game/dino_resource.h"
#include <vector>
#include "m/menu.h"
#include "m/input.h"
#include "m/motion.h"
#include "m/archive.h"
#include "game/help.h"

class DinoUiOrganizationMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiOrganizationMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    void CreateSlots();
    void CreateSlot(u32 index);
    void ClearSlots();
    void RemoveSlotLinks();

    u32 mUnkB4[25];                     // 0xB4
    MMenuItem *mSlots[15];              // 0x118
    u32 mUnk154[15];                    // 0x154
    u32 mSlotCount;                     // 0x190
    u32 mUnk194[6];                     // 0x194
    std::vector<void *> mUnk1AC;        // 0x1AC
    std::vector<void *> mUnk1B8;        // 0x1B8
    std::vector<void *> mUnk1C4;        // 0x1C4
    std::vector<void *> mUnk1D0;        // 0x1D0
    std::vector<void *> mUnk1DC;        // 0x1DC
    std::vector<void *> mUnk1E8;        // 0x1E8
    u32 mUnk1F4[18];                    // 0x1F4
    u8 unk23C[2];
    u8 mUnk23E;                         // 0x23E
    u8 unk23F;
    std::vector<MMotionFile> mMotions;  // 0x240
    u32 mUnk24C[4];                     // 0x24C
    MArchivePtr mArchive;               // 0x25C
    MArchiveFile mFile;                 // 0x260
};

class DinoUiOrganizationMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiOrganizationMenuItem();
    virtual MRect GetFrame();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    void *mOwner;       // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiOrganizationOKButton : public DinoUiOrganizationMenuItem {
public:
    virtual ~DinoUiOrganizationOKButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOrganizationBackButton : public DinoUiOrganizationMenuItem {
public:
    virtual ~DinoUiOrganizationBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOrganizationPageLButton : public DinoUiOrganizationMenuItem {
public:
    virtual ~DinoUiOrganizationPageLButton();
    virtual void OnDecide();
};

class DinoUiOrganizationPageRButton : public DinoUiOrganizationMenuItem {
public:
    virtual ~DinoUiOrganizationPageRButton();
    virtual void OnDecide();
};

class DinoUiOrganizationSlotCreateTask : public MProcess {
public:
    DinoUiOrganizationSlotCreateTask(DinoUiOrganizationMenu *menu);
    virtual ~DinoUiOrganizationSlotCreateTask();
    virtual void Idle();

    DinoUiOrganizationMenu *mMenu;  // 0x74
    u32 mIndex;                     // 0x78
};

class DinoUiOrganizationMenuBlendResetTask : public MTask {
public:
    virtual ~DinoUiOrganizationMenuBlendResetTask();
    virtual void Execute();
};

#endif
