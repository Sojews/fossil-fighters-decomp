// BattleSelect.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/battle_select.h"
#include "game/dino_battle.h"
#include "game/dino_backup.h"
#include "m/file.h"
#include "m/text_codec.h"
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
#include "game/dino_sound.h"
#include "m/fx32.h"
#include "nitro/fx.h"
#include "nitro/g2.h"
#include <printf.h>

#pragma auto_inline off

extern "C" const u32 data_ov000_02190298[4][2];    // by ability: the effect's appear and disappear animes
extern "C" const u32 data_0208a14c;     // frames the appear effect shows
extern "C" u32 data_ov000_02195d24[];   // the view's pooled statics (+0x08: the ability effect's motion)
extern "C" char data_ov000_02195e40[];  // ""
extern "C" const int data_0208a1a8;   // the counters' step per frame
extern "C" char data_ov000_02195e44[];  // "AB%d"
extern "C" char data_ov000_02195e4c[];  // " []"
extern "C" struct {
    MtxFx22 mtx;    // 0x00: the ability icons' scale
    MtxFx22 counting;   // 0x10: their scale while counting
} data_ov000_021978e8;

static inline MFx32 DinoFx32Abs(MFx32 x) {
    if (x.v >= 0) {
        return x;
    }
    return -x;
}

// Sets the shown abilities and their counters' targets; abilities that appear or go play an effect over the icon
// (the new ones' counters start from 0).
void DinoBattleViewAbilityIcon::Update(u32 mask, int *values) {
    u32 old = mMask;
    mMask = mask;
    if (values != NULL) {
        MI_CpuCopy32(values, mTarget, sizeof(mTarget));
    } else {
        MIi_CpuClear32(0, mTarget, sizeof(mTarget));
    }
    u32 changed = old ^ mMask;
    if (changed & mMask) {
        MParallelTaskSet *effects = new MParallelTaskSet(TRUE);
        for (int i = 0; i < 4; i++) {
            if (!((changed & mMask) & (1 << i))) {
                continue;
            }
            MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
            MMotion *effect = new MMotion(std::string(((const char **)data_ov000_02195d24)[2]), mOam, 0, 1);
            effect->SetPosition(mMotion->GetPosition());
            effect->SetPriority(2);
            effect->SetAnchor(2);
            mParallel.AddTail(effect);
            mTexts[i]->Show();
            mTexts[i]->SetText(std::string(data_ov000_02195e40));
            mValue[i] = 0;
            tasks->AddTail(effect->CreateShowTransfer(), effect->CreatePlayTransfer(data_ov000_02190298[i][0], 0),
                           new MTimer(data_0208a14c), MTaskExitTransfer::Create(effect));
            effects->AddTail(tasks);
        }
        GetSerial()->AddTail(DinoSoundCreatePlaySeTask(0x388, 0), effects);
    }
    if (mMask != 0) {
        GetSerial()->AddTail(mMotion->CreateShowTransfer(), mMotion->CreatePlayTransfer(mMask, 0));
    } else {
        GetSerial()->AddTail(mMotion->CreateHideTransfer());
    }
    if (changed & old) {
        MParallelTaskSet *effects = new MParallelTaskSet(TRUE);
        for (int i = 0; i < 4; i++) {
            if (!((changed & old) & (1 << i))) {
                continue;
            }
            MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
            MMotion *effect = new MMotion(std::string(((const char **)data_ov000_02195d24)[2]), mOam, 0, 1);
            effect->SetPosition(mMotion->GetPosition());
            effect->SetPriority(2);
            effect->SetAnchor(3);
            mParallel.AddTail(effect);
            tasks->AddTail(effect->CreateShowTransfer(), effect->CreatePlayTransfer(data_ov000_02190298[i][1], 0),
                           effect->CreateWaitStopTask(), MTaskExitTransfer::Create(effect));
            effects->AddTail(tasks);
            mTexts[i]->Hide();
        }
        GetSerial()->AddTail(DinoSoundCreatePlaySeTask(0x389, 0), effects);
    }
}

