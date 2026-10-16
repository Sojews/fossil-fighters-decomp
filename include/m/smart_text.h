#ifndef M_SMART_TEXT_H
#define M_SMART_TEXT_H

#include "m/task.h"
#include "m/geometry.h"

class MText;
class MDisplay;

class MSmartText : public MTask {
public:
    MSmartText(MDisplay *display, BOOL fromEnd);
    virtual ~MSmartText();
    virtual void Execute();

    void Transfer(const u16 *src, MPoint srcSize, MPoint dst, int tile, int palette, bool hflip, bool vflip);
    void Fill(MRect rect, int tile, int palette, bool hflip, bool vflip);

    void SetScroll(MPoint scroll) { mScroll = scroll; }

    u32 mUnk10;
    MText *mText;       // 0x14
    MPoint mScroll;     // 0x18
    u8 mUnk20[0x5C];
};

#endif
