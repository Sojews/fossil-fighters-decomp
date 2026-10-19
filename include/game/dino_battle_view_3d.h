#ifndef GAME_DINO_BATTLE_VIEW_3D_H
#define GAME_DINO_BATTLE_VIEW_3D_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include <deque>
#include <vector>
#include <string>
#include "m/seed_generator.h"

class DinoBattleView;

class Dino3DLoadCsv;

class DinoBattleView3D : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleView3D();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void GetUnk19C(u32 *a, u32 *b);
    void SetUnk19C(u32 value);
    u32 GetUnkB0();
    void SetUnk180(u8 value);
    u8 GetUnk180();
    void CancelQueue();
    void FlushQueue();

    u32 unkA8;
    DinoBattleView *mView;                  // 0xAC
    u32 mUnkB0;                             // 0xB0
    u32 unkB4[3];
    MTaskSet *mUnkC0;                       // 0xC0
    void (DinoBattleView3D::*mState)();     // 0xC4
    MTaskSet *mUnkCC;                       // 0xCC
    Dino3DLoadCsv *mModels[3];              // 0xD0
    u8 unkDC[0xE4 - 0xDC];
    MTask *mUnkE4;                          // 0xE4
    MTask *mUnkE8;                          // 0xE8
    u8 unkEC[0x11C - 0xEC];
    void *mSlotModels[6];                   // 0x11C: what each slot's models show (3 per slot)
    u32 mSlot;                              // 0x134
    u8 unk138[0x178 - 0x138];
    MRandom mRandom;                        // 0x178
    u8 mUnk180;                             // 0x180
    u8 unk181[0x184 - 0x181];
    std::deque<MTask *> mQueue;             // 0x184: tasks waiting to run
    u32 mUnk19C;                            // 0x19C
    u32 mUnk1A0;                            // 0x1A0
    std::vector<std::vector<std::string> > mUnk1A4[6]; // 0x1A4
};

class DelDmgMotionTask : public MTask {
public:
    virtual ~DelDmgMotionTask();
    virtual void Execute();

    u8 *mUnk10;     // 0x10
};

// Elements of vectors the battle code keeps (the element types are unknown): 16 and 52 bytes.
struct DinoBattleView3DEntry {
    u32 unk[4];
};
struct DinoBattleView3DRecord {
    u32 unk[13];
};

#endif
