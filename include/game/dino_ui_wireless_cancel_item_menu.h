#ifndef GAME_DINO_UI_WIRELESS_CANCEL_ITEM_MENU_H
#define GAME_DINO_UI_WIRELESS_CANCEL_ITEM_MENU_H

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

    u32 mUnk88[7];      // 0x88
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

    u32 mUnkB4[10];     // 0xB4
    std::string mText;  // 0xDC
};

class DinoUiChildScannerCannonConnect : public MProcess, public MMessageObserver, public DinoHelpBase {
public:
    virtual ~DinoUiChildScannerCannonConnect();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    u32 mUnkB4[10];     // 0xB4
    std::string mText;  // 0xDC
};

#endif
