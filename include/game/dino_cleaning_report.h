#ifndef GAME_DINO_CLEANING_REPORT_H
#define GAME_DINO_CLEANING_REPORT_H

#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoCleaningReport : public MProcess {
public:
    virtual ~DinoCleaningReport();
    virtual void Idle();

    void (DinoCleaningReport::*mState)(); // 0x74
    u32 mUnk7C[3];          // 0x7C
    MArchivePtr mArchive;   // 0x88
    MArchiveFile mFile;     // 0x8C
};

#endif
