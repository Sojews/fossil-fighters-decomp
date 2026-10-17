#ifndef GAME_DINO_BATTLE_VIEW_PLAYER_FIELD_H
#define GAME_DINO_BATTLE_VIEW_PLAYER_FIELD_H

#include "m/text.h"
#include "m/view.h"
#include "m/bg.h"

class MText;

class DinoBattleViewPlayerField : public MView {
public:
    virtual ~DinoBattleViewPlayerField();
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

    u32 mUnk10[2];  // 0x10
    MBg *mBg;       // 0x18
    MText *mText;   // 0x1C: the field's BG screen
    u32 mDisplay;   // 0x20: which screen origin applies
    MPoint mPos;    // 0x24: the position while there is no screen
};

#endif
