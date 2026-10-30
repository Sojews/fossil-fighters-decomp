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

// Tells the hooks, then gives up the BG, OBJ and VRAM banks.
void MDisplay::ResetHooks() {
    MScopedLock lock(&gLcd->mMutex, TRUE);
    for (std::vector<MDisplayHook *>::iterator it = mHooks.begin(); it != mHooks.end(); it++) {
        (*it)->Unk0C();
    }
    if (mBg != NULL) {
        delete mBg;
        mBg = NULL;
    }
    if (mObj != NULL) {
        delete mObj;
        mObj = NULL;
    }
    mLcd->FreeBanks(&mBanks28);
    mLcd->FreeBanks(&mBanks50);
    mLcd->FreeBanks(&mBanks78);
    mLcd->FreeBanks(&mBanks8C);
    mLcd->FreeBanks(&mBanks3C);
    mLcd->FreeBanks(&mBanks64);
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

// Sets the BG mode and moves the layers' visibility (and their blend and window settings) between BG numbers.
// NONMATCHING: the two bit masks swap registers (and the from/to pointers load the other way round).
void MDisplay::SetBgMode(int mode, const int *from, const int *to, u32 count) {
    u32 i;
    u32 bitA, bitB;

    mDispCnt.bgMode = mode;
    for (i = 0; i < count; i++) {
        bitB = 1 << (to[i] + 8);
        bitA = 1 << (from[i] + 8);
        mDispCnt.raw = (mDispCnt.raw & ~(bitB | bitA)) | ((mDispCnt.raw & bitA) ? bitB : 0) |
                       ((mDispCnt.raw & bitB) ? bitA : 0);
    }
    mBlend->SwapPlanes(mode, from, to, count);
    mWin->SwapPlanes(mode, from, to, count);
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
    OS_InitMutex(&mMutex);
    mPowCnt = REG_POWCNT;
    mCapture.raw = 0;
    mSwapped = FALSE;
    for (u32 i = 0; i < 10; i++) {
        mBankUsed[i] = FALSE;
    }
    for (u32 i = 0; i < 10; i++) {
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

#define REG_DISPCAPCNT (*(volatile u32 *)0x04000064)

// Writes the banks' settings (a bank being captured into goes to the LCDC), starts a capture, updates both engines
// and sets the power register.
void MLcd::VSync() {
    MScopedLock lock(&mMutex, TRUE);
    for (u32 i = 1; i < 10; i++) {
        if (mCapture.enable && mCapture.bank == i - 1) {
            MVramCnt cnt;
            cnt.raw = 0;
            cnt.enable = TRUE;
            *(u8 *)data_0208b378[i] = cnt.raw;
        } else {
            *(u8 *)data_0208b378[i] = mBankCnt[i].raw;
        }
    }
    if (mCapture.enable) {
        REG_DISPCAPCNT = mCapture.raw;
        mCapture.enable = FALSE;
    }
    mMain->Update();
    mSub->Update();
    u16 pow = mPowCnt;
    if (mSwapped) {
        pow ^= 0x8000;
    }
    REG_POWCNT = (REG_POWCNT & 0x20F) | (pow & ~0x20F);
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

// A bank configuration (0x1C bytes): the MST to map the banks with and the banks to try, in order.
struct MVramConfig {
    int mst;            // 0x00: -1: none
    u32 count;          // 0x04: 0: no banks
    u32 needed;         // 0x08
    u32 options[4];     // 0x0C: bank (1 for A), offset << 4, 0x100: give up if it's taken
};

void MLcd::UseBanks(const void *config, MVramBanks *out) {
    const MVramConfig *cfg = (const MVramConfig *) config;
    u32 i;

    if (cfg->mst == -1) {
        OS_Terminate();
    }
    if (cfg->count == 0) {
        for (i = 0; i < 4; i++) {
            out->banks[i] = 0;
        }
        return;
    }
    u32 used = 0;
    for (u32 j = 0; j < 4; j++) {
        u32 option = cfg->options[j];
        int bank = option & 0xF;
        int offset = (option & 0xF0) >> 4;
        BOOL required = (option & 0x100) ? TRUE : FALSE;
        if (bank == 0) {
            break;
        }
        if (IsBankUsed(bank)) {
            if (!required) {
                continue;
            }
            break;
        }
        out->banks[used] = bank;
        UseBank(bank, cfg->mst, used + offset);
        used++;
        if (used >= cfg->needed) {
            out->count = cfg->count;
            for (; used < 4; used++) {
                out->banks[used] = 0;
            }
            return;
        }
    }
    OS_Terminate();
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

void MDisplay::Setup(int a, int c, int e, int f) {
    Setup(a, 0, c, 0, e, f);
}

extern "C" {
extern MBg::ExtPalettes data_0208ba1c[];   // the BG extended palettes, by count and first bank
extern u8 data_0208b338[];                 // (pooled .data: no extended palettes at +0x1C)
}

// Gives the engine its VRAM banks (configurations: a BG, b BG extended palettes, c OBJ, d OBJ extended palettes,
// e and f the BG's other areas), then makes the BG and OBJ managers and tells the hooks.
void MDisplay::Setup(int a, int b, int c, int d, int e, int f) {
    MScopedLock lock(&gLcd->mMutex, TRUE);
    mWin->Reset();
    mBlend->Reset();
    mLcd->UseBanks(mRegs->unk54 + c * 0x1C, &mBanks50);
    mLcd->UseBanks(mRegs->unk4C + a * 0x1C, &mBanks28);
    mLcd->UseBanks(mRegs->unk5C + e * 0x1C, &mBanks78);
    mLcd->UseBanks(mRegs->unk60 + f * 0x1C, &mBanks8C);
    mLcd->UseBanks(mRegs->unk50 + b * 0x1C, &mBanks3C);
    mLcd->UseBanks(mRegs->unk58 + d * 0x1C, &mBanks64);
    mDispCnt.bgExtPalette = b != 0 ? TRUE : FALSE;
    mDispCnt.objExtPalette = d != 0 ? TRUE : FALSE;
    MBg::ExtPalettes ext;
    if (b != 0) {
        int i;
        for (i = 0; i < 5; i++) {
            if (b == data_0208ba1c[i].unk00 && mBanks3C.banks[0] == (u32)data_0208ba1c[i].entries[0].palette) {
                break;
            }
        }
        ext = data_0208ba1c[i];
    } else {
        ext = *(MBg::ExtPalettes *)(data_0208b338 + 0x1C);
    }
    u32 objExtBase = 0;
    void *objExtPalette = NULL;
    if (d != 0) {
        switch (mBanks64.banks[0]) {
        default:
            OS_Terminate();
            break;
        case 6:
            objExtBase = 0x06890000;
            break;
        case 7:
            objExtBase = 0x06894000;
            break;
        case 9:
            objExtBase = 0x068A0000;
            break;
        }
        objExtPalette = (void *)mBanks64.banks[0];
    }
    if (c != 0) {
        mObj = new MObj(this, mRegs->unk48, mRegs->unk44, mBanks50.count, mRegs->unk3C, objExtBase, objExtPalette);
    }
    if (a != 0) {
        mBg = new MBg(this, mRegs->unk40, mBanks28.count, (u32)mRegs->backdrop, ext, mBanks78.count, mBanks8C.count);
    }
    for (std::vector<MDisplayHook *>::iterator it = mHooks.begin(); it != mHooks.end(); it++) {
        (*it)->Unk08();
    }
}

// Sets up a capture for the next VBlank (bank: 1 for A to 4 for D).
void MLcd::StartCapture(int mode, int srcA, int srcB, int size, int readOffset, int bank, int writeOffset, int eva,
                        int evb) {
    if (mCapture.enable) {
        OS_Terminate();
    }
    mCapture.enable = TRUE;
    mCapture.mode = mode;
    mCapture.readOffset = readOffset;
    mCapture.srcA = srcA;
    mCapture.srcB = srcB;
    mCapture.size = size;
    mCapture.writeOffset = writeOffset;
    mCapture.bank = bank - 1;
    mCapture.eva = eva;
    mCapture.evb = evb;
}