// Counts each shown ability's value a step toward its target, redrawing the counter (enlarged while it counts),
// and plays the count sound when counting starts.
void DinoBattleViewAbilityIcon::Idle() {
    mWasCounting = mCounting;
    mCounting = FALSE;
    int step = data_0208a1a8;
    for (int i = 0; i < 4; i++) {
        if (!(mMask & (1 << i)) || mValue[i] == mTarget[i]) {
            continue;
        }
        mCounting = TRUE;
        if (DinoFx32Abs(MFx32(mValue[i] - mTarget[i])) <= MFx32(step)) {
            mValue[i] = mTarget[i];
        } else {
            mValue[i] += mValue[i] > mTarget[i] ? -step : step;
        }
        char text[8];
        if (mValue[i] < 0) {
            OS_SPrintf(text, data_ov000_02195e44, -mValue[i]);
            for (char *c = text; *c != 0; c++) {
                *c += 0x20;
            }
        } else {
            OS_SPrintf(text, mValue[i] != 0 ? data_ov000_02195e44 : data_ov000_02195e4c, mValue[i]);
        }
        if (mValue[i] == mTarget[i]) {
            mUnits[i]->SetObjMode(0);
            mUnits[i]->SetFlip(FALSE, FALSE);
        } else {
            mUnits[i]->SetObjMode(1);
            mUnits[i]->SetAffineParam(mAffine->index);
            MTX_Scale22(&data_ov000_021978e8.counting, 0xCCD, 0xCCD);
            G2_SetOBJAffine(mOamAffine, &data_ov000_021978e8.counting);
        }
        mTexts[i]->SetText(std::string(text));
    }
    if (mWasCounting != mCounting && mCounting) {
        DinoSoundPlaySe(0x38A, 0);
    }
}

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

extern "C" const u32 data_0208a42c;          // the battle number text
extern "C" const char data_ov000_0219622c[]; // "%s%d"-like format
extern const u32 kCreatureLevelLimits[2];

