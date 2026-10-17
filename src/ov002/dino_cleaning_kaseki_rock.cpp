// DinoCleaningKasekiRock, DinoCleaningKasekiRockXrayBlendManager.
// Only the destructors are decompiled so far.

#include "game/dino_cleaning_kaseki_rock.h"
#include "game/dino_cleaning_kaseki_rock_xray_blend_manager.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoCleaningKasekiRock::~DinoCleaningKasekiRock() {
}

DinoCleaningKasekiRockXrayBlendManager::~DinoCleaningKasekiRockXrayBlendManager() {
}

u32 DinoCleaningKasekiRock::GetUnkDC() {
    return mUnkDC;
}

u32 DinoCleaningKasekiRock::GetUnkE0() {
    return mUnkE0;
}

u32 DinoCleaningKasekiRock::GetUnkD4() {
    return mUnkD4;
}

u32 DinoCleaningKasekiRock::GetUnkD8() {
    return mUnkD8;
}

u32 DinoCleaningKasekiRock::GetUnkE4() {
    return mUnkE4;
}

void DinoCleaningKasekiRock::SetUnk128(BOOL value) {
    mUnk128 = value;
}

void DinoCleaningKasekiRockXrayBlendManager::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoCommonLive {};
class DinoCleaningLive : public DinoCommonLive {};
class DinoCleaning : public DinoHelpBase {};
template class MCallTask2<DinoCleaningLive, DinoCommonLive, u32, bool>;
template MTask *MCreateCallTask(DinoCleaningLive *obj, void (DinoCommonLive::*func)(u32, bool), u32 a0, bool a1);
template class MCallTask0<DinoCleaning, DinoHelpBase>;
template MTask *MCreateCallTask(DinoCleaning *obj, void (DinoHelpBase::*func)());
template class MBgCallTask0<DinoCleaning>;
template MTask *MCreateBgCallTask(DinoCleaning *obj, void (DinoCleaning::*func)(), int thread);
