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
    DinoBattleView3D(class DinoBattleView *view);
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
    void RunNextQueued();
    MTask *CreateMove(int unkA, u32 shown, int slot);
    // Loads the scene's parts (the field, the battle's background, the creature models, the two cameras), then
    // runs the queue.
    void StateLoad();
    void SetGroupShown(u32 slot, u8 shown) { mUnk148[slot] = shown; }

    u32 unkA8;
    DinoBattleView *mView;                  // 0xAC
    u32 mUnkB0;                             // 0xB0
    u32 mUnkB4;                             // 0xB4
    class MSmartOam *mOam;                  // 0xB8
    MTaskSet *mUnkBC;                       // 0xBC
    MTaskSet *mUnkC0;                       // 0xC0
    void (DinoBattleView3D::*mState)();     // 0xC4
    MTaskSet *mUnkCC;                       // 0xCC
    Dino3DLoadCsv *mModels[3];              // 0xD0
    Dino3DLoadCsv *mField;                  // 0xDC
    Dino3DLoadCsv *mBackground;             // 0xE0
    MTask *mUnkE4;                          // 0xE4
    MTask *mUnkE8;                          // 0xE8: the camera
    u32 mCreatureIds[6];                    // 0xEC: each slot's three creatures
    u32 mUnk104[6];                         // 0x104: by slot and creature, the creature before the last change
    void *mSlotModels[6];                   // 0x11C: what each slot's models show (3 per slot)
    u32 mSlot;                              // 0x134
    s32 mUnk138;                            // 0x138
    s32 mUnk13C;                            // 0x13C
    u32 mUnk140;                            // 0x140
    u32 mUnk144;                            // 0x144
    u8 mUnk148[2];                          // 0x148: by slot, whether its group is shown
    u8 mUnk14A[6];                          // 0x14A: by slot and creature
    u32 mUnk150[6];                         // 0x150: by slot and creature, flags
    u32 unk168;
    s32 mUnk16C;                            // 0x16C
    u32 mUnk170;                            // 0x170
    u32 mUnk174;                            // 0x174

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
    DelDmgMotionTask(void *defs) { mUnk10 = (u8 *)defs; }
    virtual ~DelDmgMotionTask();
    virtual void Execute();

    u8 *mUnk10;     // 0x10
};

// Elements of vectors the battle code keeps (the element types are unknown): 16 and 52 bytes.
struct DinoBattleView3DEntry {
    u32 unk[4];
};
// A side of the debug auto battle: its level, two values and its creatures.
struct DinoBattleView3DRecord {
    u32 unk00;
    u32 unk04;
    u32 unk08[2];
    struct {
        u32 id;
        u32 level;
        u32 unk08;
    } creatures[3];     // 0x10
};

#endif
