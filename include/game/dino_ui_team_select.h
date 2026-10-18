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
#include "game/dino_field_camera_hook.h"

class DinoUiTeamSelectMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiTeamSelectMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u8 unkB3[0x15];
    MMenuItem *mTeams[3];   // 0xC8
    u8 unkD4[0x24];
    MSerialTaskSet *mCursorTasks;   // 0xF8
    DinoUiFieldMenuCursor *mCursor; // 0xFC
    DinoUiPartsTeamInfo *mTeamInfo; // 0x100
    u8 unk104[0xC];
    int mSelected;          // 0x110

    void Close();
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
    virtual ~DinoUiTeamSelectTeamButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u32 unk94;
    MPoint mPos;        // 0x98
    MView *mView;       // 0xA0
    u32 mTextId;        // 0xA4
};

#endif
