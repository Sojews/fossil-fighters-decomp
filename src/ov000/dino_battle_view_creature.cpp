// DinoBattleViewCreature.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/creature_motion.h"
#include "game/dino_battle_view_creature.h"
#include "game/dino_resource.h"
#include "types.h"
#include "game/dino_static_init.h"
#include "m/smart_oam.h"
#include "game/view_status_effect_icon.h"
#include "game/dino_battle_view_lifebar.h"

void CreatureMotion::Unk44(int arg) {
    switch (arg) {
    case 1:
        DinoSoundPlaySe(0x47, 0);
        return;
    case 2:
        DinoSoundPlaySe(0x48, 0);
        return;
    }
}

#pragma auto_inline off

CreatureMotion::CreatureMotion(const char *path, MSmartOam *oam, int arg) : MMotion(std::string(path), oam, 3, 1) {
    mArg = arg;
    mUnk80 = 0;
    mUnk84 = 0;
    mUnk88 = 0;
    mOam = oam;
}

// The adjust defs' layout points.
#define DINO_ADJUST_POINT(name, field)                                          \
    static inline MPoint name(int i) {                                          \
        const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs(); \
        return MPoint(defs->field[i].x, defs->field[i].y);                       \
    }
DINO_ADJUST_POINT(SlotPos98, slotPos98)
DINO_ADJUST_POINT(SlotPosB8, slotPosB8)
DINO_ADJUST_POINT(SlotOffsetB8, slotOffsetB8)
// The same, set field by field.
#define DINO_ADJUST_POINT2(name, field)                                         \
    static inline MPoint name(int i) {                                          \
        const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs(); \
        MPoint pos;                                                             \
        pos.x = defs->field[i].x;                                               \
        pos.y = defs->field[i].y;                                               \
        return pos;                                                             \
    }
DINO_ADJUST_POINT2(SlotPos, slotPos)
DINO_ADJUST_POINT2(SlotPosAlt, slotPosAlt)
DINO_ADJUST_POINT2(SlotOffset, slotOffset)
DINO_ADJUST_POINT2(SlotOffsetAlt, slotOffsetAlt)

// Where things go for a slot kind, by the battle's adjust defs; mirrored for side 1.
extern "C" MPoint func_ov000_0216f294(int side, u32 kind) {
    MPoint pos;
    switch (kind) {
    case 0:
        pos = SlotPos98(0);
        break;
    case 1:
        pos = SlotPos98(2);
        break;
    case 2:
        pos = SlotPos98(1);
        break;
    case 3:
        pos = SlotPos98(3);
        break;
    }
    if (side == 1) {
        pos.x = -pos.x;
    }
    return pos;
}

// Where the slot is.
extern "C" MPoint func_ov000_0216f320(int side, u32 kind, BOOL own) {
    MPoint pos;
    switch (kind) {
    case 0:
        pos = own ? SlotPos(0) : SlotPosAlt(0);
        break;
    case 1:
        pos = own ? SlotPos(2) : SlotPosAlt(2);
        break;
    case 2:
        pos = own ? SlotPos(1) : SlotPosAlt(1);
        break;
    case 3:
        pos = own ? SlotPos(3) : SlotPosAlt(3);
        break;
    }
    if (side == 1) {
        pos.x = -pos.x;
    }
    return pos;
}

extern "C" MPoint func_ov000_0216f434(int side, u32 kind) {
    MPoint pos;
    switch (kind) {
    case 0:
        pos = SlotPosB8(0);
        break;
    case 1:
        pos = SlotPosB8(2);
        break;
    case 2:
        pos = SlotPosB8(1);
        break;
    case 3:
        pos = SlotPosB8(3);
        break;
    }
    if (side == 1) {
        pos.x = -pos.x;
    }
    return pos;
}

// The slot's position moved by its offset.
extern "C" MPoint func_ov000_0216f4c0(int side, u32 kind, BOOL own) {
    MPoint offset;
    switch (kind) {
    case 0:
        offset = own ? SlotOffset(0)
                     : SlotOffsetAlt(0);
        break;
    case 1:
        offset = own ? SlotOffset(2)
                     : SlotOffsetAlt(2);
        break;
    case 2:
        offset = own ? SlotOffset(1)
                     : SlotOffsetAlt(1);
        break;
    case 3:
        offset = own ? SlotOffset(3)
                     : SlotOffsetAlt(3);
        break;
    }
    if (side == 1) {
        offset.x = -offset.x;
    }
    return offset + func_ov000_0216f320(side, kind, own);
}

extern "C" MPoint func_ov000_0216f600(int side, u32 kind) {
    MPoint offset;
    switch (kind) {
    case 0:
        offset = SlotOffsetB8(0);
        break;
    case 1:
        offset = SlotOffsetB8(2);
        break;
    case 2:
        offset = SlotOffsetB8(1);
        break;
    case 3:
        offset = SlotOffsetB8(3);
        break;
    }
    if (side == 1) {
        offset.x = -offset.x;
    }
    return offset + func_ov000_0216f434(side, kind);
}

