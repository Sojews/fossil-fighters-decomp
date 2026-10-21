#ifndef GAME_DINO_BATTLE_VIEW_EFFECT_GAME_BEGIN_H
#define GAME_DINO_BATTLE_VIEW_EFFECT_GAME_BEGIN_H

#include <string>
#include "m/font.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewEffectGameBeginPlayerNamePanel : public MMotion {
public:
    DinoBattleViewEffectGameBeginPlayerNamePanel(MSmartOam *oam, std::string name, u32 arg2, u32 arg3, bool arg4);
    virtual ~DinoBattleViewEffectGameBeginPlayerNamePanel();
    virtual void Execute();
    virtual MRect GetFrame();
    virtual MPoint GetPosition();
    virtual void SetPosition(MPoint pos);
    virtual void Show();
    virtual void Hide();

    MFont *mFont;       // 0x7C
    class MSmartObjCanvas *mUnk80;  // 0x80: the name
    MMotion *mUnk84;    // 0x84
    BOOL mUnk88;        // 0x88
    u8 mUnk8C;          // 0x8C
    int mUnk90;         // 0x90: the name's width
};

class DinoBattleViewEffectGameBegin : public MProcess {
public:
    DinoBattleViewEffectGameBegin(u32 unk7C, std::string name, std::string name2);
    virtual ~DinoBattleViewEffectGameBegin();
    virtual void Idle();

    void Start();
    void StateFadeIn();
    void StateNames();

    void (DinoBattleViewEffectGameBegin::*mState)(); // 0x74
    u32 mUnk7C;             // 0x7C: the DinoBattleView
    std::string mNames[2];  // 0x80
    DinoBattleViewEffectGameBeginPlayerNamePanel *mPanels[2];   // 0x98
    MSmartOam *mOam;        // 0xA0
    MMotion *mVs;           // 0xA4
    class MTextPicture *mBack;  // 0xA8
};

#endif
