#ifndef M_MENU_H
#define M_MENU_H

#include "m/menu_item.h"

class MInput;

// One plane of a region to search: points p with (p.x - x) * a + (p.y - y) * b >= 0.
struct MMenuHalfPlane {
    int a;
    int x;
    int b;
    int y;
};

// Stands in for the std::map of links (item and direction to item) until MSL's tree is reconstructed. (A
// template, as the inlined code differs from a plain class's.)
template <class T> struct MTreeStandIn {
    struct Header {
        void *root;
        void *leftmost;
    };

    MTreeStandIn() { Init(); }
    ~MTreeStandIn();    // (out of line, like the tree's)
    void Init() {
        size = 0;
        header.root = NULL;
        header.leftmost = &header;
    }

    u32 size;
    Header header;
};

typedef MTreeStandIn<MMenuItem *> MMenuLinks;

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
    void StateMain();

    void SetDeferStart(BOOL defer);
    void AddItem(MMenuItem *item);
    void SetLinksOnly(BOOL linksOnly);
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
    u8 mLinksOnly;              // 0xA5: move only along links
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
