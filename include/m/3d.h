// The 3D engine's layer (BG 0 drawn by the 3D engine).
#ifndef M_3D_H
#define M_3D_H

#include "m/screen.h"
#include "m/looper.h"

class M3d : public MScreen {
public:
    M3d(MDisplay *display, u32 unk44, u32 unk48);
    virtual ~M3d();
    BOOL IsTransferComplete();

    u8 mUnk20[0x84];
    void *mUnkA4;       // 0xA4: transfers still queued
    u8 unkA8[0x14];
    void *mUnkBC;       // 0xBC
    u8 unkC0[0x14];
    void *mUnkD4;       // 0xD4
    u8 unkD8[0x2C];
};

extern OSMutex gM3dMutex;

// Runs the 3D engine's per-frame work at VBlank.
class M3DVInt : public MLooperVSyncTask {
public:
    virtual ~M3DVInt();
    virtual void OnVSync();
};

#endif
