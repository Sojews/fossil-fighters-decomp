#ifndef GAME_DINO_UI_FIELD_MENU_CURSOR_H
#define GAME_DINO_UI_FIELD_MENU_CURSOR_H

#include "m/view.h"

// The field menu's cursor: two views (mMode picks the one shown) kept on one frame.
class DinoUiFieldMenuCursor : public MView {
public:
    virtual ~DinoUiFieldMenuCursor();
    virtual void Execute();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetSize(MPoint size);
    virtual MPoint GetSize();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();

    MView *mUnk10;  // 0x10: shown in mode 1
    MView *mUnk14;  // 0x14: shown in mode 2
    MRect mFrame;   // 0x18
    u8 mMode;       // 0x28
    u8 mHidden;     // 0x29
    u32 mUnk2C;     // 0x2C: reset whenever the cursor changes
};

#endif
