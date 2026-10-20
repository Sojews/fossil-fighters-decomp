// DinoUiReviveStatus, DinoUiReviveStatusDigit, DinoUiReviveEffectLight, DinoUiReviveEffectParticle.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include <algorithm>
#include "m/motion.h"
#include "game/dino_backup.h"

#include "game/dino_ui_revive_status_menu.h"
#include "game/dino_ui_revive_status.h"
#include "game/dino_ui_revive_status_digit.h"
#include "game/dino_ui_revive_effect_light.h"
#include "game/dino_ui_revive_effect_particle.h"
#include "m/bg.h"
#include "m/bg_new_task.h"
#include "m/bitmap.h"
#include "m/call_task.h"
#include "game/dino_ui_revive_effect.h"
#include "m/blend.h"
#include "m/oam_canvas.h"
#include "m/smart_oam.h"
#include "m/bright.h"
#include "m/display.h"
#include "m/input.h"
#include "m/lcd.h"
#include "m/pal.h"
#include "m/picture.h"
#include "m/seed_generator.h"
#include "game/dino_resource.h"
#include "game/dino_sound.h"
#include "game/dino_event_talk_window.h"
#include "game/dino_static_init.h"

DinoUiReviveStatus::~DinoUiReviveStatus() {
}

DinoUiReviveStatusDigit::~DinoUiReviveStatusDigit() {
}

DinoUiReviveEffectLight::~DinoUiReviveEffectLight() {
}

DinoUiReviveEffectParticle::~DinoUiReviveEffectParticle() {
}

void DinoUiReviveStatusDigit::Idle() {
}

void DinoUiReviveStatus::Idle() {
    (this->*mState)();
}

DinoUiReviveStatusParts::~DinoUiReviveStatusParts() {
}

void DinoUiReviveStatusParts::Idle() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
#pragma auto_inline off
template class MCallTask1<MBg, MBg, MIndexBitmap *>;
template MTask *MCreateCallTask(MBg *obj, void (MBg::*func)(MIndexBitmap *), MIndexBitmap * a0);
template class MCallTask0<MPalUnit, MPalUnit>;
template MTask *MCreateCallTask(MPalUnit *obj, void (MPalUnit::*func)());
#pragma auto_inline reset

// Gives the palettes back (as a task: see State0215e030).
void MPalUnit::Free() {
    pal->Free(this);
}

// Puts the screens back as they were.
DinoUiReviveEffect::~DinoUiReviveEffect() {
    mParallel.Clear();
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mMain->Setup(5, 4, 0, 0);
    gLcd->mSub->Setup(5, 4, 0, 0);
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->mMain->mBlend->Reset();
    gLcd->mSub->mBlend->Reset();
    gLcd->VSyncNow();
}

void DinoUiReviveEffect::Idle() {
    (this->*mState)();
}

extern "C" u8 data_ov003_021624c0;

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x10];
    const char *mSparkPath;                         // 0x10
    u8 unk14[0x28 - 0x14];
    const char *mParticlePath;                      // 0x28
    u8 unk2C[0x34 - 0x2C];
    void (DinoUiReviveEffect::*CheckSkip_34)();     // 0x34
    u8 unk3C[0x4C - 0x3C];
    void (DinoUiReviveEffect::*Leave_4C)();         // 0x4C
    void (DinoUiReviveEffect::*State0215da04_54)(); // 0x54

    void (DinoUiReviveEffect::*StateWait_5C)();     // 0x5C
    void (DinoUiReviveEffect::*State0215dc30_64)(); // 0x64
    u8 unk6C[0x74 - 0x6C];
    void (DinoUiReviveStatus::*Start_74)();         // 0x74
    u8 unk7C[0x84 - 0x7C];
    void (DinoUiReviveEffect::*State0215ee24_84)(); // 0x84
    void (DinoUiReviveEffect::*StateScroll_8C)();   // 0x8C
    void (DinoUiReviveEffect::*Start_94)();         // 0x94
    void (DinoUiReviveEffect::*StartWait_9C)();     // 0x9C
    u8 unkA4[0xAC - 0xA4];
    void (DinoUiReviveEffect::*State0215e030_AC)(); // 0xAC
    u8 unkB4[0xCC - 0xB4];
    void (DinoUiReviveEffect::*State0215e274_CC)(); // 0xCC
    u8 unkD4[0xE4 - 0xD4];
    void (DinoUiReviveEffect::*State0215ec2c_E4)(); // 0xE4
    void (DinoUiReviveEffect::*State0215da04_EC)(); // 0xEC
    u8 unkF4[0xFC - 0xF4];
    void (DinoUiReviveEffect::*State0215e220_FC)(); // 0xFC
    void (DinoUiReviveEffect::*State0215f668_104)(); // 0x104
} data_ov003_02161d20;

