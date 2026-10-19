// DinoCleaningDrill.
// Only the destructors are decompiled so far.

#include "game/dino_cleaning_drill.h"
#include "game/dino_cleaning.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

#pragma auto_inline off
// The TU's pooled constants.
extern "C" struct {
    u8 unk00[8];
    const char *markPath;   // 0x08
    u8 unk0C[4];
    const char *rowPath;    // 0x10
    u8 unk14[0xC];
    const char *columnPath; // 0x20
} data_ov002_021651f8;

static inline void AddTasks(MTaskSet *set, MTask *a, MTask *b, MTask *c) {
    set->AddTail(a);
    set->AddTail(b);
    set->AddTail(c);
}

// NONMATCHING: the game keeps &mParallel in a register for the three adds (ours rematerializes it).
void DinoCleaningDrill::LoadGuides() {
    mUnk84 = 0;
    mRowGuide = new MMotion(std::string(data_ov002_021651f8.rowPath), (MSmartOam *)GetCleaning()->GetUnkD4(), 0, 1);
    mRowGuide->Play(1, 0);
    mRowGuide->SetPriority(2);
    mColumnGuide = new MMotion(std::string(data_ov002_021651f8.columnPath), (MSmartOam *)GetCleaning()->GetUnkD4(), 0, 1);
    mColumnGuide->Play(1, 0);
    mColumnGuide->SetPriority(2);
    mMark = new MMotion(std::string(data_ov002_021651f8.markPath), (MSmartOam *)GetCleaning()->GetUnkD4(), 0, 1);
    mMark->SetPriority(2);
    mMark->SetAnchor(1);
    mMark->Play(1, 0);
    AddTasks(&mParallel, mColumnGuide, mRowGuide, mMark);
}

void DinoCleaningDrill::Unk1C() {
    mParallel.Clear();
}

void DinoCleaningDrill::ShowGuides() {
    mRowGuide->Show();
    mColumnGuide->Show();
    mMark->Show();
}

void DinoCleaningDrill::HideGuides() {
    mRowGuide->Hide();
    mColumnGuide->Hide();
    mMark->Hide();
}

void DinoCleaningDrill::SetGuidePos(MPoint pos) {
    mRowGuide->SetPosition(0, pos.y);
    mColumnGuide->SetPosition(pos.x, 0);
    mMark->SetPosition(pos);
}
#pragma auto_inline on

DinoCleaningDrill::~DinoCleaningDrill() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoCleaningDrill>;
template MTask *MCreateBgCallTask(DinoCleaningDrill *obj, void (DinoCleaningDrill::*func)(), int thread);
