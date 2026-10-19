#ifndef GAME_DINO_UI2_FIELD_MENU_CURSOR_H
#define GAME_DINO_UI2_FIELD_MENU_CURSOR_H

#include "m/motion.h"
#include "m/view.h"

// The field menu's cursor (as DinoUiFieldMenuCursor); the mode 2 view hides itself after 120 frames.
class MSmartOam;

class DinoUi2FieldMenuCursor : public MView {
public:
    DinoUi2FieldMenuCursor(MSmartOam *oam);
    virtual ~DinoUi2FieldMenuCursor();
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

    MTask *CreateMoveTask(MRect from, MRect to, int duration, int ease);

    MMotion *mUnk10;    // 0x10: shown in mode 1
    MMotion *mUnk14;    // 0x14: shown in mode 2
    MRect mFrame;   // 0x18
    u8 mMode;       // 0x28
    u8 mHidden;     // 0x29
    u32 mTime;      // 0x2C: frames since the cursor last changed
};

#endif
