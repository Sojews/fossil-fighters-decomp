// M3d (the 3D engine's layer), M3DVInt, M3dWaitTransferCompleteTask.
// Only the destructors are decompiled so far.

#include "m/heap.h"
#include "m/3d.h"
#include "m/dma.h"
#include "m/3d_wait_transfer_complete_task.h"

extern "C" void G3X_SetClearColor(u16 rgb, int alpha, int depth, int polygonID, BOOL fog);
extern "C" u32 GX_DisableBankForTex(void);
extern "C" u32 GX_DisableBankForTexPltt(void);
extern "C" u32 GX_SetDefaultDMA(u32 dma);

M3DVInt::~M3DVInt() {
    MScopedLock lock(&gM3dMutex, TRUE);
    Disable();
}

M3d::~M3d() {
    delete mVInt;
    GX_DisableBankForTex();
    GX_DisableBankForTexPltt();
    MFreeDmaChannel(GX_SetDefaultDMA((u32)-1));
}

M3dWaitTransferCompleteTask::~M3dWaitTransferCompleteTask() {
}

BOOL M3d::IsTransferComplete() {
    MMutexLock lock(&gM3dMutex);
    return mQueues[0].empty() && mQueues[1].empty() && mQueueD0.empty();
}

void M3d::FreeTex(u32 key) {
    MScopedLock lock(&gM3dMutex, TRUE);
    mTexHeap.Free(key, FALSE);
}

void M3d::FreePltt(u32 key) {
    MScopedLock lock(&gM3dMutex, TRUE);
    mPlttHeap.Free(key, FALSE);
}

// Takes VRAM for a texture and queues its transfer there (on the given queue); returns the address.
u32 M3d::AllocTex(const void *src, u32 size, BOOL queue) {
    size = (size + 3) & ~3;
    u32 addr = mTexHeap.AllocAddr(size, FALSE, 4, 0, 0, 0);
    M3dTransfer t;
    t.src = (u32)src;
    t.dst = addr;
    t.size = size;
    MScopedLock lock(&gM3dMutex, TRUE);
    mQueues[queue].push_back(t);
    return addr;
}

// Takes palette VRAM and queues the palette's transfer there; returns the address.
u32 M3d::AllocPltt(const void *src, u32 size) {
    size = (size + 3) & ~3;
    u32 addr = mPlttHeap.AllocAddr(size, FALSE, 0x20, 0, 0, 0);
    M3dTransfer t;
    t.src = (u32)src;
    t.dst = addr;
    t.size = size;
    MScopedLock lock(&gM3dMutex, TRUE);
    mQueueD0.push_back(t);
    return addr;
}

void M3d::QueueE8(const void *dst, u32 src) {
    M3dTransfer t;
    t.src = src;
    t.dst = (u32)dst;
    MScopedLock lock(&gM3dMutex, TRUE);
    mQueueE8.push_back(t);
}

void M3d::QueueD0(const void *dst, u32 src, u32 size) {
    M3dTransfer t;
    t.src = src;
    t.dst = (u32)dst;
    t.size = size;
    MScopedLock lock(&gM3dMutex, TRUE);
    mQueueD0.push_back(t);
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
template class std::__cdeque_deleter<void *, std::allocator<void *> >::iterator;
template class std::__cdeque_deleter<void *, std::allocator<void *> &>::iterator;
template class std::__deque_common<true>;
template std::__deque_deleter<M3dTransfer, std::allocator<M3dTransfer> >::~__deque_deleter();
template std::__deque_deleter<M3dTransfer, std::allocator<M3dTransfer> >::iterator std::__deque_deleter<M3dTransfer, std::allocator<M3dTransfer> >::make_iter(size_t n);
template void std::__deque_deleter<M3dTransfer, std::allocator<M3dTransfer> >::push_back(const M3dTransfer &x, Metrowerks::int2type<true>);
template void std::__deque_deleter<M3dTransfer, std::allocator<M3dTransfer> >::erase_front(size_t n, Metrowerks::int2type<true>);
