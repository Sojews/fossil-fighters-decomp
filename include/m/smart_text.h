#ifndef M_SMART_TEXT_H
#define M_SMART_TEXT_H

#include "m/task.h"
#include "m/geometry.h"

class MText;

class MSmartText : public MTask {
public:
    virtual ~MSmartText();

    void SetScroll(MPoint scroll) { mScroll = scroll; }

    u32 mUnk10;
    MText *mText;       // 0x14
    MPoint mScroll;     // 0x18
};

#endif