DinoUiReviveEffect::DinoUiReviveEffect(const DinoCreatureState &creature, const DinoCreatureState &creature2,
                                       u32 *result, u8 flag)
    : MProcess(NULL), mCreature(creature), mUnk94(creature), mUnkAC(creature2), mResult(result), mShowText(flag) {
    mMessages = MResource<DinoMessageData>("/msg/msg_cleaning");
    mState = data_ov003_02161d20.Start_94;
}

DinoUiReviveStatus::DinoUiReviveStatus(MDisplay *display, const DinoCreatureInfo &info, u8 flag)
    : MProcess(display), mInfo(info), mFlag(flag) {
    mState = data_ov003_02161d20.Start_74;
}

extern "C" const int data_020899fc;
extern "C" const int data_02089a48;
// (The pointer-to-member constants of the next function, pooled apart.)
extern "C" void (MBg::*data_ov003_02161dc4)(MIndexBitmap *);
extern "C" void (MPalUnit::*data_ov003_02161ddc)();

// Waits for the effect's tasks; a button skips it, putting the picture away under a fade out.
void DinoUiReviveEffect::State0215e030() {
    if (!data_ov003_021624c0 && (mInput->GetPressed() & 0x1003)) {
        MTask *mainFade;
        MTask *subFade;
        MTask *deleteBitmap;
        MTask *freePal;
        MTask *exit;
        exit = MTaskExitTransfer::Create(this);
        freePal = MCreateCallTask(mPalBlock, data_ov003_02161ddc);
        deleteBitmap = MCreateCallTask(gCurrentDisplay->mBg, data_ov003_02161dc4, mBitmap);
        subFade = gLcd->mSub->mBright->CreateFadeOutToBlack(data_02089a48, data_020899fc);
        mainFade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a48, data_020899fc);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(mainFade);
        fades->AddTail(subFade);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fades);
        serial->AddTail(deleteBitmap);
        serial->AddTail(freePal);
        serial->AddTail(exit);
        return;
    }
    if (mEffectTasks->IsEmpty()) {
        mState = data_ov003_02161d20.State0215e220_FC;
    }
}

std::string DinoReplaceTag(std::string text, std::string tag, std::string value);
int DinoReviveFrames(int frames);

// The bits of a creature part that tell whether it changed.
struct DinoRevivePartBits {
    u32 level : 12;
    u32 unk12 : 7;
    u32 unk19 : 1;
    u32 rest : 12;
};

extern const u32 gKasekiRockSizeText;

// Finds the next part that changed and takes it on; past the first, says so.
void DinoUiReviveEffect::State0215da04() {
    for (mUnkC8 = 0; mUnkC8 < mPartCount; mUnkC8++) {
        const DinoRevivePartBits &has = (const DinoRevivePartBits &)mUnkAC.parts[mUnkC8];
        const DinoRevivePartBits &had = (const DinoRevivePartBits &)mUnk94.parts[mUnkC8];
        if (had.level != has.level || had.unk12 != has.unk12 || had.unk19 != has.unk19) {
            break;
        }
    }
    if (mUnkC8 == mPartCount) {
        mState = data_ov003_02161d20.StartWait_9C;
        return;
    }
    mUnk94.parts[mUnkC8] = mUnkAC.parts[mUnkC8];
    mUnk94.unk0_8 = mUnkAC.unk0_8;
    if (mShowText && mUnk114 && mUnkC8 != 0) {
        data_ov003_021624c0 = 0;
        gDinoBackup->SetSystemVar(0x12, mUnkAC.parts[mUnkC8].level);
        std::string text;
        if (gDinoBackup->GetSystemFlag(0x2E)) {
            const u8 *data = (const u8 *)mMessages.Get();
            const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[16];
            text = (const char *)entry + ((const u32 *)entry)[1];
        } else {
            gDinoBackup->SetSystemFlag(0x2E, TRUE);
            const u8 *data = (const u8 *)mMessages.Get();
            const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[38];
            text = (const char *)entry + ((const u32 *)entry)[1];
        }
        text = DinoReplaceTag(text, "%s", gDinoResource->GetText(gKasekiRockSizeText + mUnkC8));
        mSerial.AddTail(new DinoEventTalkWindow(gCurrentDisplay, text, 0, 0));
    }
    mState = data_ov003_02161d20.State0215dc30_64;
}

