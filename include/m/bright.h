// Master brightness (fades to white or black) of one display: shadows MASTER_BRIGHT.
#ifndef M_BRIGHT_H
#define M_BRIGHT_H

#include "m/task.h"
#include "m/display.h"

class MBright {
public:
    MBright(MDisplay *display);

    void Reset();
    void SetMode(int mode);
    int GetMode();
    void SetFactor(int factor);
    u32 GetFactor();
    void Update();

    MTask *CreateModeTransfer(int mode);
    MTask *CreateClearTransfer();

    MDisplay *mDisplay;     // 0x00
    union {
        u16 raw;
        struct {
            u32 factor : 5;
            u32 : 9;
            int mode : 2;   // 1: brighter, 2: darker
        };
    } mValue;               // 0x04
};

class MBrightModeTransfer : public MTransfer {
public:
    MBrightModeTransfer(MBright *bright, int mode) {
        mBright = bright;
        mMode = mode;
    }
    virtual ~MBrightModeTransfer() {}
    virtual void Transfer();

    MBright *mBright;   // 0x10
    int mMode;          // 0x14
};

class MBrightClearTransfer : public MTransfer {
public:
    MBrightClearTransfer(MBright *bright) { mBright = bright; }
    virtual ~MBrightClearTransfer() {}
    virtual void Transfer();

    MBright *mBright;   // 0x10
};

#endif
