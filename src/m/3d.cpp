// M3d (the 3D engine's layer), M3DVInt, M3dWaitTransferCompleteTask.
// Only the destructors are decompiled so far.

#include "m/heap.h"
#include "m/3d.h"
#include "m/dma.h"
#include "nitro/mi.h"
#include "m/3d_wait_transfer_complete_task.h"

extern "C" void G3X_SetClearColor(u16 rgb, int alpha, int depth, int polygonID, BOOL fog);
extern "C" u32 GX_DisableBankForTex(void);
extern "C" u32 GX_DisableBankForTexPltt(void);
extern "C" u32 GX_SetDefaultDMA(u32 dma);
extern "C" void GX_BeginLoadTex(void);
extern "C" void GX_LoadTex(const void *src, u32 dst, u32 size);
extern "C" void GX_EndLoadTex(void);
extern "C" void GX_BeginLoadTexPltt(void);
extern "C" void GX_LoadTexPltt(const void *src, u32 dst, u32 size);
extern "C" void GX_EndLoadTexPltt(void);
extern "C" BOOL MI_IsDmaBusy(u32 dma);
extern "C" u32 GXi_DmaId;
extern "C" void G3X_Init(void);
extern "C" void GX_SetBankForTex(u32 banks);
extern "C" void GX_SetBankForTexPltt(u32 banks);

// Whether the display is in VBlank (when VRAM can be written).
static inline BOOL M3dInVBlank() {
    int line = *(vu16 *)0x04000006;   // REG_VCOUNT
    return line >= 192 && line <= 211;
}

M3DVInt::~M3DVInt() {
    MScopedLock lock(&gM3dMutex, TRUE);
    Disable();
}

M3d::M3d(MDisplay *display, u32 texSize, u32 plttSize)
    : MScreen(display, 0, 0, 0, 0, 0), mTexSize(texSize), mPlttSize(plttSize), mTexHeap(0, texSize, 12),
      mPlttHeap(0, plttSize, 12) {
    // The 3D engine's DMA: the last free channel, else any.
    u32 dma = 15;
    for (u32 i = 0; i < 4; i++) {
        if (gDmaChannelUsed[i] == 0) {
            dma = i;
        }
    }
    if (dma == 15) {
        GX_SetDefaultDMA(MAllocDmaChannel());
    } else {
        MReserveDmaChannel(dma);
        GX_SetDefaultDMA(dma);
    }
    G3X_Init();
    vu16 *disp3dcnt = (vu16 *)0x04000060;
    *disp3dcnt = (u16)(*disp3dcnt & ~0x3002);           // toon shading
    *disp3dcnt = (u16)((*disp3dcnt & ~0x3000) | 0x10);  // antialiasing
    *disp3dcnt = (u16)((*disp3dcnt & ~0x3000) | 0x08);  // alpha blending
    // The display's texture and palette banks.
    mTexBanks = 0;
    mPlttBanks = 0;
    for (int i = 0; i < 4; i++) {
        switch (display->mBanks78.banks[i]) {
        case 1:
            mTexBanks |= 1;
            break;
        case 2:
            mTexBanks |= 2;
            break;
        case 3:
            mTexBanks |= 4;
            break;
        case 4:
            mTexBanks |= 8;
            break;
        }
    }
    GX_SetBankForTex(mTexBanks);
    for (int i = 0; i < 4; i++) {
        switch (display->mBanks8C.banks[i]) {
        case 5:
            mPlttBanks |= 0x10;
            break;
        case 6:
            mPlttBanks |= 0x20;
            break;
        case 7:
            mPlttBanks |= 0x40;
            break;
        }
    }
    GX_SetBankForTexPltt(mPlttBanks);
    *(vu32 *)0x04000540 = 2;    // SWAP_BUFFERS: auto sort, Z buffering
    SetClearColor(0, 31, 0x7FFF, 63, FALSE);
    SetViewport(0, 0, 255, 191);
    *(vu32 *)0x040004A4 = 0x1F0080;         // POLYGON_ATTR
    *(vu32 *)0x040004C0 = 0x7FFF7FFF;       // DIF_AMB
    *(vu32 *)0x040004C4 = 0x7FFF7FFF;       // SPE_EMI
    *(vu32 *)0x04000480 = 0x7FFF7FFF >> 16; // COLOR
    *(vu32 *)0x04000540 = 2;
    mVInt = new M3DVInt(this);
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

// Runs the queued transfers while VBlank lasts: palettes, then textures (a texture too big for the lines left
// waits once something was loaded), then, once all are done, the queued frees.
void M3DVInt::OnVSync() {
    MScopedLock lock(&gM3dMutex, TRUE);
    BOOL loaded = FALSE;
    u32 i;
    if (MI_IsDmaBusy(GXi_DmaId)) {
        return;
    }
    int line = *(vu16 *)0x04000006;
    if (line < 192 || line > 211) {
        return;
    }
    if (!mM3d->mQueueD0.empty()) {
        GX_BeginLoadTexPltt();
        while (M3dInVBlank() && !mM3d->mQueueD0.empty()) {
            M3dTransfer &t = mM3d->mQueueD0.front();
            u32 src = t.src;
            u32 dst = t.dst;
            u32 size = t.size;
            mM3d->mQueueD0.pop_front();
            GX_LoadTexPltt((const void *)src, dst, size);
            loaded = TRUE;
        }
        GX_EndLoadTexPltt();
    }
    line = *(vu16 *)0x04000006;
    if (line < 192 || line > 211) {
        return;
    }
    for (i = 0; i < 2; i++) {
        std::deque<M3dTransfer> &queue = mM3d->mQueues[i];
        if (!queue.empty()) {
            GX_BeginLoadTex();
            while (M3dInVBlank() && !queue.empty()) {
                M3dTransfer &t = queue.front();
                u32 src = t.src;
                u32 dst = t.dst;
                u32 size = t.size;
                if (loaded && (212 - *(vu16 *)0x04000006) * 0x164 < size) {
                    break;
                }
                queue.pop_front();
                GX_LoadTex((const void *)src, dst, size);
                loaded = TRUE;
            }
            GX_EndLoadTex();
        }
    }
    if (!mM3d->mQueueE8.empty() && mM3d->mQueueD0.empty() && mM3d->mQueues[0].empty() && mM3d->mQueues[1].empty()) {
        MI_WaitDma(GXi_DmaId);
        while (!mM3d->mQueueE8.empty()) {
            M3dTransfer &t = mM3d->mQueueE8.front();
            u32 addr = t.src;
            MHeap *heap = (MHeap *)t.dst;
            mM3d->mQueueE8.pop_front();
            if (heap != NULL) {
                heap->Free(addr, FALSE);
            } else {
                delete[] (u8 *)addr;
            }
        }
    }
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

void M3d::QueueE8(const void *heap, u32 addr) {
    M3dTransfer t;
    t.src = addr;
    t.dst = (u32)heap;
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

MPoint M3d::Unk10() {
    return MPoint(0, 0);
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
