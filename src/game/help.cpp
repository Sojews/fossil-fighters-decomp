// The help system.

#include "game/help.h"
#include "game/dino_help_sign.h"
#include "m/irq.h"

static DinoHelpBase *sHelpCurrentBase;
static DinoHelpDriver *sHelpDriver;

DinoHelpHook::DinoHelpHook() {
    mUnk4 = FALSE;
    sHelpDriver->AddHook(this);
}

DinoHelpHook::~DinoHelpHook() {
    sHelpDriver->RemoveHook(this);
}

void DinoHelpDriver::Enable() {
    MIrqLock lock;
    mEnableCount++;
}

void DinoHelpDriver::Disable() {
    MIrqLock lock;
    mEnableCount--;
}

DinoHelpBase::DinoHelpBase() {
    mEnabled = TRUE;
    mSelectedItem = NULL;
    if (sHelpDriver == NULL) {
        OS_Terminate();
    }
    sHelpDriver->AddBase(this);
}

DinoHelpBase::~DinoHelpBase() {
    sHelpDriver->RemoveBase(this);
}

void DinoHelpBase::Enable() {
    mEnabled = TRUE;
}

void DinoHelpBase::Disable() {
    mEnabled = FALSE;
}

BOOL DinoHelpBase::IsEnabled() {
    return mEnabled;
}

void DinoHelpBase::SetSelectedItem(DinoHelpItem *item) {
    mSelectedItem = item;
}

void DinoHelpBase::AddEntry(u32 a, u32 b, u32 c) {
    sHelpDriver->AddEntry(a, b, c);
}

void DinoHelpBase::RemoveEntry(u32 a, u32 b) {
    sHelpDriver->RemoveEntry(a, b);
}

MTask *DinoHelpBase::CreateEnableTask(DinoHelpBase *base) {
    return new EnableHelpTask(base, true);
}

DinoHelpItem::DinoHelpItem() {
    mEnabled = TRUE;
    if (sHelpCurrentBase == NULL) {
        OS_Terminate();
    }
    mBase = sHelpCurrentBase;
    sHelpDriver->AddItem(this);
}

DinoHelpItem::~DinoHelpItem() {
    sHelpDriver->RemoveItem(this);
}

void DinoHelpItem::Enable() {
    mEnabled = TRUE;
}

void DinoHelpItem::Disable() {
    mEnabled = FALSE;
}

BOOL DinoHelpItem::IsEnabled() {
    return mEnabled;
}

void DinoHelpItem::Select() {
    if (!sHelpDriver->HasBase(mBase)) {
        OS_Terminate();
    }
    mBase->SetSelectedItem(this);
}

DinoHelpDisabler::DinoHelpDisabler() {
    sHelpDriver->Disable();
}

DinoHelpDisabler::~DinoHelpDisabler() {
    sHelpDriver->Enable();
}

void EnableHelpTask::Execute() {
    if (mEnable) {
        mBase->Enable();
    } else {
        mBase->Disable();
    }
    Exit();
}

DinoHelpSign::~DinoHelpSign() {
}

EnableHelpTask::~EnableHelpTask() {
}
