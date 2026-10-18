#ifndef GAME_DINO_UI_REVIVE_STATUS_MENU_H
#define GAME_DINO_UI_REVIVE_STATUS_MENU_H

#include "game/dino_ui_field_menu_cursor.h"
#include "game/dino_resource.h"
#include "m/motion.h"
#include <string>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/archive.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiReviveStatusParts : public MProcess {
public:
    virtual ~DinoUiReviveStatusParts();
    virtual void Idle();

    u32 mUnk74[5];      // 0x74
    std::string mText;  // 0x88
};

class DinoUiReviveStatusMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiReviveStatusMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();

    u8 unkB3[0x9];
    MMenuItem *mItems[4];   // 0xBC
    MSerialTaskSet *mCursorTasks;   // 0xCC
    DinoUiFieldMenuCursor *mCursor; // 0xD0
    u8 unkD4[0x50];
    MArchivePtr mArchive;   // 0x124
    MArchiveFile mFile;     // 0x128
};

class DinoUiReviveStatusLoop : public MProcess {
public:
    DinoUiReviveStatusLoop(u32 a, u32 b);
    virtual ~DinoUiReviveStatusLoop();
    virtual void Idle();

    u32 mUnk74;     // 0x74
    u32 mUnk78;     // 0x78
};

class DinoUiReviveStatusLoopCleaning : public MProcess {
public:
    virtual ~DinoUiReviveStatusLoopCleaning();
    virtual void Idle();
};

class DinoUiReviveStatusLoopBattle : public MProcess {
public:
    virtual ~DinoUiReviveStatusLoopBattle();
    virtual void Idle();
};

class DinoUiReviveStatusMenu;

class DinoUiReviveStatusMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiReviveStatusMenuItem();
    virtual MRect GetFrame();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiReviveStatusMenu *mOwner; // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiReviveStatusBackButton : public DinoUiReviveStatusMenuItem {
public:
    virtual ~DinoUiReviveStatusBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
