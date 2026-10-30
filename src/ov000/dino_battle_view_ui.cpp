// DinoBattleViewUi.

// The game built this file without common-subexpression elimination (loops reload their bounds).
#pragma opt_common_subs off

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/lcd.h"
#include "game/dino_sound.h"
#include "game/dino_message.h"
#include "game/dino_battle_view_ui_change_menu.h"
#include "types.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_view_creature.h"
#include "game/dino_battle_view_ui_attacker_menu.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_battle_view_ui_defender_menu.h"
#include "game/dino_battle_view_ui_select_all.h"
#include "game/dino_battle_view.h"
#include "game/dino_battle_view_creature_info.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view2.h"
#include "game/dino_static_init.h"

// The tutorial step for an action (or -1): a task to run, and whether the action is blocked.
extern "C" MTask *func_ov000_0218ec8c(u32 arg0, int arg1, BOOL *blocked);

DinoBattleViewUiSelectAreaCommon::~DinoBattleViewUiSelectAreaCommon() {
}

DinoBattleViewUiChangeMenu::~DinoBattleViewUiChangeMenu() {
}

DinoBattleViewUiAttackerMenu::~DinoBattleViewUiAttackerMenu() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoBattleViewCreatureInfo, DinoBattleViewCreatureInfo>;
#pragma auto_inline off
template MTask *MCreateCallTask(DinoBattleViewCreatureInfo *obj, void (DinoBattleViewCreatureInfo::*func)());
#pragma auto_inline reset

DinoBattleViewUiDefenderMenu::~DinoBattleViewUiDefenderMenu() {
    for (int i = 0; i < mCount; i++) {
        DinoBattleViewCreature *creature = ((DinoBattleView *)mUnkEC)->FindCreatureByAC(mIds[i]);
        if (((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0)->GetUnkAC() == creature->GetUnkB0()) {
            creature->ClearFlag144(4);
        } else {
            creature->ClearFlag144(3);
        }
    }
}

DinoBattleViewUiSelectAllMenu::~DinoBattleViewUiSelectAllMenu() {
    for (int i = 0; i < mCount; i++) {
        DinoBattleViewCreature *creature = ((DinoBattleView *)mView)->FindCreatureByAC(mIds[i]);
        if (((DinoBattleView *)mView)->GetUnkE4(mPlayer)->GetUnkAC() == creature->GetUnkB0()) {
            creature->ClearFlag144(4);
        } else {
            creature->ClearFlag144(3);
        }
    }
}

void DinoBattleViewUiSelectAllMenu::OnDecide(MMenuItem *item) {
    DinoHelpBase::Disable();
    if (item == mOkItem) {
        DinoSoundPlaySe(0x63, 0);
        mTarget->Post(new DinoBattleViewUiDefenderMenuResult(mIds[0], TRUE));
    } else {
        DinoSoundPlaySe(0x61, 0);
        mTarget->Post(new DinoBattleViewUiDefenderMenuResult(mIds[0], FALSE));
    }
    Exit();
}

DinoBattleViewUiRootMenuItem::~DinoBattleViewUiRootMenuItem() {
}


DinoBattleViewUiCreatureInfoMenu::~DinoBattleViewUiCreatureInfoMenu() {
}

DinoBattleViewInfoLRItem::~DinoBattleViewInfoLRItem() {
}

DinoBattleViewCancelMenuItem::~DinoBattleViewCancelMenuItem() {
}

DinoBattleHelpCreatureInfo::~DinoBattleHelpCreatureInfo() {
}

DinoFrameItem::~DinoFrameItem() {
}

void DinoBattleHelpBase::Idle() {
}

void DinoBattleViewCancelMenuItem::OnDecide() {
}

// The view's frame, moved; off the screen while it's hidden.
MRect DinoBattleViewInfoLRItem::GetFrame() {
    if (mView->IsHidden()) {
        return MRect(0x140, 0x140, 0x140, 0x140);
    }
    return mView->GetFrame().Moved(mOffset);
}

void DinoBattleViewInfoLRItem::OnDecide() {
}

// The root menu's rows (two words each).
struct DinoBattleViewUiRootRow {
    u32 unk0;   // the creature's side (unkB0)
    u32 unk4;   // its slot (unkB4)
};
extern "C" const DinoBattleViewUiRootRow data_ov000_0218fec4[];

u32 DinoBattleViewUiRootMenuItem::GetUnk4() {
    return data_ov000_0218fec4[mIndex].unk4;
}

u32 DinoBattleViewUiRootMenuItem::GetUnk0() {
    return data_ov000_0218fec4[mIndex].unk0;
}

void DinoBattleViewUiRootMenuItem::OnDecide() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.

MRect DinoBattleViewCancelMenuItem::GetFrame() {
    return MRect(0, 0, 256, 192);
}

void DinoBattleViewUiDefenderMenu::OnFocus(MMenuItem *item) {
    DinoBattleViewUiSelectAreaCommon::OnFocus(item);
    DinoSoundPlaySe(0x5F, 0);
}

#pragma auto_inline off
BOOL DinoBattleViewUiDefenderMenu::CanDefend(u32 id) {
    for (int i = 0; i < mCount; i++) {
        if (id == mIds[i]) {
            return TRUE;
        }
    }
    return FALSE;
}
#pragma auto_inline reset

extern const u32 data_ov000_0218fd00[]; // tutorial steps by creature slot

void DinoBattleViewUiDefenderMenu::OnDecide(MMenuItem *item) {
    DinoBattleViewUiRootMenuItem *root = dynamic_cast<DinoBattleViewUiRootMenuItem *>(item);
    if (HandlePageItem(item)) {
        return;
    }
    if (root != NULL) {
        DinoBattleViewCreature *creature = GetItemCreature(root);
        if (root->mDisabled) {
            DinoSoundPlaySe(0x62, 0);
            return;
        }
        if (creature != NULL) {
            if (CanDefend(creature->GetUnkAC())) {
                BOOL blocked = FALSE;
                MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(),
                                                  data_ov000_0218fd00[creature->GetUnkB4()], &blocked);
                if (task != NULL) {
                    mSerial.AddTail(task);
                }
                if (blocked) {
                    return;
                }
                DinoHelpBase::Disable();
                DinoSoundPlaySe(0x61, 0);
                ResetRows();
                DinoHelpBase::Disable();
                MTask *exit = MTaskExitTransfer::Create(this);
                MTask *post = mTarget->CreatePostTask(new DinoBattleViewUiDefenderMenuResult(creature->GetUnkAC(), FALSE));
                MTask *wait = MTaskSetWaitEmptyTask::Create(mUnkF8);
                MSerialTaskSet *serial = &mSerial;
                serial->AddTail(wait);
                serial->AddTail(post);
                serial->AddTail(exit);
            } else {
                DinoSoundPlaySe(0x62, 0);
            }
            return;
        }
    }
    if (item == mItem120) {
        BOOL blocked = FALSE;
        MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), 0x27, &blocked);
        if (task != NULL) {
            mSerial.AddTail(task);
        }
        if (!blocked) {
            DinoHelpBase::Disable();
            DinoSoundPlaySe(0x63, 0);
            ResetRows();
            DinoHelpBase::Disable();
            MTask *exit = MTaskExitTransfer::Create(this);
            MTask *post = mTarget->CreatePostTask(new DinoBattleViewUiDefenderMenuResult(0, TRUE));
            MTask *wait = MTaskSetWaitEmptyTask::Create(mUnkF8);
            MSerialTaskSet *serial = &mSerial;
            serial->AddTail(wait);
            serial->AddTail(post);
            serial->AddTail(exit);
        }
    }
}

