// DinoUiReviveStatusMenu.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_static_init.h"
#include "game/dino_ui_revive_status_menu.h"
#include "m/resource.h"
#include "game/dino_resource.h"
#include "game/dino_event_talk_window.h"
#include "types.h"
#include "game/dino_backup.h"
#include "m/lcd.h"
#include "m/bright.h"
#include "m/obj.h"
#include "m/bg.h"
#include "m/display.h"
#include "game/dino_sound.h"
#include "m/call_task.h"
#include "game/dino_ui_rv_information.h"
#include "game/dino_common_window_message_box.h"
#include <printf.h>
#include "game/dino_creature_sort.h"
#include "game/dino_ui_parts_tab_icon_menu_item.h"
#include "game/dino_ui_field_menu_cursor.h"
#include "m/input.h"
#include "m/smart_oam.h"
#include "m/picture.h"
#include "m/ext_affine.h"
#include "m/blend.h"

#pragma auto_inline off
DinoUiReviveStatusMenu::DinoUiReviveStatusMenu(u32 id, u32 *result, u8 flag)
    : MMenu(gLcd->mMain, NULL, 0), mResult(result), mUnk118(0), mUnk11C(0), mUnk120(0xFF), mFlag(flag) {
    mUnk122 = 1;
    gDinoBackup->GetCreature(id, &mCreature);
}

DinoUiReviveStatusMenu::DinoUiReviveStatusMenu(const DinoCreatureState &creature, u32 *result, u8 flag)
    : MMenu(gLcd->mMain, NULL, 0), mCreature(creature), mResult(result), mUnk118(0), mUnk11C(0), mUnk120(0xFF),
      mFlag(flag) {
    mUnk122 = 1;
}

DinoUiReviveStatusMenu::DinoUiReviveStatusMenu(const DinoCreatureState &creature, const DinoCreatureState &creature2,
                                               u8 flag)
    : MMenu(gLcd->mMain, NULL, 0), mCreature(creature), mCreature2(creature2), mResult(NULL), mUnk118(1), mUnk11C(0),
      mUnk120(0xFF), mFlag(flag) {
    mUnk122 = 0;
}

DinoUiReviveStatusMenu::DinoUiReviveStatusMenu(const DinoCreatureState &creature, const DinoCreatureState &creature2,
                                               u32 *result, u8 flag)
    : MMenu(gLcd->mMain, NULL, 0), mCreature(creature), mCreature2(creature2), mResult(result), mUnk118(1),
      mUnk11C(0), mUnk120(0xFF), mFlag(flag) {
    mUnk122 = 1;
}
#pragma auto_inline reset

DinoUiReviveStatusMenu::~DinoUiReviveStatusMenu() {
}

DinoUiReviveStatusLoop::~DinoUiReviveStatusLoop() {
}

DinoUiReviveStatusLoopCleaning::~DinoUiReviveStatusLoopCleaning() {
}

DinoUiReviveStatusLoopBattle::DinoUiReviveStatusLoopBattle(const DinoEventReviveCreature *creature,
                                                           const DinoEventReviveCreature *creature2)
    : MProcess(NULL), mCreature(*creature), mCreature2(*creature2) {
}

DinoUiReviveStatusLoopBattle::~DinoUiReviveStatusLoopBattle() {
}

DinoUiReviveStatusMenuItem::~DinoUiReviveStatusMenuItem() {
}

DinoUiReviveStatusBackButton::~DinoUiReviveStatusBackButton() {
}

MRect DinoUiReviveStatusMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiReviveStatusMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiReviveStatusMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiReviveStatusBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

const char *DinoUiReviveStatusMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiReviveStatusBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiReviveStatusMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 4; i++) {
        if (item == mItems[i]) {
            SetTab(i);
        }
    }
}

// Where the cursor sits on each status item.
extern MPoint data_020b5890[4];

void DinoUiReviveStatusMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (int i = 0; i < 4; i++) {
        if (item == mItems[i]) {
            frame.MoveTo(data_020b5890[i]);
            mCursor->SetMode(2);
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    mCursor->Show();
}

// DinoUiReviveStatusMenu::OnStart (not decompiled yet) re-enables the help through a call task.
#pragma auto_inline off
template MTask *MCreateCallTask(DinoUiReviveStatusMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiReviveStatusMenu *obj, void (DinoUiReviveStatusMenu::*func)(u8), u8 a1);
#pragma auto_inline reset

#pragma auto_inline off

// The TU's pooled statics (the back button's path and position).
extern "C" struct {
    u8 unk00[0x4];
    const char *mBackPath;      // 0x04
    u8 unk08[0x18 - 0x8];
    MPoint mBackPos;            // 0x18
    u8 unk20[0x48 - 0x20];
    MPoint mIconSize;           // 0x48: the common layout's
    u8 unk50[0x58 - 0x50];
    MPoint mIconMargin;         // 0x58
    u8 unk60[0x68 - 0x60];
    MPoint mScreenSize;         // 0x68
} data_ov007_02191e78;
extern "C" struct {
    void (DinoHelpBase::*Enable_00)();
} data_ov007_02191ef8;
// The tabs' help texts.
extern "C" const u32 data_ov007_02191f9c[4];
// "%d"-style format of the creature's number.
extern "C" const char data_ov007_021922d4[];

extern const u32 data_0208a498;
extern const u32 data_0208a86c, data_0208a868, data_0208a864;
extern const int data_02089a4c, data_02089a44;
// The help areas, and the tabs' frames.
extern MRect data_020b5920, data_020b5930, data_020b5940;
extern MRect data_020b58e0[4];

inline DinoUiReviveStatusBackButton::DinoUiReviveStatusBackButton(DinoUiReviveStatusMenu *owner)
    : DinoUiReviveStatusMenuItem(owner, data_ov007_02191e78.mBackPath, data_ov007_02191e78.mBackPos, data_0208a498) {
    SetKeys(2);
    SetMode(4);
    mMotion->Play(2, 0);
}

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = defs.Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

static inline u32 ReviveTotal(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    u32 sum = creature.unk0_8 + (creature.parts[0].level + creature.parts[1].level + creature.parts[2].level +
                                 creature.parts[3].level);
    return std::min(kCreatureLevelLimits[0] * 11 + 50, sum);
}

static inline u32 ReviveLevel(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    return (ReviveTotal(creature) - 50) / kCreatureLevelLimits[0] + 1;
}

void DinoUiReviveStatusMenu::OnStart() {
    char number[0x100];
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    mKasekiDefs = gDinoResource->LoadKasekiDefs();
    mOam = new MSmartOam(0xA, 0, TRUE);
    mUnkDC = new MParallelTaskSet();
    mParallel.AddTail(mUnkDC);
    mParallel.AddTail(mHelp134 = new DinoHelpDummyItem(GetDisplay(), data_020b5920,
                                                       std::string(gDinoResource->GetText(data_0208a86c))));
    mParallel.AddTail(
        new DinoHelpDummyItem(GetDisplay(), data_020b5930, std::string(gDinoResource->GetText(data_0208a868))));
    mParallel.AddTail(mHelp138 = new DinoHelpDummyItem(GetDisplay(), data_020b5940,
                                                       std::string(gDinoResource->GetText(data_0208a864))));
    mBack = new DinoUiReviveStatusBackButton(this);
    AddItem(mBack);
    int i = 0;
    const MRect *frames = data_020b58e0;
    const u32 *texts = data_ov007_02191f9c;
    for (; i < 4; i++) {
        mItems[i] = new DinoUiPartsTabIconMenuItem(*texts, *frames);
        AddItem(mItems[i]);
        texts++;
        frames++;
    }
    RefreshTab();
    AddLink(mItems[0], 3, mBack);
    AddLink(mBack, 1, mItems[0]);
    mCursorTasks = new MParallelTaskSet();
    mCursor = new DinoUiFieldMenuCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(2);
    mCursor->Hide();
    mParallel.AddTail(mCursorTasks);
    mParallel.AddTail(mOam);
    mInfo = new DinoUiRvInformation(0);
    mParallel.AddTail(mInfo);
    if (mUnk118) {
        mInfo->Unk02156ee8(*(DinoCreatureInfo *)&mCreature2);
    } else {
        mInfo->Unk02156ee8(*(DinoCreatureInfo *)&mCreature);
    }
    gCurrentDisplay = gLcd->GetTopDisplay();
    sprintf(number, data_ov007_021922d4, mCreature.index);
    mParallel.AddTail(mNumber = new MTextPicture(std::string(number), TRUE, false));
    mNumber->Show();
    mNumber->SetPriority(2);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    mItems[1]->SetKeys(0x400);
    mItems[3]->SetKeys(0x800);
    if (mUnk118) {
        FadeIn();
        if (ReviveLevel(mCreature2) > ReviveLevel(mCreature)) {
            PlayLevelUpEffect();
            DinoSoundPlaySe(data_02089a4c, 0);
        } else {
            DinoSoundPlaySe(data_02089a44, 0);
        }
        ShowStatChanges();
        ShowLearnedAttacks();
        Disable();
        MTask *focus = CreateFocusItemTransfer(mItems[0]);
        MTask *enable = MCreateCallTask(this, data_ov007_02191ef8.Enable_00);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(enable);
        serial->AddTail(focus);
    } else {
        FocusItem(mItems[0]);
        FadeIn();
    }
}

DinoUiReviveStatusMenuItem::DinoUiReviveStatusMenuItem(DinoUiReviveStatusMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->Show();
    mMotion->SetPriority(2);
    mParallel.AddTail(mMotion);
}

DinoUiReviveStatusLoop::DinoUiReviveStatusLoop(u32 a, u32 b) : MProcess(NULL) {
    mUnk74 = a;
    mUnk78 = b;
}

void DinoUiReviveStatusLoop::Idle() {
    gLcd->mSub->ResetHooks();
    gLcd->mMain->ResetHooks();
    gLcd->mSub->Setup(5, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mMain->Setup(5, 0, 4, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->GetBottomDisplay()->mBright->Reset();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiReviveStatusMenu(mUnk74, (u32 *)mUnk78, 0);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}

DinoUiReviveStatusLoopCleaning::DinoUiReviveStatusLoopCleaning(const DinoEventReviveCreature *creature,
                                                               const DinoEventReviveCreature *creature2, u32 *done,
                                                               u8 flag)
    : MProcess(NULL), mCreature(*creature), mCreature2(*creature2), mDone(done), mFlag(flag), mStep(0) {
}

void DinoUiReviveStatusLoopBattle::Idle() {
    gLcd->mSub->ResetHooks();
    gLcd->mMain->ResetHooks();
    gLcd->mSub->Setup(5, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mMain->Setup(5, 0, 4, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->mBg->SetMode(3);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->GetBottomDisplay()->mBright->Reset();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiReviveStatusMenu(mCreature, mCreature2, 1);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}

void DinoUiReviveStatusLoopCleaning::Idle() {
    gLcd->mSub->ResetHooks();
    gLcd->mMain->ResetHooks();
    gLcd->mSub->Setup(5, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mMain->Setup(5, 0, 4, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->mBg->SetMode(3);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    if (mCreature.parts[0].level == 0) {
        mSerial.AddTail(new DinoUiReviveStatusMenu(mCreature2, mDone, 0));
    } else {
        mSerial.AddTail(new DinoUiReviveStatusMenu(mCreature, mCreature2, mDone, mFlag));
    }
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

#pragma auto_inline off

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoHelpBase::*Enable_00)();
} data_ov007_02191eb0;

extern "C" struct {
    void (DinoUiReviveStatusMenu::*SetPage_00)(u8);
} data_ov007_02191e80;

extern const int data_02089a78, data_02089a7c;

void DinoUiReviveStatusMenu::FadeIn() {
    mUnkE0 = new MParallelTaskSet;
    mParallel.AddTail(mUnkE0);
    if (mFlag) {
        MBright *bright = gLcd->GetBottomDisplay()->mBright;
        MTask *fadeBottom = bright->CreateFadeInFromWhite(data_02089a7c, data_02089a78);
        MTask *fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeInFromWhite(data_02089a7c, data_02089a78);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        mUnkE0->AddTail(fades);
    } else {
        MBright *bright = gLcd->GetBottomDisplay()->mBright;
        MTask *fadeBottom = bright->CreateFadeInFromBlack(data_02089a7c, data_02089a78);
        MTask *fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeInFromBlack(data_02089a7c, data_02089a78);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        mUnkE0->AddTail(fades);
    }
    mSerial.AddTail(new MTimer(data_02089a7c));
}

extern const int data_02089a6c, data_02089a70;

// NONMATCHING: the two fade constants swap registers (as DinoUiKeyboardMenu::OnBackDecided).
void DinoUiReviveStatusMenu::Leave() {
    if (mUnk122) {
        mSerial.AddTail(DinoSoundCreateStopBgmAllTask(30));
    }
    if (mFlag) {
        MTask *exit = CreateExitTransfer();
        MBright *bright = gLcd->GetBottomDisplay()->mBright;
        MTask *fadeBottom = bright->CreateFadeOutToWhite(data_02089a70, data_02089a6c);
        MTask *fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToWhite(data_02089a70, data_02089a6c);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fades);
        serial->AddTail(exit);
    } else {
        MTask *exit = CreateExitTransfer();
        MBright *bright = gLcd->GetBottomDisplay()->mBright;
        MTask *fadeBottom = bright->CreateFadeOutToBlack(data_02089a70, data_02089a6c);
        MTask *fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(data_02089a70, data_02089a6c);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fades);
        serial->AddTail(exit);
    }
}

extern "C" const char data_ov007_021922e4[]; // the level up picture
extern const int data_02089a68, data_02089a58, data_02089a54, data_02089a5c, data_020899dc, data_02089a64,
    data_02089a60;

// (The blend is loaded before the cross fade's arguments.)
static inline MBlend *DinoReviveBlendOf(MDisplay *display) {
    return display->mBlend;
}

// Tasks into a new set that exits once they're done.
static inline MTask *DinoReviveParallel(MTask *a, MTask *b) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(a, b);
    return set;
}
static inline MTask *DinoReviveSerial(MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f, MTask *g, MTask *h,
                                      MTask *i, MTask *j, MTask *k) {
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    MTaskSet *self = set;
    self->AddTail(a);
    self->AddTail(b);
    self->AddTail(c);
    self->AddTail(d);
    self->AddTail(e);
    self->AddTail(f);
    self->AddTail(g);
    self->AddTail(h);
    self->AddTail(i);
    self->AddTail(j);
    self->AddTail(k);
    return set;
}

// The rank going up: the "level up" picture drops in, shakes, then zooms away fading out.
void DinoUiReviveStatusMenu::PlayLevelUpEffect() {
    MExtAffinePicture *picture = new MExtAffinePicture(std::string(data_ov007_021922e4), TRUE, false);
    picture->GetScreen()->SetCenter(0x80, 0x60);
    picture->SetPosition(0, 0);
    mParallel.AddTail(picture);
    mSerial.AddTail(new MTimer(data_02089a68));
    mSerial.AddTail(DinoReviveSerial(
        picture->CreateShowTransfer(),
        DinoReviveParallel(DinoReviveBlendOf(gCurrentDisplay)->CreateCrossFadeInTask(picture->GetPlaneMask(),
                                                                                  (u32)data_02089a64 / 2, 0),
                           picture->GetScreen()->CreateScaleAnime(MFx32Point(MFx32(2), MFx32(2)),
                                                                  MFx32Point(MFx32(1), MFx32(1)), data_02089a64,
                                                                  data_02089a60)),
        DinoSoundCreatePlaySeTask(data_020899dc, 0), picture->CreateCoordAnime(MPoint(0, 0), MPoint(-5, 0), 1, 0),
        picture->CreateCoordAnime(MPoint(-5, 0), MPoint(5, 0), 2, 0),
        picture->CreateCoordAnime(MPoint(5, 0), MPoint(-5, 0), 2, 0),
        picture->CreateCoordAnime(MPoint(-5, 0), MPoint(5, 0), 2, 0),
        picture->CreateCoordAnime(MPoint(5, 0), MPoint(0, 0), 1, 0), new MTimer(data_02089a5c),
        picture->CreateCoordAnime(MPoint(0, 0), MPoint(0, -0xC0), data_02089a58, data_02089a54),
        MTaskExitTransfer::Create(picture)));
}

// How many of the creature's parts are revived.
static inline u32 CountRevivedParts(const DinoCreatureState &creature) {
    u32 count = 0;
    for (u32 i = 0; i < 4; i++) {
        if (creature.parts[i].level) {
            count++;
        }
    }
    return count;
}

// ", ", then "\n".
extern "C" const char data_ov007_021922f8[], data_ov007_021922fc[];
extern const u32 data_0208a4b0, data_0208a4b4, data_0208a4b8;

// NONMATCHING: register allocation and one stack slot (the game keeps the attack's level in r7 and spills i * 4).
// What the creature stats are worked out from, and the stats (main's func_02030f38).
struct DinoReviveStatsInput {
    u32 id;         // 0x00
    int rank;       // 0x04
    int bonus[4];   // 0x08: by part
    int levels[5];  // 0x18: the parts', then the creature's own
};
struct DinoReviveStats {
    int hp;         // 0x00
    int stats[5];   // 0x04
};
extern "C" DinoReviveStats func_02030f38(const DinoReviveStatsInput *info, bool fixed, MFx32 fraction);

static inline void ReviveStatsInput(DinoReviveStatsInput &info, const DinoCreatureState &creature) {
    MI_CpuFill8(&info, 0, sizeof(info));
    info.id = creature.index;
    info.rank = ReviveLevel(creature);
    info.levels[4] = creature.unk0_8;
    for (int i = 0; i < 4; i++) {
        info.bonus[i] = creature.parts[i].unk12;
        info.levels[i] = creature.parts[i].level;
    }
}

// The stat texts ("HP +%d" and so on), and what goes between them.
extern const u32 data_0208a4bc, data_0208a4c0, data_0208a4c4, data_0208a4c8, data_0208a4cc;
extern "C" const char data_ov007_021922f8[];
extern const int data_02089a50;

// The stats of what info holds. (Three inline levels deep, the game's second call puts its fraction temporary below
// the level temporaries; a direct call doesn't.)
static inline DinoReviveStats ReviveStatsOf0(const DinoReviveStatsInput &info) {
    return func_02030f38(&info, false, MFx32::FromRaw(0));
}
static inline DinoReviveStats ReviveStatsOf1(const DinoReviveStatsInput &info) {
    return ReviveStatsOf0(info);
}
static inline DinoReviveStats ReviveStatsOf2(const DinoReviveStatsInput &info) {
    return ReviveStatsOf1(info);
}

// After a revival: a message box with how much each stat went up.
void DinoUiReviveStatusMenu::ShowStatChanges() {
    DinoReviveStatsInput info;
    info.levels[0] = info.levels[1] = info.levels[2] = info.levels[3] = 0;
    ReviveStatsInput(info, mCreature);
    DinoReviveStats before = func_02030f38(&info, false, MFx32::FromRaw(0));
    ReviveStatsInput(info, mCreature2);
    DinoReviveStats after = ReviveStatsOf2(info);
    std::string text;
    char line[0x80];
    int diff;
    if ((diff = after.hp - before.hp) != 0) {
        sprintf(line, gDinoResource->GetText(data_0208a4cc), diff);
        text += line;
    }
    if ((diff = after.stats[0] - before.stats[0]) != 0) {
        sprintf(line, gDinoResource->GetText(data_0208a4c8), diff);
        if (text.size() != 0) {
            text += data_ov007_021922f8;
        }
        text += line;
    }
    if ((diff = after.stats[1] - before.stats[1]) != 0) {
        sprintf(line, gDinoResource->GetText(data_0208a4c4), diff);
        if (text.size() != 0) {
            text += data_ov007_021922f8;
        }
        text += line;
    }
    if ((diff = after.stats[2] - before.stats[2]) != 0) {
        sprintf(line, gDinoResource->GetText(data_0208a4c0), diff);
        if (text.size() != 0) {
            text += data_ov007_021922f8;
        }
        text += line;
    }
    if ((diff = after.stats[3] - before.stats[3]) != 0) {
        sprintf(line, gDinoResource->GetText(data_0208a4bc), diff);
        if (text.size() != 0) {
            text += data_ov007_021922f8;
        }
        text += line;
    }
    if (text.size() != 0) {
        mSerial.AddTail(new MTimer(data_02089a50), new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), std::string(text), 4));
    }
}

void DinoUiReviveStatusMenu::ShowLearnedAttacks() {
    if (CountRevivedParts(mCreature2) <= CountRevivedParts(mCreature)) {
        return;
    }
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(mCreature.index);
    std::string names;
    u32 count = 0;
    u32 learned = 0;
    u32 known = 0;
    for (u32 i = 0; i < def->attackCount; i++) {
        if (CountRevivedParts(mCreature) >= ((const u32 *)((const u8 *)def + def->attackLevels))[i]) {
            known++;
        }
        if (CountRevivedParts(mCreature2) >= ((const u32 *)((const u8 *)def + def->attackLevels))[i]) {
            learned++;
        }
        if (learned > known + count) {
            if (count) {
                names += data_ov007_021922fc;
            }
            count++;
            names += gDinoResource->GetAttackName(((const u32 *)((const u8 *)def + def->attackIds))[i]);
        }
    }
    if (known == learned) {
        if (!gDinoBackup->GetSystemFlag(0x27)) {
            mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x60),
                                                           std::string(gDinoResource->GetText(data_0208a4b0)), 4));
        }
    } else {
        std::string text;
        char line[0x80];
        sprintf(line, gDinoResource->GetText(data_0208a4b8), names.c_str());
        text += line;
        if (learned == def->attackCount) {
            text += data_ov007_021922f8;
            text += gDinoResource->GetText(data_0208a4b4);
        }
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), std::string(text), 4));
    }
}