// Picks a battle by number (left/right by one, Y/X by ten, L/R by a hundred, wrapping round the battle count) and
// starts it: A or start fills the battle parameters from the battle's data (for number 0, the parameters as they
// are) and exits.
void BattleSelect::Idle() {
    if (mInput->GetPressed() & 0x20) {
        if (--mUnk78 < 0) {
            mUnk78 = gDinoResource->GetBattleCount() - 1;
        }
    }
    if (mInput->GetPressed() & 0x10) {
        if (++mUnk78 >= gDinoResource->GetBattleCount()) {
            mUnk78 = 0;
        }
    }
    if (mInput->GetPressed() & 0x800) {
        if ((mUnk78 -= 10) < 0) {
            mUnk78 = gDinoResource->GetBattleCount() - 1;
        }
    }
    if (mInput->GetPressed() & 0x400) {
        mUnk78 += 10;
        if (mUnk78 >= gDinoResource->GetBattleCount()) {
            mUnk78 = 0;
        }
    }
    if (mInput->GetPressed() & 0x200) {
        if ((mUnk78 -= 100) < 0) {
            mUnk78 = gDinoResource->GetBattleCount() - 1;
        }
    }
    if (mInput->GetPressed() & 0x100) {
        mUnk78 += 100;
        if (mUnk78 >= gDinoResource->GetBattleCount()) {
            mUnk78 = 0;
        }
    }
    char text[0x100];
    sprintf(text, data_ov000_0219622c, gDinoResource->GetText(data_0208a42c), mUnk78);
    mIndicator->SetText(std::string(text));
    if (!(mInput->GetPressed() & 9)) {
        return;
    }
    if (mUnk78 != 0) {
        MResource<DinoBattleData> battle = gDinoResource->LoadBattle(mUnk78);
        ((DinoBattleParams *) mUnk74)->unk2C = battle.Get()->unk0C;
        ((DinoBattleParams *) mUnk74)->mScene = battle.Get()->scene;
        ((DinoBattleParams *) mUnk74)->unk154 = battle.Get()->unk30;
        ((DinoBattleParams *) mUnk74)->mTutorialId = battle.Get()->tutorialId;
        ((DinoBattleParams *) mUnk74)->mLiveName = (const char *) battle.Get()->liveName;
        ((DinoBattleParams *) mUnk74)->unk148 = battle.Get()->unk18;
        ((DinoBattleParams *) mUnk74)->unk14C = battle.Get()->unk1C;
        ((DinoBattleParams *) mUnk74)->unk150 = battle.Get()->unk20;
        ((DinoBattleParams *) mUnk74)->unk158 = battle.Get()->unk4C != 0;
        ((DinoBattleParams *) mUnk74)->unk15C = battle.Get()->unk50;
        if (gDinoBackup->GetSystemFlag(0x2C) || battle.Get()->unk54 == -1) {
            ((DinoBattleParams *) mUnk74)->unk160 = -1;
        } else {
            ((DinoBattleParams *) mUnk74)->unk160 = battle.Get()->unk54;
        }
        const DinoBattleSideData *enemy = battle.Get()->GetEnemy();
        ((DinoBattleParams *) mUnk74)->sides[1].mName = gDinoResource->GetText(enemy->nameText);
        ((DinoBattleParams *) mUnk74)->sides[1].unk54 = enemy->unk0C;
        ((DinoBattleParams *) mUnk74)->sides[1].unk58 = enemy->unk30;
        ((DinoBattleParams *) mUnk74)->sides[1].unk88 = enemy->unk34;
        u32 i;
        for (i = 0; i < enemy->creatureCount; i++) {
            const DinoBattleCreatureData &entry = enemy->GetCreature(i);
            ((DinoCreatureState *) &((DinoBattleParams *) mUnk74)->sides[1].mCreatures[i])->index = entry.id;
            int level = entry.level;
            DinoCreaturePart *parts =
                ((DinoCreatureState *) &((DinoBattleParams *) mUnk74)->sides[1].mCreatures[i])->parts;
            if (level > 12) {
                level = 12;
            }
            for (u32 j = 0; j < 4; j++) {
                parts[j].level = 0;
                parts[j].unk12 = 0;
            }
            switch (level) {
            case 12:
                parts[3].level += kCreatureLevelLimits[0];
            case 11:
                parts[3].level += kCreatureLevelLimits[0];
            case 10:
                parts[3].level += kCreatureLevelLimits[0];
            case 9:
                parts[2].level += kCreatureLevelLimits[0];
            case 8:
                parts[2].level += kCreatureLevelLimits[0];
            case 7:
                parts[2].level += kCreatureLevelLimits[0];
            case 6:
                parts[1].level += kCreatureLevelLimits[0];
            case 5:
                parts[1].level += kCreatureLevelLimits[0];
            case 4:
                parts[1].level += kCreatureLevelLimits[0];
            case 3:
                parts[0].level += kCreatureLevelLimits[0];
            case 2:
                parts[0].level += kCreatureLevelLimits[0];
            case 1:
                parts[0].level += 50;
            }
            ((DinoBattleParams *) mUnk74)->sides[1].mCreatures[i].unk14 = entry.unk08;
        }
        for (; i < 3; i++) {
            ((DinoCreatureState *) &((DinoBattleParams *) mUnk74)->sides[1].mCreatures[i])->index = 0;
        }
        for (u32 i = 0; i < enemy->unk10; i++) {
            ((DinoBattleParams *) mUnk74)->sides[1].unk5C[i] = enemy->GetUnk14()[i];
        }
        for (u32 i = 0; i < enemy->unk28; i++) {
            (&((DinoBattleParams *) mUnk74)->sides[1].unk68)[i] = enemy->GetUnk2C()[i];
        }
        for (u32 i = 0; i < enemy->unk18; i++) {
            (&((DinoBattleParams *) mUnk74)->sides[1].unk7C)[i] = enemy->GetUnk1C()[i];
        }
        for (u32 i = 0; i < enemy->unk20; i++) {
            ((DinoBattleParams *) mUnk74)->sides[1].mLevels[i] = enemy->GetUnk24()[i];
        }
        ((DinoBattleParams *) mUnk74)->unk164 = battle.Get()->unk58;
    }
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *timer = new MTimer(1);
    MTaskSet &serial = mSerial;
    serial.AddTail(timer);
    serial.AddTail(exit);
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
    u32 unk00;
    int creatureCount;          // 0x04: Select's static (guarded by data_ov000_02197908[1])
    const char *rowFormat;      // 0x08: with the unk5C value
    const char *tutorialPath;   // 0x0C: "/btl_tutorial/btl_tuto_%03d"
    u32 unk10;
    const char *valueFormat;    // 0x14: "%s%d:%d"
    u32 unk18;
    int minValue;               // 0x1C: 1
    const char *rowFormatShort; // 0x20
    const char *livePath;       // 0x24: "/live/live_battle_%03d"
    u8 unk28[0x38 - 0x28];
    void (UseCreatureSelect::*Start_38)();
    void (UseCreatureSelect::*Select_40)();
    void (PlayerLevelSelect::*Start_48)();
    u8 unk50[0x58 - 0x50];
    MPoint cursorOffset;        // 0x58
    u8 unk60[0x68 - 0x60];
    void (PlayerLevelSelect::*StateSelect_68)();
} data_ov000_02195e50;

