// DinoFieldPlayerControl.
// Only the destructor is decompiled so far.

#include "game/dino_field_player_control.h"
#include "m/call_task.h"
#include "m/fx32.h"

DinoFieldPlayerControl::~DinoFieldPlayerControl() {
    StoreResult();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
// NONMATCHING (the creator only): we copy the MFx32 argument to a stack temporary; the game doesn't (same as the
// DinoField picture-load creators). An aggregate MFx32 fixes it but breaks MFx32 returns (hidden pointer).
template class MCallTask4<DinoFieldPlayerControl, DinoFieldPlayerControl, MFx32Point, MFx32, u16, u32>;
template MTask *MCreateCallTask(DinoFieldPlayerControl *obj, void (DinoFieldPlayerControl::*func)(MFx32Point, MFx32, u16, u32), MFx32Point a0, MFx32 a1, u16 a2, u32 a3);