// Turns to the tab after a frame, and keys the tabs either side of it to L and R.
void DinoUiReviveStatusMenu::SetTab(u8 tab) {
    if (tab > 3 || mUnk11C == tab) {
        return;
    }
    mUnk11C = tab;
    DinoSoundPlaySe(0x94, 0);
    MTask *setPage = MCreateCallTask(this, data_ov007_02191e80.SetPage_00, tab);
    MTask *timer = new MTimer(1);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(setPage);
    Disable();
    mItems[tab % 4]->SetKeys(0);
    mItems[(tab + 1) % 4]->SetKeys(0x400);
    mItems[(tab + 2) % 4]->SetKeys(0);
    mItems[(tab + 3) % 4]->SetKeys(0x800);
    RefreshTab();
}

// Turns the information to the page; help comes back once it has.
void DinoUiReviveStatusMenu::SetPage(u8 page) {
    mInfo->SetPage(page);
    GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(&mInfo->mSerial),
                         MCreateCallTask(this, data_ov007_02191eb0.Enable_00));
    Disable();
}

extern const int data_02089a74;
struct DinoMessageData;
// The revive messages' file, then (at +0x14) what goes before the second answer.
extern "C" const char data_ov007_02192300[];
extern const u32 data_0208a49c, data_0208a4a0, data_0208a4a4, data_0208a4a8;

