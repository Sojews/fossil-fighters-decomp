#ifndef GAME_DINO_BATTLE_VIEW_ABILITY_TEXT_WINDOW_H
#define GAME_DINO_BATTLE_VIEW_ABILITY_TEXT_WINDOW_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MRectAnime;

class DinoBattleViewAbilityTextWindow : public MTask {
public:
    DinoBattleViewAbilityTextWindow(BOOL rows, u32 arg);
    virtual ~DinoBattleViewAbilityTextWindow();
    virtual void Execute();

    void StartAnime(MRect from, MRect to, u32 duration, int ease);
    void SetRect(MRect rect);
    MRect GetRect();
    void Show();
    void Hide();
    BOOL IsHidden();
    u8 GetUnk18();
    u32 GetUnk1C();
    void SetUnk1C(u32 value);
    MTask *CreateShowTask();

    class ShowTask : public MTask {
    public:
        ShowTask(DinoBattleViewAbilityTextWindow *window, u8 show) {
            mWindow = window;
            mShow = show;
        }
        virtual ~ShowTask();
        virtual void Execute();

        DinoBattleViewAbilityTextWindow *mWindow;   // 0x10
        u8 mShow;                                   // 0x14
    };

    u8 mShown;              // 0x10
    MRectAnime *mTask;      // 0x14
    u8 mUnk18;              // 0x18
    u32 mUnk1C;             // 0x1C
};

#endif
