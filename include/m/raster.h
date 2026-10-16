// Raster effects: a register rewritten at every HBlank by DMA, from a table with one value per line. The game
// writes the back table; flipping makes it the one shown.
#ifndef M_RASTER_H
#define M_RASTER_H

#include "m/looper.h"
#include "m/task.h"

class MDisplay;
class MWin;
class MRasterIO;

#define M_RASTER_LINES 192

// Runs in the VBlank interrupt: flips the tables if asked, and restarts the DMA.
class MRasterTrigger : public MTask {
public:
    MRasterTrigger(MRasterIO *io);
    virtual ~MRasterTrigger() {}
    virtual void Execute();

    MRasterIO *mIO; // 0x10
};

class MRasterIO : public MLooperVSyncTask {
public:
    // dma is the channel, or -1 for any free one; size is the bytes per line.
    MRasterIO(u32 dma, volatile void *dest, u32 size);
    virtual ~MRasterIO();
    virtual void OnVSync();

    void Flip();
    void SetFlipOnVBlank(u8 flip);
    void Stop();
    void OnVBlankIntr();
    void Start();

    MRasterTrigger *mTrigger;   // 0x1C
    u32 mDma;                   // 0x20
    u8 *mCurrent;               // 0x24: the table being shown
    volatile void *mDest;       // 0x28
    u32 mSize;                  // 0x2C
    u8 *mFront;                 // 0x30: (one more line than the screen: the first line again, for the DMA)
    u8 *mBack;                  // 0x34
    u8 mFlipOnVBlank;           // 0x38
};

// Per-line backdrop colors.
class MRasterPal : public MRasterIO {
public:
    MRasterPal(u32 dma, MDisplay *display);
    virtual ~MRasterPal() {}
};

// Per-line window 0 and 1 shapes: each line has two spans, one for each window.
class MRasterWindow : public MRasterIO {
public:
    MRasterWindow(u32 dma, MDisplay *display, u8 autoFlip);
    virtual ~MRasterWindow();
    virtual void OnVSync();

    void Show();
    void Hide();
    void SetTarget(int planes);
    void SetObjTarget(int planes);
    void SetBlendEnabled(BOOL enabled);
    void SetObjBlendEnabled(BOOL enabled);
    void Clear();

    MWin *mWin;         // 0x3C
    int *mSpans;        // 0x40: per line, the two spans' left and right edges
    u8 mAutoFlip;       // 0x44: flip after the frame when changed
    u8 mChanged;        // 0x45
};

#endif
