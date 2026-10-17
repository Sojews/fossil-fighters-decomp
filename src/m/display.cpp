// The two screens (MLcd) and their 2D graphics engines (MDisplay).

#include <algorithm>
#include <map>
#include "m/display.h"
#include "m/lcd.h"
#include "m/window.h"
#include "m/blend.h"
#include "m/bright.h"
#include "m/obj.h"
#include "m/bg.h"
#include "m/irq.h"

// NONMATCHING: this file's .bss holds more than these (0x28 bytes from 0x0214d1ac), and mwcc lays out
// same-sized variables in an order that depends on how many there are, so the code that reaches these
// through the pooled base address uses different offsets for now.
typedef std::map<u32, void *> MThreadDisplayMap;
// (Declared in this order for the .bss layout.)
MLcd *gLcd;
static OSSwitchThreadCallback sPrevSwitchThreadCallback;
static MThreadDisplayMap *sThreadDisplays;     // the display each thread draws on, while it isn't running
static u32 data_0214d1ac;
MDisplay *gCurrentDisplay;      // the display processes draw on by default



MDisplay::MDisplay(MLcd *lcd, MDisplayRegs *regs) : mLcd(lcd), mRegs(regs) {
    mBackdrop = 0x7FFF;
    mDispCnt.bgMode = 0;
    mDispCnt.bg0Is3D = FALSE;
    mDispCnt.objTileMapping = 1;
    mDispCnt.objBmpMapping = 0;
    mDispCnt.forceBlank = FALSE;
    mDispCnt.visiblePlanes = 0;
    mDispCnt.visibleWindows = 0;
    mDispCnt.displayMode = 1;
    mDispCnt.objTileBoundary = 0;
    mDispCnt.objBmpBoundary = 0;
    mDispCnt.hblankObj = TRUE;
    mDispCnt.bgCharBase = 0;
    mDispCnt.bgScreenBase = 0;
    mDispCnt.bgExtPalette = FALSE;
    mDispCnt.objExtPalette = FALSE;
    mUnk10.lo = 0;
    mWin = new MWin(this);
    mBlend = new MBlend(this);
    mBright = new MBright(this);
    mBg = NULL;
    mObj = NULL;
}

MDisplay::~MDisplay() {
    delete mWin;
    delete mBlend;
    delete mBright;
}

void MDisplay::Update() {
    *mRegs->backdrop = mBackdrop;
    *mRegs->dispcnt = mDispCnt;
    *mRegs->unk88 = mUnk10;
    if (mBg != NULL) {
        mBg->Update();
    }
    if (mObj != NULL) {
        mObj->Update();
    }
    mWin->Update();
    mBlend->Update();
    mBright->Update();
}

void MDisplay::AddHook(MDisplayHook *hook) {
    mHooks.push_back(hook);
}

void MDisplay::RemoveHook(MDisplayHook *hook) {
    mHooks.erase(std::find(mHooks.begin(), mHooks.end(), hook));
}

u32 MDisplay::GetRegs00() {
    return mRegs->unk00;
}

u32 MDisplay::GetRegs08(int i) {
    return mRegs->unk08[i];
}

u32 MDisplay::GetRegs18(int i) {
    return mRegs->unk18[i];
}

u32 MDisplay::GetRegs28(int i) {
    return mRegs->unk28[i];
}

volatile u8 *MDisplay::GetRegs64(int i) {
    return mRegs->winin[i];
}

volatile u16 *MDisplay::GetRegs74(int i) {
    return mRegs->winh[i];
}

volatile u16 *MDisplay::GetRegs7C() {
    return mRegs->bldcnt;
}

volatile u16 *MDisplay::GetRegs80() {
    return mRegs->bldalpha;
}

volatile u16 *MDisplay::GetRegs84() {
    return mRegs->bldy;
}

volatile u32 *MDisplay::GetRegs8C() {
    return mRegs->masterBright;
}

void MDisplay::ShowPlanes(u32 mask) {
    mDispCnt.visiblePlanes |= mask;
}

void MDisplay::HidePlanes(u32 mask) {
    mDispCnt.visiblePlanes &= ~mask;
}

BOOL MDisplay::ArePlanesHidden(u32 mask) {
    return (mDispCnt.visiblePlanes & mask) == 0;
}

void MDisplay::ShowWindow(int window) {
    if (window == 3) {
        window = 2;
    }
    mDispCnt.visibleWindows |= 1 << window;
}

void MDisplay::HideWindow(int window) {
    if (window == 3) {
        window = 2;
    }
    mDispCnt.visibleWindows &= ~(1 << window);
}

void MDisplay::SetBackdrop(u16 color) {
    mBackdrop = color;
}

volatile u16 *MDisplay::GetBackdropReg() {
    return mRegs->backdrop;
}

void MDisplay::SetBg0Is3D(BOOL on) {
    if (on) {
        mDispCnt.bg0Is3D = TRUE;
    } else {
        mDispCnt.bg0Is3D = FALSE;
    }
}

void MDisplay::SetObjTileBoundary(int boundary) {
    mDispCnt.objTileBoundary = boundary;
}

