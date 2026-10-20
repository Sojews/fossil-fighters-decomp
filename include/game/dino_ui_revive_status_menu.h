#ifndef GAME_DINO_UI_REVIVE_STATUS_MENU_H
#define GAME_DINO_UI_REVIVE_STATUS_MENU_H

#include "game/dino_ui_field_menu_cursor.h"
#include "game/dino_resource.h"
#include "game/dino_backup.h"
#include "m/motion.h"
#include <string>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/archive.h"
#include "m/resource.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MSmartOam;
class MTextPicture;
class DinoUiReviveStatusMenuItem;
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
    DinoUiReviveStatusMenu(u32 id, u32 *result, u8 flag);
    DinoUiReviveStatusMenu(const DinoCreatureState &creature, u32 *result, u8 flag);
    DinoUiReviveStatusMenu(const DinoCreatureState &creature, const DinoCreatureState &creature2, u8 flag);
    DinoUiReviveStatusMenu(const DinoCreatureState &creature, const DinoCreatureState &creature2, u32 *result,
                           u8 flag);
    virtual ~DinoUiReviveStatusMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    void SetPage(u8 page);
    void SetTab(u8 tab);
    // Fades out (to white, or black) and exits, stopping the music first if it should.
    // Fades both screens in (from white, or black).
    void FadeIn();
    void Leave();
    // Says which attacks the revive taught the creature (if any).
    void ShowLearnedAttacks();

    u8 unkB3[1];
    MSmartOam *mOam;        // 0xB4: the items' sprites
    DinoUiReviveStatusMenuItem *mBack;  // 0xB8
    MMenuItem *mItems[4];   // 0xBC
    MParallelTaskSet *mCursorTasks; // 0xCC
    DinoUiFieldMenuCursor *mCursor; // 0xD0
    DinoUiRvInformation *mInfo;     // 0xD4
    MTextPicture *mNumber;  // 0xD8: the creature's number (top screen)
    MParallelTaskSet *mUnkDC;   // 0xDC
    MTaskSet *mUnkE0;       // 0xE0: waited on before leaving
    DinoCreatureState mCreature;    // 0xE4
    DinoCreatureState mCreature2;   // 0xFC
    u32 *mResult;           // 0x114
    u8 mUnk118;             // 0x118
    u32 mUnk11C;            // 0x11C
    u8 mUnk120;             // 0x120
    u8 mFlag;               // 0x121
    u8 mUnk122;             // 0x122
    MResource<DinoKasekiDefs> mKasekiDefs;  // 0x124
    DinoHelpDummyItem *mHelp134;    // 0x134
    DinoHelpDummyItem *mHelp138;    // 0x138
    u8 unk13C[0x160 - 0x13C];
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
    DinoUiReviveStatusBackButton(DinoUiReviveStatusMenu *owner);
    virtual ~DinoUiReviveStatusBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
