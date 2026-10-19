// MMenuItem, MMenu and the focus transfers.
// NONMATCHING: Init (where it works out &mSerial); RemoveItem, AddLink and RemoveLink (the game keeps a stored copy of the iterator
// or of the inner pair that ours optimizes away, as in help.cpp); FindLinkedItem (the game keeps a copy of the range
// and two zeroed bytes on the stack); FindItemInDirection (the order the rect's words are loaded); the tree's
// insert_node_at (two stores swapped).

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

// Built without common-subexpression elimination (mMode is read again for the last return).
#pragma opt_common_subs off
int MMenuItem::GetMode() {
    if (mMode == -1 && mMenu != NULL) {
        return mMenu->mDefaultMode;
    }
    return GetOwnMode();
}
#pragma opt_common_subs reset

void MMenuItem::Idle() {
}

// The TU's pooled pointer-to-member constants (built ipa, the game copies each from its own slot of this pool; one
// member per slot, named after the function it points to and its offset).
typedef void (MMenu::*MMenuState)();
extern "C" struct {
    MMenuState StateTouch_00;   // 0x00
    MMenuState Update_08;       // 0x08
    MMenuState Init_10;         // 0x10
    MMenuState Init_18;         // 0x18
    MMenuState OnStart_20;      // 0x20
    MMenuState StateButton_28;  // 0x28
    MMenuState Update_30;       // 0x30
    MMenuState Init_38;         // 0x38
    MMenuState StateKeys_40;    // 0x40
    MMenuState Update_48;       // 0x48
    u8 unk50[8];
    MMenuState Update_58;       // 0x58
} data_020c46c4;

MMenu::MMenu(int defaultMode)
    : MProcess(NULL), mInput(NULL), mDefaultMode(defaultMode), mActive(TRUE), mCursor(0, 0), mLinkFallback(FALSE),
      mDeferStart(FALSE) {
    mState = data_020c46c4.Init_10;
}

MMenu::MMenu(MInput *input, int defaultMode)
    : MProcess(NULL), mInput(input), mDefaultMode(defaultMode), mActive(TRUE), mCursor(0, 0), mLinkFallback(FALSE),
      mDeferStart(FALSE) {
    mState = data_020c46c4.Init_38;
}

