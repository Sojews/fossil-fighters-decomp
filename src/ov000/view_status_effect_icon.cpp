// ViewStatusEffectIcon.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/view_status_effect_icon.h"
#include "types.h"
#include "m/smart_oam.h"
#include "game/dino_resource.h"
#include "game/dino_static_init.h"

DinoBattleViewStatusEffectIcon::~DinoBattleViewStatusEffectIcon() {
}

ViewStatusEffectIcon::~ViewStatusEffectIcon() {
}

ViewStatusEffectIconWaitStop::~ViewStatusEffectIconWaitStop() {
}

ViewStatusEffectIconLoadMotionTask::~ViewStatusEffectIconLoadMotionTask() {
}

ViewStatusEffectIconShowTask::~ViewStatusEffectIconShowTask() {
}

#pragma auto_inline off

// NONMATCHING: spill slots of dx/dy/-dx and the loop counters are permuted
DinoBattleViewStatusEffectIcon::DinoBattleViewStatusEffectIcon(MSmartOam *oam, u32 side, u32 unk74)
    : MProcess(NULL), mUnk74(unk74), mOam(oam) {
    mSide = side;
    mTimer = 0;
    const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
    int dx = defs->statusIconX;
    int dy = defs->statusIconY;
    for (int i = 0; i < 2; i++) {
        mSlots[i].mUnk10 = -1;
    }
    mIcons = new MParallelTaskSet();
    for (int i = 0; i < 2; i++) {
        mSlots[i].mIcon = new ViewStatusEffectIcon(mOam);
        mOffsets[i].x = i != mSide ? -11 : -10;
        mOffsets[i].y = 0;
        if (mSide == 1) {
            mOffsets[i].x += (int)-dx;
            mOffsets[i].y += dy;
        } else {
            mOffsets[i].x += dx;
            mOffsets[i].y += dy;
        }
        mIcons->AddTail(mSlots[i].mIcon);
    }
    mParallel.AddTail(mIcons);
    mQueue = new MSerialTaskSet();
    mParallel.AddTail(mQueue);
}

int StatusEffectEntry::GetAnime() {
    int offset = mFlag ? 12 : 0;
    int anime = mCount <= 1 ? 2 : 3;
    anime += offset;
    return anime;
}

void DinoBattleViewStatusEffectIcon::HideIcons() {
    for (u32 i = 0; i < 2; i++) {
        mSlots[i].mIcon->HideMotion();
    }
}

// NONMATCHING: the game keeps the iterator in a stack slot and walks the slots with two pointers
void DinoBattleViewStatusEffectIcon::RemoveEntry(u32 id) {
    for (int i = 0; i < 2; i++) {
        for (std::vector<StatusEffectEntry>::iterator it = mSlots[i].mEntries.begin(); it != mSlots[i].mEntries.end(); it++) {
            if (id == (*it).mId) {
                mTimer = 60;
                mSlots[i].mEntries.erase(it);
                break;
            }
        }
    }
}

MTask *DinoBattleViewStatusEffectIcon::CreateWaitTask() {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    MTask *queue = MTaskSetWaitEmptyTask::Create(mQueue);
    MTask *second = new ViewStatusEffectIconWaitStop(mSlots[1].mIcon);
    set->AddTail(new ViewStatusEffectIconWaitStop(mSlots[0].mIcon));
    set->AddTail(second);
    set->AddTail(queue);
    return set;
}

void DinoBattleViewStatusEffectIcon::ResetIconModes() {
    for (int i = 0; i < 2; i++) {
        if (mSlots[i].mIcon->mMotion != NULL) {
            mSlots[i].mIcon->mMotion->GetUnit()->SetMode(0);
        }
    }
}

void ViewStatusEffectIcon::Idle() {
}

void ViewStatusEffectIcon::SetPriorityTask::Execute() {
    mIcon->SetMotionPriority(mPriority);
    Exit();
}

#pragma auto_inline on

ViewStatusEffectIcon::PlayEraseMotionTask::~PlayEraseMotionTask() {
}

ViewStatusEffectIcon::SetPriorityTask::~SetPriorityTask() {
}

void ViewStatusEffectIconWaitStop::Execute() {
    if (!mIcon->IsPlaying()) {
        Exit();
    }
}

void ViewStatusEffectIconShowTask::Execute() {
    if (mShow) {
        mIcon->ShowMotion();
    } else {
        mIcon->HideMotion();
    }
    Exit();
}

#pragma auto_inline off
template void std::__vec_deleter<StatusEffectEntry, std::allocator<StatusEffectEntry> >::sub_push_back(const StatusEffectEntry &x, Metrowerks::int2type<true>);

#pragma auto_inline off

// The status effects' icons: {effect, icon} pairs, ended by icon 0. (The game reads the table from three places.)
extern "C" const struct {
    u8 unk00[0x30];
    u32 firstIcon;  // 0x30
} data_ov000_021952b4;
extern "C" const u32 data_ov000_021952e0[];    // the effects, every other word
extern "C" const u32 data_ov000_021952e4[];    // their icons, every other word

// The icon for the effect (0 for none).
extern "C" u32 func_ov000_0217e298(const u32 *effect) {
    int i = 0;
    if (data_ov000_021952b4.firstIcon != 0) {
        const u32 *entry = data_ov000_021952e0;
        do {
            if (*effect == entry[0]) {
                return data_ov000_021952e4[i * 2];
            }
            entry += 2;
            i++;
        } while (entry[1] != 0);
    }
    return 0;
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_02195234[];
static struct ViewStatusEffectIconStaticInit {
    ViewStatusEffectIconStaticInit() {
        DINO_POOL_WORD(data_ov000_02195234, 0x2c) = 128;
        DINO_POOL_WORD(data_ov000_02195234, 0x30) = 176;
        DINO_POOL_WORD(data_ov000_02195234, 0x3c) = 8;
        DINO_POOL_WORD(data_ov000_02195234, 0x40) = 150;
        DINO_POOL_WORD(data_ov000_02195234, 0x44) = 64;
        DINO_POOL_WORD(data_ov000_02195234, 0x48) = 112;
        DINO_POOL_WORD(data_ov000_021952b4, 0x30) = DINO_POOL_WORD(data_ov000_02195234, 0x1c);
        DINO_POOL_WORD(data_ov000_021952b4, 0x38) = DINO_POOL_WORD(data_ov000_02195234, 0x4);
        DINO_POOL_WORD(data_ov000_021952b4, 0x40) = DINO_POOL_WORD(data_ov000_02195234, 0x18);
        DINO_POOL_WORD(data_ov000_021952b4, 0x48) = DINO_POOL_WORD(data_ov000_02195234, 0x8);
        DINO_POOL_WORD(data_ov000_021952b4, 0x50) = DINO_POOL_WORD(data_ov000_02195234, 0xc);
        DINO_POOL_WORD(data_ov000_021952b4, 0x58) = DINO_POOL_WORD(data_ov000_02195234, 0x20);
        DINO_POOL_WORD(data_ov000_021952b4, 0x60) = DINO_POOL_WORD(data_ov000_02195234, 0x24);
        DINO_POOL_WORD(data_ov000_021952b4, 0x68) = DINO_POOL_WORD(data_ov000_02195234, 0x14);
        DINO_POOL_WORD(data_ov000_021952b4, 0x70) = DINO_POOL_WORD(data_ov000_02195234, 0x10);
        DINO_POOL_WORD(data_ov000_021952b4, 0x78) = DINO_POOL_WORD(data_ov000_02195234, 0x28);
    }
} sViewStatusEffectIconStaticInit;
