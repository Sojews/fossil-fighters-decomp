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
#include "game/dino_backup.h"
#include "m/picture.h"
#include "m/resource.h"
#include "game/dino_resource.h"

class MSmartOam;

// What a child sends the parent (and a parent advertises): the mode and the owner's name.
struct DinoUiWirelessSsid {
    u32 mode;
    u16 name[9];
};
class DinoUiWirelessCancelItem;
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

    u8 unkA7;            // members start in MMenu's tail padding
    DinoUiWirelessCancelItem *mButton; // 0xA8
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
class MMultiPlayMessageHub;

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
    void Leave();

    MMultiPlay *mMultiPlay;     // 0xE8
    u32 *mChosen;               // 0xEC: set when a partner was chosen
    u32 *mChoice;               // 0xF0
    u8 mSilentFocus;            // 0xF4
    u8 unkF5[7];
    DinoUiChildScannerParentItem *mParents[9]; // 0xFC: the consoles found
    MMenuItem *mStartBattleItem;    // 0x120
    MMenuItem *mStartCannonItem;    // 0x124
    MMenuItem *mCancelItem;         // 0x128
    DinoHelpItem *mParentHelp;  // 0x12C: help shown for every parent item
    void Rescan();
};

class DinoUiChildScannerBattleConnect : public MProcess, public MMessageObserver, public DinoHelpBase {
public:
    DinoUiChildScannerBattleConnect(DinoUiChildScanner *scanner, MMultiPlay *multiPlay,
                                    DinoUiChildScannerParentItem *parent);
    virtual ~DinoUiChildScannerBattleConnect();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void IgnoreMessage(MMessage *msg);
    void OnConnectMessage(MMessage *msg);
    void StateStart();
    void StateConnect();
    void StateChoose();
    void StateConnecting();
    void StateCancel();
    void StateRescan();

    void (DinoUiChildScannerBattleConnect::*mState)(); // 0xB4
    void (DinoUiChildScannerBattleConnect::*mOnMessage)(MMessage *msg); // 0xBC
    DinoUiChildScanner *mScanner;   // 0xC4
    MMultiPlay *mMultiPlay;         // 0xC8
    DinoUiChildScannerParentItem *mParent; // 0xCC: the console to connect to
    u32 mUnkD0;                     // 0xD0: its mUnk8C
    MPoint mPos;                    // 0xD4: its place
    std::string mText;  // 0xDC
    MTaskSet *mUnkE8;   // 0xE8
    u32 mChoice;        // 0xEC: the window's
};

class DinoUiChildScannerCannonConnect : public MProcess, public MMessageObserver, public DinoHelpBase {
public:
    DinoUiChildScannerCannonConnect(DinoUiChildScanner *scanner, MMultiPlay *multiPlay,
                                    DinoUiChildScannerParentItem *parent);
    virtual ~DinoUiChildScannerCannonConnect();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void IgnoreMessage(MMessage *msg);
    void OnConnectMessage(MMessage *msg);
    void StateStart();
    void StateConnect();
    void StateChoose();
    void StateConnecting();

    void (DinoUiChildScannerCannonConnect::*mState)(); // 0xB4
    void (DinoUiChildScannerCannonConnect::*mOnMessage)(MMessage *msg); // 0xBC
    DinoUiChildScanner *mScanner;   // 0xC4
    MMultiPlay *mMultiPlay;         // 0xC8
    DinoUiChildScannerParentItem *mParent; // 0xCC: the console to connect to
    u32 mUnkD0;                     // 0xD0: its mUnk8C
    MPoint mPos;                    // 0xD4: its place
    std::string mText;  // 0xDC
    MTaskSet *mUnkE8;   // 0xE8
    u32 mChoice;        // 0xEC: the window's
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
    DinoUiWirelessSsid mStartData;      // 0xE0: what the parent sends

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
    void StateListen();
    void OnCountMessage(MMessage *msg);
    void StateLost();
    void StateCancelled();
    void StateShowCancel();
    void StateWaitChildren();
    void State021664c0();
    void State02166588();

    void (DinoUiCannonParentScanner::*mState)(); // 0xA8
    void (DinoUiCannonParentScanner::*mOnMessage)(MMessage *msg); // 0xB0
    MMultiPlay *mMultiPlay;             // 0xB8
    u32 *mResult;                       // 0xBC
    MTaskSet *mTasks;                   // 0xC0
    u16 mAid;                           // 0xC4: the child that connected
    u32 mSsid[6];                       // 0xC8: what it sent (the first word is its mode)
    MMultiPlayMessageHub *mHub;         // 0xE0
    MSmartOam *mOam;                    // 0xE4
    u32 unkE8;
    MMotion *mOnes;                     // 0xEC: the count's digits
    MMotion *mTens;                     // 0xF0
    u32 mCount;                         // 0xF4
    u8 mCancelled;                      // 0xF8
    u8 mUnkF9;                          // 0xF9
    DinoUiWirelessCancel *mCancel;      // 0xFC
    std::vector<DinoBackupEntry> mRocks;        // 0x100: the fossil rocks the children sent
    std::vector<DinoPlayerProfile> mProfiles;   // 0x10C: the children's profiles
};

class DinoUiCannonChild : public MProcess, public MMessageObserver {
public:
    DinoUiCannonChild(MMultiPlay *multiPlay, u32 choice);
    virtual ~DinoUiCannonChild();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void StateStart();
    void StateShootMenu();
    void StateShot();
    void Leave();
    void StateJoin();
    void StateWaitStart();
    void StateSendRocks();
    void StateSendProfile();
    void State021672f8();

    void (DinoUiCannonChild::*mState)(); // 0xA8
    MMultiPlay *mMultiPlay;     // 0xB0
    u16 mChoice;                // 0xB4
    std::vector<void *> mUnkB8; // 0xB8
    std::vector<DinoBackupEntry> mRocks; // 0xC4: the ones shot
    MMultiPlayMessageHub *mHub; // 0xD0
    u8 mStarted;                // 0xD4: set by the start message
    MPictureFile mBack;         // 0xD8: image/m_back00
    MResource<DinoSenryuDefs> mSenryuDefs; // 0x10C
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
    void StateChosen();
    void StateAfterReset();
    void StateFinish();
    void StateAsk();
    void StateAnswered();
    void StateStartWireless();

    void (DinoUiWirelessMatch::*mState)(); // 0x74
    MMultiPlay *mMultiPlay; // 0x7C: kept in DinoResource for the next scene
    MTask *mUnk80;      // 0x80
    u32 *mResult;       // 0x84
    u32 mChosen;        // 0x88
    u32 mScanResult;    // 0x8C
    u32 mChoice;        // 0x90
    BOOL mAnswer;       // 0x94: to the start question
    u8 mUnk98;          // 0x98
    DinoHelpDisabler *mHelpDisabler; // 0x9C
};

#endif
