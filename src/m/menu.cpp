// MMenuItem, MMenu and the focus transfers. Not decompiled yet: the state functions (they load pointer-to-member
// constants, which can't be matched by name yet).
// NONMATCHING: RemoveItem, AddLink and RemoveLink (the game keeps a stored copy of the iterator or of the inner pair
// that ours optimizes away, as in help.cpp), the tree's insert_node_at (two stores swapped).

#include "m/menu.h"
#include "m/lcd.h"
#include "m/input.h"
#include "m/bg_new_task.h"
#include "nitro/os.h"

#include <stdlib.h>

MMenuItem::MMenuItem() : MProcess(NULL) {
    mMenu = NULL;
    mEnabled = TRUE;
    mFocused = FALSE;
    mSelected = FALSE;
    mNoFocus = FALSE;
    mNoTouch = FALSE;
    mKeys = 0;
    mMode = -1;
}

MMenu *MMenuItem::GetMenu() {
    return mMenu;
}

void MMenuItem::Enable() {
    if (!mEnabled) {
        mEnabled = TRUE;
        OnEnableChanged(TRUE);
    }
}

void MMenuItem::Disable() {
    if (mEnabled) {
        mEnabled = FALSE;
        OnEnableChanged(FALSE);
        if (mFocused) {
            mFocused = FALSE;
            OnFocusChanged(FALSE);
        }
    }
}

BOOL MMenuItem::IsEnabled() {
    return mEnabled;
}

BOOL MMenuItem::IsFocused() {
    return mFocused;
}

BOOL MMenuItem::IsSelected() {
    return mSelected;
}

void MMenuItem::Focus() {
    mMenu->FocusItem(this);
}

void MMenuItem::Decide() {
    mMenu->DecideItem(this);
}

void MMenuItem::OnDecide() {
}

void MMenuItem::OnFocusChanged(BOOL focused) {
}

void MMenuItem::OnEnableChanged(BOOL enabled) {
}

void MMenuItem::OnSelectChanged(BOOL selected) {
}

void MMenuItem::SetKeys(u32 keys) {
    mKeys = keys;
}

void MMenuItem::SetNoFocus(BOOL noFocus) {
    mNoFocus = noFocus;
}

void MMenuItem::SetNoTouch(BOOL noTouch) {
    mNoTouch = noTouch;
}

void MMenuItem::SetMode(int mode) {
    mMode = mode;
}

// NONMATCHING: the game reloads mMode for the last return.
int MMenuItem::GetMode() {
    if (mMode == -1 && mMenu != NULL) {
        return mMenu->mDefaultMode;
    }
    return GetOwnMode();
}

void MMenuItem::Idle() {
}

// NONMATCHING (all three constructors): the pointer-to-member constant is at a different offset in the TU's
// pool until every function that makes one is decompiled.
MMenu::MMenu(int defaultMode)
    : MProcess(NULL), mInput(NULL), mDefaultMode(defaultMode), mActive(TRUE), mCursor(0, 0), mLinksOnly(FALSE),
      mDeferStart(FALSE) {
    mState = &MMenu::Init;
}

MMenu::MMenu(MInput *input, int defaultMode)
    : MProcess(NULL), mInput(input), mDefaultMode(defaultMode), mActive(TRUE), mCursor(0, 0), mLinksOnly(FALSE),
      mDeferStart(FALSE) {
    mState = &MMenu::Init;
}

MMenu::MMenu(MDisplay *display, MInput *input, int defaultMode)
    : MProcess(display), mInput(input), mDefaultMode(defaultMode), mActive(TRUE), mCursor(0, 0), mLinksOnly(FALSE),
      mDeferStart(FALSE) {
    mState = &MMenu::Init;
}

void MMenu::SetDeferStart(BOOL defer) {
    mDeferStart = defer;
}

void MMenu::AddItem(MMenuItem *item) {
    item->mMenu = this;
    mItems->AddTail(item);
}