// What the next function reads of a fossil's definition.
struct DinoRevivePictureDef {
    u8 unk00;
    bool flip;      // 0x01
    u8 unk02[0x08 - 0x02];
    u32 picture;    // 0x08
};

extern "C" const char data_ov003_02162484[];

// Loads the part's fossil picture and slides it up into view.
void DinoUiReviveEffect::State0215dc30() {
    u32 id = mUnkC8 + (mUnk94.index - 1) * 4 + 1;
    MResource<DinoKasekiDefs> defs = gDinoResource->LoadKasekiDefs();
    const DinoRevivePictureDef *def = (const DinoRevivePictureDef *)defs.Get()->GetDef(id);
    char path[256];
    OS_SPrintf(path, data_ov003_02162484, def->picture, 1,
               ((const DinoRevivePartBits &)mUnkAC.parts[mUnkC8]).unk19 ? 'b' : 'a');
    mPicture = MPictureFile(std::string(path), 0);
    MPal *pal = gCurrentDisplay->mBg->mPal;
    mPalBlock = pal->Alloc(mPicture.GetPaletteSize(), mPicture.GetPalette(), TRUE);
    mBitmap = gCurrentDisplay->mBg->CreateIndexBitmap(1, TRUE);
    mBitmap->Show();
    mBitmap->Fill(MRect(0, 0, 256, 256), 0);
    mBitmap->Load((const u8 *)mPicture.GetChr(), MPoint(256, 192), MPoint(0, 0), mPalBlock->index, def->flip, false);
    mEffectTasks->AddTail(mBitmap->CreateOffsetAnime(MPoint(0, -192), MPoint(0, 192), DinoReviveFrames(180), 0));
    mState = data_ov003_02161d20.State0215e030_AC;
}

inline DinoUiReviveEffectParticle::DinoUiReviveEffectParticle() : MProcess(NULL) {
    mParallel.AddTail(mTasks = new MParallelTaskSet());
    mOam = new MSmartOam(gLcd->GetTopDisplay(), 50, 0, TRUE);
    mOam->SetUnk1C(7);
    mMotion = new MMotion(std::string(data_ov003_02161d20.mParticlePath), mOam, 0, 1);
    mMotion->Play(1, 0);
    mMotion->SetPriority(2);
    mMotion->SetAnchor(1);
    mMotion->SetPosition(0x80, 0x60);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    mParallel.AddTail(mOam);
    mTimer = DinoReviveFrames(48);
}

// Sends a spark up the screen from a random place along the bottom; after enough of them, waits for the last.
void DinoUiReviveEffectParticle::Idle() {
    MMotion *spark = new MMotion(std::string(data_ov003_02161d20.mSparkPath), mOam, 0, 1);
    spark->Play(gDinoResource->GetRandom()->Next(3) + 1, 0);
    spark->SetPriority(2);
    spark->Show();
    int x = gDinoResource->GetRandom()->Next(256);
    int frames = gDinoResource->GetRandom()->Next(30) + 60;
    MTask *exit = MTaskExitTransfer::Create(spark);
    MPoint from(x, 200);
    MPoint to(x, -8);
    MTask *move = spark->CreateCoordAnime(from, to, DinoReviveFrames(frames), 0);
    MSerialTaskSet *serial = new MSerialTaskSet(TRUE);
    serial->AddTail(move);
    serial->AddTail(exit);
    MParallelTaskSet *tasks = mTasks;
    tasks->AddTail(serial);
    tasks->AddTail(spark);
    mTimer--;
    if (mTimer != 0) {
        mSerial.AddTail(new MTimer(5));
        return;
    }
    GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(mTasks), MTaskExitTransfer::Create(this));
}

