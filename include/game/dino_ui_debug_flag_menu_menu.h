#ifndef GAME_DINO_UI_DEBUG_FLAG_MENU_MENU_H
#define GAME_DINO_UI_DEBUG_FLAG_MENU_MENU_H

#include "m/font.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/dino_backup.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MTextCanvas;
class MSmartOam;

class MCanvas;

class DinoUiDebugFlagMenu : public MMenu {
public:
    DinoUiDebugFlagMenu(int mode);
    virtual ~DinoUiDebugFlagMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    void ShowPage();

    int mMode;          // 0xA8
    MTextCanvas *mCanvas;   // 0xAC
    MFont *mFont;       // 0xB0
    MSmartOam *mOam;    // 0xB4
    MTaskSet *mTasks;   // 0xB8: the cursor's moves
    MView *mCursor;     // 0xBC
    int mPage;          // 0xC0
    int mPageCount;     // 0xC4
    int mLastPageCount; // 0xC8: the flags on the last page
};

class DinoUiDebugFlagMenuItem : public MMenuItem {
public:
    virtual ~DinoUiDebugFlagMenuItem();
    virtual MRect GetFrame();
    virtual void OnDecide();

    BOOL GetValue() {
        if (mMode == 0) {
            return gDinoBackup->GetFlag(mId);
        } else {
            return gDinoBackup->GetSystemFlag(mId);
        }
    }
    void SetValue(BOOL value) {
        if (mMode == 0) {
            gDinoBackup->SetFlag(mId, value);
        } else {
            gDinoBackup->SetSystemFlag(mId, value);
        }
    }

    MCanvas *mCanvas;   // 0x88
    MFont *mFont;       // 0x8C
    MPoint mPos;        // 0x90
    u32 mId;            // 0x98
    int mMode;          // 0x9C: 0 flags, else system flags
};

#endif
