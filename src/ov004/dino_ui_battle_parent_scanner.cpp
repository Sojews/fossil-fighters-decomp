// DinoUiBattleParentScanner.
// Only the template task instances it emits are here so far.

#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/observer.h"

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiBattleParentScanner : public MProcess, public MMessageObserver {};
template class MCallTask0<DinoUiBattleParentScanner, MMessageObserver>;
template MTask *MCreateCallTask(DinoUiBattleParentScanner *obj, void (MMessageObserver::*func)());
