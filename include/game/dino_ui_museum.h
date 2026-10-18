#ifndef GAME_DINO_UI_MUSEUM_H
#define GAME_DINO_UI_MUSEUM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiMuseum : public MProcess {
public:
    DinoUiMuseum();
    virtual ~DinoUiMuseum();
    virtual void Idle();
};

class DinoUiMuseumMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiMuseumMenu();
    virtual void OnIdle();

    void CreateSlots();
    void CreateSlot(u32 index);
    void ClearSlots();

    u8 unkB3[0xB8 - 0xB3];  // members start in MMenu's tail padding
    MMenuItem *mLinkFrom;   // 0xB8: linked down to the slots
    u8 unkBC[0xE4 - 0xBC];
    MMenuItem *mSlots[15];  // 0xE4
    u8 unk120[0x15C - 0x120];
    u32 mSlotCount;     // 0x15C
    u8 unk160[0x1AC - 0x160];
    u8 mUnk1AC;     // 0x1AC
};

class DinoUiMuseumEffectLight : public MProcess {
public:
    virtual ~DinoUiMuseumEffectLight();
    virtual void Idle();
};

class DinoUiMuseumMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiMuseumMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    void *mOwner;       // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiMuseumBackButton : public DinoUiMuseumMenuItem {
public:
    virtual ~DinoUiMuseumBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMuseumPageLButton : public DinoUiMuseumMenuItem {
public:
    virtual ~DinoUiMuseumPageLButton();
    virtual void OnDecide();
};

class DinoUiMuseumPageRButton : public DinoUiMuseumMenuItem {
public:
    virtual ~DinoUiMuseumPageRButton();
    virtual void OnDecide();
};

class DinoUiMuseumSlotCreateTask : public MProcess {
public:
    virtual ~DinoUiMuseumSlotCreateTask();
    virtual void Idle();

    DinoUiMuseumMenu *mMenu;    // 0x74
    u32 mIndex;         // 0x78
};

class DinoUiMuseumInformationMenu;

class DinoUiMuseumInformationMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiMuseumInformationMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiMuseumInformationMenu *mOwner; // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiMuseumInformationBackButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMuseumInformationPageLButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationPageLButton();
    virtual void OnDecide();
};

class DinoUiMuseumInformationPageRButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationPageRButton();
    virtual void OnDecide();
};

class DinoUiMuseumInformationVoiceButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationVoiceButton();
    virtual void OnDecide();
};

class DinoUiMuseumReviverButton : public MMenuItem {
public:
    virtual ~DinoUiMuseumReviverButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    MMotion *mMotion;   // 0x88
    u32 mUnk8C;         // 0x8C
    u8 mBaseAnime;      // 0x90
};

#endif
