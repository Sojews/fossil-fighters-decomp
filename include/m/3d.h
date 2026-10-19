// The 3D engine's layer (BG 0 drawn by the 3D engine).
#ifndef M_3D_H
#define M_3D_H

#include "m/screen.h"
#include "m/heap.h"
#include "m/looper.h"
#include "m/archive.h"
#include <deque>

// A queued transfer to VRAM.
struct M3dTransfer {
    u32 src;            // 0x0
    u32 dst;            // 0x4: VRAM address
    u32 size;           // 0x8
};

class M3DVInt;

class M3d : public MScreen {
public:
    M3d(MDisplay *display, u32 unk44, u32 unk48);
    virtual ~M3d();
    virtual void Unk0C();
    virtual MPoint Unk10();
    BOOL IsTransferComplete();
    void QueueE8(const void *heap, u32 addr);   // frees addr (from heap, else delete[]) once transfers are done
    u32 AllocTex(const void *src, u32 size, BOOL queue);
    u32 AllocPltt(const void *src, u32 size);
    void QueueD0(const void *dst, u32 src, u32 size);
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
    std::deque<M3dTransfer> mQueues[2]; // 0xA0: transfers still queued
    std::deque<M3dTransfer> mQueueD0;   // 0xD0: palette transfers
    std::deque<M3dTransfer> mQueueE8;   // 0xE8: memory to free (src: the address, dst: its MHeap, or 0)
    M3DVInt *mVInt;                     // 0x100
};

extern MMutex gM3dMutex;

// Runs the 3D engine's per-frame work at VBlank.
class M3DVInt : public MLooperVSyncTask {
public:
    M3DVInt(M3d *m3d) : MLooperVSyncTask(FALSE, FALSE) {
        mM3d = m3d;
        Enable();
    }
    virtual ~M3DVInt();
    virtual void OnVSync();

    M3d *mM3d;      // 0x1C
};

#endif
