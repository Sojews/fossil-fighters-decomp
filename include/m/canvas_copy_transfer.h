// Copies a rectangle of one canvas to another, a row at a time, during VBlank.
#ifndef M_CANVAS_COPY_TRANSFER_H
#define M_CANVAS_COPY_TRANSFER_H

#include "m/task.h"
#include "m/geometry.h"

class MCanvas;

class MCanvasCopyTransfer : public MTransfer {
public:
    static void Create(MCanvas *dst, MCanvas *src, MRect rect, MPoint pos);

    MCanvasCopyTransfer(MCanvas *dst, MCanvas *src, MRect rect, MPoint pos) {
        mSrc = src;
        mRect = rect;
        mDst = dst;
        mPos = pos;
    }
    virtual ~MCanvasCopyTransfer();
    virtual void Transfer();

    MCanvas *mSrc;  // 0x10
    MRect mRect;    // 0x14: in mSrc
    MCanvas *mDst;  // 0x24
    MPoint mPos;    // 0x28: in mDst
};

#endif
