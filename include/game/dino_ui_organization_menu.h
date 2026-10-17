// The team organization menu.
#ifndef GAME_DINO_UI_ORGANIZATION_MENU_H
#define GAME_DINO_UI_ORGANIZATION_MENU_H

#include "game/dino_resource.h"
#include <vector>
#include "m/menu.h"
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

    u32 mUnkB4[62];                     // 0xB4
    std::vector<void *> mUnk1AC;        // 0x1AC
    std::vector<void *> mUnk1B8;        // 0x1B8
    std::vector<void *> mUnk1C4;        // 0x1C4
    std::vector<void *> mUnk1D0;        // 0x1D0
    std::vector<void *> mUnk1DC;        // 0x1DC
    std::vector<void *> mUnk1E8;        // 0x1E8
    u32 mUnk1F4[19];                    // 0x1F4
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

    u8 unk94[0x4];
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
    virtual ~DinoUiOrganizationSlotCreateTask();
    virtual void Idle();
};

class DinoUiOrganizationMenuBlendResetTask : public MTask {
public:
    virtual ~DinoUiOrganizationMenuBlendResetTask();
    virtual void Execute();
};

#endif
