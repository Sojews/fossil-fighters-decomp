// Button and touch input.

#include "m/input.h"
#include "nitro/tp.h"

#define REG_KEYINPUT (*(volatile u16 *)0x04000130)
#define REG_EXTKEYIN (*(volatile u16 *)0x027FFFA8)  // X/Y/debug/hinge and touch, from the ARM7

extern "C" {
extern int data_0208c2d4[16];   // the direction for each combination of the d-pad buttons
}

// The touch panel, shared by all hubs.
struct MTouchState {
    MTouchState() { OS_InitMutex(&mutex); }

    u8 touched;         // 0x00
    int refCount;       // 0x04
    u32 buttons;        // 0x08
    int x;              // 0x0C
    int y;              // 0x10
    OSMutex mutex;      // 0x14
    TPData samples[5];  // 0x2C
};

static MTouchState sTouch;

MInput::MInput() {
    mButtons = 0;
    mPrevButtons = 0;
    mRepeatMask = 0;
    mUnk24 = NULL;
    mUnk28 = FALSE;
}

MInput::~MInput() {
    delete mUnk24;
}

int MInput::GetDirection() {
    return data_0208c2d4[(mButtons & 0xF0) >> 4];
}

void MInput::SetRepeat(u32 mask, int delay, int interval) {
    mRepeatMask = mask;
    mRepeatTimer = delay;
    mRepeatDelay = delay;
    mRepeatInterval = interval;
}

void MInput::Update(const MInputState &state) {
    mPrevButtons = mButtons;
    mButtons = state.buttons;
    mTouchX = state.touchX;
    mTouchY = state.touchY;
    if (mRepeatMask != 0) {
        if (mButtons & mRepeatMask) {
            if (--mRepeatTimer == 0) {
                mRepeatTimer = mRepeatInterval;
                mButtons &= ~mRepeatMask;
            }
        } else {
            mRepeatTimer = mRepeatDelay;
        }
    }
}

MPoint MInput::GetTouchPos() {
    MPoint pos;

    pos.x = mTouchX;
    pos.y = mTouchY;
    return pos;
}

static void StartTouch() {
    MScopedLock lock(&sTouch.mutex, TRUE);
    TPCalibrateParam param;
    TPData data;

    if (sTouch.refCount++ > 0) {
        return;
    }
    TP_Init();
    if (!TP_GetUserInfo(&param)) {
        OS_Terminate();
    }
    TP_SetCalibrateParam(&param);
    do {
        do {
            TP_RequestSamplingAsync();
        } while (TP_WaitCalibratedResult(&data) != 0);
    } while (data.touch != 0 && data.validity != 0);
    do {
        TP_RequestAutoSamplingStartAsync(10, 4, sTouch.samples, 5);
        TP_WaitBusy(2);
    } while (TP_CheckError(2) != 0);
    sTouch.samples[0] = data;
}

static void StopTouch() {
    MScopedLock lock(&sTouch.mutex, TRUE);

    if (--sTouch.refCount > 0) {
        return;
    }
    do {
        TP_RequestAutoSamplingStopAsync();
        TP_WaitBusy(4);
    } while (TP_CheckError(4) != 0);
}

static void ReadTouch(u8 *touched, MPoint *pos) {
    TPData data;

    TP_GetLatestCalibratedPointInAuto(&data);
    if (data.touch != 0) {
        if (data.validity == 0) {
            sTouch.x = data.x;
            sTouch.y = data.y;
        } else if (!sTouch.touched) {
            data.touch = 0;
        }
    }
    sTouch.touched = data.touch != 0 ? TRUE : FALSE;
    *touched = sTouch.touched;
    pos->x = sTouch.x;
    pos->y = sTouch.y;
}

MInputHub::MInputHub(BOOL first) : MLooperVSyncTask(first, FALSE) {
    mTouchEnabled = FALSE;
}

MInputHub::~MInputHub() {
    DisableTouch();
}

void MInputHub::UpdateInput(MInput *input, const MInputState &state) {
    input->Update(state);
}

void MInputHub::EnableTouch() {
    if (!mTouchEnabled) {
        mTouchEnabled = TRUE;
        StartTouch();
    }
}

void MInputHub::DisableTouch() {
    if (mTouchEnabled) {
        mTouchEnabled = FALSE;
        StopTouch();
    }
}

void MInputHub::ReadState(MInputState *state) {
    if ((REG_EXTKEYIN & 0x8000) >> 15) {
        state->buttons = 0;
    } else {
        state->buttons = (u16)(0x2FFF & ((REG_KEYINPUT | REG_EXTKEYIN) ^ 0x2FFF));
        if (mTouchEnabled) {
            u8 touched;
            MPoint pos;

            ReadTouch(&touched, &pos);
            if (touched) {
                state->buttons |= 0x1000;
            }
            state->touchX = pos.x;
            state->touchY = pos.y;
        }
    }
    sTouch.buttons = state->buttons;
}

MPoint MInputHub::GetTouchPos() {
    MPoint pos;

    pos.x = sTouch.x;
    pos.y = sTouch.y;
    return pos;
}

u32 MInputHub::GetButtons() {
    return sTouch.buttons;
}

MSingleInputHub::MSingleInputHub(u32 count) : MInputHub(FALSE) {
    mCount = count;
    mReset = TRUE;
    mInputs = new MInput[4];
    Enable();
}

MSingleInputHub::~MSingleInputHub() {
    Disable();
    delete[] mInputs;
}

u32 MSingleInputHub::GetCount() {
    return mCount;
}

MInput *MSingleInputHub::GetInput(int i) {
    return &mInputs[i];
}

void MSingleInputHub::OnVSync() {
    MInputState state;
    u32 i;

    ReadState(&state);
    if (mReset) {
        mReset = FALSE;
        UpdateInput(&mInputs[0], state);
    }
    UpdateInput(&mInputs[0], state);
    state.buttons = 0;
    state.touchX = 0;
    state.touchY = 0;
    for (i = 1; i < mCount; i++) {
        UpdateInput(&mInputs[i], state);
    }
}
