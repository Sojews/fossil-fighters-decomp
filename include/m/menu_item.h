// Menus: a process that owns items (themselves processes with a frame on screen) and moves the focus between
// them by d-pad, touch, links and nearest-in-direction searches.
#ifndef M_MENU_ITEM_H
#define M_MENU_ITEM_H

#include "m/task.h"
#include "m/geometry.h"

class MMenu;

class MMenuItem : public MProcess {
public:
    MMenuItem();
    virtual ~MMenuItem() {}
    virtual void Idle();
    virtual MRect GetFrame() = 0;
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnEnableChanged(BOOL enabled);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();

    MMenu *GetMenu();
    void Enable();
    void Disable();
    BOOL IsEnabled();
    BOOL IsFocused();
    BOOL IsSelected();
    void Focus();
    void Decide();
    void SetKeys(u32 keys);
    void SetNoFocus(BOOL noFocus);
    void SetNoTouch(BOOL noTouch);
    void SetMode(int mode);
    int GetMode();
    int GetOwnMode() { return mMode; }

    MMenu *mMenu;       // 0x74
    u8 mEnabled;        // 0x78
    u8 mFocused;        // 0x79
    u8 mSelected;       // 0x7A
    u8 mNoFocus;        // 0x7B: can't be focused by moving
    u8 mNoTouch;        // 0x7C: ignores the touch panel
    u32 mKeys;          // 0x80: buttons that select it directly
    int mMode;          // 0x84: how it reacts to being chosen (bits); -1: the menu's default
};

#endif