extern "C" int func_ov000_0216f698(int arg0, BOOL arg1, BOOL arg2) {
    if (arg0 == 3) {
        return 5;
    }
    if (arg1) {
        if (arg2) {
            return 2;
        }
        return 3;
    }
    return 4;
}

// Pulses the motion's highlight color while it is marked (mArg: bit 0 and bit 1 for the two colors).
void CreatureMotion::Execute() {
    MPalUnit *pal = GetPalette(0);
    if (mArg != 0 || mUnk84 != 0) {
        mUnk84 += 8;
        if (mUnk84 >= 0x1FF) {
            mUnk84 = 0;
        }
        if (mUnk84 == 0) {
            mUnk80 ^= 1;
        }
        if (!(mArg & (1 << mUnk80))) {
            mUnk80 ^= 1;
        }
        u32 level = mUnk84;
        if (level >= 0x100) {
            level = 0x1FF - level;
        }
        switch (mUnk80) {
        case 0:
            mUnk88 = ((level >> 3) << 5) | 0x8000;
            break;
        case 1:
            mUnk88 = (level >> 3) | 0x8000;
            break;
        }
    }
    pal->LoadColors(&mUnk88, 1, 1);
    MMotion::Execute();
}

void CreatureMotion::SetArg(int arg, BOOL flag) {
    mArg = arg | (flag ? 2 : 0);
}

BOOL DinoBattleViewCreature::IsUnkCC2() {
    return (mEntry.unk14 & 2) ? TRUE : FALSE;
}

// The TU's pooled constants.
extern "C" struct {
    u8 unk00[0x0C];
    const char *markPath;   // 0x0C: the advantage mark's motion
    u8 unk10[0x4C - 0x10];
    MPoint markOffset;  // 0x4C: of the advantage mark from the creature
} data_ov000_02193030;

extern "C" const u32 data_ov000_0218fce4[];   // frames before it appears, by kind
extern "C" MPoint func_ov000_0216f294(int side, u32 kind);
extern "C" MPoint func_ov000_0216f4c0(int side, u32 kind, BOOL own);
extern "C" void func_ov000_0216fcb8(DinoBattleViewCreature *creature, u32 id);

// Sets up the creature's motions (the creature, its advantage mark), status icons, life bar and appearance.
// NONMATCHING (97%): the game keeps a copy of unkB0 on the stack for the status icons (its frame is 8 bytes bigger).
DinoBattleViewCreature::DinoBattleViewCreature(u8 id, DinoBattleViewPlayer *player, u32 unkAC, u32 unkB0, u32 unkB4,
                                               DinoBattleSide::Creature entry, u32 level, MSmartOam *oam)
    : MProcess(NULL), mUnkA8(id), mUnkAC(unkAC), mUnkB0(unkB0), mUnkB4(unkB4), mEntry(entry), mUnkD8((u32)oam),
      mUnkDC((u32)player), mUnkF8(0), mLevel(level) {
    mCoordOffset.x = 0;
    mCoordOffset.y = 0;
    unk134 = 0;
    mFlags144 = 0;
    mMask148 = 0;
    mColor = 0;
    mBlend = NULL;
    unk158 = 0;
    mUnk159 = 0;
    func_ov000_0216fcb8(this, id);
    mAdvantage = mShownAdvantage = 0;
    mUnk13C = new MSerialTaskSet();
    mMotion140 = new MMotion(std::string(data_ov000_02193030.markPath), oam, 0, 1);
    mMotion140->SetPosition(data_ov000_02193030.markOffset + func_ov000_0216f294(mUnkB0, mUnkB4));
    mMotion140->SetPriority(2);
    mMotion140->SetAnchor(0);
    mMotion140->Hide();
    GetParallel()->AddTail(mUnk13C, mMotion140);
    mMotion = new CreatureMotion(gDinoResource->GetBattleCreatureMotionPath(id).c_str(), oam, 0);
    mMotion->SetPriority(2);
    mMotion->SetAnchor(3);
    mMotion->Show();
    if (unkB0 == 1) {
        mMotion->SetFlip(1, 0);
    }
    mStatusIcons = new DinoBattleViewStatusEffectIcon(oam, unkB0, (u32)this);
    mParallel.AddTail(mStatusIcons);
    mTasks11C[0] = new MParallelTaskSet();
    mParallel.AddTail(mTasks11C[0]);
    mTasks11C[1] = new MParallelTaskSet();
    mParallel.AddTail(mTasks11C[1]);
    mTasks11C[1]->AddTail(mMotion);
    SetCoord(func_ov000_0216f294(mUnkB0, mUnkB4));
    mSerial.AddTail(new MTimer(data_ov000_0218fce4[mUnkB4]), mMotion->CreatePlayTransfer(1, 0), new MTimer(30),
                    mMotion->CreateWaitStopTask());
    mUnkE0 = (u32) new DinoBattleViewLifebar(this, oam, func_ov000_0216f4c0(mUnkB0, mUnkB4, unk134), 100, 100);
    mParallel.AddTail((MTask *)mUnkE0);
}

int DinoGetTypeAdvantage(int type, int other); // 1: type beats other, 2: other beats type