void MMenu::RemoveItem(MMenuItem *item) {
    mItems->Remove(item);
    item->mMenu = NULL;
    // Drop the links to or from the item.
    MMenuLinks::iterator it = mLinks.begin();
    while (it != mLinks.end()) {
        if (it->first == item || it->second.second == item) {
            mLinks.erase(it);
            it = mLinks.begin();
        } else {
            ++it;
        }
    }
}

void MMenu::ClearItems() {
    mItems->Clear();
    mLinks.clear();
}

void MMenu::AddLink(MMenuItem *from, int dir, MMenuItem *to) {
    mLinks.insert(MMenuLinks::value_type(from, std::make_pair(dir, to)));
}

void MMenu::RemoveLink(MMenuItem *from, int dir) {
    for (MMenuLinks::iterator it = mLinks.begin(); it != mLinks.end(); ++it) {
        if (it->first == from && dir == it->second.first) {
            mLinks.erase(it);
            return;
        }
    }
}

void MMenu::SetLinksOnly(BOOL linksOnly) {
    mLinksOnly = linksOnly;
}

MInput *MMenu::GetInput() {
    return mInput;
}

void MMenu::Activate() {
    mActive = TRUE;
}

void MMenu::Deactivate() {
    mActive = FALSE;
}

void MMenu::OnStart() {
}

void MMenu::OnIdle() {
}

void MMenu::OnBlur(MMenuItem *item) {
}

void MMenu::OnFocus(MMenuItem *item) {
}

void MMenu::OnSelect(MMenuItem *item) {
}

void MMenu::OnDecide(MMenuItem *item) {
}

MPoint MMenu::GetCursor() {
    return mCursor;
}

MMenuItem *MMenu::GetFocusedItem() {
    for (MMenuItem *item = (MMenuItem *)mItems->mHead; item != NULL; item = (MMenuItem *)item->mNext) {
        if (item->mEnabled && item->mFocused) {
            return item;
        }
    }
    return NULL;
}

MMenuItem *MMenu::GetSelectedItem() {
    for (MMenuItem *item = (MMenuItem *)mItems->mHead; item != NULL; item = (MMenuItem *)item->mNext) {
        if (item->mEnabled && item->mSelected) {
            return item;
        }
    }
    return NULL;
}

MMenuItem *MMenu::FindItemByKeys(u32 keys) {
    for (MMenuItem *item = (MMenuItem *)mItems->mHead; item != NULL; item = (MMenuItem *)item->mNext) {
        if (item->mEnabled && (item->mKeys & keys)) {
            return item;
        }
    }
    return NULL;
}

void MMenu::FocusItem(MMenuItem *item) {
    if (item != NULL && item->mNoFocus) {
        return;
    }
    MMenuItem *old = GetFocusedItem();
    if (old == item) {
        return;
    }
    OnBlur(old);
    if (old != NULL) {
        old->mFocused = FALSE;
        old->OnFocusChanged(FALSE);
    }
    if (item != NULL) {
        item->mFocused = TRUE;
        item->OnFocusChanged(TRUE);
        mCursor = item->GetFrame().GetCenter();
    }
    OnFocus(item);
}

void MMenu::FocusNearestItem(MPoint pos) {
    FocusItem(FindNearestItem(pos));
}

void MMenu::SelectItem(MMenuItem *item) {
    MMenuItem *old = GetSelectedItem();
    if (old == item) {
        return;
    }
    if (old != NULL) {
        old->mSelected = FALSE;
        old->OnSelectChanged(FALSE);
    }
    if (item != NULL) {
        item->mSelected = TRUE;
        item->OnSelectChanged(TRUE);
    }
    OnSelect(item);
}

MMenuItem *MMenu::HitTest(MPoint pos) {
    for (MMenuItem *item = (MMenuItem *)mItems->mHead; item != NULL; item = (MMenuItem *)item->mNext) {
        if (item->mEnabled && !item->mNoTouch) {
            if (item->GetFrame().Contains(pos)) {
                return item;
            }
        }
    }
    return NULL;
}

