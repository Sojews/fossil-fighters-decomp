// The common menu cursor.
#ifndef GAME_DINO_COMMON_CURSOR_H
#define GAME_DINO_COMMON_CURSOR_H

#include "m/view.h"
#include "m/task.h"

class MSmartOam;

class DinoCommonCursor : public MView {
public:
    DinoCommonCursor(MSmartOam *oam);
    virtual ~DinoCommonCursor();
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
    void Layout();

    MSerialTaskSet mTasks;  // 0x10
    MView *mCorners[4];     // 0x40: top left, bottom left, top right, bottom right
    MRect mFrame;           // 0x50
};

#endif
