// DinoBattleView2.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/input.h"
#include "m/console.h"
#include "game/dino_battle_debug_log.h"
#include "game/dino_resource.h"
#include "m/obj.h"
#include "game/dino_sound.h"
#include "game/dino_battle_view2.h"
#include "game/dino_message.h"
#include "types.h"
#include "game/dino_battle_view_class.h"
#include "game/wait_action_info_bar_task.h"
#include "m/smart_oam.h"
#include "game/creature_motion.h"
#include "game/dino_battle_view_creature.h"
#include "game/dino_static_init.h"
#include <string>
#include "m/indicator.h"
#include "m/oam_canvas.h"
#include "nitro/mi.h"
#include "m/lcd.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view_ui_ability_menu_item.h"
#include "game/dino_battle_help.h"

AnyInputTimerWait::~AnyInputTimerWait() {
}

DinoBattleViewUiRootSelect::~DinoBattleViewUiRootSelect() {
}

DinoBattleViewTurnEndDialog::~DinoBattleViewTurnEndDialog() {
}

DinoBattleViewGivingUpDialog::~DinoBattleViewGivingUpDialog() {
}

DinoBattleViewGivingUpMenu::~DinoBattleViewGivingUpMenu() {
}

MotionDisableHelp::~MotionDisableHelp() {
}

DinoBattleViewCreatureAttackIcon::~DinoBattleViewCreatureAttackIcon() {
}

DinoBattleViewUiRootSelectItem::~DinoBattleViewUiRootSelectItem() {
}

MenuExitTask::~MenuExitTask() {
}

WaitEnableRootMenu::~WaitEnableRootMenu() {
}

DinoBattleViewAbilityIcon::~DinoBattleViewAbilityIcon() {
    GetDisplay()->mObj->mAfn->Free(mAffine);
}

void DinoBattleViewUiRootSelectItem::OnDecide() {
}

// NONMATCHING: the frame copy loads the offset's y before x (register order only).
MRect DinoBattleViewCreatureAttackIcon::GetFrame() {
    CreatureMotion *motion = (CreatureMotion *) mCreature->mMotion;
    MSmartOam *oam = motion->mOam;
    MPoint offset(-oam->mOffset.x, -oam->mOffset.y);
    return motion->GetFrame().Moved(offset);
}

void DinoBattleViewCreatureAttackIcon::OnFocusChanged(BOOL value) {
}

void DinoBattleViewCreatureAttackIcon::OnDecide() {
}

MRect DinoBattleViewUiRootSelectItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoBattleViewUiRootSelectItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoBattleViewUiRootSelectItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoBattleViewUiRootSelectItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoBattleViewGivingUpMenu::Idle() {
    (this->*mState)();
}

// After the dialog: if the player gave up, tells the root menu (5) and exits.
void DinoBattleViewGivingUpMenu::StateDone() {
    if (mGaveUp) {
        *mResult = TRUE;
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *post = mView->CreatePostTask(new DinoBattleViewUiRootMenuResult(0, 0, 5));
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(post);
        serial->AddTail(exit);
    } else {
        Exit();
    }
}


void WaitEnableRootMenu::Execute() {
    if (((DinoBattleView *)mMenu)->GetUnk13C() <= 0) {
        Exit();
    }
}

void DinoBattleViewUiRootSelectItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(focused ? mFocusAnime : mBlurAnime, 0);
}

void MenuExitTask::Execute() {
    if (*mFlag) {
        mMenu->Exit();
    }
    Exit();
}

extern "C" MTask *func_ov000_0218ec8c(u32 arg0, int arg1, BOOL *proceed);

extern "C" const u32 data_0208a3a0, data_0208a3a4, data_0208a444;

// NONMATCHING: 99.9%, the game tests the flag with the branch the other way round (beq to the 1).
DinoBattleViewTurnEndDialog::DinoBattleViewTurnEndDialog(u32 view, MMessageObserver *target, BOOL focusNo)
    : DinoCommonYesNoMenu(std::string(gDinoResource->GetText(data_0208a444)), NULL, data_0208a3a4, data_0208a3a0,
                          !focusNo),
      mTarget(target), mUnkE0(view) {
}

void DinoBattleViewTurnEndDialog::OnStart() {
    DinoCommonYesNoMenu::OnStart();
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkE0)->GetUnk12C(), 0x20, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
}

