#ifndef GAME_DINO_UI_CLEANING_LEAVE_MENU_H
#define GAME_DINO_UI_CLEANING_LEAVE_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_ui_container_base_menu_item_menu.h"
#include "game/dino_field_camera_hook.h"

class DinoUiCleaningLeaveMenu : public DinoUiContainerBaseMenu {
public:
    DinoUiCleaningLeaveMenu(std::vector<void *> *chosen);
    virtual ~DinoUiCleaningLeaveMenu();
    virtual BOOL TryDecide();
    virtual void Setup();
    virtual void GetEntry(u32 index, DinoBackupEntry *entry);
    virtual BOOL CanMark();
    virtual BOOL IsMarkable(DinoBackupEntry *entry);
    virtual MTask *CreateUnk2C();
    void FinishDecide();

    BOOL mConfirmed;                // 0x158: the yes/no's answer
    std::vector<void *> *mChosen;   // 0x15C: gets the marked entries
};

#endif
