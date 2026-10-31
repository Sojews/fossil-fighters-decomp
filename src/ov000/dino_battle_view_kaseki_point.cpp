// DinoBattleViewKasekiPoint, WaitKPCountTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view_kaseki_point.h"
#include "game/wait_kp_count_task.h"
#include "game/dino_battle_view_creature.h"
#include "game/dino_battle_view_class.h"
#include "m/motion.h"
#include "game/dino_resource.h"
#include "m/pal.h"
#include "m/font.h"
#include "m/oam_canvas.h"
#include "nitro/mi.h"
#include "m/indicator.h"
#include "game/dino_sound.h"
#include "game/dino_battle_view2.h"
#include "game/dino_static_init.h"
#include <algorithm>
#include "game/dino_message.h"
#include "nitro/os.h"

DinoBattleViewKasekiPoint::~DinoBattleViewKasekiPoint() {
}

WaitKPCountTask::~WaitKPCountTask() {
}

DinoBattleViewPlayer::~DinoBattleViewPlayer() {
}

#pragma auto_inline off

MPoint GetKasekiPointPos(u32 side) {
    const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
    MPoint pos;
    pos.x = defs->kasekiPointX;
    pos.y = defs->kasekiPointY;
    if (side == 1) {
        pos.x = -pos.x;
    }
    return pos;
}

KasekiPointPalette::KasekiPointPalette(const u16 *from, const u16 *to) {
    Init(from, to);
}

void KasekiPointPalette::Init(const u16 *from, const u16 *to) {
    for (int row = 0; row < 32; row++) {
        for (int i = 1; i < 16; i++) {
            u16 a = from[i];
            u16 b = to[i];
            u16 color = 0;
            int k;
            int shift;
            for (k = 0, shift = 0; k < 3; k++) {
                u16 ca = (a & (0x1F << shift)) >> shift;
                u16 cb = (b & (0x1F << shift)) >> shift;
                color |= (ca + (cb - ca) * row / 32) << shift;
                shift += 5;
            }
            mRows[row][i] = color;
        }
    }
}

// Pooled file statics.
struct KasekiPointStatics {
    u32 unk00;
    const char *motionPath;     // 0x04
    u32 unk08[4];
    MPoint textSize;            // 0x18
};
extern "C" KasekiPointStatics data_ov000_02191900;
extern "C" const char *data_ov000_02191928[];   // font paths, by player
extern "C" int data_ov000_021919e4[][2];        // text positions, by side
extern "C" int data_ov000_021919e8[][2];
extern "C" MPoint data_ov000_021919f4[];        // motion positions, by side

DinoBattleViewKasekiPoint::DinoBattleViewKasekiPoint(DinoBattleView *view, MSmartOam *oam, u32 side, u32 player) : MProcess(NULL) {
    mValue = 0;
    mUnk78 = 100;
    mUnk7C = 0;
    mOam = oam;
    mView = view;
    mUnk98 = side;
    MFontSet *font = new MFontSet(std::string(data_ov000_02191928[player]), 0, 0, 0);
    MI_CpuCopy8(font->GetPalette(), mUnk9C, sizeof(mUnk9C));
    MSmartObjCanvas *canvas = new MSmartObjCanvas(oam->mDisplay, oam, data_ov000_02191900.textSize, font->GetPalette(), TRUE);
    mPalBlock = canvas->GetPalBlock();
    canvas->SetPriority(3);
    canvas->SetAnchor(2);
    mText = new MIndicator(canvas, font, 0);
    mText->SetText(std::string("  0"));
    mText->Show();
    mText->SetPosition(MPoint(data_ov000_021919e4[side][0] - data_ov000_02191900.textSize.x / 2,
                              data_ov000_021919e8[side][0] - data_ov000_02191900.textSize.y / 2));
    mUnk84 = new MMotion(std::string(data_ov000_02191900.motionPath), mOam, 2, 1);
    mUnk84->SetPriority(3);
    mUnk84->SetAnchor(3);
    mUnk84->Show();
    mUnk84->SetPosition(data_ov000_021919f4[side]);
    MTask *play = mUnk84->CreatePlayTransfer(side == 1 ? 4 : 1, 0);
    MTimer *timer = new MTimer(30);
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    set->AddTail(timer);
    set->AddTail(play);
    mParallel.AddTail(set);
    MTask *text = mText;
    MTaskSet *parallel = &mParallel;
    parallel->AddTail(mUnk84);
    parallel->AddTail(text);
}

