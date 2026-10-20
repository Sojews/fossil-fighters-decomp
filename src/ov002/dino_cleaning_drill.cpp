// DinoCleaningDrill.
// Only the destructors are decompiled so far.

#include "game/dino_cleaning_drill.h"
#include "game/dino_cleaning.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_static_init.h"
#include "m/fx32.h"

#pragma auto_inline off
// The TU's pooled constants.
struct DinoCleaningDrillPool {
    u8 unk00[8];
    const char *markPath;   // 0x08
    u8 unk0C[4];
    const char *rowPath;    // 0x10
    u8 unk14[0xC];
    const char *columnPath; // 0x20
};
extern "C" u32 data_ov002_021651f8[];
#define sDrillPool (*(DinoCleaningDrillPool *)data_ov002_021651f8)

void DinoCleaningDrill::LoadGuides() {
    mUnk84 = 0;
    mRowGuide = new MMotion(std::string(sDrillPool.rowPath), (MSmartOam *)GetCleaning()->GetUnkD4(), 0, 1);
    mRowGuide->Play(1, 0);
    mRowGuide->SetPriority(2);
    mColumnGuide = new MMotion(std::string(sDrillPool.columnPath), (MSmartOam *)GetCleaning()->GetUnkD4(), 0, 1);
    mColumnGuide->Play(1, 0);
    mColumnGuide->SetPriority(2);
    mMark = new MMotion(std::string(sDrillPool.markPath), (MSmartOam *)GetCleaning()->GetUnkD4(), 0, 1);
    mMark->SetPriority(2);
    mMark->SetAnchor(1);
    mMark->Play(1, 0);
    GetParallel()->AddTail(mColumnGuide, mRowGuide, mMark);
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

// The file's static initialization (see dino_static_init.h), with the drill's fixed-point constants.
// NONMATCHING: the screen rect sits past the +0x80 anchor and the game hoists its zero half.
#pragma always_inline on
static struct DinoCleaningDrillStaticInit {
    DinoCleaningDrillStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_021651f8, 0x3c, 0xc0, 0x4c, 0x34, 0x90, 0xa0, 0xa8, 0xb0);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x4) = MFx32::FromRaw(0x666);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x28) = MFx32::FromRaw(0x333);
        DINO_POOL(MFx32, data_ov002_021651f8, 0xc) = MFx32(24);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x1c) = MFx32(8);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x18) = MFx32::FromRaw(0x333);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x14) = MFx32(6);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x24) = MFx32::FromRaw(0x800);
    }
} sDinoCleaningDrillStaticInit;