UseCreatureSelect::UseCreatureSelect(void *params, u32 unk7C, u32 unk80, u8 *cancelled)
    : MProcess(NULL), mUnk7C(unk7C), mUnk80(unk80), mUnk88(0), mParams(params), mCancelled(cancelled) {
    *mCancelled = FALSE;
    mState = data_ov000_02195e50.Start_38;
}

PlayerLevelSelect::PlayerLevelSelect(u32 *level, BOOL unk7C, u32 unk80)
    : MProcess(NULL), mUnk7C(unk7C), mUnk80(unk80), mRow(0), mUnkA8(0), mUnkAC(1), mLevel((DinoBattleParams *)level) {
    mState = data_ov000_02195e50.Start_48;
}

#pragma auto_inline off
// Vector members the game emits in this file (made explicitly until the code that uses them is decompiled).
template void std::__vec_deleter<DinoBattleView3DRecord, std::allocator<DinoBattleView3DRecord> >::sub_push_back(const DinoBattleView3DRecord &x, Metrowerks::int2type<true>);
template std::__vec_deleter<DinoBattleView3DRecord, std::allocator<DinoBattleView3DRecord> >::~__vec_deleter();
template void std::__vec_deleter<DinoBattleView3DEntry, std::allocator<DinoBattleView3DEntry> >::append(size_t n, Metrowerks::int2type<true>);
template std::__vec_deleter<DinoBattleView3DEntry, std::allocator<DinoBattleView3DEntry> >::~__vec_deleter();

extern "C" const MPoint data_ov000_02195fa8[4]; // by row: the cursor's position
extern "C" const u32 data_0208a418, data_0208a41c;  // the rows' labels

// The KP defs (the debug level select reads the levels from them).
struct DinoAbilityIconData {
    u32 unk0;
    u32 count;      // 0x04
    u32 entries;    // 0x08: offset of count 0x18-byte entries, each starting with a level
};

extern "C" const u32 data_0208a420;     // the level select's title
static MIndicator *CreateLevelIndicator(MSmartOam *oam, MFont *font, MPoint size, MPoint pos);
extern "C" const u32 data_0208ade0;     // the level select's cursor

