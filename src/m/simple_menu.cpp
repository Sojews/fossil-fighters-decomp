// MSimpleMenu.
// NONMATCHING: Init and Start (the pointer-to-member constants and the type infos come out in a different order in
// .data, so their offsets from the data anchor differ), AddItem and Start (the string temporary), Layout, SetCursor,
// Decide and Update (register allocation and scheduling), append_realloc (as everywhere).

#pragma ipa file

#include "m/simple_menu.h"
#include "m/display.h"
#include "m/font.h"
#include "m/indicator.h"
#include "m/input.h"
#include "m/lcd.h"
#include "m/obj.h"
#include "m/oam_canvas.h"
#include "game/dino_static_init.h"

// For fonts without a palette: black on white.
static const u16 sPalette[2] = {0x0000, 0x7FFF};
// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[4];
    void (MSimpleMenu::*Update_04)();   // 0x04
    void (MSimpleMenu::*Start_0C)();    // 0x0C
} data_020c45f8;

// Where the items are laid out (pooled at 0x70, set by the static init).
struct MSimpleMenuArea {
    int left, top, right, bottom;
};
#define sArea DINO_POOL_NV(MSimpleMenuArea, data_020c45f8, 0x70)

MSimpleMenu::MSimpleMenu(int *result, MFont *font, const Item *items, int initial)
    : MProcess(NULL), mResult(result), mFont(font), mItems(items), mInitial(initial), mHasCancel(FALSE),
      mLocked(FALSE) {
    Init();
}

void MSimpleMenu::Init() {
    mFont->AddRef();
    mFont->SetColors(0, 1, 0);
    mAnimTasks = new MParallelTaskSet;
    mViews = new MParallelTaskSet;
    mParallel.AddTail(mAnimTasks, mViews);
    mState = data_020c45f8.Start_0C;
}

MSimpleMenu::~MSimpleMenu() {
    mFont->Release();
}

void MSimpleMenu::Idle() {
    (this->*mState)();
}

void MSimpleMenu::AddEntry(int id, MView *view) {
    Entry entry;
    entry.id = id;
    entry.view = view;
    mEntries.push_back(entry);
    view->Show();
    mViews->AddTail(view);
    if (id == -2) {
        mHasCancel = TRUE;
    }
}

void MSimpleMenu::AddItem(int id, std::string text) {
    const char *s = text.c_str();
    MIndicator *item = new MIndicator(
        new MObjCanvas(GetDisplay(), GetDisplay()->mObj->mOam, mFont->MeasureText(s, 0, 0),
                       mFont->GetPalette() != NULL ? mFont->GetPalette() : sPalette, TRUE),
        mFont, 0);
    item->SetText(s);
    AddEntry(id, item);
}

// Centers the items across the area, spread evenly down it.
void MSimpleMenu::Layout() {
    for (u32 i = 0; i < mEntries.size(); i++) {
        MView *view = mEntries[i].view;
        u32 count = mEntries.size();
        int top = sArea.top;
        int width = view->GetWidth();
        int height = view->GetHeight();
        view->SetPosition((sArea.left + sArea.right) / 2 - width / 2,
                          top + (sArea.bottom - top) * (i + 1) / (count + 1) - height / 2);
    }
}

// Where the arrow on the given side (0 left, 1 right) of an item goes.
MPoint MSimpleMenu::GetArrowPos(int side, int index) {
    MRect frame = mEntries[index].view->GetFrame();
    if (side == 0) {
        int y = (frame.top + frame.bottom) / 2 - mArrows[0]->GetHeight() / 2;
        return MPoint(frame.left - mArrows[0]->GetWidth() - 3, y);
    }
    int y = (frame.top + frame.bottom) / 2 - mArrows[1]->GetHeight() / 2;
    return MPoint(frame.right + 3, y);
}

// On the item with the id, or the first one.
void MSimpleMenu::SetCursor(int id) {
    int first = -1;
    u32 i;
    for (i = 0; i < mEntries.size(); i++) {
        if (id == mEntries[i].id) {
            break;
        }
        if (first == -1) {
            first = i;
        }
    }
    const int count = mEntries.size();
    if (i == count) {
        if (first >= 0) {
            i = first;
        } else {
            i = 0;
        }
    }
    mCursor = i;
    for (u32 side = 0; side < 2; side++) {
        mArrows[side]->SetPosition(GetArrowPos(side, mCursor));
    }
}

