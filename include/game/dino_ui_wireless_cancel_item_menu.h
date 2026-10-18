#ifndef GAME_DINO_UI_WIRELESS_CANCEL_ITEM_MENU_H
#define GAME_DINO_UI_WIRELESS_CANCEL_ITEM_MENU_H

#include "m/archive.h"
#include <vector>
#include <string>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"

class MSmartOam;
class MIndicator;

class DinoUiWirelessCancel : public MMenu {
public:
    DinoUiWirelessCancel(u8 *cancelled);
    virtual ~DinoUiWirelessCancel();
    virtual void OnStart();
    virtual void OnDecide(MMenuItem *item);
    void Cancel();
    // The button's closing animation and sound, then exiting.
    MTask *CreateCloseTask();

    u8 unkA7[5];         // members start in MMenu's tail padding
    u8 *mCancelled;      // 0xAC
};

class DinoUiChildScannerParentItem : public MMenuItem {
public:
    virtual ~DinoUiChildScannerParentItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    std::string GetName();  // the parent console's name
    // Shows a parent console (mode 0: none).
    void SetParent(u32 mode, const std::wstring &name);

    MSmartOam *mOam;    // 0x88
    u32 mUnk8C;         // 0x8C
    MPoint mPos;        // 0x90
    MMotion *mMotion;   // 0x98
    MIndicator *mName;  // 0x9C: the console's owner, if it has one
    u32 mMode;          // 0xA0: what the console is doing (0: nothing)
    std::string mText;  // 0xA4
};

class MMultiPlay;

class DinoUiChildScanner : public MMenu, public MMessageObserver, public DinoHelpBase {
public:
    DinoUiChildScanner(MMultiPlay *multiPlay, u32 *chosen, u32 *choice);
    virtual ~DinoUiChildScanner();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    virtual void OnMessage(MMessage *msg);
    void ExitWithReset();
    void Cancel();
    void Choose(u32 choice);

    MMultiPlay *mMultiPlay;     // 0xE8
    u32 *mChosen;               // 0xEC: set when a partner was chosen
    u32 *mChoice;               // 0xF0
    u8 mSilentFocus;            // 0xF4
    u8 unkF5[7];
    DinoUiChildScannerParentItem *mParents[9]; // 0xFC: the consoles found
    u32 unk120[3];
    DinoHelpItem *mParentHelp;  // 0x12C: help shown for every parent item
    void Rescan();
};

class DinoUiChildScannerBattleConnect : public MProcess, public MMessageObserver, public DinoHelpBase {
public:
    virtual ~DinoUiChildScannerBattleConnect();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void StateCancel();
    void StateRescan();

    void (DinoUiChildScannerBattleConnect::*mState)(); // 0xB4
    void (DinoUiChildScannerBattleConnect::*mOnMessage)(MMessage *msg); // 0xBC
    DinoUiChildScanner *mScanner;   // 0xC4
    MMultiPlay *mMultiPlay;         // 0xC8
    u32 mUnkCC[4];
    std::string mText;  // 0xDC
    MTaskSet *mUnkE8;   // 0xE8
};

class DinoUiChildScannerCannonConnect : public MProcess, public MMessageObserver, public DinoHelpBase {
public:
    virtual ~DinoUiChildScannerCannonConnect();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void (DinoUiChildScannerCannonConnect::*mState)(); // 0xB4
    void (DinoUiChildScannerCannonConnect::*mOnMessage)(MMessage *msg); // 0xBC
    u32 mUnkC4[6];
    std::string mText;  // 0xDC
};

class DinoUiBattleParentScanner : public MProcess, public MMessageObserver {
public:
    DinoUiBattleParentScanner(MMultiPlay *multiPlay, u32 *result);
    virtual ~DinoUiBattleParentScanner();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void StateStart();
    void StateShowCancel();
    void State021655a0();
    void StateAsk();
    void State02165818();

