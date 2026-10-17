// Motions: animated OBJ graphics loaded from a motion file, shown as a view.
#ifndef M_MOTION_H
#define M_MOTION_H

#include <string>
#include "m/view.h"

class MSmartOam;

class MMotion : public MView {
public:
    MMotion(const std::string &path, MSmartOam *oam, int unk, int priority);
    virtual ~MMotion();
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
    virtual void Play(int anime, int frame);
    virtual void SetObjMode(int mode);

    u8 mUnk10[0x7C - 0x10];
};

#endif