extern "C" const u32 data_ov000_0218fa14[][2];
extern "C" const u32 data_ov000_0218fa1c[2];
extern "C" const u32 data_ov000_0218fa3c[2];

const u16 *KasekiPointPalette::GetRow(int index) {
    if (index < 0) {
        index = 0;
    }
    if (index > 31) {
        index = 31;
    }
    return mRows[index];
}

void DinoBattleViewKasekiPoint::UpdateAnime(u32 player) {
    int done = mUnk7C == mUnk78 ? 1 : 0;
    int side = mUnk98 != 0 ? 1 : 0;
    int anime = mUnk98 == player ? 2 : 3;
    int current = mUnk84->GetAnime();
    if (anime == 2) {
        if (current == data_ov000_0218fa1c[side] || current == data_ov000_0218fa3c[side]) {
            anime = 0;
        }
    }
    if (done) {
        anime += 4;
    }
    mUnk84->Play(data_ov000_0218fa14[anime][side], 0);
}

extern "C" void func_ov000_0217df4c(void *obj, int delta, u32 side);

void DinoBattleViewKasekiPoint::SetValue(int value) {
    func_ov000_0217df4c(mView->mUnkE0->mUnk140, value - mValue, mUnk98);
    mValue = value;
}

void DinoBattleViewKasekiPoint::Idle() {
    if (mValue == mUnk7C) {
        return;
    }
    int wasFull = mUnk7C == mUnk78 ? 1 : 0;
    for (int i = 0; i < 3; i++) {
        mUnk7C += mValue > mUnk7C ? 1 : -1;
        if (mValue == mUnk7C) {
            break;
        }
    }
    char buf[8];
    OS_SPrintf(buf, "%3d", mUnk7C);
    mText->SetText(std::string(buf));
    LoadPalette(mUnk7C * 31 / mUnk78);
    if (wasFull != (mUnk7C == mUnk78 ? 1 : 0)) {
        int side = mUnk98 != 0 ? 1 : 0;
        int anime = mUnk84->GetAnime();
        for (int k = 0; k < 12; k++) {
            if (anime == data_ov000_0218fa14[k][side]) {
                anime = k;
                break;
            }
        }
        int full = mUnk7C == mUnk78 ? 1 : 0;
        int next;
        if ((1 << anime) & 0x577) {
            next = full ? 10 : 8;
        } else {
            next = full ? 11 : 9;
        }
        mUnk84->Play(data_ov000_0218fa14[next][side], 0);
        if (full) {
            DinoSoundPlaySe(0x564, 0);
        }
    }
}

void DinoBattleViewKasekiPoint::LoadPalette(int index) {
    mPalBlock->Load(mPalette->GetRow(index), 0, -1);
}

DinoBattleViewKasekiPoint *DinoBattleViewPlayer::GetKasekiPoint() {
    return mKasekiPoint;
}

// Runs the task for 50 frames.
void DinoBattleViewPlayer::AddTimedTask(MTask *task, MTask *old) {
    delete old;
    MTask *exit = MTaskExitTransfer::Create(task);
    MTimer *timer = new MTimer(50);
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    set->AddTail(timer);
    set->AddTail(exit);
    MTaskSet *parallel = &mParallel;
    parallel->AddTail(task);
    parallel->AddTail(set);
}

BOOL DinoBattleViewKasekiPoint::IsCounting() {
    if (mValue != mUnk7C) {
        return TRUE;
    }
    return FALSE;
}

DinoBattleViewPlayer::DinoBattleViewPlayer(u32 index, u32 side, u32 unkB0, DinoBattleView *view, u8 unk114)
    : MProcess(NULL), mUnkA8(index), mUnkAC(side), mUnkB0(unkB0), mView(view) {
    mMotion = NULL;
    mUnk114 = unk114;
    mKasekiPoint = new DinoBattleViewKasekiPoint(mView, view->mUnk10C, side, mUnkA8);
    mParallel.AddTail(mKasekiPoint);
    mUnk108 = new DinoBattleViewAbilityIcon(view, mUnkB0, GetKasekiPointPos(side));
    mParallel.AddTail(mUnk108);
}

