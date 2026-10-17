#ifndef GAME_DINO_UI_ATM_CURSOR_H
#define GAME_DINO_UI_ATM_CURSOR_H

#include "m/view.h"

class MMotion;
class MSmartOam;

class DinoUiATMCursor : public MView {
public:
    DinoUiATMCursor(MSmartOam *oam);
    virtual ~DinoUiATMCursor();
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
