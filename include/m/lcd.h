// The two screens: owns both 2D engines (MDisplay), hands out VRAM banks, and sets the power/swap register.
#ifndef M_LCD_H
#define M_LCD_H

#include "m/task.h"
#include "m/display.h"
#include "m/looper.h"


// The value of one VRAMCNT register.
union MVramCnt {
    u8 raw;
    struct {
        u32 mst : 3;
        u32 offset : 2;
        u32 : 2;
        u32 enable : 1;
    };
};


class MLcd : public MLooperVSyncTask {
public:
    MLcd();
    virtual void OnVSync();

    BOOL IsBankUsed(int bank);
    void UseBank(int bank, int mst, int offset);
    void FreeBank(int bank);
    void SetSwapped(BOOL swapped);
    BOOL IsSwapped();
    void SetTopDisplay(MDisplay *display);
    void SetBottomDisplay(MDisplay *display);
    MDisplay *GetTopDisplay();
    MDisplay *GetBottomDisplay();
    MVramCnt SetBankCnt(int bank, MVramCnt cnt);
    void UseTextureBanks(int config);
    void FreeTextureBanks();
    int GetTextureSlotFlags();
    u32 GetBankAddress(int bank);
    void UseBanks(const void *config, MVramBanks *out);
    void FreeBanks(MVramBanks *banks);
    void VSync();

    MDisplay *mMain;        // 0x1C
    MDisplay *mSub;         // 0x20
    u8 mBankUsed[10];       // 0x24: banks A-J
    MVramCnt mBankCnt[10];  // 0x30
    u16 mPowCnt;            // 0x58
    MVramBanks mTexBanks;   // 0x5C
    u32 mCapture;           // 0x70
    u8 mSwapped;            // 0x74
    OSMutex mMutex;         // 0x78
};

extern MLcd *gLcd;

#endif
