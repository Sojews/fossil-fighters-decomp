// DinoEventTalkWindow: the window event text is shown in.
// Only the background-call template it instantiates is here so far.

#pragma ipa file

#include "game/dino_event_talk_window.h"
#include "m/bg_new_task.h"
#include "game/dino_window_choice.h"

DinoEventTalkWindow::~DinoEventTalkWindow() {
}

void DinoEventTalkWindow::SetYesNo(u32 arg, MPoint pos, u32 unkB4, const std::string &a, const std::string &b,
                                   const std::string &c, const std::string &d) {
    mUnk98 = arg;
    mPos = pos;
    mUnkB4 = unkB4;
    mUnkBC = a;
    mUnkC8 = b;
    mUnkD4 = c;
    mUnkE0 = d;
}

void DinoEventTalkWindow::SetChoices(u32 arg, MPoint pos, const std::vector<DinoWindowChoice> &choices, u32 unkB4) {
    mUnk9C = arg;
    mPos = pos;
    mChoices = choices;
    mUnkB4 = unkB4;
}

void DinoEventTalkWindow::AddEntry(u32 id, MRect rect, const std::string &text) {
    DinoCommonWindowMenuEntry entry;
    entry.id = id;
    entry.rect = rect;
    entry.text = text;
    mEntries.push_back(entry);
}

void DinoEventTalkWindow::Idle() {
    (this->*mState)();
}

// Emitted in this file in the game (the window starts background loads with MCreateBgCallTask); until the code
// that makes them is decompiled, the instances are made explicitly.
template class MBgCallTask0<DinoEventTalkWindow>;
template MTask *MCreateBgCallTask(DinoEventTalkWindow *obj, void (DinoEventTalkWindow::*func)(), int thread);
template class std::__vec_deleter<DinoWindowChoice, std::allocator<DinoWindowChoice> >;
template class std::__vec_deleter<std::string, std::allocator<std::string> >;
template class std::__vec_deleter<DinoCommonWindowMenuEntry, std::allocator<DinoCommonWindowMenuEntry> >;
