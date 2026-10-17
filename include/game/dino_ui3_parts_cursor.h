#ifndef GAME_DINO_UI3_PARTS_CURSOR_H
#define GAME_DINO_UI3_PARTS_CURSOR_H

#include "m/view.h"

// A cursor: one view kept on a frame.
class DinoUi3PartsCursor : public MView {
public:
    virtual ~DinoUi3PartsCursor();
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

    MView *mUnk10;  // 0x10
    MRect mFrame;   // 0x14
    u8 mUnk24;      // 0x24
    u8 mHidden;     // 0x25
    u32 mTime;      // 0x28: frames since the cursor last changed
};

#endif
