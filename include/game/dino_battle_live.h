#ifndef GAME_DINO_BATTLE_LIVE_H
#define GAME_DINO_BATTLE_LIVE_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_common_live.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleModel;
class DinoBattleView;

// What the battle model's 0x10C points at (only its view pointer is known).
struct DinoBattleLiveSource {
    u8 unk00[0xB4];
    MMessageObserver *mView;    // 0xB4: a DinoBattleView
};

class DinoBattleLive : public DinoCommonLive {
public:
    DinoBattleLive(DinoBattleModel *model, const char *name);
    virtual ~DinoBattleLive();
    virtual void Idle();
    virtual void Open();
    virtual void Signal(u32 id);
    virtual void Close();
    virtual void ShowTutorial(MDisplay *display, const char *text);
    virtual BOOL IsBusy();

    DinoBattleModel *mModel;        // 0xD4
    DinoBattleLiveSource *mSource;  // 0xD8
    DinoBattleView *mView;          // 0xDC
};

#endif