// NONMATCHING (FindNearestItem, FindItemInRegion): the game adds the two terms in the other operand order.
static inline int MDistance(MPoint a, MPoint b) {
    return labs(a.x - b.x) + labs(a.y - b.y);
}

MMenuItem *MMenu::FindNearestItem(MPoint pos) {
    MMenuItem *best = NULL;
    int bestDist = 0x7FFFFFFF;
    for (MMenuItem *item = (MMenuItem *)mItems->mHead; item != NULL; item = (MMenuItem *)item->mNext) {
        if (item->mEnabled && !item->mNoFocus) {
            int dist = MDistance(item->GetFrame().GetCenter(), pos);
            if (dist < bestDist) {
                best = item;
                bestDist = dist;
            }
        }
    }
    return best;
}

static inline bool MIsInside(MPoint p, const MMenuHalfPlane *planes) {
    for (u32 i = 0; i < 3; i++) {
        if ((p.x - planes->x) * planes->a + (p.y - planes->y) * planes->b < 0) {
            return false;
        }
        planes++;
    }
    return true;
}

// Finds the nearest focusable item, other than the focused one, inside the region bounded by three planes.
MMenuItem *MMenu::FindItemInRegion(MPoint from, const MMenuHalfPlane *planes) {
    MMenuItem *focused = GetFocusedItem();
    MMenuItem *best = NULL;
    int bestDist = 0x7FFFFFFF;
    for (MMenuItem *item = (MMenuItem *)mItems->mHead; item != NULL; item = (MMenuItem *)item->mNext) {
        if (item != focused && item->mEnabled && !item->mNoFocus) {
            MPoint center = item->GetFrame().GetCenter();
            if (MIsInside(center, planes)) {
                int dist = MDistance(from, center);
                if (dist < bestDist) {
                    best = item;
                    bestDist = dist;
                }
            }
        }
    }
    return best;
}

void MMenu::DecideItem(MMenuItem *item) {
    item->OnDecide();
    OnDecide(item);
}

void MMenu::Idle() {
    (this->*mState)();
}

// NONMATCHING: the pointer-to-member constant for the next state is in the TU's pooled data.
void MMenu::Init() {
    if (mInput == NULL) {
        MSingleInputHub *hub = new MSingleInputHub(1);
        if (GetDisplay() == gLcd->GetBottomDisplay()) {
            hub->EnableTouch();
        }
        mInput = hub->GetInput(0);
        mParallel.AddTail(hub);
    }
    mItems = new MParallelTaskSet();
    mUnk88 = new MParallelTaskSet();
    MTaskSet *unk88 = mUnk88;
    MParallelTaskSet *parallel = &mParallel;
    parallel->AddTail(mItems);
    parallel->AddTail(unk88);
    if (!mDeferStart) {
        OnStart();
    } else {
        mParallel.SetUseMutex(TRUE);
        mSerial.SetUseMutex(TRUE);
        MTask *run = MTaskRunTransfer::Create(&mParallel);
        MTask *start = MCreateBgCallTask(this, &MMenu::OnStart, 0);
        MTask *suspend = MTaskSuspendTransfer::Create(&mParallel);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(suspend);
        serial->AddTail(start);
        serial->AddTail(run);
    }
    mState = &MMenu::Update;
}

MTask *MMenu::CreateFocusItemTransfer(MMenuItem *item) {
    return new MMenuFocusItemTransfer(this, item);
}

MTask *MMenu::CreateFocusNearestItemTransfer(MPoint pos) {
    return new MMenuFocusNearestItemTransfer(this, pos);
}

void MMenuFocusNearestItemTransfer::Transfer() {
    mMenu->FocusNearestItem(mPos);
}

void MMenuFocusItemTransfer::Transfer() {
    mMenu->FocusItem(mItem);
}
