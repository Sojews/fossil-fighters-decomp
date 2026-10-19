// CameraWipe.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/window.h"
#include "m/raster.h"
#include "m/lcd.h"
#include "game/camera_wipe.h"
#include "types.h"
#include "m/seed_generator.h"
#include "game/dino_resource.h"

extern u8 data_ov000_02197930;

CameraWipe::CameraWipe(Owner *owner, u32 mode, u32 duration) : MProcess(NULL) {
    mOwner = owner;
    mMode = mode;
    mDuration = duration;
    mUnk88 = 0x1000;
    mStep = 0x181000 / duration;
    mMode = (u16)mode;
    mWindow = new MRasterWindow(-1, gLcd->mMain, 0);
    mWindow->SetTarget(mode & 0x10000 ? 0x3E : 0);
    mWindow->SetObjTarget(0x3F);
    mWindow->SetBlendEnabled(TRUE);
    mWindow->SetObjBlendEnabled(TRUE);
    mWindow->Clear();
    mWindow->Show();
    mParallel.AddTail(mWindow);
}

CameraWipe::~CameraWipe() {
    if ((u32)(mMode - 2) <= 1) {
        mOwner->mScreen->Hide();
        data_ov000_02197930 = TRUE;
    } else {
        data_ov000_02197930 = FALSE;
    }
    gLcd->mMain->mWin->Reset();
}

extern "C" void G3X_SetFog(BOOL enable, int fogMode, int fogSlope, int fogOffset);
extern "C" void G3X_SetFogTable(const u32 *fogTable);
extern u8 data_ov000_02197934[32];  // the fog density table

static inline void FillFogTable(u8 density) {
    for (int i = 0; i < 32; i++) {
        data_ov000_02197934[i] = density;
    }
}

M3DFadeTask::~M3DFadeTask() {
    if (mUnk14 < 0) {
        FillFogTable(0);
        G3X_SetFog(FALSE, 0, 0, 0);
    } else {
        FillFogTable(0x7F);
        G3X_SetFog(TRUE, 0, 0, 0);
        *(vu32 *)0x04000358 = 0x1F << 16;   // fog color: black, alpha 31
        G3X_SetFogTable((const u32 *)data_ov000_02197934);
    }
}

// NONMATCHING: 89%, register choice (the game keeps the density in r0).
// Steps the fog to the next density (starting from the table's), stopping at either end.
void M3DFadeTask::Execute() {
    if (mUnk1C) {
        mUnk1C = FALSE;
        // (The game reads the table through the bss block it shares with the flag before it.)
        mUnk10 = (&data_ov000_02197930)[4] << 12;
        if (mUnk18 == 0) {
            mUnk18 = 1;
        }
        mUnk14 = (0x7F << 12) / (u32)mUnk18;
        if (mIn) {
            mUnk14 = -mUnk14;
        }
    }
    int density = mUnk10 >> 12;
    mUnk10 += mUnk14;
    if (density > 127) {
        Exit();
        density = 127;
    } else if (density < 0) {
        Exit();
        density = 0;
    }
    FillFogTable(density);
    G3X_SetFog(TRUE, 0, 0, 0);
    *(vu32 *)0x04000358 = 0x1F << 16;
    G3X_SetFogTable((const u32 *)data_ov000_02197934);
}

// NONMATCHING: the game calls std::vector<std::string>'s out-of-line destructor for mLines; ours inlines it into a
// __vec_deleter call. The bytes are identical, so only callcheck sees it.
DinoBattleViewActionInfoBar::~DinoBattleViewActionInfoBar() {
    mFont->Release();
}

#pragma auto_inline off

extern u32 data_ov000_02197954[256];    // random fog values (0 to 127)

extern "C" void func_ov000_0218ada0() {
    MRandom random;
    for (int i = 0; i < 256; i++) {
        data_ov000_02197954[i] = random.Next() & 0x7F;
    }
    data_ov000_02197930 = FALSE;
}

// Fades the 3D scene out (just a frame's wait when it's out already).
extern "C" MTask *func_ov000_0218addc(int arg) {
    if (data_ov000_02197930) {
        return new MTimer(1);
    }
    data_ov000_02197930 = TRUE;
    return new M3DFadeTask(arg, FALSE);
}

// Fades it back in.
extern "C" MTask *func_ov000_0218ae50(int arg) {
    if (!data_ov000_02197930) {
        return new MTimer(1);
    }
    data_ov000_02197930 = FALSE;
    return new M3DFadeTask(arg, TRUE);
}

DinoBattleViewActionInfoBar::DinoBattleViewActionInfoBar(u32 arg)
    : MProcess(NULL), mText(NULL), mUnk84(arg), mTimer(0), mFade(0) {
    mFont = gDinoResource->CreateFont12(1, 10, 1);
    mFont->AddRef();
}
