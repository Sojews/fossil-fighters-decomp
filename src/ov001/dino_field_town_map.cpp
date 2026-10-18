// DinoFieldTownMap, TownMapIcon.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_field_town_map_menu.h"
#include "game/dino_field_town_map.h"
#include "game/town_map_icon.h"
#include "game/dino_field.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/fx32.h"

DinoFieldTownMap::~DinoFieldTownMap() {
}

TownMapIcon::~TownMapIcon() {
}

DinoFieldTownMapManager::~DinoFieldTownMapManager() {
}

void TownMapIcon::Idle() {
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