#pragma auto_inline on

void DinoBattleViewPlayer::Idle() {
    Dispatch();
}

#pragma auto_inline off

DinoBattleViewAbilityIcon *DinoBattleViewPlayer::GetUnk108() {
    return mUnk108;
}

u32 DinoBattleViewPlayer::GetUnkAC() {
    return mUnkAC;
}

std::string DinoBattleViewPlayer::GetUnkC0() {
    return mUnkC0;
}

void DinoBattleViewPlayer::SetMotions(MMotion *motion, MTask *task) {
    if (mMotion != NULL) {
        delete motion;
        delete task;
        return;
    }
    mMotion = motion;
    mMotionTask = task;
    mMotion->Play(2, 0);
    MTask *t = mMotionTask;
    MTaskSet *set = &mParallel;
    set->AddTail(mMotion);
    set->AddTail(t);
}

void DinoBattleViewPlayer::TakeMotions(MMotion **motion, MTask **task) {
    if (mMotion != NULL) {
        mParallel.Remove(mMotion);
        mParallel.Remove(mMotionTask);
        *motion = mMotion;
        *task = mMotionTask;
        mMotion = NULL;
    }
}

extern "C" void func_ov000_0216fee0(DinoBattleViewCreature *creature, MPoint offset);
extern "C" void func_ov000_0218b4dc(DinoBattleViewActionInfoBar *bar, const char *text);
extern "C" const u32 data_0208a2ec;   // a creature down (the player's side)
extern "C" const u32 data_0208a2f0;   // (the other side)

