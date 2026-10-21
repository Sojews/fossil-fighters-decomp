#ifndef GAME_SET_VAR_TASK_H
#define GAME_SET_VAR_TASK_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "m/console_filter.h"

// Sets a variable, then exits.
class SetVarTask : public MTask {
public:
    SetVarTask(int *dst, int value) : mDst(dst), mValue(value) {}
    virtual ~SetVarTask();
    virtual void Execute();

    int *mDst;      // 0x10
    int mValue;     // 0x14
};

class DinoCommonFont;
class DinoBattleTalkWindow;
class MConsole;

// The battle talk window's console filter: knows the creatures on both sides.
class DinoBattleLiveConsoleFilter : public MConsoleFilter {
public:
    DinoBattleLiveConsoleFilter(DinoCommonFont *font, DinoBattleTalkWindow *window, u32 view) {
        mFont = font;
        mWindow = window;
        mView = view;
    }
    virtual ~DinoBattleLiveConsoleFilter();
    // Handles the window's $ commands.
    virtual BOOL Filter(MConsole *console, int c);
    // Takes in each side's creatures (by their B4 id).
    inline void Refresh();

    DinoCommonFont *mFont;          // 0x04
    DinoBattleTalkWindow *mWindow;  // 0x08
    u32 mView;                      // 0x0C: the DinoBattleView
    u32 mCreatures[2][4];           // 0x10: each one's unkA8, by side
};

#endif