// (Called, not inlined, by the root menu's OnDecide.)
#pragma auto_inline off
AnyInputTimerWait::AnyInputTimerWait(int frames) : MProcess(NULL), mTimer(frames) {
    MSingleInputHub *hub = new MSingleInputHub(1);
    mParallel.AddTail(hub);
    mInput = hub->GetInput(0);
}
#pragma auto_inline on

void AnyInputTimerWait::Idle() {
    if (mInput->GetPressed() & 0x1003) {
        mTimer = 0;
    }
    if (mTimer-- == 0) {
        Exit();
    }
}

void DinoBattleViewTurnEndDialog::OnFocus(MMenuItem *item) {
    DinoHelpItem *help = dynamic_cast<DinoHelpItem *>(item);
    if (help != NULL) {
        SetSelectedItem(help);
    }
    DinoSoundPlaySe(0x5E, 0);
}

void DinoBattleViewTurnEndDialog::OnDecide(MMenuItem *item) {
    DinoCommonYesNoMenu::OnDecide(item);
    DinoCommonYesNoMenuItem *yesNo = static_cast<DinoCommonYesNoMenuItem *>(item);
    BOOL proceed = TRUE;
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkE0)->GetUnk12C(), yesNo->IsYes() ? 0xC : 0xD, &proceed);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
    if (proceed) {
        if (yesNo->IsYes()) {
            DinoSoundPlaySe(0x61, 0);
            mSerial.AddTail(mTarget->CreatePostTask(new DinoBattleViewUiTurnEndResult(TRUE)));
        } else {
            DinoSoundPlaySe(0x63, 0);
            mSerial.AddTail(mTarget->CreatePostTask(new DinoBattleViewUiTurnEndResult(FALSE)));
        }
        mSerial.AddTail(MTaskExitTransfer::Create(this));
    }
}

void DinoBattleViewGivingUpDialog::OnFocus(MMenuItem *item) {
    DinoHelpItem *help = dynamic_cast<DinoHelpItem *>(item);
    if (help != NULL) {
        SetSelectedItem(help);
    }
    DinoSoundPlaySe(0x5E, 0);
}