// Shows the creature's new numbers under a burst of particles, waiting for the particles to finish.
void DinoUiReviveEffect::State0215f668() {
    mStatus->SetCreature(mUnk94);
    MParallelTaskSet *effects = new MParallelTaskSet();
    mParallel.AddTail(effects);
    DinoUiReviveEffectParticle *particle = new DinoUiReviveEffectParticle();
    effects->AddTail(particle);
    MSmartOam *oam = new MSmartOam(10, 0, TRUE);
    MMotion *motion = new MMotion(std::string(data_ov003_02161d20.mParticlePath), oam, 0, 1);
    motion->Play(1, 0);
    motion->SetPosition(0x80, 0x60);
    motion->Show();
    particle->GetParallel()->AddTail(motion, oam);
    MTask *exit = MTaskExitTransfer::Create(effects);
    MTask *wait = MTaskSetWaitEmptyTask::Create(effects);
    MSerialTaskSet *serial = new MSerialTaskSet(TRUE);
    serial->AddTail(wait);
    serial->AddTail(exit);
    mEffectTasks->AddTail(serial);
    mState = data_ov003_02161d20.CheckSkip_34;
}

// Raises the light and shows the creature's new numbers under the particles, then scrolls.
void DinoUiReviveEffect::State0215e9e8() {
    mScrollFrame = 0;
    DinoUiReviveEffectLight *light = mLight;
    light->mPicture->Show();
    light->mTimer = DinoReviveFrames(120);
    mStatus->SetCreature(mUnk94);
    mParallel.AddTail(new DinoUiReviveEffectParticle());
    mState = data_ov003_02161d20.StateScroll_8C;
}

// Puts the picture away.
void DinoUiReviveEffect::State0215e220() {
    gCurrentDisplay->mBg->DeleteIndexBitmap(mBitmap);
    mPalBlock->pal->Free(mPalBlock);
    if (mUnk114 && mUnkC8 == 0) {
        mState = data_ov003_02161d20.State0215e274_CC;
        return;
    }
    mState = data_ov003_02161d20.State0215ee24_84;
}

// A duration of the effect, scaled for which version of it plays.
int DinoReviveFrames(int frames);

// Puts value in place of the first tag in text (unless value has the tag itself).
std::string DinoReplaceTag(std::string text, std::string tag, std::string value);

// Fades the effect out, then says what was revived.
void DinoUiReviveEffect::State0215ec2c() {
    MTask *timer = new MTimer(1);
    MTask *fade;
    MTask *exit;
    exit = MTaskExitTransfer::Create(mUnk124);
    MBlend *blend = gCurrentDisplay->mBlend;
    fade = blend->CreateFadeRatioAnime(16, 0, DinoReviveFrames(60), 0);
    MPoint from(0, 0);
    MPoint to(0, 192);
    MTask *move = mUnk124->CreateCoordAnime(from, to, DinoReviveFrames(60), 2);
    MParallelTaskSet *anims = new MParallelTaskSet(TRUE);
    anims->AddTail(move);
    anims->AddTail(fade);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(anims);
    serial->AddTail(exit);
    serial->AddTail(timer);
    if (mShowText) {
        const u8 *data = (const u8 *)mMessages.Get();
        const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[15];
        std::string text((const char *)entry + ((const u32 *)entry)[1]);
        text = DinoReplaceTag(text, "%s", gDinoResource->GetCreatureText08(mUnkAC.index));
        mSerial.AddTail(new DinoEventTalkWindow(gCurrentDisplay, text, 0, 0));
    }
    mState = data_ov003_02161d20.State0215da04_EC;
}

// Scrolls the picture up from a random place across, with a sound on the way.
void DinoUiReviveEffect::StateScroll() {
    mScrollFrame++;
    int x = gDinoResource->GetRandom()->Next(mScroller->GetWidth() - 256);
    int height = mScroller->GetHeight();
    mScroller->SetPosition(-x, mScrollFrame * -(height - 192) / DinoReviveFrames(360));
    if (mScrollFrame == DinoReviveFrames(200)) {
        DinoSoundPlaySe(mUnkAC.index + 0x475, 0);
    }
    if (mScrollFrame >= DinoReviveFrames(360)) {
        mState = data_ov003_02161d20.State0215ec2c_E4;
    }
}
extern "C" const int data_020899d0;  // how long the effect waits

