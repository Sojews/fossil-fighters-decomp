// M3d (the 3D engine's layer), M3DVInt, M3dWaitTransferCompleteTask.
// Only the destructors are decompiled so far.

#include "m/heap.h"
#include "m/3d.h"
#include "m/3d_wait_transfer_complete_task.h"

extern "C" void G3X_SetClearColor(u16 rgb, int alpha, int depth, int polygonID, BOOL fog);

M3DVInt::~M3DVInt() {
    MScopedLock lock(&gM3dMutex, TRUE);
    Disable();
}

M3dWaitTransferCompleteTask::~M3dWaitTransferCompleteTask() {
}

BOOL M3d::IsTransferComplete() {
    MMutexLock lock(&gM3dMutex);
    return mUnkA4 == NULL && mUnkBC == NULL && mUnkD4 == NULL;
}

void M3d::FreeTex(u32 key) {
    MScopedLock lock(&gM3dMutex, TRUE);
    mTexHeap.Free(key, FALSE);
}

void M3d::FreePltt(u32 key) {
    MScopedLock lock(&gM3dMutex, TRUE);
    mPlttHeap.Free(key, FALSE);
}

MTask *M3d::CreateWaitTransferCompleteTask() {
    return new M3dWaitTransferCompleteTask(this);
}

void M3dWaitTransferCompleteTask::Execute() {
    if (mM3d->IsTransferComplete()) {
        Exit();
    }
}

void M3d::Unk0C() {
}

void M3d::SetClearColor(u16 rgb, int alpha, int depth, int polygonID, BOOL fog) {
    G3X_SetClearColor(rgb, alpha, depth, polygonID, fog);
}

void M3d::SetViewport(int x1, int y1, int x2, int y2) {
    *(vu32 *)0x04000580 = (u32)(x1 | (y1 << 8) | (x2 << 16) | (y2 << 24));   // G3_Viewport
}
