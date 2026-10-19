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
#include "m/multi_play.h"

class DinoMultiPlayScanner : public DinoHelpBase, public MMenu, public MMessageObserver {
public:
    DinoMultiPlayScanner(MMultiPlay *multiPlay, u8 *result);
    virtual ~DinoMultiPlayScanner();
    virtual void OnStart();
    virtual void OnDecide(MMenuItem *item);
    virtual void OnIdle();
    virtual void OnMessage(MMessage *msg);

    void Start();
    void Reset();
    void Cancel();

    MMultiPlay *mMultiPlay; // 0xE8
    int mMode;          // 0xEC: 1: parent, else child
    u8 *mResult;        // 0xF0
    MRandom mRandom;    // 0xF4
    MMenuItem *mCancel; // 0xFC
    int mWait;          // 0x100: frames before trying the other side
    u8 mFound;          // 0x104: a partner was found
};

// The "searching" text scrolling across the bottom screen.
class DinoMultiPlayScanTelop : public MProcess {
public:
    DinoMultiPlayScanTelop(MSmartOam *oam);
    virtual ~DinoMultiPlayScanTelop();
    virtual void Idle();

    MView *mView;       // 0x74
    MFont *mFont;       // 0x78
    MPoint mFrom;       // 0x7C
    MPoint mTo;         // 0x84
};

#endif
