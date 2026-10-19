// The battle's view: the screens' processes, driven by the model's messages.
#ifndef GAME_DINO_BATTLE_VIEW_CLASS_H
#define GAME_DINO_BATTLE_VIEW_CLASS_H

#include "m/task.h"
#include "m/observer.h"
#include "m/view.h"
#include "m/archive.h"
#include "m/font.h"

class DinoBattleViewPlayer;
class DinoBattleViewCreature;
class MSmartOam;

class DinoBattleViewUnkE0 : public MProcess, public MMessageObserver {
public:
    u8 unkA8[0xC0 - 0xA8];
    MView *mSlotViewsC0[0x1E];  // 0xC0: by slot group
    void *mUnk138;  // 0x138
    u8 unk13C[4];
    void *mUnk140;  // 0x140
};

class DinoBattleView : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleView();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    MFont *GetUnk11C();
    MFont *GetUnk118();
    MFont *GetUnk120();
    MFont *GetUnk124();
    DinoBattleViewPlayer *GetUnkE4(int index);
    DinoBattleViewCreature *FindCreatureByAC(u32 id);
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
    const void *GetUnk11CPalette();
    DinoBattleViewPlayer *GetPlayerByAC(u32 id);
    int GetUnk140();
    MTask *CreateEnableRootSelectTask();

    MArchivePtr mArchive;       // 0xA8
    MArchiveFile mFile;         // 0xAC
    MMessageSubject mSubject;   // 0xB8
    DinoBattleViewUnkE0 *mUnkE0; // 0xE0
    DinoBattleViewPlayer *mUnkE4[2]; // 0xE4: players
    u32 mUnkEC;                 // 0xEC
    u32 unkF0[4];
    u8 *mUnk100;                // 0x100
    u32 mUnk104;                // 0x104
    u32 mUnk108;                // 0x108
    MSmartOam *mUnk10C;         // 0x10C
    u32 unk110[2];
    MFont *mUnk118;             // 0x118
    MFont *mUnk11C;             // 0x11C
    MFont *mUnk120;             // 0x120
    MFont *mUnk124;             // 0x124
    u32 mUnk128;                // 0x128
    u32 mUnk12C;                // 0x12C
    u32 mUnk130;                // 0x130
    u32 mUnk134;                // 0x134
    u8 mUnk138;                 // 0x138
    int mUnk13C;                // 0x13C
    int mUnk140;                // 0x140
};

#endif
