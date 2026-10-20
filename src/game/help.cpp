// The help system.

#define MSL_STRING_PLUS_DEFINITIONS
#pragma ipa file
#include <algorithm>
#include "game/help.h"
#include "game/dino_help_sign.h"
#include "m/input.h"
#include "game/dino_sound.h"
#include "m/call_task.h"
#include "m/irq.h"
#include "m/lcd.h"
#include "m/oam_canvas.h"
#include "m/blend.h"
#include "m/smart_oam.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_resource.h"
#include "m/font.h"

static DinoHelpBase *sHelpCurrentBase;
static DinoHelpDriver *sHelpDriver;
static MMutex sHelpMutex;

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

DinoHelpDriver::DinoHelpDriver(MTask *cursor) : MProcess(NULL), mCursor(cursor), mEnableCount(1) {
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
    MScopedLock lock(&sHelpMutex.mutex, TRUE);
    mBases.push_back(base);
    sHelpCurrentBase = base;
}

void DinoHelpDriver::RemoveBase(DinoHelpBase *base) {
    MScopedLock lock(&sHelpMutex.mutex, TRUE);
    std::list<DinoHelpBase *>::iterator it = std::find(mBases.begin(), mBases.end(), base);
    if (it == mBases.end()) {
        OS_Terminate();
    }
    mBases.erase(it);
    sHelpCurrentBase = mBases.size() == 0 ? NULL : mBases.back();
}

BOOL DinoHelpDriver::HasBase(DinoHelpBase *base) {
    OSMutex *mutex = &sHelpMutex.mutex;
    OS_LockMutex(mutex);
    BOOL found = std::find(mBases.begin(), mBases.end(), base) != mBases.end();
    OS_UnlockMutex(mutex);
    return found;
}

void DinoHelpDriver::AddItem(DinoHelpItem *item) {
    MScopedLock lock(&sHelpMutex.mutex, TRUE);
    mItems.push_back(item);
}

void DinoHelpDriver::RemoveItem(DinoHelpItem *item) {
    OS_LockMutex(&sHelpMutex.mutex);
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
    OS_UnlockMutex(&sHelpMutex.mutex);
}

void DinoHelpDriver::AddHook(DinoHelpHook *hook) {
    MScopedLock lock(&sHelpMutex.mutex, TRUE);
    mHooks.push_back(hook);
}

void DinoHelpDriver::RemoveHook(DinoHelpHook *hook) {
    MScopedLock lock(&sHelpMutex.mutex, TRUE);
    mHooks.erase(std::find(mHooks.begin(), mHooks.end(), hook));
}

void DinoHelpDriver::NotifyOpen() {
    MScopedLock lock(&sHelpMutex.mutex, TRUE);
    for (std::list<DinoHelpHook *>::iterator it = mHooks.begin(); it != mHooks.end(); it++) {
        if ((*it)->IsActive()) {
            (*it)->Unk08();
        }
    }
}

void DinoHelpDriver::NotifyClose() {
    MScopedLock lock(&sHelpMutex.mutex, TRUE);
    for (std::list<DinoHelpHook *>::iterator it = mHooks.begin(); it != mHooks.end(); it++) {
        if ((*it)->IsActive()) {
            (*it)->Unk0C();
        }
    }
}

// Whether there is help to show: help is on, the current base is enabled, neither screen is faded and one of the
// base's enabled items has text.
// NONMATCHING: the game keeps copies of the list iterators on the stack.
BOOL DinoHelpDriver::CanShowHelp() {
    MScopedLock lock(&sHelpMutex.mutex, TRUE);
    if (mEnableCount <= 0) {
        return FALSE;
    }
    if (sHelpCurrentBase == NULL) {
        return FALSE;
    }
    if (!sHelpCurrentBase->IsEnabled()) {
        return FALSE;
    }
    if ((gLcd->GetTopDisplay()->mBright->GetMode() != 0 && gLcd->GetTopDisplay()->mBright->GetFactor() != 0) ||
        (gLcd->GetBottomDisplay()->mBright->GetMode() != 0 && gLcd->GetBottomDisplay()->mBright->GetFactor() != 0)) {
        return FALSE;
    }
    for (std::list<DinoHelpItem *>::iterator it = mItems.begin(); it != mItems.end(); it++) {
        if ((*it)->mBase == sHelpCurrentBase && (*it)->IsEnabled() && (*it)->GetText() != NULL &&
            (*it)->GetText()[0] != '\0') {
            return TRUE;
        }
    }
    return FALSE;
}

