#ifndef GAME_DINO_UI_REVIVE_STATUS_H
#define GAME_DINO_UI_REVIVE_STATUS_H

#include "m/task.h"
#include "game/dino_ui_rv_information.h"

class DinoUiReviveStatus : public MProcess {
public:
    DinoUiReviveStatus(MDisplay *display, const DinoCreatureInfo &info, u8 flag);
    virtual ~DinoUiReviveStatus();
    virtual void Idle();

    void (DinoUiReviveStatus::*mState)(); // 0x74
    DinoCreatureInfo mInfo;     // 0x7C
    u8 unk94[0xC4 - 0x94];
    u8 mFlag;                   // 0xC4
};

#endif
