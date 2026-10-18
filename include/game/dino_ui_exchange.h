#ifndef GAME_DINO_UI_EXCHANGE_H
#define GAME_DINO_UI_EXCHANGE_H

#include <map>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiExchange : public MProcess, public MMessageObserver {
public:
    virtual ~DinoUiExchange();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void (DinoUiExchange::*mState)();               // 0xA8
    void (DinoUiExchange::*mOnMessage)(MMessage *msg); // 0xB0
    u8 unkB8[0x30];
    std::map<u32, void *> mUnkE8;   // 0xE8
    u8 unkF4[0x4C];
    MArchivePtr mArchive;           // 0x140
    MArchiveFile mFile;             // 0x144
};

#endif