void DinoHelpDriver::AddEntry(u32 a, u32 b, u32 c) {
    MScopedLock lock(&sHelpMutex.mutex, TRUE);
    mEntries.insert(std::multimap<u32, std::pair<u32, u32> >::value_type(a, std::make_pair(b, c)));
}

void DinoHelpDriver::RemoveEntry(u32 a, u32 b) {
    OS_LockMutex(&sHelpMutex.mutex);
    for (std::multimap<u32, std::pair<u32, u32> >::iterator it = mEntries.begin(); it != mEntries.end(); ++it) {
        if (a == it->first && b == it->second.first) {
            mEntries.erase(it);
            OS_UnlockMutex(&sHelpMutex.mutex);
            return;
        }
    }
    OS_UnlockMutex(&sHelpMutex.mutex);
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

DinoHelpDummyItem::DinoHelpDummyItem(MDisplay *display, MRect rect, std::string text)
    : mDisplay(display), mRect(rect), mText(text) {
    mTextPtr = mText.c_str();
}

MDisplay *DinoHelpDummyItem::GetDisplay() {
    return mDisplay;
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

void DinoHelpCursor::Execute() {
    if (mTasks.mState == STATE_RUN) {
        mTasks.Execute();
    }
}

void DinoHelpCursor::SetPosition(MPoint pos) {
    mFrame.MoveTo(pos);
    Layout();
}

MPoint DinoHelpCursor::GetPosition() {
    return MPoint(mFrame.left, mFrame.top);
}

void DinoHelpCursor::SetSize(MPoint size) {
    mFrame.right = mFrame.left + size.x;
    mFrame.bottom = mFrame.top + size.y;
    Layout();
}

MPoint DinoHelpCursor::GetSize() {
    return MPoint(mFrame.GetWidth(), mFrame.GetHeight());
}

void DinoHelpCursor::Layout() {
    mCorners[0]->SetPosition(MPoint(mFrame.left, mFrame.top));
    mCorners[1]->SetPosition(MPoint(mFrame.left, mFrame.bottom));
    mCorners[2]->SetPosition(MPoint(mFrame.right, mFrame.top));
    mCorners[3]->SetPosition(MPoint(mFrame.right, mFrame.bottom));
}

void DinoHelpCursor::SetPriority(int priority) {
    for (u32 i = 0; i < 4; i++) {
        mCorners[i]->SetPriority(priority);
    }
}

int DinoHelpCursor::GetPriority() {
    return mCorners[0]->GetPriority();
}

void DinoHelpCursor::Show() {
    for (u32 i = 0; i < 4; i++) {
        mCorners[i]->Show();
    }
}

void DinoHelpCursor::Hide() {
    for (u32 i = 0; i < 4; i++) {
        mCorners[i]->Hide();
    }
}

BOOL DinoHelpCursor::IsHidden() {
    return mCorners[0]->IsHidden();
}

u32 DinoHelpCursor::GetPlaneMask() {
    return 0x10;
}

void DinoHelpCursor::SetAnchor(int anchor) {
    for (u32 i = 0; i < 4; i++) {
        mCorners[i]->SetAnchor(anchor);
    }
}

// How far the top screen is from the bottom one, in the help's coordinates.
extern const int kHelpTopScreenOffset;

DinoHelpSign::DinoHelpSign(DinoHelpItem *item) {
    mItem = item;
}

DinoHelpItem *DinoHelpSign::GetItem() {
    return mItem;
}

void DinoHelpSign::OnDecide() {
}

MRect DinoHelpSign::GetFrame() {
    if (mItem->GetDisplay() == gLcd->GetTopDisplay()) {
        MRect frame = mItem->GetRect();
        frame.top += kHelpTopScreenOffset;
        frame.bottom += kHelpTopScreenOffset;
        return frame;
    }
    return mItem->GetRect();
}

// The status bar's fill colour.
extern const int kHelpStatusColor;

// The TU's pooled constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0xC];
    const char *mMotionPath;    // 0x0C
    const char *mCursorPath;    // 0x10
    u8 unk14[0x1C - 0x14];
    void (DinoHelpStatus::*Scroll_1C)();
    void (DinoHelpStatus::*OpenStep_24)();
    u8 unk2C[0x4C - 0x2C];
    void (DinoHelpStatus::*CloseStep_4C)();
    u8 unk54[0x5C - 0x54];
    void (DinoHelpStatus::*Start_5C)();
} data_020ba91c;

DinoHelpStatus::DinoHelpStatus(MSmartOam *oam)
    : MProcess(NULL), mOam(oam), mCanvas(NULL), mMotion(data_020ba91c.mMotionPath, 2) {
    mState = data_020ba91c.Start_5C;
}

DinoHelpStatus::~DinoHelpStatus() {
    mFont->Release();
}

void DinoHelpStatus::Idle() {
    (this->*mState)();
}

extern const int data_020899ec, data_02089a14, data_02089a80, data_02089a04;
void SetBlendedMode(MSmartOamUnit *unit);

// Makes the font and the bar's canvas (out of sight above the top screen), then opens it.
void DinoHelpStatus::Start() {
    mFont = gDinoResource->CreateFont12(data_020899ec, data_02089a14, data_02089a80);
    mFont->AddRef();
    mPosition = 1;
    mPositions[0] = MPoint(0, 8);
    mPositions[1] = MPoint(0, data_02089a04 + 0xA8);
    mCanvas = new MSmartObjCanvas(mOam, MPoint(256, 16), (const u8 *) mMotion.GetPalette(0, NULL) + 4, TRUE);
    mCanvas->Clear();
    mCanvas->SetPosition(mPositions[mPosition]);
    mCanvas->Show();
    SetBlendedMode(mCanvas->GetOamUnit());
    mParallel.AddTail(mCanvas);
    mLines = 0;
    mState = data_020ba91c.OpenStep_24;
}

extern const char data_020bad5c[];  // put in front of every text
extern const int data_020899f0;     // frames before the text scrolls

// Remakes the bar's canvas with the text (wider texts get drawn twice, to scroll round) and moves it to the
// position (0: top, 1: bottom). With no text, the bar closes.
void DinoHelpStatus::SetText(u32 position, const char *text) {
    mSerial.Clear();
    mParallel.Remove(mCanvas);
    delete mCanvas;
    mCanvas = NULL;
    if (text == NULL) {
        mCanvas = new MSmartObjCanvas(mOam, MPoint(256, 16), (const u8 *) mMotion.GetPalette(0, NULL) + 4, TRUE);
        mCanvas->Clear();
        mCanvas->SetPosition(mPositions[mPosition]);
        mCanvas->Show();
        SetBlendedMode(mCanvas->GetOamUnit());
        mParallel.AddTail(mCanvas);
        mLines = 32;
        mState = data_020ba91c.CloseStep_4C;
        return;
    }
    std::string str = std::string(data_020bad5c) + text;
    MPoint size = mFont->MeasureText(str, 0, 0);
    size.y = 16;
    if (size.x <= 256) {
        mCanvas = new MSmartObjCanvas(mOam, MPoint(256, 16), (const u8 *) mMotion.GetPalette(0, NULL) + 4, TRUE);
        mCanvas->FillRect(MRect(MPoint(0, 0), mCanvas->GetSize()), kHelpStatusColor);
        mCanvas->DrawText(mFont, MPoint(0, 2), 0, std::string(str));
    } else {
        mCanvas = new MSmartObjCanvas(mOam, MPoint(size.x + 256, size.y), (const u8 *) mMotion.GetPalette(0, NULL) + 4,
                                      TRUE);
        mCanvas->FillRect(MRect(MPoint(0, 0), mCanvas->GetSize()), kHelpStatusColor);
        mCanvas->DrawText(mFont, MPoint(0, 2), 0, std::string(str));
        mCanvas->DrawText(mFont, MPoint(size.x, 2), 0, std::string(str));
    }
    mCanvas->Show();
    SetBlendedMode(mCanvas->GetOamUnit());
    mParallel.AddTail(mCanvas);
    if (mPosition == position) {
        mCanvas->SetPosition(mPositions[position]);
    } else {
        mSerial.AddTail(mCanvas->CreateCoordAnime(mPositions[mPosition], mPositions[position], 30, -2));
        mPosition = position;
    }
    mSerial.AddTail(new MTimer(data_020899f0));
    mState = data_020ba91c.Scroll_1C;
}

void DinoHelpStatus::OpenStep() {
    mLines++;
    mCanvas->Clear();
    mCanvas->FillRect(MRect(0, 8 - mLines / 2, 256, mLines / 2 + 8), kHelpStatusColor);
}

extern const MFx32 data_020b5814; // frames per pixel

// NONMATCHING: stack slots only (the game has the frame count's temporaries below the points', as if all one
// statement, yet works out dx before the first point's position).
void DinoHelpStatus::Scroll() {
    if (mCanvas != NULL && mCanvas->GetWidth() > 256) {
        int frames = MFx32ToInt(MFx32(mCanvas->GetWidth() - 256) * data_020b5814);
        int dx = mCanvas->GetWidth() - 256;
        MPoint to(-dx, mCanvas->GetPosition().y);
        MPoint from(0, mCanvas->GetPosition().y);
        mSerial.AddTail(mCanvas->CreateCoordAnime(from, to, frames, 0));
    }
}

void DinoHelpStatus::CloseStep() {
    mLines--;
    mCanvas->Clear();
    mCanvas->FillRect(MRect(0, 8 - mLines / 2, 256, mLines / 2 + 8), kHelpStatusColor);
}

#define PAD_BUTTON_SELECT 0x0004

DinoHelp::DinoHelp(MInput *input) : MMenu(input, 0) {
    mUnkE4 = TRUE;
}

DinoHelp::~DinoHelp() {
    DinoSoundResume();
}

// Select closes help: the panels slide off their screens, then the menu exits.
// NONMATCHING: the panel's and the label's point temporaries swap stack slots.
void DinoHelp::OnIdle() {
    if (!(GetInput()->GetPressed() & (PAD_BUTTON_SELECT | 0x2000))) {
        return;
    }
    DinoSoundPlaySe(0x8E, 0);
    mTasks->Clear();
    mUnkAC->Clear();
    for (u32 i = 0; i < 2; i++) {
        mStatus[i]->SetText(0, NULL);
        MTask *panelMove = mPanels[i]->CreateCoordAnime(
            mPanels[i]->GetPosition(),
            mPanels[i]->GetPosition() + (mPanels[i]->GetFrame().GetCenter().x <= 128 ? MPoint(-256, 0) : MPoint(256, 0)),
            30, 2);
        MTask *labelMove =
            mLabels[i]->CreateCoordAnime(mLabels[i]->GetPosition(), mLabels[i]->GetPosition().Offset(-256, 0), 30, 2);
        MParallelTaskSet *tasks = mTasks;
        tasks->AddTail(labelMove);
        tasks->AddTail(panelMove);
        u32 id = mUnkD0[i];
        if (id == mUnkE0) {
            mTasks->AddTail(mMarks[i]->CreatePlayTransfer(4, 0));
        } else {
            mTasks->AddTail(mMarks[i]->CreatePlayTransfer(3, 0));
        }
    }
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *wait = MTaskSetWaitEmptyTask::Create(mTasks);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(exit);
}

void DinoHelpDriver::Idle() {
    MScopedLock lock(&sHelpMutex.mutex, TRUE);
    if (mTasks->IsEmpty()) {
        Exit();
        return;
    }
    u32 buttons = mInput->mButtons;
    if ((buttons & (mInput->mPrevButtons ^ buttons) & PAD_BUTTON_SELECT) || (buttons & 0x2000)) {
        if (CanShowHelp()) {
            DinoSoundPause();
            DinoSoundPlaySe(0x8D, 0);
            MTask *close;
            MTask *run;
            run = MTaskRunTransfer::Create(mCursor);
            close = MCreateCallTask(this, &DinoHelpDriver::NotifyClose);
            DinoHelp *help = new DinoHelp(mInput);
            MTask *suspend = MTaskSuspendTransfer::Create(mCursor);
            MTask *open = MCreateCallTask(this, &DinoHelpDriver::NotifyOpen);
            MSerialTaskSet *serial = &mSerial;
            serial->AddTail(open);
            serial->AddTail(suspend);
            serial->AddTail(help);
            serial->AddTail(close);
            serial->AddTail(run);
        }
    }
}

typedef std::map<u32, void *> DinoHelpPtrMap;
typedef std::__red_black_tree<u32, std::pair<const u32, void *>, std::__select_first<std::pair<const u32, void *> >,
                              std::less<u32>, std::allocator<std::pair<const u32, void *> > >
    DinoHelpPtrTree;
template DinoHelpPtrMap::value_type &DinoHelpPtrMap::find_or_insert(const u32 &k);
template DinoHelpPtrTree::node_base *DinoHelpPtrTree::find_insert_position(const u32 &k, node_base *&last_not_greater,
                                                                          bool &left, bool &leftmost);
template DinoHelpPtrTree::iterator DinoHelpPtrTree::find(const u32 &k);
template DinoHelpPtrTree::node_base *DinoHelpPtrTree::lower_bound(const u32 &k, node_base *x, node_base *y);
template DinoHelpPtrTree::node_base *DinoHelpPtrTree::insert_node_at(node_base *parent, bool left, bool leftmost,
                                                                    const value_type &v);
template void DinoHelpPtrTree::destroy(node_base *n);
template DinoHelpPtrTree::~__red_black_tree();

// (Destroying one here emits the destructor the game's code in this file calls.)
#pragma dont_inline on
void DinoHelpParallelAnchor(MParallelTaskSet *set) {
    set->MParallelTaskSet::~MParallelTaskSet();
}
#pragma dont_inline reset

// Makes the unit semi-transparent when its display blends sprites over everything at full strength. (The game
// emits it first in the file; defined first here, it moves the statics in .bss.)
void SetBlendedMode(MSmartOamUnit *unit) {
    MBlend *blend = unit->owner->mDisplay->mBlend;
    if (blend->GetMode() != 0 && (blend->GetTarget1() & 0x10) && blend->GetEva() == 16 && blend->GetEvb() == 0) {
        unit->SetMode(1);
    }
}

extern "C" const int data_0208ae10[4]; // the corners' animes

// NONMATCHING: the game keeps the corner's address on the stack across the path's destructor.
DinoHelpCursor::DinoHelpCursor(MSmartOam *oam) {
    mFrame = MRect(0, 0, 0, 0);
    for (u32 i = 0; i < 4; i++) {
        mCorners[i] = new MMotion(std::string(data_020ba91c.mCursorPath), oam, 0, 1);
        mCorners[i]->Play(data_0208ae10[i], 0);
        mCorners[i]->SetAnchor(2);
        SetBlendedMode(mCorners[i]->GetUnit());
        mTasks.AddTail(mCorners[i]);
    }
}