// NONMATCHING: the game keeps the two loops' &mParallel below the oam on the stack.
void PlayerLevelSelect::Start() {
    mCounts[0] = mLevel->sides[1].unk68;
    mCounts[1] = mLevel->sides[1].unk6C;
    MSmartOam *oam = new MSmartOam(100, 0, 1);
    MFontSet *font = gDinoResource->CreateFont12(0, 10, 1);
    mInputHub = new MSingleInputHub(1);
    mParallel.AddTail(mInputHub);
    MIndicator *title = new MIndicator(new MSmartObjCanvas(oam, MPoint(128, 16), gDinoResource->GetUnkTable(), TRUE), font, 0);
    title->SetText(std::string(gDinoResource->GetText(data_0208a420)));
    title->SetPosition(MPoint(64, 8));
    title->SetPriority(0);
    title->Show();
    mParallel.AddTail(title);
    int i = 0;
    if (i <= mUnk7C) {
    MParallelTaskSet *parallel = &mParallel;
    for (; i <= mUnk7C; i++) {
        char text[64];
        OS_SPrintf(text, "%s %d", gDinoResource->GetText(data_0208a418), mValues[i]);
        mTexts[i] = new MIndicator(new MSmartObjCanvas(oam, MPoint(192, 16), gDinoResource->GetUnkTable(), TRUE), font, 0);
        mTexts[i]->SetText(std::string(text));
        mTexts[i]->SetPosition(data_ov000_02195fa8[i]);
        mTexts[i]->SetPriority(0);
        mTexts[i]->Show();
        parallel->AddTail(mTexts[i]);
    }
    }
    MParallelTaskSet *rows = &mParallel;
    for (int i = 2; i <= 3; i++) {
        MIndicator *text = CreateLevelIndicator(oam, font, MPoint(96, 16), data_ov000_02195fa8[i]);
        char line[64];
        OS_SPrintf(line, data_ov000_02195e50.valueFormat, gDinoResource->GetText(data_0208a41c), i - 1, mValues[i]);
        text->SetText(std::string(line));
        rows->AddTail(text);
        mTexts[i] = text;
    }
    mCursor = new MIndicator(new MSmartObjCanvas(oam, MPoint(128, 16), gDinoResource->GetUnkTable(), TRUE), font, 0);
    ((MIndicator *)mCursor)->SetText(std::string(gDinoResource->GetText(data_0208ade0)));
    const MPoint &pos = data_ov000_02195fa8[mRow];
    mCursor->SetPosition(MPoint(pos.x + data_ov000_02195e50.cursorOffset.x, pos.y + data_ov000_02195e50.cursorOffset.y));
    mCursor->SetPriority(0);
    mCursor->Show();
    mTutorialText = CreateLevelIndicator(oam, font, MPoint(128, 16), MPoint(32, 128));
    mLiveText = CreateLevelIndicator(oam, font, MPoint(64, 16), MPoint(32, 144));
    mTutorialText->SetText(std::string("tutorial : 0"));
    mLiveText->SetText(std::string("live : 1"));
    GetParallel()->AddTail(mCursor, mTutorialText, mLiveText, oam);
    MResource<DinoAbilityIconData> defs((std::string("/etc/btl_kp_defs")));
    for (u32 i = 0; i < defs.Get()->count; i++) {
        const DinoAbilityIconData *data = defs.Get();
        int level = *(const int *)((const u8 *)data + data->entries + i * 0x18);
        if (i == 0) {
            mMin = mMax = 1;
        } else if (mMax < level) {
            mMax = level;
        }
    }
    mValues[0] = mLevel->sides[0].unk54;
    mValues[1] = mLevel->sides[1].unk54;
    mUnkA8 = mLevel->mTutorialId;
    mUnkAC = (int)mLevel->mLiveName;
    mState = data_ov000_02195e50.StateSelect_68;
}

