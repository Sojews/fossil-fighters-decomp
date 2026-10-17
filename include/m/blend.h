// Color special effects (alpha blending and brightness fades) of one display: shadows BLDCNT, BLDALPHA and BLDY.
#ifndef M_BLEND_H
#define M_BLEND_H

#include "m/task.h"
#include "m/display.h"

class MBlend {
public:
    MBlend(MDisplay *display);

    void Update();
    void Reset();
    void SwapPlanes(u32 unused, const int *a, const int *b, u32 count);
    void SetMode(int mode);
    int GetMode();
    void SetAlpha(int eva, int evb);
    u8 GetEva();
    u8 GetEvb();
    void SetBrightness(int evy);
    void SetTarget1(int planes);
    void SetTarget2(int planes);
    int GetTarget1();

    MTask *CreateModeTransfer(int mode);
    MTask *CreateAlphaTransfer(int eva, int evb);
    MTask *CreateTarget1Transfer(int planes);
    MTask *CreateTarget2Transfer(int planes);
    MTask *CreateClearTransfer();
    MTask *CreateFadeRatioAnime(int from, int to, u32 duration, int ease);
    MTask *CreateAlphaRatioAnime(int evaFrom, int evaTo, int evbFrom, int evbTo, u32 duration, int ease);
    // Blends the planes in over the others (or out).
    MTask *CreateCrossFadeInTask(int planes, u32 duration, int ease);
    MTask *CreateCrossFadeOutTask(int planes, u32 duration, int ease);

    MDisplay *mDisplay;     // 0x00
    u16 mBldCnt;            // 0x04
    u16 mBldAlpha;          // 0x06
    u16 mBldY;              // 0x08
};

class BlendModeTransfer : public MTransfer {
public:
    BlendModeTransfer(MBlend *blend, int mode) {
        mBlend = blend;
        mMode = mode;
    }
    virtual ~BlendModeTransfer() {}
    virtual void Transfer();

    MBlend *mBlend;     // 0x10
    int mMode;          // 0x14
};

class BlendAlphaRatioTransfer : public MTransfer {
public:
    BlendAlphaRatioTransfer(MBlend *blend, int eva, int evb) {
        mBlend = blend;
        mEva = eva;
        mEvb = evb;
    }
    virtual ~BlendAlphaRatioTransfer() {}
    virtual void Transfer();

    MBlend *mBlend;     // 0x10
    int mEva;           // 0x14
    int mEvb;           // 0x18
};

class BlendTarget1Transfer : public MTransfer {
public:
    BlendTarget1Transfer(MBlend *blend, int planes) {
        mBlend = blend;
        mPlanes = planes;
    }
    virtual ~BlendTarget1Transfer() {}
    virtual void Transfer();

    MBlend *mBlend;     // 0x10
    int mPlanes;        // 0x14
};

class BlendTarget2Transfer : public MTransfer {
public:
    BlendTarget2Transfer(MBlend *blend, int planes) {
        mBlend = blend;
        mPlanes = planes;
    }
    virtual ~BlendTarget2Transfer() {}
    virtual void Transfer();

    MBlend *mBlend;     // 0x10
    int mPlanes;        // 0x14
};

class MBlendClearTransfer : public MTransfer {
public:
    MBlendClearTransfer(MBlend *blend) { mBlend = blend; }
    virtual ~MBlendClearTransfer() {}
    virtual void Transfer();

    MBlend *mBlend;     // 0x10
};

#endif