    void (DinoUiBattleParentScanner::*mState)(); // 0xA8
    MMultiPlay *mMultiPlay;             // 0xB0
    u32 *mResult;                       // 0xB4
    MTaskSet *mTasks;                   // 0xB8
    u16 mAid;                           // 0xBC: the child that connected
    u32 mSsid[6];                       // 0xC0: what it sent (its mode, then its owner's name)
    BOOL mAnswer;                       // 0xD8: whether to play with it
    u8 mCancelled;                      // 0xDC
    u8 mDisconnected;                   // 0xDD
    u32 mStartData[6];                  // 0xE0: what the parent sends

    DinoUiWirelessCancel *mCancel;      // 0xF8
};

class DinoUiCannonParentScanner : public MProcess, public MMessageObserver {
public:
    DinoUiCannonParentScanner(MMultiPlay *multiPlay, u32 *result);
    virtual ~DinoUiCannonParentScanner();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void ShowCount();
    void OnConnectMessage(MMessage *msg);
    void StateSync();
    void StateCheckFailed();
    void StateWait();

    void (DinoUiCannonParentScanner::*mState)(); // 0xA8
    void (DinoUiCannonParentScanner::*mOnMessage)(MMessage *msg); // 0xB0
    MMultiPlay *mMultiPlay;             // 0xB8
    u32 *mResult;                       // 0xBC
    u32 unkC0;
    u16 mAid;                           // 0xC4: the child that connected
    u32 mSsid[6];                       // 0xC8: what it sent (the first word is its mode)
    u32 unkE0[3];
    MMotion *mOnes;                     // 0xEC: the count's digits
    MMotion *mTens;                     // 0xF0
    u32 mCount;                         // 0xF4
    u32 mUnkF8[2];                      // 0xF8
    std::vector<unsigned int> mUnk100; // 0x100: (a distinct element type from other overlays' copies)
    std::vector<int> mUnk10C;  // 0x10C
};

class DinoUiCannonChild : public MProcess, public MMessageObserver {
public:
    DinoUiCannonChild(MMultiPlay *multiPlay, u32 choice);
    virtual ~DinoUiCannonChild();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void StateShootMenu();
    void StateShot();

    void (DinoUiCannonChild::*mState)(); // 0xA8
    u32 mUnkB0[2];
    std::vector<void *> mUnkB8; // 0xB8
    std::vector<unsigned int> mUnkC4; // 0xC4
    u32 mUnkD0;                 // 0xD0
    u8 mStarted;                // 0xD4: set by the start message
    MArchivePtr mArchive;       // 0xD8
    MArchiveFile mFileDC;       // 0xDC
    MArchiveFile mFileE8;       // 0xE8
    MArchiveFile mFileF4;       // 0xF4
    MArchiveFile mFile100;      // 0x100
    MArchivePtr mArchive2;      // 0x10C
    MArchiveFile mFile110;      // 0x110
};

class DinoUiWirelessMatch : public MProcess {
public:
    DinoUiWirelessMatch(u32 *result, u8 arg);
    virtual ~DinoUiWirelessMatch();
    virtual void Idle();

    void StateKeep();
    void StateStart();
    void StateInit();
    void StateChildScan();
    void StateCannonChild();
    void StateBattleParentScan();
    void StateParentScanned();
    void StateCannonParentScan();
    void StateRescan();

    void (DinoUiWirelessMatch::*mState)(); // 0x74
    MMultiPlay *mMultiPlay; // 0x7C: kept in DinoResource for the next scene
    MTask *mUnk80;      // 0x80
    u32 *mResult;       // 0x84
    u32 mChosen;        // 0x88
    u32 mScanResult;    // 0x8C
    u32 mChoice;        // 0x90
    u32 unk94;
    u8 mUnk98;          // 0x98
    DinoHelpDisabler *mHelpDisabler; // 0x9C
};

#endif