void DinoUiReviveEffect::StartWait() {
    gCurrentDisplay->mBg->SetMode(0);
    if (data_ov003_021624c0) {
        mTimer = DinoReviveFrames(0xB4);
    } else {
        mTimer = data_020899d0;
    }
    mState = data_ov003_02161d20.StateWait_5C;
}

// Waits, or until a button.
void DinoUiReviveEffect::StateWait() {
    if (!(mInput->GetPressed() & 0x100B)) {
        if (--mTimer != 0) {
            return;
        }
    }
    mState = data_ov003_02161d20.Leave_4C;
}

// A button skips the rest of the effect.
void DinoUiReviveEffect::CheckSkip() {
    if (!data_ov003_021624c0 && (mInput->GetPressed() & 0x1003)) {
        MTask *mainFade;
        MTask *subFade;
        MTask *exit;
        exit = MTaskExitTransfer::Create(this);
        subFade = gLcd->mSub->mBright->CreateFadeOutToBlack(data_02089a48, data_020899fc);
        mainFade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a48, data_020899fc);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(mainFade);
        fades->AddTail(subFade);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fades);
        serial->AddTail(exit);
        return;
    }
    mState = data_ov003_02161d20.State0215da04_54;
}

// The same, going on elsewhere. (Nothing uses this one.)
void DinoUiReviveEffect::CheckSkip2() {
    if (!data_ov003_021624c0 && (mInput->GetPressed() & 0x1003)) {
        MTask *mainFade;
        MTask *subFade;
        MTask *exit;
        exit = MTaskExitTransfer::Create(this);
        subFade = gLcd->mSub->mBright->CreateFadeOutToBlack(data_02089a48, data_020899fc);
        mainFade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a48, data_020899fc);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(mainFade);
        fades->AddTail(subFade);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fades);
        serial->AddTail(exit);
        return;
    }
    mState = data_ov003_02161d20.State0215f668_104;
}