void DinoBattleViewUiSelectAreaCommon::OnDecide(MMenuItem *item) {
    Exit();
}

extern "C" void func_ov000_02177590(void *owner, DinoBattleViewUiRootMenuItem *item, int arg2, int arg3);

void DinoBattleViewUiRootMenuItem::OnSelectChanged(BOOL selected) {
    Refresh();
}

#pragma auto_inline off

void DinoBattleViewUiRootMenuItem::Refresh() {
}

#pragma auto_inline on

MRect DinoFrameItem::GetFrame() {
    return mFrame;
}


void DinoBattleViewUiChangeMenu::OnIdle() {
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), -1, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
}

// Has the 3D view show the focused creature's status, and marks its type advantages against the other side.
void DinoBattleViewUiCreatureInfoMenu::OnFocus(MMenuItem *item) {
    DinoBattleViewUiSelectAreaCommon::OnFocus(item);
    if (item == mPageItems[0] || item == mPageItems[1] || item == mItem120) {
        return;
    }
    DinoSoundPlaySe(0x5F, 0);
    DinoBattleViewCreature *creature = GetItemCreature(item);
    ((DinoBattleView *)mUnkEC)->Post(new DinoBattleView3DWatchStatusRequest(0, creature->GetUnkAC()));
    DinoBattleViewUiRootMenuItem *root = dynamic_cast<DinoBattleViewUiRootMenuItem *>(item);
    if (root != NULL) {
        DinoBattleViewCreature *focused = GetItemCreature(root);
        DinoBattleViewPlayer *player = (DinoBattleViewPlayer *)focused->GetUnkDC();
        DinoBattleViewPlayer *opponent = player->GetOpponent();
        for (int i = 0; i < opponent->GetCreatureCount(); i++) {
            opponent->GetCreature(i)->SetAdvantage(focused);
        }
        for (int i = 0; i < player->GetCreatureCount(); i++) {
            player->GetCreature(i)->SetAdvantage(NULL);
        }
    }
}

// Where the creature info help is shown, and its text.
extern "C" const MRect data_ov000_02193c10;
extern "C" const u32 data_0208a3c4[];

static inline u8 GetUnk12CFlag(DinoBattleView *view) {
    return ((const u8 *)view->GetUnk12C())[0xD];
}

void DinoBattleViewUiCreatureInfoMenu::OnStart() {
    mParallel.AddTail(
        new DinoBattleHelpCreatureInfo(gLcd->GetBottomDisplay(), data_ov000_02193c10, data_0208a3c4[0], this));
    DinoBattleViewUiSelectAreaCommon::OnStart();
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), 0x19, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
    if (!GetUnk12CFlag((DinoBattleView *)mUnkEC)) {
        mSerial.AddTail(DinoHelpBase::CreateEnableTask(this));
    }
}

// Where the change help is shown for each side, and its text.
extern "C" const MRect data_ov000_02193ca0[];
extern "C" const u32 data_0208a34c;

void DinoBattleViewUiChangeMenu::OnStart() {
    mParallel.AddTail(new DinoBattleHelpBase(gLcd->GetBottomDisplay(), data_ov000_02193ca0[mSide], data_0208a34c));
    DinoBattleViewUiSelectAreaCommon::OnStart();
    if (!GetUnk12CFlag((DinoBattleView *)mUnkEC)) {
        DinoHelpBase &help = *this;
        mSerial.AddTail(DinoHelpBase::CreateEnableTask(&help));
    }
}

// Per side and row of the root menu: where each arrow goes, and which entry it points at.
struct ChangeMenuArrow {
    MPoint offset;  // from the slot
    u32 unk8;
    u32 id;         // 0x0C
    u32 anime;      // 0x10: while focused
    u32 offAnime;   // 0x14: another was chosen
    u32 onAnime;    // 0x18: this one was chosen
};
extern "C" const ChangeMenuArrow data_ov000_02193d20[][3];

// Shows the arrows pointing at the focused row's entry.
void DinoBattleViewUiChangeMenu::OnFocus(MMenuItem *item) {
    DinoBattleViewUiSelectAreaCommon::OnFocus(item);
    DinoBattleViewUiRootMenuItem *root = dynamic_cast<DinoBattleViewUiRootMenuItem *>(item);
    if (root != NULL) {
        DinoSoundPlaySe(0x5F, 0);
        mFocusedRow = root->mIndex;
        for (int i = 0; i < 2; i++) {
            if (root->GetUnk4() == data_ov000_02193d20[root->GetUnk0()][i].id) {
                mArrows[i]->Show();
                mArrows[i]->Play(data_ov000_02193d20[root->GetUnk0()][i].anime, 1);
            } else {
                mArrows[i]->Hide();
            }
        }
    }
}

// The TU's pooled constants.
extern "C" struct {
    u8 unk00[0x8];
    const char *crossPath;  // 0x08
    const char *markPath;   // 0x0C
    const char *arrowPath;  // 0x10
    const char *cursorPath; // 0x14: the cursor with mUnk12C set
    void (DinoBattleViewCreatureInfo::*showInfo)();     // 0x18
    u8 unk20[0x10];
    MPoint unk30;           // 0x30
    u8 unk38[0x20];
    MPoint unk58;           // 0x58
} data_ov000_02193ae4;

