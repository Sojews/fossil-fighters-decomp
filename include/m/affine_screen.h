// Affine BG screens: a screen with a rotation, a scale and a center to rotate and scale about.
#ifndef M_AFFINE_SCREEN_H
#define M_AFFINE_SCREEN_H

#include "m/screen.h"

class MAffineScreen : public MScreen {
public:
    MAffineScreen(MDisplay *display, u32 screenBase, u32 base, u32 size, u32 screenSize, int bg);
    virtual ~MAffineScreen() {}
    virtual void Update();

    void SetRot(u16 rot);
    void SetScale(MFx32 x, MFx32 y);
    void SetScale(MFx32Point scale);
    void SetCenter(int x, int y);
    void SetCenter(MPoint center);

    MTask *CreateRotTransfer(u16 rot);
    MTask *CreateRotAnime(int from, int to, int duration, int ease);
    MTask *CreateScaleAnime(MFx32Point from, MFx32Point to, int duration, int ease);

    u16 mRot;           // 0x20
    MFx32Point mScale;  // 0x24
    MPoint mCenter;     // 0x2C
};

class MAffineScreenRotTransfer : public MTransfer {
public:
    MAffineScreenRotTransfer(MAffineScreen *screen, u16 rot) {
        mScreen = screen;
        mRot = rot;
    }
    virtual ~MAffineScreenRotTransfer() {}
    virtual void Transfer();

    MAffineScreen *mScreen; // 0x10
    u16 mRot;               // 0x14
};

class MAffineScreenRotAnime : public MValueAnime {
public:
    MAffineScreenRotAnime(MAffineScreen *screen, int from, int to, int duration, int ease) : MValueAnime(from, to) {
        mScreen = screen;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~MAffineScreenRotAnime() {}
    virtual void Apply(int step);

    MAffineScreen *mScreen; // 0x40
};

class MAffineScreenScaleAnime : public MFx32PointAnime {
public:
    MAffineScreenScaleAnime(MAffineScreen *screen, MFx32Point from, MFx32Point to, int duration, int ease)
        : MFx32PointAnime(from, to) {
        mScreen = screen;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~MAffineScreenScaleAnime() {}
    virtual void Apply(int step);

    MAffineScreen *mScreen; // 0x48
};

#endif
