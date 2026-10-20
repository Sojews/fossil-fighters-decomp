// The battle's view: the screens' processes, driven by the model's messages.
#ifndef GAME_DINO_BATTLE_VIEW_CLASS_H
#define GAME_DINO_BATTLE_VIEW_CLASS_H

#include "m/task.h"
#include "m/observer.h"
#include "m/view.h"
#include "m/archive.h"
#include "m/font.h"
#include "m/resource.h"

class DinoBattleViewPlayer;
class DinoBattleViewCreature;
class MSmartOam;

class DinoBattleView;
class DinoBattleViewBackdrop;
class DinoBattleViewActionInfoBar;
class DinoBattleViewFocusLCDFrame;
class DinoBattleView3D;
class KasekiPointPalette;
// The battle view's data file.
struct DinoBattleViewData {
    u32 unk00[2];
    u32 creatureOffsets;    // 0x08: offset of the creatures' MPoint offsets, by creature
};

// What the field shows for each side.
class DinoBattleViewFieldSlot : public MView {
public:
    u32 unk10;
    DinoBattleViewPlayer *mPlayer;  // 0x14
};

class DinoBattleViewUnkE0 : public MProcess, public MMessageObserver {
public:
    u32 unkA8;
    DinoBattleView *mView;      // 0xAC
    u32 unkB0;
    MTaskSet *mTasksB4;         // 0xB4
    MTaskSet *mTasksB8;         // 0xB8
    u32 unkBC;
    DinoBattleViewFieldSlot *mSlotViewsC0[2]; // 0xC0: by side
    u8 unkC8[0x120 - 0xC8];
    DinoBattleViewBackdrop *mBackdrop;  // 0x120
    u32 unk124;
    MTask *mTask128;            // 0x128
    MTask *mTask12C;            // 0x12C
    u32 mUnk130;                // 0x130
    u32 unk134;
    void *mUnk138;  // 0x138
    u8 unk13C[4];
    void *mUnk140;  // 0x140
    u32 unk144[3];
    int mUnk150;    // 0x150
};

struct DinoBattleParams;

class DinoBattleView : public MProcess, public MMessageObserver {
public:
    DinoBattleView(const DinoBattleParams *params);
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

    MResource<DinoBattleViewData> mData;    // 0xA8
    MMessageSubject mSubject;   // 0xB8
    DinoBattleViewUnkE0 *mUnkE0; // 0xE0: (a DinoBattleViewField)
    DinoBattleViewPlayer *mUnkE4[2]; // 0xE4: players
    DinoBattleView3D *mUnkEC;   // 0xEC: (only with the 3D view on)
    u32 unkF0[4];
    KasekiPointPalette *mUnk100;    // 0x100
    DinoBattleViewActionInfoBar *mUnk104;   // 0x104
    DinoBattleViewFocusLCDFrame *mUnk108;   // 0x108
    MSmartOam *mUnk10C;         // 0x10C
    MSmartOam *mPlayerOams[2];  // 0x110
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