// Keeps gCurrentDisplay per thread.
static void SwitchThreadDisplay(u32 from, u32 to) {
    if (sPrevSwitchThreadCallback != NULL) {
        sPrevSwitchThreadCallback((OSThread *) from, (OSThread *) to);
    }
    MIrqLock lock;
    MThreadDisplayMap::iterator it = sThreadDisplays->find(from);
    if (it != sThreadDisplays->end()) {
        it->second = gCurrentDisplay;
    }
    it = sThreadDisplays->find(to);
    gCurrentDisplay = it != sThreadDisplays->end() ? (MDisplay *) it->second : NULL;
}

void InitThreadDisplays() {
    sThreadDisplays = new MThreadDisplayMap;
    sPrevSwitchThreadCallback = OS_SetSwitchThreadCallback((OSSwitchThreadCallback) SwitchThreadDisplay);
    AddThreadDisplay((u32) OS_GetCurrentThread());
}

void AddThreadDisplay(u32 thread) {
    MIrqLock lock;
    (*sThreadDisplays)[thread] = gCurrentDisplay;
}

MDisplayHook::MDisplayHook(MDisplay *display) {
    mDisplay = display;
    display->AddHook(this);
}

MDisplayHook::~MDisplayHook() {
    mDisplay->RemoveHook(this);
}

void InitLcd() {
    gLcd = new MLcd;
    InitThreadDisplays();
}

extern "C" {
extern void *data_0208b378[];   // VRAMCNT register addresses
extern u8 data_0208b410[];      // texture VRAM configurations, 0x1C bytes each
extern u32 data_0208bcc8[][5][4];   // bank addresses, by bank, MST and offset
}

#define REG_POWCNT (*(volatile u16 *)0x04000304)

extern "C" {
extern MDisplayRegs data_0208b65c;  // main engine
extern MDisplayRegs data_0208b6ec;  // sub engine
}

MLcd::MLcd() : MLooperVSyncTask(FALSE, TRUE) {
    u32 i;

    OS_InitMutex(&mMutex);
    mPowCnt = REG_POWCNT;
    mCapture = 0;
    mSwapped = FALSE;
    for (i = 0; i < 10; i++) {
        mBankUsed[i] = FALSE;
    }
    for (i = 0; i < 10; i++) {
        mBankCnt[i].raw = 0;
    }
    mMain = new MDisplay(this, &data_0208b65c);
    mSub = new MDisplay(this, &data_0208b6ec);
    gCurrentDisplay = mMain;
}

void MLcd::OnVSync() {
    VSync();
}

void MLcd::VSyncNow() {
    if (OS_GetCurrentThread() == gLooper->GetThread()) {
        OS_WaitIrq(TRUE, 1);
        VSync();
    }
}

BOOL MLcd::IsBankUsed(int bank) {
    return mBankUsed[bank];
}

void MLcd::UseBank(int bank, int mst, int offset) {
    if (mBankUsed[bank]) {
        OS_Terminate();
    }
    mBankUsed[bank] = TRUE;
    mBankCnt[bank].mst = mst;
    mBankCnt[bank].offset = offset;
    mBankCnt[bank].enable = TRUE;
}

void MLcd::FreeBank(int bank) {
    if (!mBankUsed[bank]) {
        OS_Terminate();
    }
    mBankUsed[bank] = FALSE;
    mBankCnt[bank].raw = 0;
}

void MLcd::SetSwapped(BOOL swapped) {
    mSwapped = swapped;
}

BOOL MLcd::IsSwapped() {
    return mSwapped;
}

void MLcd::SetTopDisplay(MDisplay *display) {
    if (display == mMain) {
        mPowCnt |= 0x8000;
    } else {
        mPowCnt &= ~0x8000;
    }
}

void MLcd::SetBottomDisplay(MDisplay *display) {
    if (display == mSub) {
        mPowCnt |= 0x8000;
    } else {
        mPowCnt &= ~0x8000;
    }
}

MDisplay *MLcd::GetTopDisplay() {
    if (mPowCnt & 0x8000) {
        return mMain;
    }
    return mSub;
}

MDisplay *MLcd::GetBottomDisplay() {
    if (mPowCnt & 0x8000) {
        return mSub;
    }
    return mMain;
}

void MLcd::FreeBanks(MVramBanks *banks) {
    for (u32 i = 0; i < 4; i++) {
        if (banks->banks[i]) {
            FreeBank(banks->banks[i]);
        }
        banks->banks[i] = 0;
    }
    banks->count = 0;
}

MVramCnt MLcd::SetBankCnt(int bank, MVramCnt cnt) {
    MVramCnt old;

    old.raw = mBankCnt[bank].raw;
    mBankCnt[bank].raw = cnt.raw;
    *(u8 *)data_0208b378[bank] = cnt.raw;
    return old;
}

void MLcd::UseTextureBanks(int config) {
    UseBanks(data_0208b410 + config * 0x1C, &mTexBanks);
}

void MLcd::FreeTextureBanks() {
    FreeBanks(&mTexBanks);
}

int MLcd::GetTextureSlotFlags() {
    if (mTexBanks.banks[0] == 3 && mTexBanks.banks[1] == 4) {
        return 0xC;
    }
    if (mTexBanks.banks[0] == 3) {
        return 4;
    }
    if (mTexBanks.banks[0] == 4) {
        return 8;
    }
    return 0;
}

u32 MLcd::GetBankAddress(int bank) {
    return data_0208bcc8[bank][mBankCnt[bank].mst][mBankCnt[bank].offset];
}
