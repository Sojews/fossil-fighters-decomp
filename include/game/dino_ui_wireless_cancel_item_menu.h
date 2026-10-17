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

class DinoUiWirelessCancel : public MMenu {
public:
    virtual ~DinoUiWirelessCancel();
    virtual void OnStart();
    virtual void OnDecide(MMenuItem *item);
};

class DinoUiChildScannerParentItem : public MMenuItem {
public:
    virtual ~DinoUiChildScannerParentItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    u32 mUnk88[4];      // 0x88
    MMotion *mMotion;   // 0x98
    u32 mUnk9C;         // 0x9C
    void *mUnkA0;       // 0xA0
    std::string mText;  // 0xA4
};

class DinoUiChildScanner : public MMenu, public MMessageObserver, public DinoHelpBase {
public:
    virtual ~DinoUiChildScanner();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    virtual void OnMessage(MMessage *msg);
};

class DinoUiChildScannerBattleConnect : public MProcess, public MMessageObserver, public DinoHelpBase {
public:
    virtual ~DinoUiChildScannerBattleConnect();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void (DinoUiChildScannerBattleConnect::*mState)(); // 0xB4
    void (DinoUiChildScannerBattleConnect::*mOnMessage)(MMessage *msg); // 0xBC
    u32 mUnkC4[6];
    std::string mText;  // 0xDC
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
    virtual ~DinoUiBattleParentScanner();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void (DinoUiBattleParentScanner::*mState)(); // 0xA8
};

class DinoUiCannonParentScanner : public MProcess, public MMessageObserver {
public:
    virtual ~DinoUiCannonParentScanner();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void (DinoUiCannonParentScanner::*mState)(); // 0xA8
    void (DinoUiCannonParentScanner::*mOnMessage)(MMessage *msg); // 0xB0
    u32 mUnkB8[18];
    std::vector<unsigned int> mUnk100; // 0x100: (a distinct element type from other overlays' copies)
    std::vector<int> mUnk10C;  // 0x10C
};

class DinoUiCannonChild : public MProcess, public MMessageObserver {
public:
    virtual ~DinoUiCannonChild();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void (DinoUiCannonChild::*mState)(); // 0xA8
    u32 mUnkB0[2];
    std::vector<void *> mUnkB8; // 0xB8
    std::vector<unsigned int> mUnkC4; // 0xC4
    u32 mUnkD0[2];              // 0xD0
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
    virtual ~DinoUiWirelessMatch();
    virtual void Idle();

    void (DinoUiWirelessMatch::*mState)(); // 0x74
    u32 mUnk7C[8];
    MTask *mUnk9C;      // 0x9C
};

#endif