void PlayerLevelSelect::StateSelect() {
    static const int sMaxCount = 10;
    MInput *input = mInputHub->GetInput(0);
    u32 key = input->GetPressed() & 0x3FFF;
    switch (key) {
    case 0x40:  // up
        mRow = (mRow - 1 + 4u) % 4u;
        break;
    case 0x80:  // down
        mRow = (mRow + 1) % 4u;
        break;
    case 0x400: {   // R: the next tutorial (none past the last)
        char path[64];
        OS_SPrintf(path, data_ov000_02195e50.tutorialPath, ++mUnkA8);
        MFile file((std::string(path)));
        if (!file.IsUnk10()) {
            mUnkA8 = 0;
        }
        if (mUnkA8 != 0) {
            mUnkAC = 0;
        }
        break;
    }
    case 0x800: {   // L: the next commentary
        char path[64];
        OS_SPrintf(path, data_ov000_02195e50.livePath, ++mUnkAC);
        MFile file((std::string(path)));
        if (!file.IsUnk10()) {
            mUnkAC = 0;
        }
        if (mUnkAC != 0) {
            mUnkA8 = 0;
        }
        break;
    }
    case 0x10:  // right
    case 0x20: {    // left
        int step = key == 0x20 ? -1 : 1;
        switch (mRow) {
        case 0:
        case 1:
            mValues[mRow] += step;
            break;
        case 2:
        case 3:
        {
            int &count = mCounts[mRow - 2];
            count = std::max(data_ov000_02195e50.minValue, std::min(sMaxCount, step + count));
        }
            break;
        }
        break;
    }
    case 0x8:   // start
        mLevel->sides[0].unk54 = mValues[0];
        mLevel->sides[1].unk54 = mValues[1];
        mLevel->mTutorialId = mUnkA8;
        *(int *)&mLevel->mLiveName = mUnkAC;
        if (mUnk7C) {
            mLevel->sides[0].mName = "Player1";
            mLevel->sides[1].mName = "Player2";
            for (u32 i = 0; i < 2; i++) {
                (&mLevel->sides[1].unk68)[i] = mCounts[i];
            }
        } else {
            mLevel->sides[0].mName = MGetOwnerNameUtf8();
            mLevel->sides[1].mName = "Dummy";
        }
        Exit();
        return;
    }
    mValues[mRow] = std::max(mMin, std::min(mMax, mValues[mRow]));
    const MPoint &pos = data_ov000_02195fa8[mRow];
    mCursor->SetPosition(MPoint(pos.x + data_ov000_02195e50.cursorOffset.x, pos.y + data_ov000_02195e50.cursorOffset.y));
    for (int i = 0; i <= mUnk7C; i++) {
        char text[64];
        OS_SPrintf(text, "%s %d", gDinoResource->GetText(data_0208a418), mValues[i]);
        mTexts[i]->SetText(std::string(text));
    }
    for (int i = 2; i <= 3; i++) {
        char text[64];
        OS_SPrintf(text, data_ov000_02195e50.valueFormat, gDinoResource->GetText(data_0208a41c), i - 1, mValues[i]);
        mTexts[i]->SetText(std::string(text));
    }
    char line[64];
    OS_SPrintf(line, "tutorial : %d", mUnkA8);
    mTutorialText->SetText(std::string(line));
    OS_SPrintf(line, "live : %d", mUnkAC);
    mLiveText->SetText(std::string(line));
}

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

extern "C" const u32 data_0208a424, data_0208a428;
extern const u32 kCreatureLevelLimits[2];
extern "C" const MPoint data_ov000_02195f88[2];    // by mUnk7C: the first row's position
extern "C" const MPoint data_ov000_02195f98[2];    // by mUnk7C: the step between rows

// A side's creature as packed bits (DinoBattleSide::Creature).
struct DinoBattleSelectCreature {
    u32 id : 8;
    u32 points : 10;    // the bits above the id
    u32 : 14;
    u32 base : 12;      // 0x04: none without it
    u32 : 20;
    u32 bonus1 : 12;    // 0x08
    u32 : 20;
    u32 bonus2 : 12;    // 0x0C
    u32 : 20;
    u32 bonus3 : 12;    // 0x10
    u32 : 20;
    u32 unk14;
};

static inline const DinoBattleSelectCreature *DinoBattleSelectCreatureAt(void *params, int side, int slot) {
    return (const DinoBattleSelectCreature *)&((DinoBattleParams *)params)->sides[side].mCreatures[slot];
}

// A creature's points, capped at the top level's.
static inline u32 DinoBattleSelectCreatureTotal(const DinoBattleSelectCreature *creature) {
    if (creature->base == 0) {
        return 0;
    }
    u32 sum = creature->points + (creature->bonus3 + (creature->bonus2 + (creature->base + creature->bonus1)));
    return std::min(kCreatureLevelLimits[0] * 11 + 50, sum);
}

static inline u32 DinoBattleSelectCreatureLevel(const DinoBattleSelectCreature *creature) {
    return (DinoBattleSelectCreatureTotal(creature) - 50) / kCreatureLevelLimits[0] + 1;
}

// The row's position.
static inline MPoint DinoRowPos(u32 both, int row) {
    const MPoint &base = data_ov000_02195f88[both];
    const MPoint &step = data_ov000_02195f98[both];
    return MPoint(base.x + step.x * row, base.y + step.y * row);
}

