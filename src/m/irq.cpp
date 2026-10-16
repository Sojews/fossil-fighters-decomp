// The task sets run in interrupts.

#include "m/irq.h"
#include "m/task.h"

extern "C" void GX_VBlankIntr(BOOL enable);
extern "C" void GX_HBlankIntr(BOOL enable);

// By interrupt number; only VBlank, HBlank, VCount, keypad and cartridge interrupts have one.
static MTaskSet *sIntrTaskSets[14];

static void HBlankIntr();
static void VBlankIntr();
static void VCountIntr();
static void KeyIntr();
static void CartridgeIntr();

void MInitIrq() {
    OS_InitIrqTable();
    sIntrTaskSets[1] = new MParallelTaskSet();
    OS_SetIrqFunction(OS_IE_H_BLANK, HBlankIntr);
    sIntrTaskSets[0] = new MParallelTaskSet();
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    sIntrTaskSets[2] = new MParallelTaskSet();
    OS_SetIrqFunction(OS_IE_V_COUNT, VCountIntr);
    sIntrTaskSets[12] = new MParallelTaskSet();
    OS_SetIrqFunction(OS_IE_KEY, KeyIntr);
    sIntrTaskSets[13] = new MParallelTaskSet();
    OS_SetIrqFunction(OS_IE_CARTRIDGE, CartridgeIntr);
    MEnableIrqs();
}

void MAddIntrTask(int intr, MTask *task) {
    MIrqLock lock;
    sIntrTaskSets[intr]->AddTail(task);
}

void MRemoveIntrTask(int intr, MTask *task) {
    MIrqLock lock;
    sIntrTaskSets[intr]->Remove(task);
}

void MEnableIrqs() {
    u16 prev = REG_IME;
    REG_IME = 1;
}

void MEnableIntr(int intr) {
    OS_EnableIrqMask(1 << intr);
    switch (intr) {
    case 0:
        GX_VBlankIntr(TRUE);
        break;
    case 1:
        GX_HBlankIntr(TRUE);
        break;
    }
}

#pragma section ITCM begin
#pragma thumb off

static void HBlankIntr() {
    MTaskSet *set = sIntrTaskSets[1];
    if (set->mState == MTask::STATE_RUN) {
        set->Execute();
    }
    OS_SetIrqCheckFlag(OS_IE_H_BLANK);
}

static void VBlankIntr() {
    MTaskSet *set = sIntrTaskSets[0];
    if (set->mState == MTask::STATE_RUN) {
        set->Execute();
    }
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

static void VCountIntr() {
    MTaskSet *set = sIntrTaskSets[2];
    if (set->mState == MTask::STATE_RUN) {
        set->Execute();
    }
    OS_SetIrqCheckFlag(OS_IE_V_COUNT);
}

static void KeyIntr() {
    MTaskSet *set = sIntrTaskSets[12];
    if (set->mState == MTask::STATE_RUN) {
        set->Execute();
    }
    OS_SetIrqCheckFlag(OS_IE_KEY);
}

static void CartridgeIntr() {
    MTaskSet *set = sIntrTaskSets[13];
    if (set->mState == MTask::STATE_RUN) {
        set->Execute();
    }
    OS_SetIrqCheckFlag(OS_IE_CARTRIDGE);
}

#pragma thumb on
#pragma section ITCM end