// Adds and removes the side's creatures, sets up the side, updates its kaseki point and ability icon; anything
// else goes to the creatures.
// NONMATCHING (98%): the byte temporaries of the inlined vector push_back/erase take rotated stack slots, and the
// begin signal's flag test reuses a nearby branch stub where the game makes its own.
void DinoBattleViewPlayer::OnMessage(MMessage *msg) {
    switch (msg->mId) {
    default:
        mSubject.Notify(msg);
        break;
    case 0x10007: {
        DinoBattleViewCreatureBeginSignal *req = (DinoBattleViewCreatureBeginSignal *)msg;
        if (req->mUnk0C != mUnkA8) {
            break;
        }
        DinoBattleViewCreature *creature = new DinoBattleViewCreature(
            *(u32 *)req->mUnk18, this, req->mUnk10, mUnkAC, req->mUnk14, *(DinoBattleSide::Creature *)req->mUnk18,
            req->mUnk30, (MSmartOam *)mUnkB0);
        u8 id = *(u32 *)req->mUnk18;
        const DinoBattleViewData *data =
            (const DinoBattleViewData *)mView->mData.mFile.archive->GetData(mView->mData.mFile.index);
        MPoint offset = ((const MPoint *)((const u8 *)data + data->creatureOffsets))[id];
        func_ov000_0216fee0(creature, offset);
        creature->GetUnkB4();
        mCreatures.push_back(creature);
        mSubject.AddObserver(creature);
        mParallel.AddTail(creature);
        break;
    }
    case 0x10008: {
        DinoBattleViewPlayerBeginSignal *req = (DinoBattleViewPlayerBeginSignal *)msg;
        if (req->mUnk0C != mUnkA8) {
            break;
        }
        if (mKasekiPoint != NULL) {
            mKasekiPoint->mUnk78 = req->mUnk30;
        }
        mUnkC0 = (const char *)req->mUnk10;
        u32 count = req->mUnk34;
        if (count == 0) {
            count = 1;
        }
        mUnkCC = count;
        if (req->mUnk38) {
            mUnk114 = TRUE;
        }
        return;
    }
    case 0x10033: {
        DinoBattleViewCreatureEndRequest *req = (DinoBattleViewCreatureEndRequest *)msg;
        if (req->mUnk10 != mUnkA8) {
            break;
        }
        DinoBattleViewCreature *creature = FindCreatureByAC(req->mUnk14);
        if (creature->GetUnkB4() == 0 && mUnk108 != NULL) {
            mUnk108->Update(0, NULL);
        }
        const char *format = gDinoResource->GetText(mUnkAC == 1 ? data_0208a2ec : data_0208a2f0);
        char text[64];
        OS_SPrintf(text, format, gDinoResource->GetCreatureText08(creature->GetUnkA8()));
        func_ov000_0218b4dc((DinoBattleViewActionInfoBar *)mView->GetUnk104(), text);
        mCreatures.erase(std::find(mCreatures.begin(), mCreatures.end(), creature));
        mSubject.RemoveObserver(creature);
        mSerial.AddTail(creature->CreateMotionCallTask());
        if (req->mUnk0C != 0) {
            mSerial.AddTail(((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewCreatureEndResult()));
        }
        return;
    }
    case 0x10038: {
        DinoBattleViewPlayerUpdateRequest *req = (DinoBattleViewPlayerUpdateRequest *)msg;
        if (req->mUnk10 != mUnkA8) {
            break;
        }
        if (mKasekiPoint != NULL) {
            mKasekiPoint->SetValue(req->mUnk14);
        }
        if (req->mUnk0C != 0) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleViewPlayerUpdateResult());
        }
        return;
    }
    case 0x10049: {
        DinoBattleViewPlayerUpdateAbilityIconRequest *req = (DinoBattleViewPlayerUpdateAbilityIconRequest *)msg;
        if (req->mUnk10 != mUnkA8 && !req->mUnk14) {
            break;
        }
        UpdateUnk108();
        if (req->mUnk0C != 0 && req->mUnk10 == mUnkA8) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleViewPlayerUpdateAbilityIconResult());
        }
        return;
    }
    case 0x10092: {
        DinoBattleViewPlayerWaitKPCountStopRequest *req = (DinoBattleViewPlayerWaitKPCountStopRequest *)msg;
        if (req->mUnk10 != mUnkA8) {
            break;
        }
        MTask *post = ((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewPlayerWaitKPCountStopResult());
        WaitKPCountTask *wait = new WaitKPCountTask(mKasekiPoint);
        MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
        tasks->AddTail(wait);
        tasks->AddTail(post);
        mSerial.AddTail(tasks);
        break;
    }
    case 0x10029:
    case 0x1002D:
        if (mMotion != NULL) {
            MTask *task = mMotionTask;
            MTaskSet *parallel = &mParallel;
            parallel->Remove(mMotion);
            parallel->Remove(task);
            delete mMotion;
            delete mMotionTask;
            mMotion = NULL;
        }
        mSubject.Notify(msg);
        break;
    case 0x10036:
        if (mUnk108 != NULL) {
            mUnk108->Exit();
            mUnk108 = NULL;
        }
        if (mKasekiPoint != NULL) {
            mKasekiPoint->Exit();
            mKasekiPoint = NULL;
        }
        mSubject.Notify(msg);
        break;
    }
}

void DinoBattleViewPlayer::UpdateUnk108() {
    if (mUnk108 == 0) {
        return;
    }
    mOpponent = mView->GetUnkE4(mUnkA8 ^ 1);
    int sums[4] = {0, 0, 0, 0};
    u32 mask = 0;
    DinoBattleViewCreature *creature = FindCreatureByB4(0);
    if (creature != NULL) {
        if (creature->GetDefUnk61() == 8) {
            mUnk108->Update(0, sums);
            return;
        }
        for (int side = 0; side < 2; side++) {
            DinoBattleViewPlayer *player = side ? mOpponent : this;
            for (int i = 0; i < player->GetCreatureCount(); i++) {
                DinoBattleViewCreature *member = player->GetCreature(i);
                AccumulateCreature(&mask, sums, member, side != 0);
            }
        }
    }
    mUnk108->Update(mask, sums);
}