// NONMATCHING: the game loads the row tables' entries in another order (step before base) and some stack slots differ.
void UseCreatureSelect::Start() {
    mInputHub = new MSingleInputHub(1);
    mInputHub->GetInput(0)->SetRepeat(0xF0, 20, 3);
    mParallel.AddTail(mInputHub);
    MSmartOam *top = new MSmartOam(gLcd->mMain, 100, 0, TRUE);
    MSmartOam *oam = new MSmartOam(gLcd->mSub, 100, 0, TRUE);
    MFontSet *font = gDinoResource->CreateFont12(0, 10, 1);
    CreateTexts(top, font);
    mParallel.AddTail(top);
    mCount = mUnk7C ? 6 : 3;
    mHeader = new MIndicator(new MSmartObjCanvas(oam, MPoint(192, 16), gDinoResource->GetUnkTable(), TRUE), font, 0);
    mHeader->SetPosition(32, 8);
    mHeader->SetPriority(0);
    mHeader->Show();
    mHeader->SetText(std::string(gDinoResource->GetText(data_0208a428)));
    mParallel.AddTail(mHeader);
    MIndicator *label = CreateIndicator(oam, font, MPoint(96, 32), MPoint(32, 24));
    label->SetText(std::string(gDinoResource->GetText(data_0208a424)));
    mParallel.AddTail(label);
    for (int i = 0; i < mCount; i++) {
        int slot = i % 3;
        int side = i / 3;
        mIds[i] = DinoBattleSelectCreatureAt(mParams, side, slot)->id;
        mLevels[i] = DinoBattleSelectCreatureAt(mParams, side, slot)->base == 0
                         ? 0
                         : DinoBattleSelectCreatureLevel(DinoBattleSelectCreatureAt(mParams, side, slot));
        mUnkA4[i] = ((DinoBattleParams *)mParams)->sides[side].unk5C[slot];
    }
    int i = 0;
    if (i < mCount) {
    MParallelTaskSet *parallel = &mParallel;
    for (; i < mCount; i++) {
        char text[64];
        if (mUnk80 & (1 << (i / 3))) {
            OS_SPrintf(text, data_ov000_02195e50.rowFormat, mLevels[i], mIds[i], gDinoResource->GetCreatureName(mIds[i]),
                       mUnkA4[i]);
        } else {
            OS_SPrintf(text, data_ov000_02195e50.rowFormatShort, mLevels[i], mIds[i],
                       gDinoResource->GetCreatureName(mIds[i]));
        }
        mRows[i] = new MIndicator(new MSmartObjCanvas(oam, MPoint(192, 16), gDinoResource->GetUnkTable(), TRUE), font, 0);
        mRows[i]->SetText(std::string(text));
        mRows[i]->SetPosition(DinoRowPos(mUnk7C, i));
        mRows[i]->SetPriority(0);
        mRows[i]->Show();
        parallel->AddTail(mRows[i]);
    }
    }
    mCursor = new MIndicator(new MSmartObjCanvas(oam, MPoint(128, 16), gDinoResource->GetUnkTable(), TRUE), font, 0);
    mCursor->SetText(std::string(gDinoResource->GetText(data_0208ade0)));
    mCursor->SetPosition(DinoRowPos(mUnk7C, mUnk88) + data_ov000_02195e50.cursorOffset);
    mCursor->SetPriority(0);
    mCursor->Show();
    mParallel.AddTail(mCursor, oam);
    ShowCreature(mIds[mUnk88]);
    mState = data_ov000_02195e50.Select_40;
}

extern "C" u32 data_ov000_02197908[];   // (+4: the guard of Select's static)

