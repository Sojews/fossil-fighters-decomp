#ifndef GAME_DINO_FIELD_LOWER_SPOT_H
#define GAME_DINO_FIELD_LOWER_SPOT_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MRasterWindow;
class MTextPicture;

// Closes (Out) or opens (In) a spotlight on the bottom screen over a black picture.
class DinoFieldLowerSpotIn : public MProcess {
public:
    DinoFieldLowerSpotIn(int duration);
    virtual ~DinoFieldLowerSpotIn();
    virtual void Idle();

    MRasterWindow *mWindow;     // 0x74
    u32 mDuration;              // 0x78
    u32 mFrame;                 // 0x7C
    u32 mStep;                  // 0x80
    MTextPicture *mBlack;       // 0x84
};

class DinoFieldLowerSpotOut : public MProcess {
public:
    DinoFieldLowerSpotOut(int duration);
    virtual ~DinoFieldLowerSpotOut();
    virtual void Idle();

    MRasterWindow *mWindow;     // 0x74
    u32 mDuration;              // 0x78
    u32 mFrame;                 // 0x7C
    u32 mStep;                  // 0x80
    MTextPicture *mBlack;       // 0x84
};

#endif
