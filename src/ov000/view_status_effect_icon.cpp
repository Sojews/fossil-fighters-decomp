// ViewStatusEffectIcon.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/view_status_effect_icon.h"
#include "types.h"
#include "m/smart_oam.h"
#include "game/dino_resource.h"
#include "game/dino_static_init.h"
#include "game/dino_battle_view_creature.h"
#include "game/dino_sound.h"

DinoBattleViewStatusEffectIcon::~DinoBattleViewStatusEffectIcon() {
}

ViewStatusEffectIcon::~ViewStatusEffectIcon() {
}

ViewStatusEffectIconWaitStop::~ViewStatusEffectIconWaitStop() {
}

ViewStatusEffectIconLoadMotionTask::~ViewStatusEffectIconLoadMotionTask() {
}

// Replaces the icon's motion (keeping whether it was shown) and plays the anime, raising the priority after.
void ViewStatusEffectIconLoadMotionTask::Execute() {
    ViewStatusEffectIcon *icon;
    int anime = mAnime;
    const char *path = mUnk14.c_str();
    icon = (ViewStatusEffectIcon *)mUnk10;
    BOOL hidden = TRUE;
    if (icon->mMotion != NULL) {
        icon->mTasks->Clear();
        hidden = icon->mMotion->IsHidden();
        icon->mParallel.Remove(icon->mMotion);
        delete icon->mMotion;
    }
    icon->mMotion = new MMotion(std::string(path), icon->mOam, 2, 1);
    icon->mMotion->SetPosition(icon->mPos);
    icon->mMotion->SetPriority(2);
    icon->mMotion->SetAnchor(1);
    if (!hidden) {
        icon->mMotion->Show();
    }
    icon->mParallel.AddTail(icon->mMotion);
    if (icon->mMotion != NULL && anime != -1) {
        icon->mTasks->Clear();
        icon->mMotion->Play(anime, 1);
        BOOL front = TRUE;
        switch (anime) {
        case 1:
        case 2:
        case 3:
        case 13:
        case 14:
        case 15:
            front = FALSE;
            break;
        }
        icon->mMotion->SetPriority(front ? 1 : 2);
        icon->mTasks->AddTail(new MTimer(1), icon->mMotion->CreateWaitStopTask(),
                              new ViewStatusEffectIcon::SetPriorityTask(icon, 2));
    }
    Exit();
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

// Called with the icon and the creature's coordinate when the creature moves; the icon doesn't follow.
extern "C" void func_ov000_0217e590(DinoBattleViewStatusEffectIcon *icon, MPoint coord) {
}

// A slot's change while updating: what happened to its effects, and which effects it plays it for.
struct StatusEffectChange {
    int mKind;      // -1: none, 0: a count changed, 1: removed, 2: added
    u32 mEffects;   // the effects to play the change for
    u32 mPresent;   // the effects now present
};
struct StatusEffectChanges {
    StatusEffectChange mSlots[2];
};
extern "C" const StatusEffectChanges data_ov000_02190040;  // both {-1, 0, 0}

extern "C" u32 func_ov000_0217e298(const u32 *effect);

// Effects 8-11 go on the first icon, the rest on the second; each icon cycles through its effects. Added effects show
// at once, removed ones queue the erase anime and the next effect, kept ones replay when their flag or count changes.
// NONMATCHING: same control flow and calls; register and stack-slot allocation differ (the game keeps the slot's
// offset in r7 throughout, and its frame is 8 bytes smaller).
void DinoBattleViewStatusEffectIcon::SetEffects(int count, const u8 *effects) {
    u32 present = 0;
    for (int i = 0; i < count; i++) {
        present |= 1 << (effects[i] & 0xf);
    }
    u32 shown[2] = {0, 0};
    for (int s = 0; s < 2; s++) {
        for (StatusEffectEntry *it = &*mSlots[s].mEntries.begin(); it != &*mSlots[s].mEntries.end(); it++) {
            shown[s] |= 1 << it->mId;
        }
    }
    u8 oldCounts[12] = {0};
    for (int s = 0; s < 2; s++) {
        for (int j = 0; j < mSlots[s].mEntries.size(); j++) {
            oldCounts[mSlots[s].mEntries[j].mId] = mSlots[s].mEntries[j].mCount;
        }
    }
    u8 counts[12] = {0};
    for (int i = 0; i < count; i++) {
        counts[effects[i] & 0xf] = (effects[i] >> 5) & 3;
    }
    for (int s = 0; s < 2; s++) {
        for (int j = 0; j < mSlots[s].mEntries.size(); j++) {
            mSlots[s].mEntries[j].mCount = counts[mSlots[s].mEntries[j].mId];
        }
    }

    StatusEffectChanges changes = data_ov000_02190040;
    for (int e = 1; e < 12; e++) {
        int slot = 1;
        switch (e) {
        case 8:
        case 9:
        case 10:
        case 11:
            slot = 0;
            break;
        }
        StatusEffectChange *change = &changes.mSlots[slot];
        u32 bit = 1 << e;
        u32 has = present & bit;
        change->mPresent |= has;
        if ((present ^ shown[slot]) & bit) {
            if (has) {
                if (change->mKind < 2) {
                    change->mKind = 2;
                    change->mEffects = bit;
                }
            } else if (change->mKind <= 1) {
                change->mEffects = change->mKind == 1 ? bit | change->mEffects : bit;
                change->mKind = 1;
            }
        } else if (bit & shown[slot]) {
            if (change->mKind < 0 && oldCounts[e] != counts[e]) {
                change->mKind = 0;
                change->mEffects = bit;
            }
        }
    }
    if (changes.mSlots[0].mEffects | changes.mSlots[1].mEffects) {
        mTimer = 60;
    }

    MSerialTaskSet *queue = new MSerialTaskSet(TRUE);
    for (int e = 1; e < 12; e++) {
        int slot = 1;
        switch (e) {
        case 8:
        case 9:
        case 10:
        case 11:
            slot = 0;
            break;
        }
        u32 bit = 1 << e;
        int num = 0;
        BOOL flag = FALSE;
        u32 has = present & bit;
        if (has) {
            for (int i = 0; i < count; i++) {
                if (e == (effects[i] & 0xf)) {
                    num = (effects[i] >> 5) & 3;
                    flag = (effects[i] & 0x80) ? TRUE : FALSE;
                    if (num > 2) {
                        num = 2;
                    }
                    break;
                }
            }
        }
        if ((present ^ shown[slot]) & bit) {
            if (has) {
                // Added: shown at once if it's what the slot plays.
                mTimer = 60;
                StatusEffectEntry entry;
                entry.mId = e;
                entry.mCount = num;
                entry.mFlag = flag;
                mSlots[slot].mEntries.push_back(entry);
                if (changes.mSlots[slot].mEffects & bit) {
                    mSlots[slot].mIcon->LoadMotion((const char *)func_ov000_0217e298(&entry.mId), -1);
                    mSlots[slot].mIcon->ShowMotion();
                    mSlots[slot].mIcon->PlayAnime(num + 4 + (flag ? 12 : 0));
                    if (mSlots[slot].mIcon->mMotion != NULL) {
                        mSlots[slot].mIcon->mMotion->GetUnit()->SetMode(1);
                    }
                    mShown[slot] = mSlots[slot].mEntries.size() - 1;
                    DinoSoundPlaySe(0x58, 0);
                }
            } else {
                // Removed: queue its motion for the erase anime, then the next effect being played (or hide).
                for (std::vector<StatusEffectEntry>::iterator it = mSlots[slot].mEntries.begin(); it != mSlots[slot].mEntries.end(); it++) {
                    if (e == (*it).mId) {
                        queue->AddTail(new ViewStatusEffectIconLoadMotionTask(mSlots[slot].mIcon, &*it, (*it).GetAnime()));
                        mSlots[slot].mEntries.erase(it);
                        break;
                    }
                }
                StatusEffectChange *change = &changes.mSlots[slot];
                if (change->mEffects & bit) {
                    ViewStatusEffectIcon *icon = mSlots[slot].mIcon;
                    if (icon != NULL) {
                        MTask *erase = new ViewStatusEffectIcon::PlayEraseMotionTask(icon);
                        MTask *se = DinoSoundCreatePlaySeTask(0x5a, 0);
                        queue->AddTail(erase);
                        queue->AddTail(se);
                    }
                    change->mEffects &= ~bit;
                    queue->AddTail(new ViewStatusEffectIconWaitStop(mSlots[slot].mIcon));
                    if (mSlots[slot].mEntries.size() != 0) {
                        if (change->mEffects == 0) {
                            StatusEffectEntry *first = &mSlots[slot].mEntries[0];
                            queue->AddTail(new ViewStatusEffectIconLoadMotionTask(mSlots[slot].mIcon, first, first->GetAnime()));
                        } else {
                            for (int next = e + 1; next < 12; next++) {
                                for (u32 j = 0; j < mSlots[slot].mEntries.size(); j++) {
                                    if (next == mSlots[slot].mEntries[j].mId && (change->mEffects & (1 << next))) {
                                        queue->AddTail(new ViewStatusEffectIconLoadMotionTask(
                                            mSlots[slot].mIcon, &mSlots[slot].mEntries[j],
                                            mSlots[slot].mEntries[j].GetAnime()));
                                        next = 12;
                                        break;
                                    }
                                }
                            }
                        }
                    } else {
                        queue->AddTail(new ViewStatusEffectIconShowTask(mSlots[slot].mIcon, FALSE));
                    }
                    mShown[slot] = 0;
                }
            }
        } else if (shown[slot] & bit) {
            // Kept: the flag changing replays the effect; else a count change plays its anime.
            int num = counts[e];
            for (int j = 0; j < mSlots[slot].mEntries.size(); j++) {
                if (e == mSlots[slot].mEntries[j].mId) {
                    mShown[slot] = j;
                    break;
                }
            }
            StatusEffectEntry *entry = &mSlots[slot].mEntries[mShown[slot]];
            if (flag != entry->mFlag) {
                entry->mFlag = flag;
                if (mSlots[slot].mIcon->mMotion != NULL) {
                    mSlots[slot].mIcon->mMotion->GetUnit()->SetMode(1);
                }
                mSlots[slot].mIcon->PlayAnime(num + 4 + (flag ? 12 : 0));
                DinoSoundPlaySe(0x58, 0);
            } else if (changes.mSlots[slot].mEffects & bit) {
                if (num == 1) {
                    mSlots[slot].mIcon->PlayAnime((flag ? 12 : 0) + 11);
                    DinoSoundPlaySe(0x59, 0);
                } else if (num == 2) {
                    mSlots[slot].mIcon->PlayAnime((flag ? 12 : 0) + 3);
                    DinoSoundPlaySe(0x59, 0);
                }
            }
        }
    }
    if (queue->IsEmpty()) {
        delete queue;
    } else {
        mQueue->AddTail(queue);
    }
}

int StatusEffectEntry::GetAnime() {
    int offset = mFlag ? 12 : 0;
    int anime = mCount <= 1 ? 2 : 3;
    anime += offset;
    return anime;
}

// Keeps the icons on the creature; once neither is playing, every 60 frames each icon with several effects shows
// the next one.
// NONMATCHING: the new motion's string-temp flag and the wait-stop task swap stack slots.
void DinoBattleViewStatusEffectIcon::Idle() {
    for (int i = 0; i < 2; i++) {
        mSlots[i].mIcon->SetPosition(mOffsets[i] + ((DinoBattleViewCreature *)mUnk74)->GetMotionPosition());
        if (mSlots[i].mIcon->IsPlaying()) {
            mTimer = 60;
            return;
        }
    }
    if (--mTimer > 0) {
        return;
    }
    mTimer = 60;
    for (int i = 0; i < 2; i++) {
        if (mSlots[i].mEntries.size() >= 2) {
            mShown[i] = (mShown[i] + 1) % mSlots[i].mEntries.size();
            StatusEffectEntry *entry = &mSlots[i].mEntries[mShown[i]];
            mSlots[i].mIcon->LoadMotion((const char *)func_ov000_0217e298(&entry->mId), entry->GetAnime());
        }
    }
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

// Plays the icon's erase anime: 12 for the kinds that end on 2, 5, 14, 17 or 23, else 10 (both 12 later for the
// second set of animes, from 13).
void ViewStatusEffectIcon::PlayEraseMotionTask::Execute() {
    mIcon->SetMotionPriority(1);
    BOOL second = mIcon->GetMotionAnime() >= 13;
    switch (mIcon->GetMotionAnime()) {
    case 2:
    case 5:
    case 14:
    case 17:
    case 23:
        mIcon->PlayAnime((second ? 12 : 0) + 12);
        break;
    default:
        mIcon->PlayAnime((second ? 12 : 0) + 10);
        break;
    }
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

// The status effects' paths (see StatusEffectPath; the game reads the table from three places).
extern "C" const u32 data_ov000_021952e4[];    // the paths, every other word

// The icon for the effect (0 for none).
extern "C" u32 func_ov000_0217e298(const u32 *effect) {
    int i = 0;
    if (data_ov000_021952b4.firstIcon != 0) {
        const u32 *entry = (const u32 *)data_ov000_021952e0;
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