// NONMATCHING: the game keeps `this` on the stack (sp+0x14) and the start key's loop recomputes each creature's
// address; ours is 0x4C bytes shorter. The logic follows the game case by case.
void UseCreatureSelect::Select() {
    if (!(data_ov000_02197908[1] & 1)) {
        data_ov000_02195e50.creatureCount = gDinoResource->GetCreatureCount();
        data_ov000_02197908[1] |= 1;
    }
    BOOL changed = FALSE;
    switch (mInputHub->GetInput(0)->GetPressed() & 0x3FFF) {
    case 0x2000:    // a fixed pair of creatures
        mIds[0] = 0x46;
        mIds[1] = changed;
        mIds[2] = changed;
        mIds[3] = 0x47;
        mIds[4] = changed;
        mIds[5] = changed;
        mLevels[0] = 3;
        mLevels[3] = 4;
        changed = TRUE;
        break;
    case 0x100:
        if (mLevels[mUnk88] < 12) {
            mLevels[mUnk88]++;
            changed = TRUE;
        }
        break;
    case 0x200:
        if (mLevels[mUnk88] > 1) {
            mLevels[mUnk88]--;
            changed = TRUE;
        }
        break;
    case 0x1:
        mIds[mUnk88] = (mIds[mUnk88] + 10) % data_ov000_02195e50.creatureCount;
        changed = TRUE;
        break;
    case 0x2:
        mIds[mUnk88] = (data_ov000_02195e50.creatureCount + (mIds[mUnk88] - 10)) % data_ov000_02195e50.creatureCount;
        changed = TRUE;
        break;
    case 0x400: {
        u32 row = mUnk88;
        if (mUnk80 & (1 << (row / 3))) {
            mUnkA4[row] = (mUnkA4[row] + 1) % 10;
            changed = TRUE;
        }
        break;
    }
    case 0x800: {
        u32 row = mUnk88;
        if (mUnk80 & (1 << (row / 3))) {
            mUnkA4[row] = (mUnkA4[row] + 9) % 10;
            changed = TRUE;
        }
        break;
    }
    case 0x40:
        mUnk88 = (mUnk88 + mCount - 1) % mCount;
        ShowCreature(mIds[mUnk88]);
        break;
    case 0x80:
        mUnk88 = (mUnk88 + 1) % mCount;
        ShowCreature(mIds[mUnk88]);
        break;
    case 0x20:
        mIds[mUnk88] = (data_ov000_02195e50.creatureCount + (mIds[mUnk88] - 1)) % data_ov000_02195e50.creatureCount;
        changed = TRUE;
        ShowCreature(mIds[mUnk88]);
        break;
    case 0x10:
        mIds[mUnk88] = (mIds[mUnk88] + 1) % data_ov000_02195e50.creatureCount;
        changed = TRUE;
        ShowCreature(mIds[mUnk88]);
        break;
    case 0x4:
        *mCancelled = TRUE;
        Exit();
        return;
    case 0x8:
        for (int i = 0; i < mCount; i++) {
            int slot = i % 3;
            int side = i / 3;
            DinoBattleSelectCreature *creature = (DinoBattleSelectCreature *)DinoBattleSelectCreatureAt(mParams, side, slot);
            creature->id = mIds[i];
            int level = mLevels[i];
            if (level > 12) {
                level = 12;
            }
            DinoCreaturePart *parts = (DinoCreaturePart *)&((DinoBattleParams *)mParams)->sides[side].mCreatures[slot].unk04[0];
            for (u32 j = 0; j < 4; j++) {
                parts[j].level = 0;
                parts[j].unk12 = 0;
            }
            switch (level) {
            case 12:
                parts[3].level += kCreatureLevelLimits[0];
            case 11:
                parts[3].level += kCreatureLevelLimits[0];
            case 10:
                parts[3].level += kCreatureLevelLimits[0];
            case 9:
                parts[2].level += kCreatureLevelLimits[0];
            case 8:
                parts[2].level += kCreatureLevelLimits[0];
            case 7:
                parts[2].level += kCreatureLevelLimits[0];
            case 6:
                parts[1].level += kCreatureLevelLimits[0];
            case 5:
                parts[1].level += kCreatureLevelLimits[0];
            case 4:
                parts[1].level += kCreatureLevelLimits[0];
            case 3:
                parts[0].level += kCreatureLevelLimits[0];
            case 2:
                parts[0].level += kCreatureLevelLimits[0];
            case 1:
                parts[0].level += 50;
            }
            ((DinoBattleParams *)mParams)->sides[side].unk5C[slot] = mUnkA4[i];
        }
        Exit();
        return;
    }
    if (changed) {
        char text[64];
        int i = mUnk88;
        if (mUnk80 & (1 << (i / 3))) {
            OS_SPrintf(text, data_ov000_02195e50.rowFormat, mLevels[i], mIds[i], gDinoResource->GetCreatureName(mIds[i]),
                       mUnkA4[i]);
        } else {
            OS_SPrintf(text, data_ov000_02195e50.rowFormatShort, mLevels[i], mIds[i],
                       gDinoResource->GetCreatureName(mIds[i]));
        }
        mRows[mUnk88]->SetText(std::string(text));
    }
    mCursor->SetPosition(DinoRowPos(mUnk7C, mUnk88) + data_ov000_02195e50.cursorOffset);
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
