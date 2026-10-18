// DinoFieldTownMap, TownMapIcon.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#pragma ipa file

#include "game/dino_field_town_map_menu.h"
#include "game/dino_field_town_map.h"
#include "game/town_map_icon.h"
#include "game/dino_field.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/fx32.h"
#include "game/dino_field_player_control.h"
#include "m/lcd.h"

DinoFieldTownMap::~DinoFieldTownMap() {
}

TownMapIcon::~TownMapIcon() {
}

DinoFieldTownMapManager::~DinoFieldTownMapManager() {
}

void TownMapIcon::Idle() {
}

DinoFieldDance::~DinoFieldDance() {
}

void DinoFieldDance::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask2<DinoField, DinoField, MFx32Point, long>;
template MTask *MCreateCallTask(DinoField *obj, void (DinoField::*func)(MFx32Point, long), MFx32Point a0, long a1);

void DinoFieldTownMapManager::CameraUnk0(u32 map) {
    if (mTownMap) {
        mParallel.Remove(mTownMap);
        delete mTownMap;
        mTownMap = NULL;
    }
}

#pragma auto_inline off

DinoFieldTownMapManager::DinoFieldTownMapManager(DinoField *field) : MProcess(gLcd->GetTopDisplay()) {
    mField = field;
    mTownMap = NULL;
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
// NONMATCHING (the creator only): we copy the MFx32 argument to a stack temporary; the game doesn't (same as the
// DinoField picture-load creators). An aggregate MFx32 fixes it but breaks MFx32 returns (hidden pointer).
template class MCallTask4<DinoFieldPlayerControl, DinoFieldPlayerControl, MFx32Point, MFx32, u16, u32>;
template MTask *MCreateCallTask(DinoFieldPlayerControl *obj, void (DinoFieldPlayerControl::*func)(MFx32Point, MFx32, u16, u32), MFx32Point a0, MFx32 a1, u16 a2, u32 a3);
