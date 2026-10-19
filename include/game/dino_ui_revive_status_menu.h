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

class MSmartOam;
class DinoUiReviveStatusParts : public MProcess {
public:
    virtual ~DinoUiReviveStatusParts();
    virtual void Idle();

    u32 mUnk74[5];      // 0x74
    std::string mText;  // 0x88
};

class DinoUiRvInformation;

class DinoUiReviveStatusMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    virtual ~DinoUiReviveStatusMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    void SetPage(u8 page);

    u8 unkB3[1];
    MSmartOam *mOam;        // 0xB4: the items' sprites
    u8 unkB8[4];
    MMenuItem *mItems[4];   // 0xBC
    MSerialTaskSet *mCursorTasks;   // 0xCC
    DinoUiFieldMenuCursor *mCursor; // 0xD0
    DinoUiRvInformation *mInfo;     // 0xD4
    u8 unkD8[0x4C];
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

#include "game/dino_event_revive_effect.h"

class DinoUiReviveStatusLoopCleaning : public MProcess {
public:
    DinoUiReviveStatusLoopCleaning(const DinoEventReviveCreature *creature, const DinoEventReviveCreature *creature2,
                                   u32 *done, u8 flag);
    virtual ~DinoUiReviveStatusLoopCleaning();
    virtual void Idle();

    DinoEventReviveCreature mCreature;  // 0x74
    DinoEventReviveCreature mCreature2; // 0x8C
    u32 *mDone;                         // 0xA4
    u8 mFlag;                           // 0xA8
    u32 mStep;                          // 0xAC
};

class DinoUiReviveStatusLoopBattle : public MProcess {
public:
    DinoUiReviveStatusLoopBattle(const DinoEventReviveCreature *creature, const DinoEventReviveCreature *creature2);
    virtual ~DinoUiReviveStatusLoopBattle();
    virtual void Idle();

    DinoEventReviveCreature mCreature;  // 0x74
    DinoEventReviveCreature mCreature2; // 0x8C
};

class DinoUiReviveStatusMenu;

class DinoUiReviveStatusMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoUiReviveStatusMenuItem(DinoUiReviveStatusMenu *owner, const std::string &path, MPoint pos, u32 textId);
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
