#ifndef GAME_DINO_UI_SAVE_MENU_H
#define GAME_DINO_UI_SAVE_MENU_H

#include "m/task.h"
#include "game/help.h"

class DinoCommonWindowYesNoDialog;

class DinoUiSaveMenu : public MProcess, public DinoHelpBase {
public:
    DinoUiSaveMenu(u32 *result, u8 arg);
    virtual ~DinoUiSaveMenu();
    virtual void Idle();
    void StateFinish();
    void StateConfirmed();
    void StateStart();
    void StateAskSave();
    void StateAskOverwrite();
    void AddDialogHelp(DinoCommonWindowYesNoDialog *dialog);

    void (DinoUiSaveMenu::*mState)(); // 0x80
    u32 *mResult;                     // 0x88
    BOOL mUnk8C;                      // 0x8C: the dialogs' answer
    u8 mUnk90;                        // 0x90
    MTask *mUnk94;                    // 0x94
};

#endif
