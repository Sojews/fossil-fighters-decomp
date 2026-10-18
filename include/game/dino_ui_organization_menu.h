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

class DinoUiFieldMenuFraction;

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
    void FinishPageRight();
    void FinishPageLeft();
    void SetTab(u8 tab);
    void TabLeft();
    void TabRight();

    u32 mUnkB4[5];                      // 0xB4
    MMenuItem *mTabs[4];                // 0xC8
    u32 mUnkD8[16];                     // 0xD8
    MMenuItem *mSlots[15];              // 0x118
    u32 mUnk154[15];                    // 0x154
    u32 mSlotCount;                     // 0x190
    u32 mUnk194[3];                     // 0x194
    DinoUiFieldMenuFraction *mFraction; // 0x1A0: "page n/m"
    u32 mUnk1A4[2];                     // 0x1A4
    std::vector<void *> mUnk1AC;        // 0x1AC
    std::vector<void *> mUnk1B8;        // 0x1B8
    std::vector<void *> mUnk1C4;        // 0x1C4
    std::vector<void *> mUnk1D0;        // 0x1D0
    std::vector<void *> mUnk1DC;        // 0x1DC
    std::vector<void *> mUnk1E8;        // 0x1E8
    u32 mPage;                          // 0x1F4
    u32 mUnk1F8;                        // 0x1F8
    u8 mTab;                            // 0x1FC
    u8 unk1FD;
    u8 mFocusTabs;                      // 0x1FE: focus the new tab's item when paging
    u8 unk1FF;
    u32 mUnk200[15];                    // 0x200
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