// The message's text.
static inline const char *GetMessageText(const DinoMessageData *messages, u32 id) {
    const u8 *data = (const u8 *)messages;
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[id];
    return (const char *)entry + ((const u32 *)entry)[1];
}

// Leaves; with a result to give and rocks still to clean, first asks whether to keep reviving.
void DinoUiReviveStatusMenu::OnBackDecided() {
    Disable();
    MTask *wait = MTaskSetWaitEmptyTask::Create(mUnkE0);
    MTask *timer = new MTimer(data_02089a74);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(wait);
    if (mResult != NULL) {
        if (gDinoBackup->HasInactiveUnk98()) {
            gLcd->mMain->mBg->SetMode(0);
            *mResult = 0;
            MResource<DinoMessageData> messages((std::string(data_ov007_02192300)));
            DinoEventTalkWindow *talk = new DinoEventTalkWindow(
                gCurrentDisplay, std::string(GetMessageText(messages.Get(), 17)), (int)mOam, 0);
            talk->SetYesNo((u32)mResult, MPoint(0xFE, 0x8E), 3, std::string(gDinoResource->GetText(data_0208a4a8)),
                           std::string(data_ov007_02192300 + 0x14) + gDinoResource->GetText(data_0208a4a4),
                           std::string(gDinoResource->GetText(data_0208a4a0)),
                           std::string(gDinoResource->GetText(data_0208a49c)));
            mSerial.AddTail(talk);
            Disable();
        } else {
            *mResult = 1;
        }
    }
    Leave();
}

