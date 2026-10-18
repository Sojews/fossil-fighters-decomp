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
    void State0215670c();
    void State02156808();
    // Shows message id (formatted with arg) with the choices, the answer going to result.
    void ShowMessage(u32 id, const std::string &arg, u32 *result, const std::string &a, const std::string &b,
                     const std::string &c, const std::string &d);

    void (DinoCleaningReport::*mState)(); // 0x74
    u32 mUnk7C;             // 0x7C
    u32 unk80[2];
    MResource<DinoMessageData> mMessages;   // 0x88: msg/msg_cleaning
    u32 mAnswer;            // 0x98
};

#endif
