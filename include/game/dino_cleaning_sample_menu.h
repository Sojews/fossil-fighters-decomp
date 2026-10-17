#ifndef GAME_DINO_CLEANING_SAMPLE_MENU_H
#define GAME_DINO_CLEANING_SAMPLE_MENU_H

#include "m/view.h"
#include "m/archive.h"
#include "m/font.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoCleaningSampleMenu : public MMenu {
public:
    virtual ~DinoCleaningSampleMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);

    u32 mUnkA8[4];      // 0xA8
    MFont *mFont;       // 0xB8
};

class DinoCleaningSampleMenuKasekiRockIdItem : public MMenuItem {
public:
    virtual ~DinoCleaningSampleMenuKasekiRockIdItem();
    virtual void Idle();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    u32 mUnk88[2];
    MView *mView;           // 0x90
    u32 mUnk94;
    MMenuItem *mKeyItemA; // 0x98
    MMenuItem *mKeyItemB; // 0x9C
    MArchivePtr mArchive;   // 0xA0
    MArchiveFile mFile;     // 0xA4
    u8 *mBuffer;            // 0xB0
};

#endif
