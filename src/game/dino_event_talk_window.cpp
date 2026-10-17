// DinoEventTalkWindow: the window event text is shown in.
// Only the background-call template it instantiates is here so far.

#include "game/dino_event_talk_window.h"
#include "m/bg_new_task.h"
#include "game/dino_window_choice.h"

// Emitted in this file in the game (the window starts background loads with MCreateBgCallTask); until the code
// that makes them is decompiled, the instances are made explicitly.
template class MBgCallTask0<DinoEventTalkWindow>;
template MTask *MCreateBgCallTask(DinoEventTalkWindow *obj, void (DinoEventTalkWindow::*func)(), int thread);
template class std::__vec_deleter<DinoWindowChoice, std::allocator<DinoWindowChoice> >;
template class std::__vec_deleter<std::string, std::allocator<std::string> >;
