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
    return mQueueA0.empty() && mQueueB8.empty() && mQueueD0.empty();
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

// The queues' members the game emits in this file.
#pragma auto_inline off
template class std::__cdeque_deleter<void *, std::allocator<void *> >;
template class std::__cdeque_deleter<void *, std::allocator<void *> &>;
template std::deque<M3dTransfer>::~deque();
template std::deque<M3dTransfer>::iterator std::deque<M3dTransfer>::make_iter(size_t n);
template void std::deque<M3dTransfer>::push_back(const M3dTransfer &x, Metrowerks::int2type<true>);
template void std::deque<M3dTransfer>::erase_front(size_t n);