MMenu::MMenu(MDisplay *display, MInput *input, int defaultMode)
    : MProcess(display), mInput(input), mDefaultMode(defaultMode), mActive(TRUE), mCursor(0, 0), mLinkFallback(FALSE),
      mDeferStart(FALSE) {
    mState = data_020c46c4.Init_18;
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

void MMenu::SetLinkFallback(BOOL fallback) {
    mLinkFallback = fallback;
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

static inline int MDistance(MPoint a, MPoint b) {
    int dy = labs(a.y - b.y);
    int dx = labs(a.x - b.x);
    return dx + dy;
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

// The item linked from `from` in the direction (1 right, 2 down, 3 left, 4 up). Without links from it, or when
// they miss the direction and the menu falls back, the nearest item that way.
MMenuItem *MMenu::FindLinkedItem(MMenuItem *from, int dir) {
    std::pair<MMenuLinks::iterator, MMenuLinks::iterator> range = mLinks.equal_range(from);
    if (range.first != range.second) {
        for (MMenuLinks::iterator it = range.first; it != range.second; ++it) {
            if (dir == it->second.first) {
                MMenuItem *to = it->second.second;
                if (to != NULL && to->mEnabled && !to->mNoFocus) {
                    return to;
                }
                return NULL;
            }
        }
        if (!mLinkFallback) {
            return NULL;
        }
    }
    return FindItemInDirection(from->GetFrame(), dir);
}

// The nearest item in the direction: inside the 90 degree wedge from the frame's center that way.
MMenuItem *MMenu::FindItemInDirection(MRect from, int dir) {
    switch (dir) {
    default:
        return NULL;
    case 1: {
        int x = (from.left + from.right) >> 1;
        int y = (from.top + from.bottom) >> 1;
        MMenuHalfPlane planes[3] = {{1, x, 0, y}, {1, x, -1, from.top}, {1, x, 1, from.bottom}};
        return FindItemInRegion(MPoint(x, y), planes);
    }
    case 2: {
        int x = (from.left + from.right) >> 1;
        int y = (from.top + from.bottom) >> 1;
        MMenuHalfPlane planes[3] = {{0, x, 1, y}, {-1, from.right, 1, y}, {1, from.left, 1, y}};
        return FindItemInRegion(MPoint(x, y), planes);
    }
    case 3: {
        int x = (from.left + from.right) >> 1;
        int y = (from.top + from.bottom) >> 1;
        MMenuHalfPlane planes[3] = {{-1, x, 0, y}, {-1, x, -1, from.top}, {-1, x, 1, from.bottom}};
        return FindItemInRegion(MPoint(x, y), planes);
    }
    case 4: {
        int x = (from.left + from.right) >> 1;
        int y = (from.top + from.bottom) >> 1;
        MMenuHalfPlane planes[3] = {{0, x, -1, y}, {-1, from.right, -1, y}, {1, from.left, -1, y}};
        return FindItemInRegion(MPoint(x, y), planes);
    }
    }
}

static inline bool MIsInside(MPoint p, const MMenuHalfPlane *planes) {
    const MMenuHalfPlane *plane = planes;
    for (u32 i = 0; i < 3; i++) {
        if ((p.x - plane->x) * plane->a + (p.y - plane->y) * plane->b < 0) {
            return false;
        }
        plane++;
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

// NONMATCHING: the game works out &mSerial after making the tasks.
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
        MParallelTaskSet *deferred = &mParallel;
        deferred->SetUseMutex(TRUE);
        MSerialTaskSet *serial = &mSerial;
        serial->SetUseMutex(TRUE);
        MTask *run = MTaskRunTransfer::Create(deferred);
        MTask *start = MCreateBgCallTask(this, data_020c46c4.OnStart_20, 0);
        MTask *suspend = MTaskSuspendTransfer::Create(&mParallel);
        serial->AddTail(suspend);
        serial->AddTail(start);
        serial->AddTail(run);
    }
    mState = data_020c46c4.Update_30;
}

void MMenu::Update() {
    if (!mActive) {
        return;
    }
    if (mInput->GetPressed() & 0x1000) {
        MMenuItem *item = HitTest(mInput->GetTouchPos());
        if (item != NULL) {
            if (item->GetMode() & 1) {
                // Held: decided when the touch is let go.
                mUnkA4 = GetFocusedItem() == item;
                FocusItem(item);
                SelectItem(item);
                mState = data_020c46c4.StateTouch_00;
                return;
            }
            if (item->GetMode() == 2 && !item->mNoFocus && GetFocusedItem() != item) {
                // The first touch only focuses it.
                FocusItem(item);
                return;
            }
            FocusItem(item);
            DecideItem(item);
            return;
        }
    }
    u32 keys = mInput->GetPressed() & 0x3FFF;
    if (keys != 0) {
        MMenuItem *item = FindItemByKeys(keys);
        if (item != NULL) {
            if (item->GetMode() & 1) {
                FocusItem(item);
                SelectItem(item);
                mState = data_020c46c4.StateKeys_40;
                return;
            }
            if ((item->GetMode() & 4) && !item->mNoFocus && GetFocusedItem() != item) {
                FocusItem(item);
                return;
            }
            FocusItem(item);
            DecideItem(item);
            return;
        }
    }
    MMenuItem *focused = GetFocusedItem();
    if (focused != NULL && (mInput->GetPressed() & 1)) {
        if (focused->GetMode() & 1) {
            SelectItem(focused);
            mState = data_020c46c4.StateButton_28;
            return;
        }
        mCursor = focused->GetFrame().GetCenter();
        DecideItem(focused);
        if (GetFocusedItem() == NULL) {
            FocusItem(FindNearestItem(mCursor));
        }
        return;
    }
    int dir = 0;
    if (mInput->GetPressed() & 0x10) {
        dir = 1;
    }
    if (mInput->GetPressed() & 0x20) {
        dir = 3;
    }
    if (mInput->GetPressed() & 0x40) {
        dir = 4;
    }
    if (mInput->GetPressed() & 0x80) {
        dir = 2;
    }
    if (dir != 0) {
        MMenuItem *next;
        if (focused == NULL) {
            next = FindItemInDirection(MRect(mCursor, mCursor), dir);
            if (next == NULL) {
                FindNearestItem(mCursor); // (the game drops the result)
            }
        } else {
            next = FindLinkedItem(focused, dir);
        }
        if (next != NULL) {
            FocusItem(next);
            return;
        }
    }
    OnIdle();
}

// A held item touched: follows the touch until it's let go, then decides the item still under it.
void MMenu::StateTouch() {
    if (!(mInput->mButtons & 0x1000)) {
        MMenuItem *item = GetSelectedItem();
        if (item != NULL) {
            SelectItem(NULL);
            FocusItem(item);
            if (!(item->GetMode() & 2) || item->mNoFocus || mUnkA4) {
                DecideItem(item);
            }
        }
        mState = data_020c46c4.Update_48;
        return;
    }
    MMenuItem *item = HitTest(mInput->GetTouchPos());
    if (item != GetSelectedItem()) {
        mUnkA4 = FALSE;
    }
    if (item != NULL) {
        SelectItem(item);
        FocusItem(item);
    } else {
        SelectItem(NULL);
    }
    OnIdle();
}

// A held item chosen with A: decided when A is let go; the pad moves the selection meanwhile.
void MMenu::StateButton() {
    if (!(mInput->mButtons & 1)) {
        MMenuItem *item = GetSelectedItem();
        if (item != NULL) {
            SelectItem(NULL);
            mCursor = item->GetFrame().GetCenter();
            DecideItem(item);
            if (GetFocusedItem() == NULL) {
                FocusItem(FindNearestItem(mCursor));
            }
        }
        mState = data_020c46c4.Update_58;
        return;
    }
    int dir = 0;
    MMenuItem *focused = GetFocusedItem();
    if (mInput->GetPressed() & 0x10) {
        dir = 1;
    }
    if (mInput->GetPressed() & 0x20) {
        dir = 3;
    }
    if (mInput->GetPressed() & 0x40) {
        dir = 4;
    }
    if (mInput->GetPressed() & 0x80) {
        dir = 2;
    }
    if (dir != 0) {
        MMenuItem *next;
        if (focused == NULL) {
            next = FindItemInDirection(MRect(mCursor, mCursor), dir);
            if (next == NULL) {
                FindNearestItem(mCursor); // (the game drops the result)
            }
        } else {
            next = FindLinkedItem(focused, dir);
        }
        if (next != NULL) {
            FocusItem(next);
            SelectItem(next);
        }
    } else {
        OnIdle();
    }
}

// A held item chosen with its keys: decided when they're let go.
void MMenu::StateKeys() {
    MMenuItem *item = GetSelectedItem();
    if (!(mInput->mButtons & item->mKeys)) {
        SelectItem(NULL);
        DecideItem(item);
        mState = data_020c46c4.Update_08;
        return;
    }
    OnIdle();
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
