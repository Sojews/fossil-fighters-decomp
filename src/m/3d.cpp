// M3d (the 3D engine's layer), M3DVInt, M3dWaitTransferCompleteTask.
// Only the destructors are decompiled so far.

#include "m/heap.h"
#include "m/3d.h"
#include "m/3d_wait_transfer_complete_task.h"

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
