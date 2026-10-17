// Button and touch input. An input hub reads the hardware at each VBlank and feeds its MInputs.
#ifndef M_INPUT_H
#define M_INPUT_H

#include "m/looper.h"
#include "m/geometry.h"

// One reading of the buttons and touch panel.
struct MInputState {
    u32 buttons;    // bit 12: the panel is touched
    int touchX;
    int touchY;
};

class MInput {
public:
    MInput();
    virtual ~MInput();

    int GetDirection();
    void SetRepeat(u32 mask, int delay, int interval);
    void Update(const MInputState &state);
    MPoint GetTouchPos();
    // Buttons pressed since the last update.
    u32 GetPressed() { return mButtons & (mButtons ^ mPrevButtons); }

    u32 mButtons;           // 0x04
    u32 mPrevButtons;       // 0x08
    u32 mRepeatMask;        // 0x0C: buttons that repeat while held
    int mRepeatDelay;       // 0x10
    int mRepeatInterval;    // 0x14
    int mRepeatTimer;       // 0x18
    int mTouchX;            // 0x1C
    int mTouchY;            // 0x20
    MInput *mUnk24;         // 0x24
    u8 mUnk28;              // 0x28
};

class MInputHub : public MLooperVSyncTask {
public:
    MInputHub(BOOL first);
    virtual ~MInputHub();
    virtual u32 GetCount() = 0;
    virtual MInput *GetInput(int i) = 0;
    virtual int Unk28();
    virtual int Unk2C();

    void EnableTouch();
    void DisableTouch();
    void ReadState(MInputState *state);
    void UpdateInput(MInput *input, const MInputState &state);
    static MPoint GetTouchPos();
    static u32 GetButtons();

    u8 mTouchEnabled;   // 0x19 (in the hook base's padding)
};

// A hub with a fixed array of inputs, all fed the same readings.
class MSingleInputHub : public MInputHub {
public:
    MSingleInputHub(u32 count);
    virtual ~MSingleInputHub();
    virtual void OnVSync();
    virtual u32 GetCount();
    virtual MInput *GetInput(int i);

    MInput *mInputs;    // 0x1C
    u32 mCount;         // 0x20
    u8 mReset;          // 0x24
};

#endif
