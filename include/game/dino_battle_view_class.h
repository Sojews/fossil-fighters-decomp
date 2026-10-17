// The battle's view: the screens' processes, driven by the model's messages.
#ifndef GAME_DINO_BATTLE_VIEW_CLASS_H
#define GAME_DINO_BATTLE_VIEW_CLASS_H

#include "m/task.h"
#include "m/observer.h"
#include "m/view.h"

class DinoBattleViewPlayer;

struct DinoBattleViewUnkE0 {
    u8 unk00[0x140];
    void *mUnk140;  // 0x140
};

class DinoBattleView : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleView();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void *GetUnk11C();
    u32 GetUnk118();
    u32 GetUnk120();
    u32 GetUnk124();
    DinoBattleViewPlayer *GetUnkE4(int index);
    u8 GetUnk138();
    void SetUnk128(u32 value);
    void SetUnk12C(u32 value);
    u32 GetUnk12C();
    u32 GetUnk130();
    u32 GetUnk134();
    u32 GetUnkEC();
    u32 GetUnk104();
    u32 GetUnk108();
    void IncUnk13C();
    void DecUnk13C();
    int GetUnk13C();

    u32 unkA8[4];
    MMessageSubject mSubject;   // 0xB8
    DinoBattleViewUnkE0 *mUnkE0; // 0xE0
    DinoBattleViewPlayer *mUnkE4[2]; // 0xE4: players
    u32 mUnkEC;                 // 0xEC
    u32 unkF0[5];
    u32 mUnk104;                // 0x104
    u32 mUnk108;                // 0x108
    u32 unk10C[3];
    u32 mUnk118;                // 0x118
    void *mUnk11C;              // 0x11C
    u32 mUnk120;                // 0x120
    u32 mUnk124;                // 0x124
    u32 mUnk128;                // 0x128
    u32 mUnk12C;                // 0x12C
    u32 mUnk130;                // 0x130
    u32 mUnk134;                // 0x134
    u8 mUnk138;                 // 0x138
    int mUnk13C;                // 0x13C
};

#endif
