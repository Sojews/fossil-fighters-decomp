// The windows of one display (window 0, window 1, the OBJ window and the outside): which planes show inside
// each, and the rectangles of windows 0 and 1.
#ifndef M_WINDOW_H
#define M_WINDOW_H

#include "m/task.h"
#include "m/display.h"
#include "m/geometry.h"

// One window.
class MWinControl {
public:
    MWinControl(MDisplay *display, int index);

    void SwapPlanes(u32 unused, const int *a, const int *b, u32 count);
    void Show();
    void Hide();
    void SetTarget(int planes);
    void SetBlendEnabled(BOOL enabled);
    void SetShape(int left, int top, int right, int bottom);
    void SetShape(MRect rect);
    volatile u16 *GetShapeRegs();
    void Reset();
    void Update();

    MTask *CreateShowTransfer();
    MTask *CreateTargetTransfer(int planes);
    MTask *CreateBlendEnabledTransfer(bool enabled);
    MTask *CreateShapeTransfer(MRect rect);

    MDisplay *mDisplay;     // 0x00
    int mIndex;             // 0x04
    u8 mInside;             // 0x08: planes shown inside (bit 5: color effects)
    MRect mShape;           // 0x0C
};

// All the windows of a display.
class MWin {
public:
    MWin(MDisplay *display);
    ~MWin();

    void SwapPlanes(u32 unused, const int *a, const int *b, u32 count);
    void Reset();
    void Update();
    MWinControl *GetWindow0();
    MWinControl *GetWindow1();
    MWinControl *GetObjWindow();

    MTask *CreateClearTransfer();

    MDisplay *mDisplay;         // 0x00
    MWinControl *mWindows[4];   // 0x04
};

class MWinControlShowTransfer : public MTransfer {
public:
    MWinControlShowTransfer(MWinControl *win) { mWin = win; }
    virtual ~MWinControlShowTransfer() {}
    virtual void Transfer();

    MWinControl *mWin;  // 0x10
};

class MWinControlTargetTransfer : public MTransfer {
public:
    MWinControlTargetTransfer(MWinControl *win, int planes) {
        mWin = win;
        mPlanes = planes;
    }
    virtual ~MWinControlTargetTransfer() {}
    virtual void Transfer();

    MWinControl *mWin;  // 0x10
    int mPlanes;        // 0x14
};

class MWinControlBlendEnabledTransfer : public MTransfer {
public:
    MWinControlBlendEnabledTransfer(MWinControl *win, bool enabled) {
        mWin = win;
        mEnabled = enabled;
    }
    virtual ~MWinControlBlendEnabledTransfer() {}
    virtual void Transfer();

    MWinControl *mWin;  // 0x10
    bool mEnabled;      // 0x14
};

class MWinControlShapeTransfer : public MTransfer {
public:
    MWinControlShapeTransfer(MWinControl *win, MRect rect) {
        mWin = win;
        mRect = rect;
    }
    virtual ~MWinControlShapeTransfer() {}
    virtual void Transfer();

    MWinControl *mWin;  // 0x10
    MRect mRect;        // 0x14
};

class MWinClearTransfer : public MTransfer {
public:
    MWinClearTransfer(MWin *win) { mWin = win; }
    virtual ~MWinClearTransfer() {}
    virtual void Transfer();

    MWin *mWin;         // 0x10
};

#endif