void DinoBattleViewPlayer::AccumulateCreature(u32 *mask, int *sums, DinoBattleViewCreature *creature, bool opponent) {
    if (creature->GetUnkB4() - 1 > 1) {
        return;
    }
    int bit = opponent ? 2 : 1;
    if (!(creature->GetUnk118() & bit)) {
        return;
    }
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(creature->GetUnkA8());
    const s8 *src;
    if (opponent) {
        src = def->unk58 == 0 ? NULL : (const s8 *)def + def->unk58;
    } else {
        src = def->unk54 == 0 ? NULL : (const s8 *)def + def->unk54;
    }
    s8 values[4];
    values[0] = src[0];
    values[1] = src[1];
    values[2] = src[2];
    values[3] = src[3];
    for (int i = 0; i < 4; i++) {
        if (values[i] != 0) {
            *mask |= 1 << i;
            sums[i] += values[i];
        }
    }
}

int DinoBattleViewPlayer::GetCreatureCount() {
    return mCreatures.size();
}

DinoBattleViewCreature *DinoBattleViewPlayer::GetCreature(int index) {
    return mCreatures[index];
}

DinoBattleViewCreature *DinoBattleViewPlayer::FindCreatureByB4(u32 id) {
    int count = GetCreatureCount();
    for (int i = 0; i < count; i++) {
        DinoBattleViewCreature *creature = GetCreature(i);
        if (id == creature->GetUnkB4()) {
            return creature;
        }
    }
    return NULL;
}

DinoBattleViewCreature *DinoBattleViewPlayer::FindCreatureByAC(u32 id) {
    for (std::vector<DinoBattleViewCreature *>::iterator it = mCreatures.begin(); it != mCreatures.end(); it++) {
        if (id == (*it)->GetUnkAC()) {
            return *it;
        }
    }
    return NULL;
}

DinoBattleViewPlayer *DinoBattleViewPlayer::GetOpponent() {
    return mView->GetUnkE4(mUnkA8 ^ 1);
}

BOOL DinoBattleViewPlayer::HasCommonCreatureFlags() {
    int count = GetCreatureCount();
    if (count < 3) {
        return FALSE;
    }
    u32 flags = GetCreature(0)->mDef->mUnk24;
    for (int i = 1; i < count; i++) {
        flags &= GetCreature(i)->mDef->mUnk24;
    }
    if (flags != 0) {
        return TRUE;
    }
    return FALSE;
}

#pragma auto_inline on

void WaitKPCountTask::Execute() {
    if (((DinoBattleViewKasekiPoint *)mOwner)->IsCounting() == 0) {
        Exit();
    }
}

// The file's static initialization (see dino_static_init.h); the pool is one object, its second 0x80 bytes addressed
// through the first anchor.
extern "C" u32 data_ov000_021919ec[];
extern "C" u32 data_ov000_021919fc[];
static struct DinoBattleViewKasekiPointStaticInit {
    DinoBattleViewKasekiPointStaticInit() {
        DINO_POOL_WORD(data_ov000_02191900, 0x20) = 128;
        DINO_POOL_WORD(data_ov000_02191900, 0x24) = 176;
        DINO_POOL_WORD(data_ov000_02191900, 0x38) = 8;
        DINO_POOL_WORD(data_ov000_02191900, 0x3c) = 150;
        DINO_POOL_WORD(data_ov000_02191900, 0x30) = 64;
        DINO_POOL_WORD(data_ov000_02191900, 0x34) = 112;
        DINO_POOL_WORD(data_ov000_02191900, 0x18) = 48;
        DINO_POOL_WORD(data_ov000_02191900, 0x1c) = 32;
        DINO_POOL_WORD(data_ov000_02191900, 0xe4) = 24;
        DINO_POOL_WORD(data_ov000_02191900, 0xe8) = 176;
        volatile s32 *p = (volatile s32 *) &data_ov000_021919ec;
        p[0] = 231;
        p[1] = 176;
        DINO_POOL_WORD(data_ov000_02191900, 0xf4) = 48;
        DINO_POOL_WORD(data_ov000_02191900, 0xf8) = 178;
        p = (volatile s32 *) &data_ov000_021919fc;
        p[0] = 240;
        p[1] = 178;
    }
} sDinoBattleViewKasekiPointStaticInit;
