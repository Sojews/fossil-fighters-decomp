// The 3D engine's layer (BG 0 drawn by the 3D engine).
#ifndef M_3D_H
#define M_3D_H

#include "m/screen.h"
#include "m/heap.h"
#include "m/looper.h"
#include <deque>

// A queued transfer to VRAM.
struct M3dTransfer {
    const void *src;    // 0x0
    u32 size;           // 0x4
    u32 arg;            // 0x8
};

class M3d : public MScreen {
public:
    M3d(MDisplay *display, u32 unk44, u32 unk48);
    virtual ~M3d();
    virtual void Unk0C();
    BOOL IsTransferComplete();
    void QueueE8(const void *src, u32 size);
    u32 AllocTex(const void *src, u32 size, BOOL arg);
    u32 AllocPltt(const void *src, u32 size);
    void *QueueD0(const void *src, u32 size, u32 arg);
    void FreeTex(u32 key);
    void FreePltt(u32 key);
    MTask *CreateWaitTransferCompleteTask();
    void SetClearColor(u16 rgb, int alpha, int depth, int polygonID, BOOL fog);
    void SetViewport(int x1, int y1, int x2, int y2);

    u32 mTexSize;       // 0x20
    u32 mPlttSize;      // 0x24
    u32 mPlttBanks;     // 0x28
    u32 mTexBanks;      // 0x2C
    MHeap mTexHeap;     // 0x30
    MHeap mPlttHeap;    // 0x68
    std::deque<M3dTransfer> mQueueA0;   // 0xA0: transfers still queued
    std::deque<M3dTransfer> mQueueB8;   // 0xB8
    std::deque<M3dTransfer> mQueueD0;   // 0xD0
    std::deque<M3dTransfer> mQueueE8;   // 0xE8
    u32 unk100;
};

extern OSMutex gM3dMutex;

// Runs the 3D engine's per-frame work at VBlank.
class M3DVInt : public MLooperVSyncTask {
public:
    virtual ~M3DVInt();
    virtual void OnVSync();
};

#endif
