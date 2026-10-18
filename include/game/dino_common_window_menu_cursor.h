#ifndef GAME_DINO_COMMON_WINDOW_MENU_CURSOR_H
#define GAME_DINO_COMMON_WINDOW_MENU_CURSOR_H

#include "m/view.h"

class MMotion;
class MSmartOam;

// The cursor of a window's menu: one view kept on a frame.
class DinoCommonWindowMenuCursor : public MView {
public:
    DinoCommonWindowMenuCursor(MSmartOam *oam);
    virtual ~DinoCommonWindowMenuCursor();
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

    MMotion *mUnk10;    // 0x10
    MRect mFrame;   // 0x14
};

#endif