void DinoBattleViewGivingUpDialog::OnDecide(MMenuItem *item) {
    DinoCommonYesNoMenu::OnDecide(item);
    DinoCommonYesNoMenuItem *yesNo = static_cast<DinoCommonYesNoMenuItem *>(item);
    DinoSoundPlaySe(yesNo->IsYes() ? 0x61 : 0x63, 0);
    *mGivingUp = yesNo->IsYes();
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

// The tutorial's hook first; otherwise START offers to give up.
void DinoBattleViewUiRootSelect::OnIdle() {
    MTask *task = func_ov000_0218ec8c(mView->GetUnk12C(), -1, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
        return;
    }
    if ((GetInput()->GetPressed() & 8) && ((const u8 *)mView->GetUnk12C())[0xD] == 0) {
        mGivingUp = FALSE;
        MenuExitTask *exit = new MenuExitTask(this, &mGivingUp);
        DinoBattleViewGivingUpMenu *menu = new DinoBattleViewGivingUpMenu(mObserver, &mGivingUp);
        MSerialTaskSet *serial = GetSerial();
        serial->AddTail(menu);
        serial->AddTail(exit);
    }
}

void DinoBattleViewUiRootSelect::OnFocus(MMenuItem *item) {
    DinoHelpItem *help = dynamic_cast<DinoHelpItem *>(item);
    if (help != NULL) {
        SetSelectedItem(help);
    }
    if (mSilentFocus) {
        mSilentFocus = FALSE;
        DinoSoundPlaySe(0x5D, 0);
        return;
    }
    DinoSoundPlaySe(0x5E, 0);
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u32 unk00;
    const char *disableHelpMotion;  // 0x04
    const char *rootItemMotion;     // 0x08
    void (DinoBattleViewGivingUpMenu::*StateDone_0C)();
    void (DinoBattleViewUiRootSelect::*StateIdle_14)();
    u8 unk1C[0x2C - 0x1C];
    MPoint rootItemOffset;          // 0x2C
    u8 unk34[0x4C - 0x34];
    void (DinoBattleViewGivingUpMenu::*StateAsk_4C)();
    u8 unk54[0x5C - 0x54];
    void (DinoBattleViewUiRootSelect::*StateBegin_5C)();
    u8 unk64[0x6C - 0x64];
    void (DinoBattleViewGivingUpMenu::*StateAnswered_6C)();
} data_ov000_0219569c;

// "Give up the battle?" (a text id).
extern "C" const u32 data_0208a3c4;

DinoBattleViewGivingUpMenu::DinoBattleViewGivingUpMenu(MMessageObserver *view, u8 *result)
    : MProcess(NULL), mView(view), mResult(result) {
    mState = data_ov000_0219569c.StateAsk_4C;
}

void DinoBattleViewGivingUpMenu::StateAsk() {
    *mResult = FALSE;
    mSerial.AddTail(new DinoBattleViewGivingUpDialog(gDinoResource->GetText(data_0208a3c4), &mGaveUp));
    mState = data_ov000_0219569c.StateAnswered_6C;
}

// Asks again to be sure.
void DinoBattleViewGivingUpMenu::StateAnswered() {
    if (mGaveUp) {
        mSerial.AddTail(new DinoBattleViewGivingUpDialog(gDinoResource->GetText(data_0208a3c4), &mGaveUp));
        mState = data_ov000_0219569c.StateDone_0C;
        return;
    }
    Exit();
}

#pragma auto_inline off

// R shows the next page of the log (when the log is on).
void DinoBattleDebugLog::Idle() {
    if (gDinoResource->GetBattleAdjustDefs()->debugLog) {
        if (mInputHub->GetInput(0)->mButtons & 0x400) {
            Clear();
        }
    }
}

// Adds a line to the log (when the log is on).
void DinoBattleDebugLog::Print(const char *text) {
    if (gDinoResource->GetBattleAdjustDefs()->debugLog) {
        mConsole->Print(std::string(text) + "\n");
    }
}

void DinoBattleDebugLog::Clear() {
    if (gDinoResource->GetBattleAdjustDefs()->debugLog) {
        mConsole->ClearText();
        mConsole->Clear();
    }
}

#pragma auto_inline off

extern "C" const u32 data_0208a39c, data_0208a3a0;

DinoBattleViewGivingUpDialog::DinoBattleViewGivingUpDialog(const char *text, u8 *givingUp)
    : DinoCommonYesNoMenu(std::string(text), NULL, data_0208a39c, data_0208a3a0, 1), mGivingUp(givingUp) {
}

inline DinoBattleViewUiRootSelectItem::DinoBattleViewUiRootSelectItem(DinoBattleViewUiRootSelect *menu, int index,
                                                                      u32 textId, int focusAnime, int blurAnime,
                                                                      int unkAC, int unkB0)
    : mMenu(menu), mIndex(index), mTextId(textId), mFocusAnime(focusAnime), mBlurAnime(blurAnime), mUnkAC(unkAC),
      mUnkB0(unkB0) {
    mMotion = new MMotion(std::string(data_ov000_0219569c.rootItemMotion), mMenu->GetOam(), 0, 1);
    mMotion->GetUnit()->SetMode(1);
    mParallel.AddTail(mMotion);
}

// A root menu item's layout.
struct DinoBattleRootItemDef {
    int link;       // 0x00: the direction other items link to it with
    int focusAnime; // 0x04
    int blurAnime;  // 0x08
    int unk0C;      // 0x0C
    int unk10;      // 0x10
    MPoint pos;     // 0x14
    u32 textId;     // 0x1C
};

// A help area on the bottom screen.
struct DinoBattleRootHelpDef {
    u16 slot;       // 0x00: its index in the help slots
    u16 textId;     // 0x02: 0 ends the table
    MRect rect;     // 0x04
};

struct DinoU32x4 {
    u32 v[4];
};

extern "C" DinoBattleRootItemDef data_ov000_02195870[4];   // by item
extern "C" DinoBattleRootItemDef data_ov000_02195830;      // the last item, when it's disabled
extern "C" struct {
    u8 unk00[0x14];
    DinoBattleRootHelpDef helps[1];     // 0x14
} data_ov000_021958dc;
extern "C" const u32 data_ov000_02190058[4];            // the items' help slots
extern "C" const DinoU32x4 data_ov000_02190068;         // the help directions
extern "C" const u32 data_ov000_02190078[17][4][2];     // the help slots each slot leads to, by direction
extern "C" const u32 data_0208a398, data_0208a374, data_0208a388;

// (A non-member inline: the loop-hoisted temp then takes the lowest spill slot, as in the game.)
static inline MTaskSet *DinoRootParallel(DinoBattleViewUiRootSelect *menu) {
    return &menu->mParallel;
}

// Makes the attack icons, the help areas and the four commands, and links them.
// NONMATCHING: the help loop's guard. The game loads the table pointer after the guard and hoists &mParallel between
// the guard and the loop's own check (the slots are right now: &mParallel through a non-member inline, the peer's
// find).
void DinoBattleViewUiRootSelect::OnStart() {
    mOam = new MSmartOam(4, 0, 1);
    for (u32 i = 0; i < 3; i++) {
        if (mUnkC8 & (1 << i)) {
            DinoBattleViewCreature *creature = mView->GetUnkE4(mUnkC0)->FindCreatureByB4(i);
            mIcons[i] = new DinoBattleViewCreatureAttackIcon(creature);
            AddItem(mIcons[i]);
        } else {
            mIcons[i] = NULL;
        }
    }

    DinoHelpItem *helps[17];
    MIi_CpuClear32(0, helps, sizeof(helps));
    DinoBattleHelpBase *help = new DinoBattleHelpBase(gLcd->GetTopDisplay(), MRect(4, 4, 0xFC, 0xBC), data_0208a398);
    mParallel.AddTail(help);
    helps[0] = help;
    DinoBattleRootHelpDef *def = data_ov000_021958dc.helps;
    if (def->textId != 0) {
        for (; def->textId != 0; def++) {
            help = new DinoBattleHelpBase(gLcd->GetBottomDisplay(), def->rect, def->textId);
            MTaskSet *parallel = DinoRootParallel(this);
            parallel->AddTail(help);
            helps[def->slot] = help;
        }
    }

    for (u32 i = 0; i < 2; i++) {
        DinoBattleViewPlayer *player = mView->GetUnkE4(i);
        if (!player->GetUnk108()->IsHidden()) {
            DinoBattleHelpSupport *support =
                new DinoBattleHelpSupport(player->GetUnk108(), i == mUnkC0 ? data_0208a388 : data_0208a374);
            mParallel.AddTail(support);
            helps[player->GetUnkAC() == 0 ? 9 : 15] = support;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (!(mUnkC4 & (1 << i))) {
            DinoBattleRootItemDef *def = &data_ov000_02195870[i];
            if (i == 3 && mUnkCC == 0) {
                def = &data_ov000_02195830;
            }
            mItems[i] = new DinoBattleViewUiRootSelectItem(this, i, def->textId, def->focusAnime, def->blurAnime,
                                                           def->unk0C, def->unk10);
            AddItem(mItems[i]);
            mUnk100.push_back((void *&)mItems[i]);
            MMotion *motion = mItems[i]->mMotion;
            motion->SetPosition(MPoint(def->pos.x + data_ov000_0219569c.rootItemOffset.x,
                                       def->pos.y + data_ov000_0219569c.rootItemOffset.y));
            motion->Play(def->blurAnime, 0);
            motion->SetPriority(1);
            motion->SetAnchor(1);
            motion->Show();
        } else {
            mItems[i] = NULL;
        }
        helps[data_ov000_02190058[i]] = mItems[i];
    }
    mItems[0]->SetKeys(2);

    DinoU32x4 dirs = data_ov000_02190068;
    for (u32 i = 0; i < 17; i++) {
        for (u32 dir = 0; dir < 4; dir++) {
            for (u32 j = 0; j < 2; j++) {
                u32 to = data_ov000_02190078[i][dir][j];
                if (to != 0 && helps[i] != NULL && helps[to - 1] != NULL) {
                    AddEntry((u32)helps[i], dirs.v[dir], (u32)helps[to - 1]);
                    break;
                }
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        if (mItems[i] != NULL) {
            for (int j = 0; j < 4; j++) {
                if (i != j && mItems[j] != NULL) {
                    AddLink(mItems[i], data_ov000_02195870[j].link, mItems[j]);
                }
            }
        }
    }

    if (mUnkC4 & (1 << mUnkBC)) {
        FocusItem(mItems[1]);
    } else {
        FocusItem(mItems[mUnkBC]);
    }
    if (!((u8 *)mView->GetUnk12C())[0xD]) {
        DinoHelpBase::Enable();
    }
    mParallel.AddTail(mOam);
    if (mUnkC0 == 0) {
        MTask *task = func_ov000_0218ec8c(mView->GetUnk12C(), 2, 0);
        if (task != NULL) {
            MTask *enable = new MenuEnableTask(this, TRUE);
            MTask *wait = new MTimer(1);
            MTask *disable = new MenuEnableTask(this, FALSE);
            MSerialTaskSet *serial = &mSerial;
            serial->AddTail(disable);
            serial->AddTail(wait);
            serial->AddTail(task);
            serial->AddTail(enable);
        }
    }
}

extern "C" const int data_0208a140;    // frames the disabled-attack help stays up

static inline void DinoAddTail5(MTaskSet *set, MTask *a, MTask *b, MTask *c, MTask *d, MTask *e) {
    MTaskSet *self = set;
    self->AddTail(a);
    self->AddTail(b);
    self->AddTail(c);
    self->AddTail(d);
    self->AddTail(e);
}

// A creature's attack icon decides the attack item for that creature. Past the tutorial's hook, a disabled attack
// shows the help for a while; anything else plays the items' decide animes and posts the choice.
void DinoBattleViewUiRootSelect::OnDecide(MMenuItem *item) {
    DinoBattleViewCreature *creature = NULL;
    for (u32 i = 0; i < 3; i++) {
        if (item == mIcons[i]) {
            creature = mView->GetUnkE4(mUnkC0)->FindCreatureByB4(i);
            mUnkD0 = creature->GetUnkAC();
            item = mItems[3];
            break;
        }
    }
    DinoBattleViewUiRootSelectItem *selected = (DinoBattleViewUiRootSelectItem *)item;
    int index = selected->mIndex;
    if (mUnkC4 & (1 << index)) {
        return;
    }
    int event;
    switch (index) {
    case 0:
        event = 6;
        break;
    case 2:
        event = 5;
        break;
    case 3:
        event = 4;
        break;
    case 1:
    default:
        event = 3;
        break;
    }
    BOOL proceed = TRUE;
    MTask *task = func_ov000_0218ec8c(mView->GetUnk12C(), event, &proceed);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
    if (!proceed) {
        return;
    }
    u32 usable = mUnkCC;
    if (creature != NULL) {
        usable &= 1 << creature->GetUnkB4();
    }
    if (selected->mIndex == 3 && usable == 0) {
        DinoSoundPlaySe(0x62, 0);
        MParallelTaskSet *set = new MParallelTaskSet(TRUE);
        MSmartOam *oam = new MSmartOam(4, 0, 1);
        MotionDisableHelp *help = new MotionDisableHelp(data_ov000_0219569c.disableHelpMotion, oam, 0, 1);
        help->SetPosition(MPoint(128, 96));
        help->Show();
        help->SetPriority(0);
        set->AddTail(help);
        set->AddTail(oam);
        mParallel.AddTail(set);
        DinoAddTail5(&mSerial, help->CreatePlayTransfer(1, 0), new AnyInputTimerWait(data_0208a140),
                     help->CreatePlayTransfer(2, 0), help->CreateWaitStopTask(), MTaskExitTransfer::Create(set));
        return;
    }
    for (int i = 0; i < mUnk100.size(); i++) {
        DinoBattleViewUiRootSelectItem *other = (DinoBattleViewUiRootSelectItem *)mUnk100[i];
        if (other == selected) {
            other->mMotion->Play(other->mUnkAC, 0);
        } else {
            other->mMotion->Play(other->mUnkB0, 0);
        }
    }
    MMessage *result = NULL;
    DinoBattleViewCreature *first = mView->GetUnkE4(mUnkC0)->FindCreatureByB4(0);
    if (first != NULL) {
        first->GetUnkAC();
    }
    switch (selected->mIndex) {
    case 0:
    case 1:
    case 2:
    case 3:
        DinoSoundPlaySe(0x61, 0);
        result = new DinoBattleViewUiRootMenuResult(mUnkC4, mUnkD0, selected->mIndex);
        break;
    }
    if (result != NULL) {
        DinoHelpBase::Disable();
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *post = mObserver->CreatePostTask(result);
        mSerial.AddTail(selected->mMotion->CreateWaitStopTask(), post, exit);
    }
}

// Defined after OnStart, so the item constructor inlined there calls it; the game has the out-of-line copy here.
inline MSmartOam *DinoBattleViewUiRootSelect::GetOam() {
    return mOam;
}

void DinoBattleViewUiRootSelect::StateBegin() {
    mState = data_ov000_0219569c.StateIdle_14;
}

void DinoBattleViewUiRootSelect::StateIdle() {
}

DinoBattleViewUiRootSelect::DinoBattleViewUiRootSelect(DinoBattleView *view, MMessageObserver *observer, u32 unkC0,
                                                       u32 unkC4, u32 unkBC, u32 unkC8, u32 unkCC)
    : MMenu(NULL, 0), mUnkBC(unkBC), mUnkC0(unkC0), mUnkC4(unkC4), mUnkC8(unkC8), mUnkCC(unkCC), mUnkD0(0),
      mSilentFocus(TRUE), mView(view), mObserver(observer) {
    DinoHelpBase::Disable();
    WaitEnableRootMenu *enable = new WaitEnableRootMenu(mView);
    MTask *bar = new WaitActionInfoBarTask((DinoBattleViewActionInfoBar *)mView->GetUnk104());
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(bar);
    serial->AddTail(enable);
    mState = data_ov000_0219569c.StateBegin_5C;
}

extern "C" struct {
    MtxFx22 mtx;    // 0x00: the ability icons' scale
    MtxFx22 counting;   // 0x10: their scale while counting
} data_ov000_021978e8;
extern "C" u32 data_ov000_02195d24[];   // the view's pooled statics (+0x04: the ability icon's motion)
extern "C" u32 data_ov000_02195da4[];   // the ability counters' offsets, by ability (MPoint)

// Sets up the ability icon and its four counters, shown at half size.
DinoBattleViewAbilityIcon::DinoBattleViewAbilityIcon(DinoBattleView *view, u32 oam, MPoint pos)
    : MProcess(NULL), mOam((MSmartOam *)oam), mMask(0), mWasCounting(0), mCounting(0) {
    mMotion = new MMotion(std::string(((const char **)data_ov000_02195d24)[1]), (MSmartOam *)oam, 0, 1);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(2);
    mMotion->SetAnchor(3);
    mMotion->Hide();
    mAffine = GetDisplay()->mObj->mAfn->Alloc(1, TRUE);
    mOamAffine = (GXOamAffine *)mAffine->addr;
    MFont *font = view->GetUnk124();
    for (int i = 0; i < 4; i++) {
        MSmartObjCanvas *canvas = new MSmartObjCanvas(mOam, MPoint(0x20, 0x10), view->GetUnk11CPalette(), TRUE);
        canvas->SetAnchor(1);
        mTexts[i] = new MIndicator(canvas, font, 0);
        mTexts[i]->SetPosition(MPoint(pos.x + ((const MPoint *)data_ov000_02195da4)[i].x - 12,
                                      pos.y + ((const MPoint *)data_ov000_02195da4)[i].y - 8));
        mTexts[i]->SetPriority(2);
        GetParallel()->AddTail(mTexts[i]);
        mUnits[i] = canvas->GetOamUnit();
        mUnits[i]->SetObjMode(1);
        mUnits[i]->SetAffineParam(mAffine->index);
        MTX_Scale22(&data_ov000_021978e8.mtx, 0x800, 0x800);
        G2_SetOBJAffine(mOamAffine, &data_ov000_021978e8.mtx);
    }
    mParallel.AddTail(mMotion);
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_02195dac[];
extern "C" u32 data_ov000_02195db4[];
extern "C" u32 data_ov000_02195dbc[];
static struct DinoBattleView2StaticInit {
    DinoBattleView2StaticInit() {
        DINO_POOL_WORD(data_ov000_02195d24, 0x24) = 128;
        DINO_POOL_WORD(data_ov000_02195d24, 0x28) = 176;
        DINO_POOL_WORD(data_ov000_02195d24, 0x1c) = 8;
        DINO_POOL_WORD(data_ov000_02195d24, 0x20) = 150;
        DINO_POOL_WORD(data_ov000_02195d24, 0xc) = 64;
        DINO_POOL_WORD(data_ov000_02195d24, 0x10) = 112;
        DINO_POOL_WORD(data_ov000_02195da4, 0x0) = -16;
        DINO_POOL_WORD(data_ov000_02195da4, 0x4) = 0;
        DINO_POOL_WORD(data_ov000_02195dac, 0x0) = 8;
        DINO_POOL_WORD(data_ov000_02195dac, 0x4) = 0;
        DINO_POOL_WORD(data_ov000_02195db4, 0x0) = -16;
        DINO_POOL_WORD(data_ov000_02195db4, 0x4) = 24;
        DINO_POOL_WORD(data_ov000_02195dbc, 0x0) = 8;
        DINO_POOL_WORD(data_ov000_02195dbc, 0x4) = 24;
    }
} sDinoBattleView2StaticInit;
