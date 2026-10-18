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
#include "game/dino_resource.h"

class DinoCleaningReport : public MProcess {
public:
    DinoCleaningReport(u32 arg);
    virtual ~DinoCleaningReport();
    virtual void Idle();

    void State02155b28();

    void (DinoCleaningReport::*mState)(); // 0x74
    u32 mUnk7C;             // 0x7C
    u32 unk80[2];
    MResource<DinoMessageData> mMessages;   // 0x88: msg/msg_cleaning
    u32 unk98;
};

#endif
