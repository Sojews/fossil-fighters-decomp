#ifndef GAME_DINO_UI_TEAM_SELECT_H
#define GAME_DINO_UI_TEAM_SELECT_H

#include "game/dino_ui_field_menu_cursor.h"
#include "m/view.h"
#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_ui_parts_team_info.h"
#include "game/dino_ui_parts_team_indicator.h"
#include "m/indicator.h"
#include "m/motion.h"
#include "game/dino_field_camera_hook.h"

class MSmartOam;
class MTextPicture;

class DinoUiTeamSelectMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiTeamSelectMenu(int *result, u8 unk108, u8 unk109);
    virtual ~DinoUiTeamSelectMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u8 unkB3;
    MSmartOam *mOam;            // 0xB4
    u8 unkB8[0xC4 - 0xB8];
    MTextPicture *mTitle;       // 0xC4
    MMenuItem *mTeams[3];   // 0xC8
    DinoUiPartsTeamIndicator *mNames[3];    // 0xD4: the saved teams' names
    MIndicator *mLabels[3];     // 0xE0
    MMotion *mUnkEC[3];         // 0xEC: shown over an empty team
    MParallelTaskSet *mCursorTasks; // 0xF8
    DinoUiFieldMenuCursor *mCursor; // 0xFC
    DinoUiPartsTeamInfo *mTeamInfo; // 0x100
    int *mResult;           // 0x104
    u8 mUnk108;             // 0x108
    u8 mUnk109;             // 0x109
    int mUnk10C;            // 0x10C: 1 once a team is chosen
    int mSelected;          // 0x110

    MSmartOam *GetOam() { return mOam; }
    void Close();
    void StateDecided();
    // Opens the menu to choose the team with (or not).
    void OpenMenu(int team);
    // Shows the saved teams' names (a blank for an empty team), highlighting the current one.
    void RefreshTeams();
};

class DinoUiTeamSelect : public MProcess {
public:
    DinoUiTeamSelect(u32 *result, u8 unk78, u8 unk79);
    virtual ~DinoUiTeamSelect();
    virtual void Idle();

    u32 *mResult;   // 0x74
    u8 mUnk78;      // 0x78
    u8 mUnk79;      // 0x79
};

class DinoUiTeamSelectTeamButton : public MMenuItem, public DinoHelpItem {
public:
    DinoUiTeamSelectTeamButton(DinoUiTeamSelectMenu *owner, MPoint pos, u32 textId);
    virtual ~DinoUiTeamSelectTeamButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiTeamSelectMenu *mOwner;   // 0x94
    MPoint mPos;        // 0x98
    MMotion *mView;     // 0xA0: shown while focused
    u32 mTextId;        // 0xA4
};

#endif
