#ifndef GAME_DINO_BATTLE_VIEW_FIELD_PLAYER_BG_H
#define GAME_DINO_BATTLE_VIEW_FIELD_PLAYER_BG_H

#include "m/task.h"

class MTextPicture;
class DinoBattleView;

// The battles' fixed backgrounds (/etc/btl_bg_defs).
struct DinoBattleBgDef {
    u32 battle;     // 0x00: the battle it's for
    u32 path;       // 0x04: offset of the picture's path
};
struct DinoBattleBgDefs {
    u32 unk0;
    u32 count;      // 0x04
    u32 entries;    // 0x08: offset of count offsets of DinoBattleBgDef

    const DinoBattleBgDef *Get(u32 i) const {
        return (const DinoBattleBgDef *)((const u8 *)this + ((const u32 *)((const u8 *)this + entries))[i]);
    }
    const char *GetPath(u32 i) const {
        const DinoBattleBgDef *def = Get(i);
        return (const char *)def + def->path;
    }
};

// The picture behind a side's creatures: slides off and the other side's slides on.
class DinoBattleViewFieldPlayerBG : public MProcess {
public:
    DinoBattleViewFieldPlayerBG(DinoBattleView *view);
    virtual ~DinoBattleViewFieldPlayerBG();
    virtual void Idle();

    BOOL SetSide(u32 side);
    void StateLoad();
    void StateShow();
    void StateHide();
    void StateDone();

    void (DinoBattleViewFieldPlayerBG::*mState)(); // 0x74
    u32 mSide;                  // 0x7C: 2 for none
    MTextPicture *mPicture;     // 0x80
    u8 mChanging;               // 0x84
    DinoBattleView *mView;      // 0x88
    u32 mFixed;                 // 0x8C: the battle's own background is shown and never changes
};

#endif