// Fades both screens out and exits.
void DinoUiReviveEffect::Leave() {
    *mResult = 1;
    MTask *mainFade;
    MTask *subFade;
    MTask *exit;
    exit = MTaskExitTransfer::Create(this);
    subFade = gLcd->mSub->mBright->CreateFadeOutToBlack(60, 0);
    mainFade = gLcd->mMain->mBright->CreateFadeOutToBlack(60, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(mainFade);
    fades->AddTail(subFade);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

extern const u32 kCreatureLevelLimits[2];

// A creature's experience, up to what the top level needs.
u32 DinoGetCreatureTotal(const DinoCreatureState *creature) {
    if (creature->parts[0].level == 0) {
        return 0;
    }
    u32 max, sum;
    sum = creature->unk0_8 + (creature->parts[0].level + creature->parts[1].level + creature->parts[2].level +
                              creature->parts[3].level);
    max = kCreatureLevelLimits[0] * 11 + 50;
    return std::min(max, sum);
}

u32 DinoGetCreatureLevel(const DinoCreatureState *creature) {
    if (creature->parts[0].level == 0) {
        return 0;
    }
    return (DinoGetCreatureTotal(creature) - 50) / kCreatureLevelLimits[0] + 1;
}

// The experience a creature still needs for its next level (0 at the top level).
// NONMATCHING: 98%, the two inlined std::min temporaries swap stack slots.
u32 DinoGetCreatureExpToNext(const DinoCreatureState *creature) {
    u32 next;
    if (creature->parts[0].level == 0) {
        next = 0;
    } else {
        u32 level = DinoGetCreatureLevel(creature);
        next = level == 12 ? 0 : level + 1;
    }
    if (next == 0) {
        return 0;
    }
    return kCreatureLevelLimits[0] * (next - 1) + 50 - DinoGetCreatureTotal(creature);
}

DinoUiReviveStatusParts *DinoUiReviveStatus::GetParts(int i) {
    return mParts[i];
}

#pragma dont_inline on
void DinoUiReviveStatus::SetCreature(const DinoCreatureState &creature) {
    *(DinoCreatureState *)&mInfo = creature;
    MTask *wait3 = new MTimer(50);
    MSerialTaskSet *serial3 = new MSerialTaskSet(TRUE);
    MTask *wait2 = new MTimer(30);
    MSerialTaskSet *serial2 = new MSerialTaskSet(TRUE);
    MTask *wait1 = new MTimer(10);
    MSerialTaskSet *serial1 = new MSerialTaskSet(TRUE);
    MTask *wait0 = new MTimer(10);
    MSerialTaskSet *serial0 = new MSerialTaskSet(TRUE);
    MParallelTaskSet *counts = new MParallelTaskSet(TRUE);
    GetParallel()->Add(
        counts->Add(serial0->Add(wait0)->Add(mDigits[0]->CreateCountTask(((DinoCreatureState *)&mInfo)->unk0_8, 0, 0)))
            ->Add(serial1->Add(wait1)->Add(mDigits[1]->CreateCountTask(DinoGetCreatureTotal((DinoCreatureState *)&mInfo), 0, 0)))
            ->Add(serial2->Add(wait2)->Add(mDigits[2]->CreateCountTask(DinoGetCreatureLevel((DinoCreatureState *)&mInfo), 0, 0)))
            ->Add(serial3->Add(wait3)->Add(mDigits[3]->CreateCountTask(DinoGetCreatureExpToNext((DinoCreatureState *)&mInfo), 0, 0))));
}
#pragma dont_inline reset

// Rises into place, bobbing up and down.
void DinoUiReviveEffectLight::Idle() {
    if (mTimer != 0) {
        mTimer--;
        mHeight = 0x60 * DinoReviveFrames(0x78 - mTimer) / DinoReviveFrames(0x78);
    }
    mAngle += 0x190;
    mPicture->SetPosition(0, MFx32ToInt(MFx32::FromRaw(FX_SinIdx(mAngle)) * MFx32(10)) - mHeight);
}

#pragma auto_inline off
// A vector member the game emits in this file (made explicitly until the code that uses it is decompiled).
template void std::__vec_deleter<MMotionFile, std::allocator<MMotionFile> >::sub_push_back(const MMotionFile &x, Metrowerks::int2type<false>);

// (Calling these here emits the out-of-line copies of inline functions the game emits in this file.)
#pragma dont_inline on
MPoint DinoReviveStatusPointAnchor(int x, int y) {
    return MPoint(x, y);
}
MPoint DinoReviveStatusAddAnchor(const MPoint &a, const MPoint &b) {
    return a + b;
}
MTask *DinoReviveStatusTimerAnchor(int frames) {
    return new MTimer(frames);
}
MParallelTaskSet *DinoReviveStatusParallelAnchor(MProcess *process) {
    return process->GetParallel();
}
u32 DinoReviveStatusKasekiAnchor(const DinoKasekiDef *def) {
    return def->IsUncleanable() + def->GetUnk38();
}
u32 DinoReviveStatusCreatureAnchor(const DinoCreatureDef *def) {
    return def->GetVoice() + def->GetUnk0D();
}
MSmartObjCanvas *DinoReviveStatusCanvasAnchor(MSmartOam *oam, MPoint size, const void *palette) {
    return new MSmartObjCanvas(oam, size, palette, FALSE);
}
MParallelTaskSet *DinoReviveStatusSetsAnchor(MTask *task) {
    new MSerialTaskSet(TRUE);
    return (new MParallelTaskSet(TRUE))->Add(task);
}
void DinoReviveStatusStateAnchor(DinoCreatureState *a, const DinoCreatureState &b) {
    *a = b;
}
#pragma dont_inline reset

// (Empty; nothing in the game calls it.)
extern "C" void func_ov003_0215cf1c() {
}

// The file's static initialization (see dino_static_init.h).
#pragma always_inline on
static struct DinoUiReviveStatusStaticInit {
    DinoUiReviveStatusStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov003_02161d20, 0x7c, 0x1e4, 0x44, 0xb4, 0x1f4, 0x214, 0x21c, 0x204);
    }
} sDinoUiReviveStatusStaticInit;
