#ifndef GAME_DINO_UI_ATM_MENU_H
#define GAME_DINO_UI_ATM_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoUiATMBackButton;
class DinoUiATMTeamButton;
class DinoUiPartsTeamIndicator;
class DinoUiPartsTeamInfo;
class MIndicator;

class DinoUiATMCursor;
class DinoUiFieldMenuCursor;
class DinoCommonWindowMessage;

class DinoUiATMMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiATMMenu(u32 *result, u8 arg1, DinoUiPartsTeamInfo *teamInfo, u8 arg3);
    virtual ~DinoUiATMMenu();
    void OnBackDecided();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void ExitMoveMode();
    void MoveTeam(int team);
    void RefreshButtons();
    void Leave();
    void RefreshTeams();
    void Unk02173d08();
    void ClearTeam();
    void LinkTeamButtons();

    u32 unkB4;          // 0xB4
    DinoUiATMBackButton *mBackButton; // 0xB8
    u32 unkBC;          // 0xBC
    DinoUiATMTeamButton *mTeamButtons[3]; // 0xC0
    DinoUiPartsTeamIndicator *mTeamNames[3]; // 0xCC
    MIndicator *mTeamTexts[3];  // 0xD8
    MView *mEmptyMarks[3];      // 0xE4
    MTaskSet *mFocusTasks;      // 0xF0
    DinoUiFieldMenuCursor *mFocusCursor;    // 0xF4
    u32 unkF8;                  // 0xF8
    DinoUiATMCursor *mCursor;   // 0xFC: marks the team being moved
    u32 unk100[2];      // 0x100
    DinoUiPartsTeamInfo *mTeamInfo; // 0x108
    DinoCommonWindowMessage *mMessage;  // 0x10C
    u32 *mResult;       // 0x110
    int mUnk114;        // 0x114
    int mUnk118;        // 0x118
    u32 mUnk11C;        // 0x11C
    int mUnk120;        // 0x120
    u8 mUnk124;         // 0x124
    u8 mUnk125;         // 0x125
};

#endif