#pragma opt_common_subs off
// The id of the item at pos, or near it, or -1.
int MSimpleMenu::HitTest(MPoint pos) {
    u32 i;
    for (i = 0; i < mEntries.size(); i++) {
        if (mEntries[i].view->GetFrame().Contains(pos)) {
            return mEntries[i].id;
        }
    }
    for (i = 0; i < mEntries.size(); i++) {
        if (mEntries[i].view->GetFrame().Inflate(8).Contains(pos)) {
            return mEntries[i].id;
        }
    }
    return -1;
}
#pragma opt_common_subs reset

// Clears away the other items, then exits.
void MSimpleMenu::Decide(int id) {
    *mResult = id;
    for (u32 i = 0; i < mEntries.size(); i++) {
        MView *view = mEntries[i].view;
        if (id != mEntries[i].id) {
            view->Exit();
        } else {
            MoveCursor(i);
        }
    }
    mSerial.AddTail(new MTimer(10), MTaskExitTransfer::Create(this));
}

void MSimpleMenu::MoveCursor(int index) {
    mAnimTasks->Clear();
    for (u32 side = 0; side < 2; side++) {
        mAnimTasks->AddTail(
            mArrows[side]->CreateCoordAnime(mArrows[side]->GetPosition(), GetArrowPos(side, index), 5, -2));
    }
}

void MSimpleMenu::Start() {
    if (mItems != NULL) {
        for (; mItems->id != -1; mItems++) {
            AddItem(mItems->id, mItems->text);
        }
    }
    const char *arrow;
    if (mFont->HasGlyph(0x819A)) {
        arrow = "\x81\x9a";
    } else if (mFont->HasGlyph('*')) {
        arrow = "*";
    } else {
        OS_Terminate();
    }
    for (int side = 0; side < 2; side++) {
        MIndicator *indicator = new MIndicator(
            new MObjCanvas(GetDisplay(), GetDisplay()->mObj->mOam, mFont->MeasureText(arrow, 0, 0),
                           mFont->GetPalette() != NULL ? mFont->GetPalette() : sPalette, TRUE),
            mFont, 0);
        if (!mLocked) {
            indicator->Show();
        }
        indicator->SetText(arrow);
        mViews->AddTail(indicator);
        mArrows[side] = indicator;
    }
    Layout();
    SetCursor(mInitial);
    MSingleInputHub *hub = new MSingleInputHub(1);
    if (GetDisplay() == gLcd->GetBottomDisplay()) {
        hub->EnableTouch();
    }
    mInput = hub->GetInput(0);
    mParallel.AddTail(hub);
    mState = data_020c45f8.Update_04;
}

// The buttons: touch, A or Start, B, up and down.
void MSimpleMenu::Update() {
    u32 cursor = mCursor;
    if (mInput->GetPressed() & 0x1000) {
        int id = HitTest(mInput->GetTouchPos());
        if (id != -1) {
            Decide(id);
            return;
        }
    }
    if (!mLocked) {
        if (mInput->GetPressed() & (0x1 | 0x8)) {
            Decide(mEntries[mCursor].id);
            return;
        }
        if (mHasCancel && (mInput->GetPressed() & 0x2)) {
            Decide(-2);
            return;
        }
        if ((mInput->GetPressed() & 0x40) && mCursor != 0) {
            cursor--;
        }
        if ((mInput->GetPressed() & 0x80) && mCursor < mEntries.size() - 1) {
            cursor++;
        }
        if (cursor != mCursor) {
            mCursor = cursor;
            MoveCursor(mCursor);
        }
    }
}

#pragma auto_inline off
template class std::__vec_deleter<MSimpleMenu::Entry, std::allocator<MSimpleMenu::Entry> >;

// The file's static initialization (see dino_static_init.h).
static struct SimpleMenuStaticInit {
    SimpleMenuStaticInit() {
        DINO_POOL_WORD(data_020c45f8, 0x70) = 0;
        DINO_POOL_WORD(data_020c45f8, 0x74) = 0;
        DINO_POOL_WORD(data_020c45f8, 0x78) = 256;
        DINO_POOL_WORD(data_020c45f8, 0x7c) = 192;
    }
} sSimpleMenuStaticInit;