DinoBattleViewUiSelectAreaCommon::DinoBattleViewUiSelectAreaCommon(DinoBattleView *view, int player,
                                                                   MMessageObserver *target, u32 arg3, u8 arg4,
                                                                   u8 arg5)
    : MMenu(2), mUnkB0(player), mUnkB4(arg3), mUnkE0(data_ov000_02193ae4.unk30), mUnkE8(0), mUnkEC((u32)view),
      mTarget(target), mCursorTask(NULL), mCursor(NULL), mInfoKind(0), mUnk12C(0), mUnk13C(0), mUnk178(1), mUnk179(arg4),
      mUnk17A(arg5) {
    SetDeferStart(TRUE);
    mUnkAC = ((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0)->GetUnkAC() ? 4 : 0;
    mUnkA8 = 2;
    mSideC0 = ((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0)->GetUnkAC();
    mUnkBC = mSideC0;
}

// Where a creature slot is on screen.
extern "C" MPoint func_ov000_02175b40(DinoBattleView *view, u32 slot);
extern "C" DinoBattleViewCreature *func_ov000_02177430(DinoBattleViewUiSelectAreaCommon *menu, u32 unkB0, u32 unkB4);

// The rows' links for the d-pad: the direction, and the row it leads to. Ends with direction 0.
struct DinoBattleViewUiRootLink {
    int dir;
    int row;
};
extern "C" const DinoBattleViewUiRootLink *const data_ov000_02193ce0[];

// Adds the row's item when there is a creature in its slot (the game repeats this in every case).
#define ADD_ROOT_ITEM(row, side, slot)                                                                         \
    {                                                                                                          \
        u32 creature = (u32)func_ov000_02177430(this, side, slot);                                             \
        if (creature != 0) {                                                                                   \
            mRootItems[row] =                                                                                  \
                new DinoBattleViewUiRootMenuItem(this, row, func_ov000_02175b40((DinoBattleView *)mUnkEC, row)); \
            AddItem(mRootItems[row]);                                                                          \
            mUnk130[creature] = mRootItems[row];                                                               \
        }                                                                                                      \
    }

// Sets up the info panel, the cursor and the items of the creatures that can be chosen (mUnkA8 says which), then
// links the items up and focuses the first.
void DinoBattleViewUiSelectAreaCommon::OnStart() {
    mUnkF8 = new MSerialTaskSet;
    mParallel.AddTail(mUnkF8);
    if (mCursorTask == NULL) {
        mCursorTask = new MSmartOam(2, 0, 1);
        ((MSmartOam *)mCursorTask)->mHidden = TRUE;
        mSerial.AddTail(((MSmartOam *)mCursorTask)->CreateShowTransfer());
    }
    if (mUnk17A) {
        mInfo = new DinoBattleViewCreatureInfo((DinoBattleView *)mUnkEC, mUnkE0);
        mParallel.AddTail(mInfo);
        mSerial.AddTail(MCreateCallTask(mInfo, data_ov000_02193ae4.showInfo));
    } else {
        mInfo = NULL;
    }
    if (mCursor != NULL) {
        mCursor->Play(4, 0);
    } else if (mUnk179 && mCursor == NULL) {
        mCursor = new MMotion(std::string(mUnk12C == 0 ? data_ov000_02193ae4.markPath : data_ov000_02193ae4.cursorPath),
                              (MSmartOam *)mCursorTask, 0, 1);
        mCursor->SetPosition(func_ov000_02175b40((DinoBattleView *)mUnkEC, 0));
        mCursor->Play(1, 0);
        mCursor->SetPriority(1);
        mCursor->SetAnchor(0);
        mCursor->Show();
    }
    mCursorAnime = new MParallelTaskSet;
    if (mUnk17A) {
        mPageItems[0] = new DinoBattleViewInfoLRItem(mInfo->GetMotion(0), mUnkE0, 0x200);
        AddItem(mPageItems[0]);
        mPageItems[1] = new DinoBattleViewInfoLRItem(mInfo->GetMotion(1), mUnkE0, 0x100);
        AddItem(mPageItems[1]);
    }
    for (int row = 0; row < 8; row++) {
        mRootItems[row] = NULL;
        const DinoBattleViewUiRootRow &slot = data_ov000_0218fec4[row];
        u32 mode = mUnkA8;
        switch (mode) {
        case 3:
            if (slot.unk0 == 0 && slot.unk4 - 1 <= 1) {
                ADD_ROOT_ITEM(row, slot.unk0, slot.unk4)
            }
            break;
        case 4:
            if (slot.unk0 == 1 && slot.unk4 - 1 <= 1) {
                ADD_ROOT_ITEM(row, slot.unk0, slot.unk4)
            }
            break;
        case 9:
            if (slot.unk0 == 0) {
                ADD_ROOT_ITEM(row, slot.unk0, slot.unk4)
            }
            break;
        case 10:
            if (slot.unk0 == 1) {
                ADD_ROOT_ITEM(row, slot.unk0, slot.unk4)
            }
            break;
        case 5:
        case 6:
        case 7:
        case 8: {
            u32 side = 1;
            if (mode == 5 || mode == 7) {
                side = 0;
            }
            BOOL support = FALSE;
            if (mode - 7 <= 1) {
                support = TRUE;
            }
            if (side == slot.unk0) {
                u32 creature = (u32)func_ov000_02177430(this, slot.unk0, slot.unk4);
                if (creature != 0) {
                    mRootItems[row] =
                        new DinoBattleViewUiRootMenuItem(this, row, func_ov000_02175b40((DinoBattleView *)mUnkEC, row));
                    AddItem(mRootItems[row]);
                    mUnk130[creature] = mRootItems[row];
                    if (slot.unk4 == 3 || (support && (((DinoBattleViewCreature *)creature)->mUnk159 ||
                                                       (((DinoBattleViewCreature *)creature)->mFlags14C & 8)))) {
                        mRootItems[row]->SetNoFocus(TRUE);
                        mRootItems[row]->SetMode(0);
                        mRootItems[row]->mDisabled = TRUE;
                    }
                }
            }
            break;
        }
        default:
            if (mode == 2 || mode == slot.unk0) {
                ADD_ROOT_ITEM(row, slot.unk0, slot.unk4)
            }
            break;
        }
    }
    mItem120 = new DinoBattleViewCancelMenuItem;
    AddItem(mItem120);
    for (int row = 0; row < 8; row++) {
        if (mRootItems[row] != NULL) {
            u32 used = 0;
            int dir;
            for (const DinoBattleViewUiRootLink *link = data_ov000_02193ce0[row]; (dir = link->dir) != 0; link++) {
                if (!((1 << dir) & used) && mRootItems[link->row] != NULL) {
                    AddLink(mRootItems[row], link->dir, mRootItems[link->row]);
                    used |= 1 << link->dir;
                }
            }
        }
    }
    FocusItem(mRootItems[mUnkAC]);
    MoveCursorTo(mRootItems[mUnkAC], 0, 0);
    mParallel.AddTail(mCursorAnime);
    if (mCursor != NULL) {
        mParallel.AddTail(mCursor);
    }
    MMenuItem *item = mRootItems[mFocusedRow];
    if (item != NULL) {
        DinoBattleViewCreature *creature = GetItemCreature(item);
        if (creature != NULL) {
            OnFocusCreature(creature, item);
        }
    }
    mParallel.AddTail(mCursorTask);
}

DinoBattleViewUiChangeMenu::DinoBattleViewUiChangeMenu(DinoBattleView *view, int player, u32 arg2,
                                                       MMessageObserver *target, const u32 *ids, int count,
                                                       MSmartOam *oam)
    : DinoBattleViewUiSelectAreaCommon(view, player, target, 0, 1, 1), unk18C(arg2) {
    Disable();
    mSide = ((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0)->GetUnkAC();
    mUnkA8 = mSide ? 4 : 3;
    for (int i = 0; i < 2; i++) {
        if (view->GetUnkE4(player)->FindCreatureByB4(i + 1) != NULL) {
            mUnkAC = mSide ? i + 5 : i + 1;
            break;
        }
    }
    for (int i = 0; i < 3; i++) {
        mArrows[i] = new MMotion(data_ov000_02193ae4.arrowPath, oam, 0, 1);
        const ChangeMenuArrow &arrow = data_ov000_02193d20[mSide][i];
        mArrows[i]->SetPosition(func_ov000_02175b40(view, arrow.unk8) + arrow.offset);
        if (mSide == 1) {
            mArrows[i]->SetFlip(1, 0);
        }
        mArrows[i]->SetPriority(2);
        mArrows[i]->SetAnchor(0);
        mArrows[i]->Show();
        mParallel.AddTail(mArrows[i]);
    }
    mUnkE0.x += view->GetUnkE4(player)->GetUnkAC() == 1 ? -0x30 : 0x30;
    MTask *task = func_ov000_0218ec8c(view->GetUnk12C(), 0x1A, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
    mCount = count;
    for (int i = 0; i < mCount; i++) {
        mIds[i] = ids[i];
    }
}

void DinoBattleViewUiCreatureInfoMenu::OnIdle() {
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), -1, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
}

void DinoBattleViewUiRootMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        func_ov000_02177590(mOwner, this, 8, -2);
    }
    Refresh();
}


// Marks the focused creature and its type advantages against the other side.
void DinoBattleViewUiAttackerMenu::OnFocus(MMenuItem *item) {
    DinoBattleViewUiSelectAreaCommon::OnFocus(item);
    DinoSoundPlaySe(0x5F, 0);
    DinoBattleViewUiRootMenuItem *root = dynamic_cast<DinoBattleViewUiRootMenuItem *>(item);
    if (root != NULL) {
        DinoBattleViewCreature *creature = GetItemCreature(root);
        DinoBattleViewPlayer *opponent = ((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0)->GetOpponent();
        creature->SetFlag144(1);
        for (int i = 0; i < opponent->GetCreatureCount(); i++) {
            opponent->GetCreature(i)->SetAdvantage(creature);
        }
    }
}

void DinoBattleViewUiAttackerMenu::OnBlur(MMenuItem *item) {
    DinoBattleViewUiRootMenuItem *root = dynamic_cast<DinoBattleViewUiRootMenuItem *>(item);
    if (root != NULL) {
        GetItemCreature(root)->ClearFlag144(1);
    }
}

#pragma auto_inline off

BOOL DinoBattleViewUiChangeMenu::HasId(u32 id) {
    for (int i = 0; i < mCount; i++) {
        if (id == mIds[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL DinoBattleViewUiAttackerMenu::HasId(u32 id) {
    for (int i = 0; i < mCount; i++) {
        if (id == mIds[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// Where each creature slot is: the view group it is in, and its place there.
struct DinoBattleSlotPlace {
    u8 place;
    u8 group;
};
extern "C" const DinoBattleSlotPlace data_ov000_0218fd0c[];
// A place's offset in a view group.
extern "C" MPoint func_ov000_0216f320(int group, int place, BOOL arg2);

// (The game loads the place before the call, as it does without copy propagation.)
#pragma opt_propagation off
extern "C" MPoint func_ov000_02175b40(DinoBattleView *view, u32 slot) {
    u8 group = data_ov000_0218fd0c[slot].group;
    u8 place = data_ov000_0218fd0c[slot].place;
    return func_ov000_0216f320(group, place, TRUE) + view->mUnkE0->mSlotViewsC0[group]->GetPosition();
}
#pragma opt_propagation reset

// Where a creature is on screen.
extern "C" MPoint func_ov000_02175b90(DinoBattleViewCreature *creature) {
    DinoBattleView *view = ((DinoBattleViewPlayer *)creature->GetUnkDC())->mView;
    return data_ov000_02193ae4.unk58 +
           func_ov000_02175b40(view, creature->GetUnkB4() + creature->GetUnkB0() * 4);
}

// The creature in the slot, on either side.
extern "C" DinoBattleViewCreature *func_ov000_02177430(DinoBattleViewUiSelectAreaCommon *menu, u32 unkB0, u32 unkB4) {
    for (int i = 0; i < 2; i++) {
        DinoBattleViewPlayer *player = ((DinoBattleView *)menu->mUnkEC)->GetUnkE4(i);
        for (int j = 0; j < player->GetCreatureCount(); j++) {
            DinoBattleViewCreature *creature = player->GetCreature(j);
            if (unkB0 == creature->GetUnkB0() && unkB4 == creature->GetUnkB4()) {
                return creature;
            }
        }
    }
    return NULL;
}

DinoBattleViewUiRootMenuItem::DinoBattleViewUiRootMenuItem(DinoBattleViewUiSelectAreaCommon *owner, u32 index,
                                                           MPoint pos)
    : mIndex(index), mPos(pos), mOwner(owner), mDisabled(FALSE) {
    mHasCreature = ((DinoBattleViewUiSelectAreaCommon *)mOwner)->GetItemCreature(this) != NULL ? TRUE : FALSE;
}

void DinoBattleViewUiSelectAreaCommon::MoveCursorTo(DinoBattleViewUiRootMenuItem *item, int frames, int arg) {
    MoveCursor(item->GetCursorPos() + data_ov000_02193ae4.unk58, frames, arg);
}

void DinoBattleViewUiSelectAreaCommon::MoveCursor(MPoint pos, int frames, int arg) {
    mCursorAnime->Clear();
    if (mCursor != NULL) {
        if (frames != 0) {
            mCursorAnime->AddTail(mCursor->CreateCoordAnime(mCursor->GetPosition(), pos, frames, arg));
        } else {
            mCursor->SetPosition(pos);
        }
    }
}

// Where the cursor goes for the item: at its creature, or where the item says.
MPoint DinoBattleViewUiRootMenuItem::GetCursorPos() {
    DinoBattleViewCreature *creature = ((DinoBattleViewUiSelectAreaCommon *)mOwner)->GetItemCreature(this);
    if (creature != NULL) {
        MPoint pos = creature->mMotion->GetPosition();
        MSmartOam *oam = creature->mMotion->mOam;
        return MPoint(pos.x - oam->mOffset.x, pos.y - oam->mOffset.y);
    }
    return mPos;
}

// NONMATCHING: the game loads the frame before the offset and adds in another order.
MRect DinoBattleViewUiRootMenuItem::GetFrame() {
    DinoBattleViewCreature *creature = ((DinoBattleViewUiSelectAreaCommon *)mOwner)->GetItemCreature(this);
    if (creature != NULL) {
        MSmartOam *oam = creature->mMotion->mOam;
        MPoint offset(-oam->mOffset.x, -oam->mOffset.y);
        return creature->mMotion->GetFrame() + offset;
    }
    return MRect(0, 0, 0, 0);
}

// Per info kind: what it shows, and how many attacks fit.
struct DinoBattleInfoKind {
    u32 kind;
    u32 attacks;
};
extern "C" const DinoBattleInfoKind data_ov000_0218fd1c[];

// Whether the info kind can show the creature (or the one shown).
// The game built these with common-subexpression elimination.
#pragma opt_common_subs on
extern "C" BOOL func_ov000_02176454(DinoBattleViewUiSelectAreaCommon *menu, int kind,
                                    DinoBattleViewCreature *creature) {
    if (creature == NULL) {
        creature = menu->mInfo->mCreature;
    }
    if (creature != NULL && data_ov000_0218fd1c[kind].kind == 2) {
        gDinoResource->GetCreatureDef(creature->GetUnkA8());
        int attacks = creature->CountAttacks();
        if (creature->GetSpecialAttack()) {
            attacks++;
        }
        if (attacks <= (int)data_ov000_0218fd1c[kind].attacks * 2) {
            return FALSE;
        }
    }
    return TRUE;
}

void DinoBattleViewUiSelectAreaCommon::OnFocus(MMenuItem *item) {
    if (mUnk178) {
        mUnk178 = FALSE;
    }
    if (item == mPageItems[0] || item == mPageItems[1] || item == mItem120) {
        return;
    }
    DinoBattleViewCreature *creature = GetItemCreature(item);
    mFocusedRow = ((DinoBattleViewUiRootMenuItem *)item)->mIndex;
    if (mInfo != NULL) {
        if (creature != NULL) {
            ((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0);
            while (!func_ov000_02176454(this, mInfoKind, creature) && mInfoKind >= 0) {
                mInfoKind--;
            }
            mInfo->Update(TRUE, creature, data_ov000_0218fd1c[mInfoKind].kind, TRUE,
                          data_ov000_0218fd1c[mInfoKind].attacks);
        } else {
            mInfo->Update(FALSE, NULL, 0, 0, 0);
        }
    }
    if (creature != NULL) {
        mUnkBC = creature->GetUnkB0();
    }
    OnFocusCreature(creature, item);
}

void DinoBattleViewUiSelectAreaCommon::OnFocusCreature(DinoBattleViewCreature *creature, MMenuItem *item) {
}

// (Declared slot first: the game keeps the side in a register and the slot on the stack.)
DinoBattleViewCreature *DinoBattleViewUiSelectAreaCommon::GetItemCreature(MMenuItem *item) {
    u32 row = ((DinoBattleViewUiRootMenuItem *)item)->mIndex;
    u32 slot, side;
    side = data_ov000_0218fec4[row].unk0;
    slot = data_ov000_0218fec4[row].unk4;
    for (int i = 0; i < 2; i++) {
        DinoBattleViewPlayer *player = ((DinoBattleView *)mUnkEC)->GetUnkE4(i);
        for (int j = 0; j < player->GetCreatureCount(); j++) {
            DinoBattleViewCreature *creature = player->GetCreature(j);
            if (side == creature->GetUnkB0() && slot == creature->GetUnkB4()) {
                return creature;
            }
        }
    }
    return NULL;
}

#pragma opt_common_subs off
// The creature info help's texts.
extern "C" const u32 data_0208a3a8, data_0208a3ac, data_0208a3b0, data_0208a3b4, data_0208a3c8;

const char *DinoBattleHelpCreatureInfo::GetText() {
    u32 id = data_0208a3c8;
    switch (mMenu->mInfo->mUnkE8) {
    case 0:
        if (mMenu->mInfo->mUnkEC) {
            id = data_0208a3b4;
        } else {
            id = data_0208a3a8;
        }
        break;
    case 1:
        id = data_0208a3b0;
        break;
    case 2:
        id = data_0208a3ac;
        break;
    case 3:
        break;
    }
    return gDinoResource->GetText(id);
}

DinoBattleViewUiSelectAllMenu::DinoBattleViewUiSelectAllMenu(DinoBattleView *view, int player,
                                                             MMessageObserver *target, const u32 *ids, int count)
    : MMenu(0), mView((u32)view), mPlayer(player), mTarget(target) {
    Disable();
    mCount = count;
    for (int i = 0; i < mCount; i++) {
        mIds[i] = ids[i];
    }
}

DinoBattleViewUiAttackerMenu::DinoBattleViewUiAttackerMenu(DinoBattleView *view, int player, MMessageObserver *target,
                                                           const u32 *ids, int count, u32 attacker, u32 arg6)
    : DinoBattleViewUiSelectAreaCommon(view, player, target, 2, 1, 1), mUnk19C(arg6) {
    Disable();
    mSide = ((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0)->GetUnkAC();
    mUnkA8 = mSide ? 8 : 7;
    mUnk12C = 1;
    mCount = count;
    for (int i = 0; i < mCount; i++) {
        mIds[i] = ids[i];
        DinoBattleViewCreature *creature = ((DinoBattleView *)mUnkEC)->FindCreatureByAC(mIds[i]);
        if (attacker == creature->GetUnkAC()) {
            mUnkAC = creature->GetUnkB4() + creature->GetUnkB0() * 4;
        }
    }
    mSerial.AddTail(new MTimer(1));
}

DinoBattleViewUiDefenderMenu::DinoBattleViewUiDefenderMenu(DinoBattleView *view, int player, MMessageObserver *target,
                                                           const u32 *ids, int count)
    : DinoBattleViewUiSelectAreaCommon(view, player, target, 2, 1, 1) {
    Disable();
    mCount = count;
    for (int i = 0; i < mCount; i++) {
        mIds[i] = ids[i];
    }
    mSide = ((DinoBattleView *)mUnkEC)->FindCreatureByAC(mIds[0])->GetUnkB0();
    mSideC0 = mSide;
    mUnkBC = mSideC0;
    bool back = false;
    int i;
    for (i = 0; i < count; i++) {
        if (((DinoBattleView *)mUnkEC)->FindCreatureByAC(mIds[i])->GetUnkB4() == 3) {
            back = true;
            break;
        }
    }
    if (back) {
        mUnkA8 = mSide ? 10 : 9;
    } else {
        mUnkA8 = mSide ? 6 : 5;
    }
    mUnkAC = mSide ? 4 : 0;
    mUnkE0 += mSide ? MPoint(-0x30, -0x20) : MPoint(0x30, -0x20);
}

void DinoBattleViewUiAttackerMenu::OnStart() {
    mParallel.AddTail(new DinoBattleHelpBase(gLcd->GetBottomDisplay(), data_ov000_02193ca0[mSide], data_0208a34c));
    ((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0)->TakeMotions(&mCursor, &mCursorTask);
    mUnk1A0 = mCursor != NULL ? TRUE : FALSE;
    DinoBattleViewUiSelectAreaCommon::OnStart();
    mUnk1A0 = FALSE;
    for (int i = 0; i < mCount; i++) {
        ((DinoBattleView *)mUnkEC)->FindCreatureByAC(mIds[i])->ClearFlag144(2);
    }
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), 0x21, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
    if (!GetUnk12CFlag((DinoBattleView *)mUnkEC)) {
        DinoHelpBase &help = *this;
        mSerial.AddTail(DinoHelpBase::CreateEnableTask(&help));
    }
}

// The defender help's texts: with one of the player's own creatures among the defenders, or not.
extern "C" const u32 data_0208a354, data_0208a358;

// (Compiled with common subexpressions on, unlike its neighbours.)
// NONMATCHING: text and the map end iterator swap stack slots.
#pragma opt_common_subs on
void DinoBattleViewUiDefenderMenu::OnStart() {
    DinoBattleViewUiSelectAreaCommon::OnStart();
    u32 text = data_0208a358;
    for (int i = 0; i < mCount; i++) {
        DinoBattleViewCreature *creature = ((DinoBattleView *)mUnkEC)->FindCreatureByAC(mIds[i]);
        if (((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0)->GetUnkAC() == creature->GetUnkB0()) {
            text = data_0208a354;
            creature->SetFlag144(4);
        } else {
            creature->SetFlag144(3);
        }
    }
    mParallel.AddTail(new DinoBattleHelpBase(gLcd->GetBottomDisplay(), data_ov000_02193ca0[mSide], text));
    for (std::map<u32, void *>::iterator it = mUnk130.begin(), end = mUnk130.end(); it != end; ++it) {
        BOOL other = TRUE;
        for (int i = 0; i < mCount; i++) {
            DinoBattleViewCreature *c = ((DinoBattleView *)mUnkEC)->FindCreatureByAC(mIds[i]);
            u32 key = it->first;
            if (key == (u32)c) {
                other = FALSE;
                break;
            }
        }
        if (other) {
            DinoBattleViewUiRootMenuItem *root = dynamic_cast<DinoBattleViewUiRootMenuItem *>((MMenuItem *)it->second);
            if (root == NULL || !root->mDisabled) {
                ((MMenuItem *)it->second)->Disable();
            }
        }
    }
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), 0x26, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
    if (!GetUnk12CFlag((DinoBattleView *)mUnkEC)) {
        DinoHelpBase &help = *this;
        mSerial.AddTail(DinoHelpBase::CreateEnableTask(&help));
    }
}

#pragma opt_common_subs on
// Pages the info on a page item: on to the next kind it can show.
BOOL DinoBattleViewUiSelectAreaCommon::HandlePageItem(MMenuItem *item) {
    for (int i = 0; i < 2; i++) {
        if (item == mPageItems[i]) {
            if (mInfo->mUnkEC) {
                int old = mInfoKind;
                if (i != 0) {
                    do {
                        mInfoKind = (u32)(mInfoKind + 1) % 5;
                        mInfo->PlayMotion(1);
                        DinoSoundPlaySe(0x60, 0);
                    } while (!func_ov000_02176454(this, mInfoKind, NULL));
                }
                if (i == 0) {
                    do {
                        mInfoKind = ((u32)(mInfoKind - 1) + 5) % 5;
                        mInfo->PlayMotion(0);
                        DinoSoundPlaySe(0x60, 0);
                    } while (!func_ov000_02176454(this, mInfoKind, NULL));
                }
                if (old != mInfoKind) {
                    mInfo->Update(TRUE, NULL, data_ov000_0218fd1c[mInfoKind].kind, TRUE,
                                  data_ov000_0218fd1c[mInfoKind].attacks);
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}

// Where a slot group rests.
extern "C" MPoint func_ov000_0215f22c(int arg0, int group);

// The two tasks at once (the arguments are evaluated last to first).
static inline MTask *Together(MTask *a, MTask *b) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(a);
    set->AddTail(b);
    return set;
}

// A side's slot view. (Through a non-member inline, the other side's view gets the game's lower stack slot.)
static inline MView *DinoSlotView(u32 view, int side) {
    return ((DinoBattleView *)view)->mUnkE0->mSlotViewsC0[side];
}

void DinoBattleViewUiDefenderMenu::ResetRows() {
    if (mUnkBC == mSideC0) {
        return;
    }
    mCursor->Hide();
    mUnkF8->Clear();
    int side = mSideC0;
    int other = mUnkBC;
    MView *otherView;
    MView *sideView = ((DinoBattleView *)mUnkEC)->mUnkE0->mSlotViewsC0[side];
    otherView = DinoSlotView(mUnkEC, other);
    mUnkF8->AddTail(Together(sideView->CreateCoordAnime(sideView->GetPosition(), func_ov000_0215f22c(2, side), 8, -2),
                             otherView->CreateCoordAnime(otherView->GetPosition(), func_ov000_0215f22c(3, other), 8, -2)));
}

// Marks every creature that can be chosen, with a frame item over each.
static inline MParallelTaskSet *DinoSelectAllParallel(DinoBattleViewUiSelectAllMenu *menu) {
    return &menu->mParallel;
}

void DinoBattleViewUiSelectAllMenu::OnStart() {
    MSmartOam *oam = new MSmartOam(3, 0, 1);
    DinoSoundPlaySe(0x5F, 0);
    u32 text = data_0208a358;
    for (u32 i = 0; i < mCount; i++) {
        MParallelTaskSet *parallel = DinoSelectAllParallel(this);
        DinoBattleViewCreature *creature = ((DinoBattleView *)mView)->FindCreatureByAC(mIds[i]);
        if (((DinoBattleView *)mView)->GetUnkE4(mPlayer)->GetUnkAC() == creature->GetUnkB0()) {
            text = data_0208a354;
            creature->SetFlag144(4);
        } else {
            creature->SetFlag144(3);
        }
        MSmartOam *creatureOam = creature->mMotion->mOam;
        MPoint offset(-creatureOam->mOffset.x, -creatureOam->mOffset.y);
        MRect frame = creature->mMotion->GetFrame().Moved(offset);
        mMarks[i] = new MMotion(data_ov000_02193ae4.markPath, oam, 0, 1);
        mMarks[i]->SetPosition(frame.GetCenter());
        mMarks[i]->Play(1, 0);
        mMarks[i]->SetPriority(1);
        mMarks[i]->SetAnchor(0);
        mMarks[i]->Show();
        parallel->AddTail(mMarks[i]);
        AddItem(new DinoFrameItem(frame));
    }
    int side;
    if (mCount != 0) {
        side = ((DinoBattleViewPlayer *)((DinoBattleView *)mView)->FindCreatureByAC(mIds[0])->GetUnkDC())->GetUnkAC();
    } else {
        side = ((DinoBattleView *)mView)->GetUnkE4(mPlayer)->GetUnkAC();
    }
    mParallel.AddTail(new DinoBattleHelpBase(gLcd->GetBottomDisplay(), data_ov000_02193ca0[side], text));
    mParallel.AddTail(oam);
    mOkItem = new DinoBattleViewCancelMenuItem();
    AddItem(mOkItem);
    FocusNearestItem(MPoint(0x80, 0x60));
    if (!GetUnk12CFlag((DinoBattleView *)mView)) {
        DinoHelpBase &help = *this;
        mSerial.AddTail(DinoHelpBase::CreateEnableTask(&help));
    }
}
// The cancel item backs out; a creature that can be swapped in is told to the target.
void DinoBattleViewUiChangeMenu::OnDecide(MMenuItem *item) {
    if (HandlePageItem(item)) {
        return;
    }
    if (mItem120 == item) {
        BOOL ok = TRUE;
        MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), 0x1F, &ok);
        if (task != NULL) {
            mSerial.AddTail(task);
        }
        if (!ok) {
            return;
        }
        DinoSoundPlaySe(0x63, 0);
        for (int i = 0; i < 3; i++) {
            mArrows[i]->Play(data_ov000_02193d20[mSide][i].offAnime, 0);
        }
        Disable();
        GetSerial()->AddTail(mArrows[0]->CreateWaitStopTask(),
                        mTarget->CreatePostTask(new DinoBattleViewUiChangeMenuResult(1, 0, 0)),
                        MTaskExitTransfer::Create(this));
        return;
    }
    DinoBattleViewUiChangeMenuResult *result = NULL;
    DinoBattleViewCreature *creature = GetItemCreature(item);
    if (creature != NULL && HasId(creature->GetUnkAC())) {
        BOOL ok = TRUE;
        MTask *task = NULL;
        switch (creature->GetUnkB4()) {
        case 1:
            task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), 0x1D, &ok);
            break;
        case 2:
            task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), 0x1E, &ok);
            break;
        }
        if (task != NULL) {
            mSerial.AddTail(task);
        }
        if (!ok) {
            return;
        }
        DinoSoundPlaySe(0x61, 0);
        if (((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0)->GetUnkAC() == creature->GetUnkB0()) {
            result = new DinoBattleViewUiChangeMenuResult(0, unk18C, creature->GetUnkAC());
        }
    }
    if (result == NULL) {
        return;
    }
    int chosen = 0;
    for (int i = 0; i < 3; i++) {
        if (i == 2 || data_ov000_02193d20[mSide][i].id == ((DinoBattleViewUiRootMenuItem *)item)->GetUnk4()) {
            mArrows[i]->Play(data_ov000_02193d20[mSide][i].onAnime, 1);
            chosen = i;
        } else {
            mArrows[i]->Play(data_ov000_02193d20[mSide][i].offAnime, 0);
        }
    }
    Disable();
    GetSerial()->AddTail(mArrows[chosen]->CreateWaitStopTask(), mTarget->CreatePostTask(result),
                    MTaskExitTransfer::Create(this));
}

// The tutorial step for attacking a slot.
extern "C" const u32 data_ov000_0218fcf4[];
// How long the cross over a slot that can't be attacked stays.
extern "C" const u32 data_0208a140;
extern "C" void func_ov000_02161748(DinoBattleViewUnkE0 *views);

#pragma opt_common_subs on
void DinoBattleViewUiAttackerMenu::OnDecide(MMenuItem *item) {
    if (HandlePageItem(item)) {
        return;
    }
    DinoBattleViewUiRootMenuItem *root = dynamic_cast<DinoBattleViewUiRootMenuItem *>(item);
    if (root != NULL) {
        DinoBattleViewCreature *creature = GetItemCreature(root);
        if ((creature != NULL && (creature->GetUnkB4() == 3 || (creature->mFlags14C & 8))) || root->mDisabled) {
            DinoSoundPlaySe(0x62, 0);
            return;
        }
        if (creature != NULL && !(mUnk19C & (1 << creature->GetUnkB4()))) {
            // Out of reach: a cross over it for a while.
            DinoSoundPlaySe(0x62, 0);
            MParallelTaskSet *set = new MParallelTaskSet(TRUE);
            MSmartOam *oam = new MSmartOam(4, 0, 1);
            MMotion *cross = new MMotion(std::string(data_ov000_02193ae4.crossPath), oam, 0, 1);
            cross->SetPosition(MPoint(0x80, 0x60));
            cross->Show();
            cross->SetPriority(0);
            set->AddTail(cross, oam);
            mParallel.AddTail(set);
            GetSerial()->AddTail(cross->CreatePlayTransfer(1, 0), new AnyInputTimerWait(data_0208a140),
                                 cross->CreatePlayTransfer(2, 0), cross->CreateWaitStopTask(),
                                 MTaskExitTransfer::Create(set));
            return;
        }
        if (creature != NULL && HasId(creature->GetUnkAC())) {
            BOOL blocked = FALSE;
            MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(),
                                              data_ov000_0218fcf4[creature->GetUnkB4()], &blocked);
            if (task != NULL) {
                mSerial.AddTail(task);
            }
            if (blocked) {
                return;
            }
            DinoSoundPlaySe(0x61, 0);
            creature->ClearFlag144(1);
            creature->SetFlag144(2);
            GetParallel()->Remove(mCursor, mCursorTask);
            ((DinoBattleView *)mUnkEC)->GetUnkE4(mUnkB0)->SetMotions(mCursor, mCursorTask);
            Disable();
            GetSerial()->AddTail(mTarget->CreatePostTask(new DinoBattleViewUiAttackerMenuResult(creature->GetUnkAC(), 0)),
                                 MTaskExitTransfer::Create(this));
            return;
        }
    }
    if (item == mItem120) {
        BOOL blocked = FALSE;
        MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnkEC)->GetUnk12C(), 0x22, &blocked);
        if (task != NULL) {
            mSerial.AddTail(task);
        }
        if (blocked) {
            return;
        }
        func_ov000_02161748(((DinoBattleView *)mUnkEC)->mUnkE0);
        DinoSoundPlaySe(0x63, 0);
        GetItemCreature(dynamic_cast<DinoBattleViewUiRootMenuItem *>(GetFocusedItem()))->ClearFlag144(1);
        GetSerial()->AddTail(mTarget->CreatePostTask(new DinoBattleViewUiAttackerMenuResult(0, 1)),
                             MTaskExitTransfer::Create(this));
    }
}

#pragma opt_common_subs off
DinoBattleViewUiCreatureInfoMenu::DinoBattleViewUiCreatureInfoMenu(DinoBattleView *view, int player,
                                                                   MMessageObserver *target)
    : DinoBattleViewUiSelectAreaCommon(view, player, target, 0, 1, 1) {
    Disable();
    // Moves every creature's info view, waiting until they all have.
    int count = 0;
    for (u32 i = 0; i < 2; i++) {
        count += view->GetUnkE4(i)->GetCreatureCount();
    }
    if (count != 0) {
        DinoSignalWait *wait = new DinoSignalWait(0x100A1, NULL, NULL, count);
        mSerial.AddTail(wait);
        for (u32 i = 0; i < 2; i++) {
            DinoBattleViewPlayer *player = view->GetUnkE4(i);
            for (u32 j = 0; j < player->GetCreatureCount(); j++) {
                player->GetCreature(j)->Post(new DinoBattleViewCreatureViewInfoMoveRequest(wait, TRUE));
            }
        }
    }
    mUnkF8 = new MSerialTaskSet;
    mParallel.AddTail(mUnkF8);
    mUnkF8->Clear();
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    for (int i = 0; i < 2; i++) {
        int side = view->GetUnkE4(i)->GetUnkAC();
        MView *slots = ((DinoBattleView *)mUnkEC)->mUnkE0->mSlotViewsC0[side];
        set->AddTail(slots->CreateCoordAnime(slots->GetPosition(), func_ov000_0215f22c(1, side), 8, -2));
    }
    mUnkF8->AddTail(set);
}

#pragma opt_common_subs on
// The page items page the info (handled twice over, as the game does); the close item puts everything back.
void DinoBattleViewUiCreatureInfoMenu::OnDecide(MMenuItem *item) {
    if (HandlePageItem(item)) {
        return;
    }
    for (int i = 0; i < 2; i++) {
        if (item == mPageItems[i]) {
            if (mInfo->mUnkEC) {
                int old = mInfoKind;
                if (i != 0) {
                    do {
                        mInfoKind = (u32)(mInfoKind + 1) % 5;
                        mInfo->PlayMotion(1);
                        DinoSoundPlaySe(0x60, 0);
                    } while (!func_ov000_02176454(&(DinoBattleViewUiSelectAreaCommon &)*this, mInfoKind, NULL));
                }
                if (i == 0) {
                    do {
                        mInfoKind = ((u32)(mInfoKind - 1) + 5) % 5;
                        mInfo->PlayMotion(0);
                        DinoSoundPlaySe(0x60, 0);
                    } while (!func_ov000_02176454(&(DinoBattleViewUiSelectAreaCommon &)*this, mInfoKind, NULL));
                }
                if (old != mInfoKind) {
                    mInfo->Update(TRUE, NULL, data_ov000_0218fd1c[mInfoKind].kind, TRUE,
                                  data_ov000_0218fd1c[mInfoKind].attacks);
                }
            }
            return;
        }
    }
    if (mItem120 == item) {
        DinoSoundPlaySe(0x63, 0);
        mCursor->Hide();
        mUnkF8->Clear();
        int other;
        int side = mSideC0;
        other = 1 - side;
        MView *sideView = ((DinoBattleView *)mUnkEC)->mUnkE0->mSlotViewsC0[side];
        MView *otherView = DinoSlotView(mUnkEC, other);
        mUnkF8->AddTail(Together(sideView->CreateCoordAnime(sideView->GetPosition(), func_ov000_0215f22c(2, side), 8, -2),
                                 otherView->CreateCoordAnime(otherView->GetPosition(), func_ov000_0215f22c(3, other), 8, -2)));
        int count = 0;
        for (u32 i = 0; i < 2; i++) {
            count += ((DinoBattleView *)mUnkEC)->GetUnkE4(i)->GetCreatureCount();
        }
        if (count != 0) {
            DinoSignalWait *wait = new DinoSignalWait(0x100A1, NULL, NULL, count);
            mSerial.AddTail(wait);
            for (u32 i = 0; i < 2; i++) {
                DinoBattleViewPlayer *player = ((DinoBattleView *)mUnkEC)->GetUnkE4(i);
                for (u32 j = 0; j < player->GetCreatureCount(); j++) {
                    player->GetCreature(j)->Post(new DinoBattleViewCreatureViewInfoMoveRequest(wait, FALSE));
                }
            }
        }
        func_ov000_02161748(((DinoBattleView *)mUnkEC)->mUnkE0);
        Disable();
        GetSerial()->AddTail(MTaskSetWaitEmptyTask::Create(mUnkF8),
                             mTarget->CreatePostTask(new DinoBattleViewUiCreatureInfoMenuResult()),
                             MTaskExitTransfer::Create(&(MProcess &)*this));
        return;
    }
    GetItemCreature(item);
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_02193c64[];
extern "C" u32 data_ov000_02193cb0[];
extern "C" u32 data_ov000_02193be4[];
extern "C" u32 data_ov000_02193ce4[];
extern "C" u32 data_ov000_02193d64[];
static struct DinoBattleViewUiStaticInit {
    DinoBattleViewUiStaticInit() {
        DINO_POOL_WORD(data_ov000_02193ae4, 0x48) = 128;
        DINO_POOL_WORD(data_ov000_02193ae4, 0x4c) = 176;
        DINO_POOL_WORD(data_ov000_02193ae4, 0x20) = 8;
        DINO_POOL_WORD(data_ov000_02193ae4, 0x24) = 150;
        DINO_POOL_WORD(data_ov000_02193ae4, 0x30) = 64;
        DINO_POOL_WORD(data_ov000_02193ae4, 0x34) = 112;
        DINO_POOL_WORD(data_ov000_02193c64, 0x3c) = 4;
        DINO_POOL_WORD(data_ov000_02193c64, 0x40) = 4;
        DINO_POOL_WORD(data_ov000_02193c64, 0x44) = 124;
        DINO_POOL_WORD(data_ov000_02193c64, 0x48) = 188;
        volatile s32 *p = (volatile s32 *) &data_ov000_02193cb0;
        p[0] = 132;
        p[1] = 4;
        DINO_POOL_WORD(data_ov000_02193cb0, 0x8) = 252;
        DINO_POOL_WORD(data_ov000_02193cb0, 0xc) = 188;
        DINO_POOL_WORD(data_ov000_02193be4, 0x2c) = 65;
        DINO_POOL_WORD(data_ov000_02193be4, 0x30) = 113;
        DINO_POOL_WORD(data_ov000_02193be4, 0x34) = 190;
        DINO_POOL_WORD(data_ov000_02193be4, 0x38) = 190;
        DINO_POOL_WORD(data_ov000_02193ae4, 0x58) = 0;
        DINO_POOL_WORD(data_ov000_02193ae4, 0x5c) = 0;
        DINO_POOL_WORD(data_ov000_02193ce4, 0x3c) = 48;
        DINO_POOL_WORD(data_ov000_02193ce4, 0x40) = 16;
        DINO_POOL_WORD(data_ov000_02193ce4, 0x58) = 48;
        DINO_POOL_WORD(data_ov000_02193ce4, 0x5c) = 0;
        DINO_POOL_WORD(data_ov000_02193ce4, 0x74) = -32;
        DINO_POOL_WORD(data_ov000_02193ce4, 0x78) = 16;
        DINO_POOL_WORD(data_ov000_02193d64, 0x10) = -48;
        DINO_POOL_WORD(data_ov000_02193d64, 0x14) = 16;
        DINO_POOL_WORD(data_ov000_02193d64, 0x2c) = -48;
        DINO_POOL_WORD(data_ov000_02193d64, 0x30) = 0;
        DINO_POOL_WORD(data_ov000_02193d64, 0x48) = 32;
        DINO_POOL_WORD(data_ov000_02193d64, 0x4c) = 16;
    }
} sDinoBattleViewUiStaticInit;