extern const u32 data_0208a7b0, data_0208a7b4, data_0208a7b8, data_0208a7bc, data_0208a7c0, data_0208a7c4, data_0208a7c8, data_0208a7cc, data_0208a7d0, data_0208a7d4, data_0208a7d8, data_0208a7dc, data_0208a7e0, data_0208a7e4, data_0208a7e8, data_0208a7f4, data_0208a7f8, data_0208a7fc, data_0208a800, data_0208a804, data_0208a808, data_0208a80c, data_0208a810, data_0208a81c, data_0208a820, data_0208a840, data_0208a844, data_0208a848, data_0208a84c, data_0208a850, data_0208a854, data_0208a858, data_0208a85c, data_0208a860;
extern MRect data_020b5950, data_020b5960, data_020b5970, data_020b5980, data_020b5990, data_020b59a0, data_020b59b0, data_020b59c0, data_020b59d0, data_020b59e0, data_020b59f0, data_020b5a00, data_020b5a10, data_020b5a20, data_020b5a30, data_020b5a40, data_020b5a50, data_020b5a60, data_020b5a70, data_020b5a80;

// The help areas of the top screen's page, by tab (and for the fossil tab, by the part shown).
void DinoUiReviveStatusMenu::RefreshTab() {
    u8 mode;
    switch (mUnk11C) {
    case 0:
        mode = 0;
        break;
    case 1:
    case 2:
        mode = 1;
        break;
    case 3: {
        int index = (mCreature.index - 1) * 4 + 1;
        u8 uncleanable = mKasekiDefs.Get()->GetDef(index)->IsUncleanable();
        u32 part = mKasekiDefs.Get()->GetDef(index)->GetUnk38();
        if (uncleanable) {
            mode = part + 2;
        } else {
            mode = 2;
        }
        break;
    }
    }
    if (mUnk120 == mode) {
        return;
    }
    mUnk120 = mode;
    mUnkDC->Clear();
    switch (mUnk120) {
    case 0:
        mUnkDC->AddTail(mPageHelps[0] = new DinoHelpDummyItem(GetDisplay(), data_020b5950, std::string(gDinoResource->GetText(data_0208a860))));
        mUnkDC->AddTail(mPageHelps[1] = new DinoHelpDummyItem(GetDisplay(), data_020b5960, std::string(gDinoResource->GetText(data_0208a85c))));
        mUnkDC->AddTail(mPageHelps[2] = new DinoHelpDummyItem(GetDisplay(), data_020b5970, std::string(gDinoResource->GetText(data_0208a858))));
        mUnkDC->AddTail(new DinoHelpDummyItem(GetDisplay(), data_020b5980, std::string(gDinoResource->GetText(data_0208a854))));
        mUnkDC->AddTail(mPageHelps[4] = new DinoHelpDummyItem(GetDisplay(), data_020b5990, std::string(gDinoResource->GetText(data_0208a850))));
        mUnkDC->AddTail(mPageHelps[5] = new DinoHelpDummyItem(GetDisplay(), data_020b59a0, std::string(gDinoResource->GetText(data_0208a84c))));
        mUnkDC->AddTail(mPageHelps[6] = new DinoHelpDummyItem(GetDisplay(), data_020b59b0, std::string(gDinoResource->GetText(data_0208a848))));
        mUnkDC->AddTail(mPageHelps[7] = new DinoHelpDummyItem(GetDisplay(), data_020b59c0, std::string(gDinoResource->GetText(data_0208a844))));
        mUnkDC->AddTail(mPageHelps[8] = new DinoHelpDummyItem(GetDisplay(), data_020b59d0, std::string(gDinoResource->GetText(data_0208a840))));
        AddEntry((u32)mHelp134, 2, (u32)mPageHelps[0]);
        AddEntry((u32)mHelp138, 2, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[0], 2, (u32)mPageHelps[1]);
        AddEntry((u32)mPageHelps[0], 3, 0);
        AddEntry((u32)mPageHelps[0], 1, 0);
        AddEntry((u32)mPageHelps[1], 4, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[1], 1, (u32)mPageHelps[2]);
        AddEntry((u32)mPageHelps[2], 1, 0);
        AddEntry((u32)mPageHelps[4], 1, 0);
        AddEntry((u32)mPageHelps[5], 2, (u32)mPageHelps[7]);
        AddEntry((u32)mPageHelps[6], 1, 0);
        AddEntry((u32)mPageHelps[7], 3, 0);
        AddEntry((u32)mPageHelps[7], 1, 0);
        AddEntry((u32)mPageHelps[8], 3, 0);
        AddEntry((u32)mPageHelps[8], 1, 0);
        for (int i = 0; i < 4; i++) {
            AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mItems[i], 4, (u32)mPageHelps[8]);
        }
        AddEntry((u32)(DinoHelpItem *)mBack, 4, (u32)mPageHelps[8]);
        break;
    case 1:
        mUnkDC->AddTail(mPageHelps[0] = new DinoHelpDummyItem(GetDisplay(), data_020b59e0, std::string(gDinoResource->GetText(data_0208a820))));
        mUnkDC->AddTail(new DinoHelpDummyItem(GetDisplay(), data_020b59f0, std::string(gDinoResource->GetText(data_0208a81c))));
        AddEntry((u32)mPageHelps[0], 2, (u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mItems[0]);
        break;
    case 2:
        mUnkDC->AddTail(mPageHelps[0] = new DinoHelpDummyItem(GetDisplay(), data_020b5a00, std::string(gDinoResource->GetText(data_0208a810))));
        mUnkDC->AddTail(new DinoHelpDummyItem(GetDisplay(), data_020b5a10, std::string(gDinoResource->GetText(data_0208a80c))));
        mUnkDC->AddTail(new DinoHelpDummyItem(GetDisplay(), data_020b5a20, std::string(gDinoResource->GetText(data_0208a808))));
        mUnkDC->AddTail(mPageHelps[3] = new DinoHelpDummyItem(GetDisplay(), data_020b5a30, std::string(gDinoResource->GetText(data_0208a804))));
        mUnkDC->AddTail(mPageHelps[4] = new DinoHelpDummyItem(GetDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a7f4))));
        mUnkDC->AddTail(mPageHelps[5] = new DinoHelpDummyItem(GetDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a800))));
        mUnkDC->AddTail(mPageHelps[6] = new DinoHelpDummyItem(GetDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a7fc))));
        mUnkDC->AddTail(mPageHelps[7] = new DinoHelpDummyItem(GetDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a7f8))));
        AddEntry((u32)mPageHelps[0], 2, (u32)mPageHelps[4]);
        AddEntry((u32)mPageHelps[3], 2, (u32)mPageHelps[4]);
        AddEntry((u32)mPageHelps[4], 4, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[4], 3, 0);
        AddEntry((u32)mPageHelps[4], 1, 0);
        AddEntry((u32)mPageHelps[5], 3, 0);
        AddEntry((u32)mPageHelps[5], 1, 0);
        AddEntry((u32)mPageHelps[6], 3, 0);
        AddEntry((u32)mPageHelps[6], 1, 0);
        AddEntry((u32)mPageHelps[7], 3, 0);
        AddEntry((u32)mPageHelps[7], 1, 0);
        for (int i = 0; i < 4; i++) {
            AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mItems[i], 4, (u32)mPageHelps[7]);
        }
        AddEntry((u32)(DinoHelpItem *)mBack, 4, (u32)mPageHelps[7]);
        break;
    case 3:
    case 4:
    case 5:
        switch (mUnk120) {
        case 3:
            mUnkDC->AddTail(mPageHelps[0] = new DinoHelpDummyItem(GetDisplay(), data_020b5a80, std::string(gDinoResource->GetText(data_0208a7e8))));
            mUnkDC->AddTail(mPageHelps[1] = new DinoHelpDummyItem(GetDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a7e4))));
            mUnkDC->AddTail(mPageHelps[2] = new DinoHelpDummyItem(GetDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a7e0))));
            mUnkDC->AddTail(mPageHelps[3] = new DinoHelpDummyItem(GetDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a7dc))));
            mUnkDC->AddTail(mPageHelps[4] = new DinoHelpDummyItem(GetDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a7d8))));
            break;
        case 4:
            mUnkDC->AddTail(mPageHelps[0] = new DinoHelpDummyItem(GetDisplay(), data_020b5a80, std::string(gDinoResource->GetText(data_0208a7d4))));
            mUnkDC->AddTail(mPageHelps[1] = new DinoHelpDummyItem(GetDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a7d0))));
            mUnkDC->AddTail(mPageHelps[2] = new DinoHelpDummyItem(GetDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a7cc))));
            mUnkDC->AddTail(mPageHelps[3] = new DinoHelpDummyItem(GetDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a7c8))));
            mUnkDC->AddTail(mPageHelps[4] = new DinoHelpDummyItem(GetDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a7c4))));
            break;
        case 5:
            mUnkDC->AddTail(mPageHelps[0] = new DinoHelpDummyItem(GetDisplay(), data_020b5a80, std::string(gDinoResource->GetText(data_0208a7c0))));
            mUnkDC->AddTail(mPageHelps[1] = new DinoHelpDummyItem(GetDisplay(), data_020b5a40, std::string(gDinoResource->GetText(data_0208a7bc))));
            mUnkDC->AddTail(mPageHelps[2] = new DinoHelpDummyItem(GetDisplay(), data_020b5a50, std::string(gDinoResource->GetText(data_0208a7b8))));
            mUnkDC->AddTail(mPageHelps[3] = new DinoHelpDummyItem(GetDisplay(), data_020b5a60, std::string(gDinoResource->GetText(data_0208a7b4))));
            mUnkDC->AddTail(mPageHelps[4] = new DinoHelpDummyItem(GetDisplay(), data_020b5a70, std::string(gDinoResource->GetText(data_0208a7b0))));
            break;
        }
        AddEntry((u32)mHelp134, 2, (u32)mPageHelps[0]);
        AddEntry((u32)mPageHelps[0], 3, 0);
        AddEntry((u32)mPageHelps[0], 1, 0);
        AddEntry((u32)mPageHelps[1], 3, 0);
        AddEntry((u32)mPageHelps[1], 1, 0);
        AddEntry((u32)mPageHelps[2], 3, 0);
        AddEntry((u32)mPageHelps[2], 1, 0);
        AddEntry((u32)mPageHelps[3], 3, 0);
        AddEntry((u32)mPageHelps[3], 1, 0);
        AddEntry((u32)mPageHelps[4], 3, 0);
        AddEntry((u32)mPageHelps[4], 1, 0);
        for (int i = 0; i < 4; i++) {
            AddEntry((u32)(DinoHelpItem *)(DinoUiPartsTabIconMenuItem *)mItems[i], 4, (u32)mPageHelps[4]);
        }
        AddEntry((u32)(DinoHelpItem *)mBack, 4, (u32)mPageHelps[4]);
        break;
    }
}

