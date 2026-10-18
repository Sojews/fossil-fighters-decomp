#ifndef M_MENU_H
#define M_MENU_H

#include "m/menu_item.h"
#include <map>

class MInput;

// One plane of a region to search: points p with (p.x - x) * a + (p.y - y) * b >= 0.
struct MMenuHalfPlane {
    int a;
    int x;
    int b;
    int y;
};

// The links between items: from an item, in a direction, to an item.
typedef std::multimap<MMenuItem *, std::pair<int, MMenuItem *> > MMenuLinks;

class MMenu : public MProcess {
public:
    MMenu(int defaultMode);
    MMenu(MInput *input, int defaultMode);
    MMenu(MDisplay *display, MInput *input, int defaultMode);
    virtual ~MMenu() {}
    virtual void Idle();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnBlur(MMenuItem *item);
    virtual void OnSelect(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    void Init();
    void Update();
    void StateTouch();
    void StateButton();
    void StateKeys();

    void SetDeferStart(BOOL defer);
    void AddItem(MMenuItem *item);
    void RemoveItem(MMenuItem *item);
    void ClearItems();
    void RemoveLink(MMenuItem *from, int dir);
    void AddLink(MMenuItem *from, int dir, MMenuItem *to);
    void SetLinkFallback(BOOL fallback);
    MInput *GetInput();
    void Activate();
    void Deactivate();
    MPoint GetCursor();
    int GetDefaultMode() { return mDefaultMode; }
    MMenuItem *GetFocusedItem();
    MMenuItem *GetSelectedItem();
    MMenuItem *FindItemByKeys(u32 keys);
    void FocusItem(MMenuItem *item);
    void FocusNearestItem(MPoint pos);
    void SelectItem(MMenuItem *item);
    MMenuItem *HitTest(MPoint pos);
    MMenuItem *FindNearestItem(MPoint pos);
    MMenuItem *FindLinkedItem(MMenuItem *from, int dir);
    MMenuItem *FindItemInDirection(MRect from, int dir);
    MMenuItem *FindItemInRegion(MPoint from, const MMenuHalfPlane *planes);
    void DecideItem(MMenuItem *item);

    MTask *CreateFocusItemTransfer(MMenuItem *item);
    MTask *CreateFocusNearestItemTransfer(MPoint pos);

    void (MMenu::*mState)();    // 0x74
    MInput *mInput;             // 0x7C
    int mDefaultMode;           // 0x80
    MTaskSet *mItems;           // 0x84
    MTaskSet *mUnk88;           // 0x88
    u8 mActive;                 // 0x8C
    MPoint mCursor;             // 0x90: the center of the focused item
    MMenuLinks mLinks;          // 0x98
    u8 mUnkA4;                  // 0xA4
    u8 mLinkFallback;           // 0xA5: when an item's links miss a direction, search that way anyway
    u8 mDeferStart;             // 0xA6
};

class MMenuFocusItemTransfer : public MTransfer {
public:
    MMenuFocusItemTransfer(MMenu *menu, MMenuItem *item) {
        mMenu = menu;
        mItem = item;
    }
    virtual ~MMenuFocusItemTransfer() {}
    virtual void Transfer();

    MMenu *mMenu;       // 0x10
    MMenuItem *mItem;   // 0x14
};

class MMenuFocusNearestItemTransfer : public MTransfer {
public:
    MMenuFocusNearestItemTransfer(MMenu *menu, MPoint pos) {
        mMenu = menu;
        mPos = pos;
    }
    virtual ~MMenuFocusNearestItemTransfer() {}
    virtual void Transfer();

    MMenu *mMenu;   // 0x10
    MPoint mPos;    // 0x14
};

#endif
