// BattleSelect.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/battle_select.h"
#include "m/input.h"
#include "m/lcd.h"
#include "nitro/os.h"
#include "m/font.h"
#include "game/dino_resource.h"
#include "m/smart_oam.h"
#include "m/oam_canvas.h"
#include "m/indicator.h"
#include "types.h"
#include "m/resource.h"
#include "game/dino_battle_view2.h"
#include "game/dino_battle_view_3d.h"
#include "game/dino_static_init.h"

#pragma auto_inline off

BOOL DinoBattleViewAbilityIcon::IsAnimating() {
    for (int i = 0; i < 4; i++) {
        if ((mMask & (1 << i)) && mValue[i] != mTarget[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// NONMATCHING: the game keeps the position on the stack below the offset point; ours swaps the two slots.
MRect DinoBattleViewAbilityIcon::GetRect() {
    MPoint pos = mMotion->GetPosition();
    MPoint p(pos.x - mOam->mOffset.x, pos.y - mOam->mOffset.y);
    MRect r(-20, -20, 18, 18);
    r.Offset(p);
    return r;
}

BOOL DinoBattleViewAbilityIcon::IsHidden() {
    return mMotion->IsHidden();
}

#pragma auto_inline on

DebugAutoBattle::~DebugAutoBattle() {
    delete mRandom;
}

void DebugAutoBattle::Idle() {
    Exit();
}

// NONMATCHING: 99.9%, the canvas size and the position temporaries swap stack slots.
BattleSelect::BattleSelect(u32 unk74) : MProcess(NULL) {
    MPoint size;
    mUnk74 = unk74;
    mUnk78 = 0;
    MSingleInputHub *hub = new MSingleInputHub(1);
    mInput = hub->GetInput(0);
    mInput->SetRepeat(0xF30, 20, 5);
    mParallel.AddTail(hub);
    mIndicator = new MIndicator(new MObjCanvas(gLcd->GetBottomDisplay(), MPoint(0x54, 0xC), gDinoResource->GetUnkTable(),
                                               TRUE),
                                gDinoResource->CreateFont12(0, 10, 1), 0);
    size = mIndicator->GetSize();
    mIndicator->SetPosition(MPoint((256 - size.x) / 2, (192 - size.y) / 2));
    mIndicator->Show();
    mParallel.AddTail(mIndicator);
}

BattleSelect::~BattleSelect() {
}

UseCreatureSelect::~UseCreatureSelect() {
}

PlayerLevelSelect::~PlayerLevelSelect() {
}

IncValTask::~IncValTask() {
}

TimeCountTask::~TimeCountTask() {
}

void TimeCountTask::Idle() {
}

void UseCreatureSelect::Idle() {
    (this->*mState)();
}

void PlayerLevelSelect::Idle() {
    (this->*mState)();
}

void IncValTask::Execute() {
    (*mValue)++;
}

// A resource the debug battle select loads (its data type is unknown).
struct DinoBattleSelectData;
// (Not decompiled yet: the select screen's setup constructs it; this keeps an out-of-line copy of the constructor.)
#pragma dont_inline on
MResource<DinoBattleSelectData> DinoBattleSelectLoadData(std::string path) {
    return MResource<DinoBattleSelectData>(path);
}
#pragma dont_inline reset

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x38];
    void (UseCreatureSelect::*Start_38)();
    u8 unk40[0x48 - 0x40];
    void (PlayerLevelSelect::*Start_48)();
} data_ov000_02195e50;

UseCreatureSelect::UseCreatureSelect(void *params, u32 unk7C, u32 unk80, u8 *cancelled)
    : MProcess(NULL), mUnk7C(unk7C), mUnk80(unk80), mUnk88(0), mParams(params), mCancelled(cancelled) {
    *mCancelled = FALSE;
    mState = data_ov000_02195e50.Start_38;
}

PlayerLevelSelect::PlayerLevelSelect(u32 *level, BOOL unk7C, u32 unk80)
    : MProcess(NULL), mUnk7C(unk7C), mUnk80(unk80), mUnk9C(0), mUnkA8(0), mUnkAC(1), mLevel(level) {
    mState = data_ov000_02195e50.Start_48;
}

#pragma auto_inline off
// Vector members the game emits in this file (made explicitly until the code that uses them is decompiled).
template void std::__vec_deleter<DinoBattleView3DRecord, std::allocator<DinoBattleView3DRecord> >::sub_push_back(const DinoBattleView3DRecord &x, Metrowerks::int2type<true>);
template std::__vec_deleter<DinoBattleView3DRecord, std::allocator<DinoBattleView3DRecord> >::~__vec_deleter();
template void std::__vec_deleter<DinoBattleView3DEntry, std::allocator<DinoBattleView3DEntry> >::append(size_t n, Metrowerks::int2type<true>);
template std::__vec_deleter<DinoBattleView3DEntry, std::allocator<DinoBattleView3DEntry> >::~__vec_deleter();

// A debug switch, always off.
extern "C" BOOL func_ov000_0218472c(int flag) {
    return FALSE;
}

// A line of text on the sprites at pos.
MIndicator *UseCreatureSelect::CreateIndicator(MSmartOam *oam, MFont *font, MPoint size, MPoint pos) {
    MIndicator *indicator = new MIndicator(new MSmartObjCanvas(oam, size, gDinoResource->GetUnkTable(), TRUE), font, 0);
    indicator->SetPosition(pos);
    indicator->SetPriority(0);
    indicator->Show();
    return indicator;
}

// The title and the five rows' names and values.
void UseCreatureSelect::CreateTexts(MSmartOam *oam, MFont *font) {
    mTitle = CreateIndicator(oam, font, MPoint(32, 16), MPoint(16, 10));
    mParallel.AddTail(mTitle);
    for (int i = 0; i < 5; i++) {
        mNames[i] = CreateIndicator(oam, gDinoResource->CreateFont12(0, 1, 2), MPoint(128, 16), MPoint(24, 32 + i * 30));
        mValues[i] = CreateIndicator(oam, font, MPoint(64, 32), MPoint(32, 44 + i * 30));
        GetParallel()->AddTail(mNames[i], mValues[i]);
    }
}

extern "C" const u32 data_0208a330, data_0208a334, data_0208a348;
extern "C" const char data_ov000_02196238[], data_ov000_02196240[];

// The creature's i-th attack.
static inline u32 DinoCreatureAttack(const DinoCreatureDef *def, int i) {
    return ((const u32 *)((const u8 *)def + def->attackIds))[i];
}

// Shows the creature's name and level, and each of its attacks (the special one last).
// NONMATCHING: 94%, our loop keeps the attack index as a second induction variable (one more spill).
void UseCreatureSelect::ShowCreature(u32 creature) {
    for (int i = 0; i < 5; i++) {
        mNames[i]->Hide();
        mValues[i]->Hide();
    }
    if (creature != 0) {
        const DinoCreatureDef *def = gDinoResource->GetCreatureDef(creature);
        char text[64];
        OS_SPrintf(text, data_ov000_02196238, gDinoResource->GetText(data_0208a348 + def->GetUnk0D() - 1),
                   def->unk61);
        mTitle->SetText(std::string(text));
        int count = def->attackCount;
        int color = 10;
        for (int i = 0; i <= count; i++) {
            u32 attack;
            if (i < count) {
                attack = DinoCreatureAttack(def, i);
            } else {
                color = 3;
                attack = def->specialAttack;
            }
            if (attack == 0) {
                continue;
            }
            const DinoAttackDef *attackDef = gDinoResource->GetAttackDef(attack);
            const u8 *kind = DinoOffsetPtr<u8>(attackDef, attackDef->unk10);
            const u8 *range = DinoOffsetPtr<u8>(attackDef, attackDef->unk14);
            u32 k = *kind;
            if (i < count) {
                switch (k) {
                case 8:
                case 9:
                case 10:
                case 11:
                    color = 2;
                    break;
                }
            }
            mNames[i]->GetFont()->SetColors(color, 0, 1);
            mNames[i]->SetText(std::string(gDinoResource->GetAttackName(attack)));
            OS_SPrintf(text, data_ov000_02196240, gDinoResource->GetText(data_0208a334 + k - 1),
                       gDinoResource->GetText(data_0208a330 + *range - 1));
            mValues[i]->SetText(std::string(text));
            mNames[i]->Show();
            mValues[i]->Show();
        }
    }
}

// The same, for the level select.
static MIndicator *CreateLevelIndicator(MSmartOam *oam, MFont *font, MPoint size, MPoint pos) {
    MIndicator *indicator = new MIndicator(new MSmartObjCanvas(oam, size, gDinoResource->GetUnkTable(), TRUE), font, 0);
    indicator->SetPosition(pos);
    indicator->SetPriority(0);
    indicator->Show();
    return indicator;
}

// (Uses the level select's copy, so the game's static function is emitted.)
extern "C" MIndicator *DinoBattleSelectLevelIndicatorAnchor(MSmartOam *oam, MFont *font, MPoint size, MPoint pos) {
    return CreateLevelIndicator(oam, font, size, pos);
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_02195f50[];
extern "C" u32 data_ov000_02195f90[];
extern "C" u32 data_ov000_02195fa0[];
extern "C" u32 data_ov000_02195fb0[];
extern "C" u32 data_ov000_02195fb8[];
extern "C" u32 data_ov000_02195fc0[];
// NONMATCHING: register choice of the anchors (the game keeps the first anchor in r2).
static struct BattleSelectStaticInit {
    BattleSelectStaticInit() {
        DINO_POOL_WORD(data_ov000_02195e50, 0x70) = 128;
        DINO_POOL_WORD(data_ov000_02195e50, 0x74) = 176;
        DINO_POOL_WORD(data_ov000_02195e50, 0x60) = 8;
        DINO_POOL_WORD(data_ov000_02195e50, 0x64) = 150;
        DINO_POOL_WORD(data_ov000_02195e50, 0x30) = 64;
        DINO_POOL_WORD(data_ov000_02195e50, 0x34) = 112;
        volatile s32 *p = (volatile s32 *)&data_ov000_02195f50;
        p[0x38 / 4] = 32;
        p[0x3c / 4] = 48;
        DINO_POOL_WORD(data_ov000_02195f90, 0x0) = 32;
        DINO_POOL_WORD(data_ov000_02195f90, 0x4) = 48;
        DINO_POOL_WORD(data_ov000_02195f50, 0x48) = 0;
        DINO_POOL_WORD(data_ov000_02195f50, 0x4c) = 32;
        DINO_POOL_WORD(data_ov000_02195fa0, 0x0) = 0;
        DINO_POOL_WORD(data_ov000_02195fa0, 0x4) = 20;
        DINO_POOL_WORD(data_ov000_02195e50, 0x58) = -16;
        DINO_POOL_WORD(data_ov000_02195e50, 0x5c) = 0;
        DINO_POOL_WORD(data_ov000_02195f50, 0x58) = 32;
        DINO_POOL_WORD(data_ov000_02195f50, 0x5c) = 32;
        p = (volatile s32 *)&data_ov000_02195fb0;
        p[0x0 / 4] = 32;
        p[0x4 / 4] = 56;
        p = (volatile s32 *)&data_ov000_02195fb8;
        p[0x0 / 4] = 32;
        p[0x4 / 4] = 70;
        DINO_POOL_WORD(data_ov000_02195fc0, 0x0) = 32;
        DINO_POOL_WORD(data_ov000_02195fc0, 0x4) = 84;
        DINO_POOL_WORD(data_ov000_02195e50, 0x28) = 40;
        DINO_POOL_WORD(data_ov000_02195e50, 0x2c) = 80;
    }
} sBattleSelectStaticInit;
