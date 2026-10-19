#ifndef GAME_DINO_UI_CANON_PUTIN_MENU_H
#define GAME_DINO_UI_CANON_PUTIN_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_ui_container_base_menu_item_menu.h"
#include "game/dino_field_camera_hook.h"

class DinoUiCanonPutinMenu : public DinoUiContainerBaseMenu {
public:
    DinoUiCanonPutinMenu();
    virtual ~DinoUiCanonPutinMenu();
    virtual BOOL TryDecide();
    virtual void Setup();
    virtual void GetEntry(u32 index, DinoBackupEntry *entry);
    virtual BOOL CanMark();
    virtual BOOL IsMarkable(DinoBackupEntry *entry);
    virtual MTask *CreateUnk2C();
    // Moves the marked entries into the cannon, if the player said yes.
    void PutIn();

    u32 mUnk158;    // 0x158
};

class DinoUiCanonTakeoutMenu : public DinoUiContainerBaseMenu {
public:
    DinoUiCanonTakeoutMenu();
    virtual ~DinoUiCanonTakeoutMenu();
    virtual BOOL TryDecide();
    virtual void Setup();
    virtual void GetEntry(u32 index, DinoBackupEntry *entry);
    virtual BOOL CanMark();
    virtual BOOL IsMarkable(DinoBackupEntry *entry);
    virtual MTask *CreateUnk2C();
    // Moves the marked entries into the container, if the player said yes.
    void TakeOut();

    u32 mUnk158;        // 0x158
};

#endif
