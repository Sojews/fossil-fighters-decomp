#ifndef GAME_DINO_BATTLE_VIEW_CREATURE_H
#define GAME_DINO_BATTLE_VIEW_CREATURE_H

#include "game/dino_battle_creature_def.h"

#include <string>
#include "game/creature_motion.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "game/dino_battle.h"

class MMotion;

class DinoBattleViewStatusEffectIcon;
class MPalAnime;

class DinoBattleViewPlayer;

class DinoBattleViewCreature : public MProcess, public MMessageObserver {
public:
    DinoBattleViewCreature(u8 id, DinoBattleViewPlayer *player, u32 unkAC, u32 unkB0, u32 unkB4,
                           DinoBattleSide::Creature entry, u32 level, MSmartOam *oam);
    virtual ~DinoBattleViewCreature();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    MTask *CreateMotionCallTask();
    u32 GetUnkAC();
    u32 GetUnkB0();
    void SetUnitModes(BOOL on);
    void HideAll();
    MTask *CreateDownTask();
    MPalAnime *CreatePalBlend(BOOL withBase, u16 color, int weight, u32 duration);
    u32 CountAttacks();
    u32 GetAttack(u32 index);
    u32 GetSpecialAttack();
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
    BOOL IsInPinch();
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
    // Sets the life; a pinch-ability creature going into or out of a pinch says so on the action bar, and going in
    // flashes the pinch mark.
    void SetLife(int life);
    void ClearFlag144(int bit);
    BOOL IsUnkCC2();
    void ClearAdvantage();
    void SetAdvantage(DinoBattleViewCreature *other);
    MPoint GetCoord();
    // Moves back to its place (in its own group or the other), with its life bar.
    void MoveHome(BOOL own);
    // Colors the creature for its strongest status (again, if force).
    void UpdateStatusColor(BOOL force);
    MPoint GetMotionPosition();
    void SetCoord(MPoint pos);

    class SetCoordTask : public MTask {
    public:
        SetCoordTask(DinoBattleViewCreature *creature, MPoint pos) {
            mCreature = creature;
            mPos = pos;
        }
        virtual ~SetCoordTask();
        virtual void Execute();

        DinoBattleViewCreature *mCreature;  // 0x10
        MPoint mPos;                        // 0x14
    };

    u32 mUnkA8;             // 0xA8
    u32 mUnkAC;             // 0xAC
    u32 mUnkB0;             // 0xB0
    u32 mUnkB4;             // 0xB4
    DinoBattleSide::Creature mEntry;    // 0xB8: the side's entry (unk14 at 0xCC: flags)
    CreatureMotion *mMotion;    // 0xD0
    u32 unkD4;
    u32 mUnkD8;             // 0xD8
    u32 mUnkDC;             // 0xDC
    u32 mUnkE0;             // 0xE0
    DinoBattleCreatureDef *mDef;    // 0xE4
    std::string mUnkE8;     // 0xE8
    s32 mUnkF4;             // 0xF4: maximum life
    s32 mUnkF8;             // 0xF8: life
    u32 mUnkFC;             // 0xFC
    u32 mUnk100;            // 0x100
    u32 mUnk104;            // 0x104
    u32 mUnk108;            // 0x108
    u32 mUnk10C;            // 0x10C
    u32 mSupportSides;      // 0x110: bit 0 its support effect touches its own side, bit 1 the other
    u32 mLevel;             // 0x114
    u32 mUnk118;            // 0x118
    MParallelTaskSet *mTasks11C[2]; // 0x11C
    MPoint mCoord;          // 0x124
    MPoint mCoordOffset;    // 0x12C: added to the coordinate to place the motion
    u8 unk134;
    DinoBattleViewStatusEffectIcon *mStatusIcons; // 0x138
    MTaskSet *mUnk13C;      // 0x13C
    MMotion *mMotion140;    // 0x140
    u32 mFlags144;          // 0x144: statuses shown
    u32 mMask148;           // 0x148: statuses not shown
    u32 mFlags14C;          // 0x14C: 8 while it can't be attacked
    int mColor;             // 0x150: the status color shown (0 none)
    class MPalAnime *mBlend;    // 0x154: its palette animation
    u8 unk158;
    u8 mUnk159;             // 0x159: set, it can't be chosen in the support modes
    u8 unk15A[2];
    u32 mShownAdvantage;    // 0x15C: what mMotion140 shows
    u32 mAdvantage;         // 0x160: its type advantage over the creature shown
};

#endif
