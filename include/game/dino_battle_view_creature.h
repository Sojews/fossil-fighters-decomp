#ifndef GAME_DINO_BATTLE_VIEW_CREATURE_H
#define GAME_DINO_BATTLE_VIEW_CREATURE_H

#include "game/dino_battle_creature_def.h"

#include <string>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MMotion;

class DinoBattleViewCreature : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleViewCreature();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    u32 GetUnkAC();
    u32 GetUnkB0();
    u32 GetUnkB4();
    u32 GetUnkA8();
    u32 GetUnkF4();
    u32 GetUnkF8();
    u32 GetUnk10C();
    u32 GetUnkFC();
    u32 GetUnk100();
    u32 GetUnk104();
    u32 GetUnk108();
    u8 GetDefUnk0D();
    u8 GetDefUnk61();
    u32 GetUnkE0();
    u32 GetUnkDC();
    u32 GetUnkD8();
    MMotion *GetMotion();
    u32 GetUnk118();
    void SetUnk118(u32 value);
    void SetMotionPriority(int priority);
    void SetMotionAnchor(int anchor);
    void PlayMotion3();
    void SetFlag144(int bit);
    void ClearFlag144(int bit);
    BOOL IsUnkCC2();
    void ClearUnk160();
    void SetAdvantage(DinoBattleViewCreature *other);
    MPoint GetCoord();
    MPoint GetMotionPosition();
    void SetCoord(MPoint pos);

    class SetCoordTask : public MTask {
    public:
        virtual ~SetCoordTask();
        virtual void Execute();

        DinoBattleViewCreature *mCreature;  // 0x10
        MPoint mPos;                        // 0x14
    };

    u32 mUnkA8;             // 0xA8
    u32 mUnkAC;             // 0xAC
    u32 mUnkB0;             // 0xB0
    u32 mUnkB4;             // 0xB4
    u32 unkB8[5];
    u32 mUnkCC;             // 0xCC
    MMotion *mMotion;       // 0xD0
    u32 unkD4;
    u32 mUnkD8;             // 0xD8
    u32 mUnkDC;             // 0xDC
    u32 mUnkE0;             // 0xE0
    DinoBattleCreatureDef *mDef;    // 0xE4
    std::string mUnkE8;     // 0xE8
    u32 mUnkF4;             // 0xF4
    u32 mUnkF8;             // 0xF8
    u32 mUnkFC;             // 0xFC
    u32 mUnk100;            // 0x100
    u32 mUnk104;            // 0x104
    u32 mUnk108;            // 0x108
    u32 mUnk10C;            // 0x10C
    u32 unk110[2];
    u32 mUnk118;            // 0x118
    u32 unk11C[2];
    MPoint mCoord;          // 0x124
    MPoint mCoordOffset;    // 0x12C: added to the coordinate to place the motion
    u32 unk134[4];
    u32 mFlags144;          // 0x144
    u32 unk148[6];
    u32 mUnk160;            // 0x160
};

#endif