void DinoBattleViewCreature::SetAdvantage(DinoBattleViewCreature *other) {
    if (other != NULL) {
        mAdvantage = DinoGetTypeAdvantage(GetDefUnk0D(), other->GetDefUnk0D());
    } else {
        mAdvantage = 0;
    }
}

void DinoBattleViewCreature::ClearAdvantage() {
    mAdvantage = 0;
}

void DinoBattleViewCreature::SetCoord(MPoint pos) {
    mCoord = pos;
    mMotion->SetPosition(mCoord + mCoordOffset);
}

// Loads the creature's definition and notes which sides its support effects touch.
extern "C" void func_ov000_0216fcb8(DinoBattleViewCreature *creature, u32 id) {
    creature->mDef = (DinoBattleCreatureDef *)gDinoResource->GetCreatureDef(id);
    creature->mSupportSides = 0;
    for (int i = 0; i < 2; i++) {
        const s8 *effect = i == 0 ? DinoOffsetPtr<s8>(creature->mDef, creature->mDef->mUnk54)
                                  : DinoOffsetPtr<s8>(creature->mDef, creature->mDef->mUnk58);
        if (effect[0] != 0 || effect[1] != 0 || effect[2] != 0 || effect[3] != 0) {
            creature->mSupportSides |= 1 << i;
        }
    }
}

// Shows the type advantage mark when the advantage changed: pointing at the other side for 1, at its own for 2.
extern "C" void func_ov000_0216fd74(DinoBattleViewCreature *creature) {
    if (creature->mAdvantage == creature->mShownAdvantage) {
        return;
    }
    creature->mShownAdvantage = creature->mAdvantage;
    switch (creature->mShownAdvantage) {
    case 0:
        creature->mUnk13C->Clear();
        if (!creature->mMotion140->IsHidden()) {
            creature->mUnk13C->AddTail(creature->mMotion140->CreatePlayTransfer(2, 0),
                                       creature->mMotion140->CreateWaitStopTask(),
                                       creature->mMotion140->CreateHideTransfer());
        }
        break;
    case 1:
        creature->mUnk13C->Clear();
        creature->mMotion140->SetFlip(creature->mUnkB0 == 1, 0);
        creature->mMotion140->Show();
        creature->mMotion140->Play(1, 1);
        break;
    case 2:
        creature->mUnk13C->Clear();
        creature->mMotion140->SetFlip(creature->mUnkB0 == 0, 0);
        creature->mMotion140->Show();
        creature->mMotion140->Play(1, 1);
        break;
    }
}


// Moves the creature (shuffling over, or sliding) and its advantage mark from one coordinate to another.
// NONMATCHING: 96%, the game keeps to.y in a register for the new coordinate.
extern "C" MTask *func_ov000_0216ff18(DinoBattleViewCreature *creature, MPoint from, MPoint to, int frames, int ease,
                                       BOOL shuffle, int arg) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    if (shuffle) {
        set->AddTail(new ShuffleMoveTask(arg, creature->mMotion, from, to));
    } else {
        set->AddTail(creature->mMotion->CreateCoordAnime(from + creature->mCoordOffset, to + creature->mCoordOffset,
                                                         frames, ease));
    }
    const MPoint &markOffset = data_ov000_02193030.markOffset;
    set->AddTail(creature->mMotion140->CreateCoordAnime(markOffset + from, markOffset + to, frames, ease));
    creature->mCoord = to;
    return set;
}

// Sets where the motion goes from the coordinate (mirrored on side 1).
extern "C" void func_ov000_0216fee0(DinoBattleViewCreature *creature, MPoint offset) {
    creature->mCoordOffset = offset;
    if (creature->mUnkB0 == 1) {
        creature->mCoordOffset.x = -creature->mCoordOffset.x;
    }
    creature->SetCoord(creature->mCoord);
}

MPoint DinoBattleViewCreature::GetCoord() {
    return mCoord;
}

MPoint DinoBattleViewCreature::GetMotionPosition() {
    return mMotion->GetPosition();
}

// The file's static initialization (see dino_static_init.h).
static struct DinoBattleViewCreatureStaticInit {
    DinoBattleViewCreatureStaticInit() {
        DINO_POOL_WORD(data_ov000_02193030, 0x24) = 128;
        DINO_POOL_WORD(data_ov000_02193030, 0x28) = 176;
        DINO_POOL_WORD(data_ov000_02193030, 0x54) = 8;
        DINO_POOL_WORD(data_ov000_02193030, 0x58) = 150;
        DINO_POOL_WORD(data_ov000_02193030, 0x14) = 64;
        DINO_POOL_WORD(data_ov000_02193030, 0x18) = 112;
        DINO_POOL_WORD(data_ov000_02193030, 0x5c) = 0;
        DINO_POOL_WORD(data_ov000_02193030, 0x60) = -18;
        DINO_POOL_WORD(data_ov000_02193030, 0x4c) = 0;
        DINO_POOL_WORD(data_ov000_02193030, 0x50) = 12;
    }
} sDinoBattleViewCreatureStaticInit;
