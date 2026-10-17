// MCanvasCopyTransfer.

#include "m/canvas_copy_transfer.h"
#include "m/canvas.h"
#include "m/looper.h"
#include "game/dino_message.h"

void MCanvasCopyTransfer::Create(MCanvas *dst, MCanvas *src, MRect rect, MPoint pos) {
    gLooper->AddTransferTask(new MCanvasCopyTransfer(dst, src, rect, pos));
}

void MCanvasCopyTransfer::Transfer() {
    int width;
    u8 srcLine[0x100];
    u8 dstLine[0x100];
    int height = mRect.bottom - mRect.top;
    width = mRect.right - mRect.left;
    for (int i = 0; i < height; i++) {
        mSrc->GetLine(mRect.top + i, srcLine);
        mDst->GetLine(mPos.y + i, dstLine);
        MI_CpuCopy8(srcLine + mRect.left, dstLine + mPos.x, width);
        mDst->SetLine(mPos.y + i, dstLine);
    }
}

MCanvasCopyTransfer::~MCanvasCopyTransfer() {
}
