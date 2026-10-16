// The help system.

#include <algorithm>
#include "game/help.h"
#include "game/dino_help_sign.h"
#include "m/input.h"
#include "m/irq.h"

static DinoHelpDriver *sHelpDriver;
static DinoHelpBase *sHelpCurrentBase;
static OSMutex sHelpMutex;

DinoHelpHook::DinoHelpHook() {
    mActive = FALSE;
    sHelpDriver->AddHook(this);
}

DinoHelpHook::~DinoHelpHook() {
    sHelpDriver->RemoveHook(this);
}

BOOL DinoHelpHook::IsActive() {
    return mActive;
}

void DinoHelpHook::Activate() {
    mActive = TRUE;
}

void DinoHelpHook::Deactivate() {
    mActive = FALSE;
}

DinoHelpDriver::DinoHelpDriver(MTask *cursor) : MProcess(NULL) {
    mCursor = cursor;
    mEnableCount = 1;
    sHelpDriver = this;
    MSingleInputHub *hub = new MSingleInputHub(1);
    hub->EnableTouch();
    mInput = hub->GetInput(0);
    mParallel.AddTail(hub);
    mTasks = new MParallelTaskSet();
    mTasks->AddTail(mCursor);
    mParallel.AddTail(mTasks);
}

DinoHelpDriver::~DinoHelpDriver() {
    // Every base must be gone before the driver.
    if (mBases.size() != 0) {
        OS_Terminate();
    }
    sHelpDriver = NULL;
}

void DinoHelpDriver::AddBase(DinoHelpBase *base) {
    MScopedLock lock(&sHelpMutex, TRUE);
    mBases.push_back(base);
    sHelpCurrentBase = base;
}

void DinoHelpDriver::RemoveBase(DinoHelpBase *base) {
    MScopedLock lock(&sHelpMutex, TRUE);
    std::list<DinoHelpBase *>::iterator it = std::find(mBases.begin(), mBases.end(), base);
    if (it == mBases.end()) {
        OS_Terminate();
    }
    mBases.erase(it);
    sHelpCurrentBase = mBases.size() == 0 ? NULL : mBases.back();
}

BOOL DinoHelpDriver::HasBase(DinoHelpBase *base) {
    OSMutex *mutex = &sHelpMutex;
    OS_LockMutex(mutex);
    BOOL found = std::find(mBases.begin(), mBases.end(), base) != mBases.end();
    OS_UnlockMutex(mutex);
    return found;
}

void DinoHelpDriver::AddItem(DinoHelpItem *item) {
    MScopedLock lock(&sHelpMutex, TRUE);
    mItems.push_back(item);
}

void DinoHelpDriver::RemoveItem(DinoHelpItem *item) {
    OS_LockMutex(&sHelpMutex);
    mItems.erase(std::find(mItems.begin(), mItems.end(), item));
    // Drop the entries linking to or from the item.
    std::multimap<u32, std::pair<u32, u32> >::iterator it = mEntries.begin();
    while (it != mEntries.end()) {
        if (it->first == (u32) item || it->second.second == (u32) item) {
            mEntries.erase(it);
            it = mEntries.begin();
        } else {
            ++it;
        }
    }
    OS_UnlockMutex(&sHelpMutex);
}

void DinoHelpDriver::AddHook(DinoHelpHook *hook) {
    MScopedLock lock(&sHelpMutex, TRUE);
    mHooks.push_back(hook);
}

void DinoHelpDriver::RemoveHook(DinoHelpHook *hook) {
    OSMutex *mutex = &sHelpMutex;
    OS_LockMutex(mutex);
    mHooks.erase(std::find(mHooks.begin(), mHooks.end(), hook));
    OS_UnlockMutex(mutex);
}

void DinoHelpDriver::NotifyOpen() {
    MScopedLock lock(&sHelpMutex, TRUE);
    for (std::list<DinoHelpHook *>::iterator it = mHooks.begin(); it != mHooks.end(); it++) {
        if ((*it)->IsActive()) {
            (*it)->Unk08();
        }
    }
}

void DinoHelpDriver::NotifyClose() {
    MScopedLock lock(&sHelpMutex, TRUE);
    for (std::list<DinoHelpHook *>::iterator it = mHooks.begin(); it != mHooks.end(); it++) {
        if ((*it)->IsActive()) {
            (*it)->Unk0C();
        }
    }
}

void DinoHelpDriver::AddEntry(u32 a, u32 b, u32 c) {
    MScopedLock lock(&sHelpMutex, TRUE);
    mEntries.insert(std::make_pair(a, std::make_pair(b, c)));
}

void DinoHelpDriver::RemoveEntry(u32 a, u32 b) {
    OS_LockMutex(&sHelpMutex);
    for (std::multimap<u32, std::pair<u32, u32> >::iterator it = mEntries.begin(); it != mEntries.end(); ++it) {
        if (it->first == a && b == it->second.first) {
            mEntries.erase(it);
            OS_UnlockMutex(&sHelpMutex);
            return;
        }
    }
    OS_UnlockMutex(&sHelpMutex);
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

DinoHelpDummyItem::DinoHelpDummyItem(u32 id, MRect rect, std::string text)
    : mHelpId(id), mRect(rect), mText(text) {
    mTextPtr = mText.c_str();
}

u32 DinoHelpDummyItem::GetHelpId() {
    return mHelpId;
}

MRect DinoHelpDummyItem::GetRect() {
    return mRect;
}

void DinoHelpDummyItem::SetText(const std::string &text) {
    mText = text;
    mTextPtr = mText.c_str();
}

const char *DinoHelpDummyItem::GetText() {
    return mTextPtr;
}

void DinoHelpDummyItem::Execute() {
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