// NONMATCHING: the game interleaves the screen rect's stores with the size's and keeps one copy of each size.
// The file's static initialization (see dino_static_init.h): the common layout, the button and panel positions and the
// tabs' texts.
extern "C" struct {
    u8 unk00[0x4];
    MPoint mBarPos;     // 0x04
    MPoint mBarSize;    // 0x0C
    MRect mBarRect;     // 0x14
    u8 unk24[0x34 - 0x24];
    MRect mScreenRect;  // 0x34
    MRect mIconRect;    // 0x44
} data_ov007_02191f78;
static inline MRect DinoBarRect(MPoint size) {
    return MRect(0, 0, size.x, 24);
}
extern const u32 data_0208a830, data_0208a834, data_0208a838, data_0208a83c;
#pragma always_inline on
static struct DinoUiReviveStatusMenuStaticInit {
    DinoUiReviveStatusMenuStaticInit() {
        data_ov007_02191e78.mScreenSize = MPoint(256, 192);
        data_ov007_02191f78.mScreenRect = DinoSizeRect(data_ov007_02191e78.mScreenSize);
        data_ov007_02191e78.mIconMargin = MPoint(2, 2);
        data_ov007_02191e78.mIconSize = MPoint(36, 28);
        data_ov007_02191f78.mIconRect = DinoSizeRect(data_ov007_02191e78.mIconSize);
        data_ov007_02191f78.mBarPos = MPoint(0, 24);
        data_ov007_02191f78.mBarSize = MPoint(256, 192);
        data_ov007_02191f78.mBarRect = MRect(0, 0, 256, 24);
        DINO_POOL(MPoint, data_ov007_02191e78, 0x28) = MPoint(128, 176);
        DINO_POOL(MPoint, data_ov007_02191e78, 0x60) = MPoint(8, 150);
        DINO_POOL(MPoint, data_ov007_02191e78, 0x40) = MPoint(64, 112);
        DINO_POOL(MPoint, data_ov007_02191e78, 0x70) = MPoint(19, 179);
        DINO_POOL(MPoint, data_ov007_02191e78, 0x30) = MPoint(27, 0);
        DINO_POOL(MPoint, data_ov007_02191e78, 0x78) = MPoint(128, 96);
        DINO_POOL(MPoint, data_ov007_02191ef8, 0x10) = MPoint(128, 96);
        DINO_POOL(MPoint, data_ov007_02191e78, 0x18) = MPoint(128, 98);
        DINO_POOL(MPoint, data_ov007_02191e78, 0x20) = MPoint(246, 28);
        DINO_POOL_WORD(data_ov007_02191f78, 0x24) = data_0208a83c;
        DINO_POOL_WORD(data_ov007_02191f78, 0x28) = data_0208a838;
        DINO_POOL_WORD(data_ov007_02191f78, 0x2c) = data_0208a834;
        DINO_POOL_WORD(data_ov007_02191f78, 0x30) = data_0208a830;
    }
} sDinoUiReviveStatusMenuStaticInit;
