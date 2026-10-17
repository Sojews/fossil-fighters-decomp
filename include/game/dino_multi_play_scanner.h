#ifndef GAME_DINO_MULTI_PLAY_SCANNER_H
#define GAME_DINO_MULTI_PLAY_SCANNER_H

#include "m/font.h"
#include "m/seed_generator.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoMultiPlayScanner : public DinoHelpBase, public MMenu, public MMessageObserver {
public:
    virtual ~DinoMultiPlayScanner();
    virtual void OnStart();
    virtual void OnDecide(MMenuItem *item);
    virtual void OnIdle();
    virtual void OnMessage(MMessage *msg);

    u32 mUnkE8[3];      // 0xE8
    MRandom mRandom;    // 0xF4
};

class DinoMultiPlayScanTelop : public MProcess {
public:
    virtual ~DinoMultiPlayScanTelop();
    virtual void Idle();

    u32 mUnk74;         // 0x74
    MFont *mFont;       // 0x78
};

#endif
