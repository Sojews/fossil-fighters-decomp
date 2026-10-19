// InfoPanelWindow and DinoBattleViewCreatureInfo.
// NONMATCHING: the DinoBattleViewCreatureInfo constructor (the game keeps the address of the motion slot on the stack
// across the path string's destructor).

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/info_panel_window.h"
#include "types.h"
#include "game/dino_battle_view_creature_info.h"
#include "m/picture.h"
#include "m/smart_oam.h"
#include "m/motion.h"

inline InfoPanelWindow::InfoPanelWindow(MPictureFile *file, MSmartOam *oam) : MProcess(NULL), mOam(oam), mUnk7C(0) {
    mPicture = new MTextPicture(*file, 1, false);
    mParallel.AddTail(mPicture);
}

InfoPanelWindow::~InfoPanelWindow() {
}

// Pooled file statics.
struct PopNumberStatics {
    u8 mUnk00;      // 0x00
    int mCount;     // 0x04: live pop numbers
};
extern "C" PopNumberStatics data_ov000_021978e0;

DinoBattleViewPopNumber::~DinoBattleViewPopNumber() {
    data_ov000_021978e0.mCount--;
}

DinoBattleViewCreatureInfo::~DinoBattleViewCreatureInfo() {
    delete mPicture;
}

#pragma auto_inline off

// The TU's pooled constants.
extern "C" struct {
    u8 unk00[4];
    const char *attackPath;     // 0x04
    const char *effectPath;     // 0x08
    const char *leftPath;       // 0x0C: the page arrows
    const char *elementPath;    // 0x10
    const char *rightPath;      // 0x14
    u8 unk18[0x10];
    MPoint arrowPos;            // 0x28
} data_ov000_02194d8c;

DinoBattleViewCreatureInfo::DinoBattleViewCreatureInfo(DinoBattleView *view, MPoint pos)
    : MProcess(NULL), mUnk80((u32)view), mUnkE8(3), mCreature(NULL) {
    mPicture = new MPictureFile(std::string("image/btl_info_panel"), 7);
    mOam = new MSmartOam(0x22, 0, 1);
    mOam->SetUnk1C(1);
    mOam->mHidden = TRUE;
    SetPosition(pos);
    mWindow = new InfoPanelWindow(mPicture, mOam);
    mWindow->mPicture->SetPriority(0);
    mParallel.AddTail(mWindow);
    for (int i = 0; i < 2; i++) {
        mMotions[i] = new MMotion(std::string(i != 0 ? data_ov000_02194d8c.rightPath : data_ov000_02194d8c.leftPath),
                                  mOam, 2, 1);
        mMotions[i]->SetPriority(0);
        mMotions[i]->SetAnchor(0);
        mMotions[i]->Show();
        mMotions[i]->SetPosition(data_ov000_02194d8c.arrowPos);
        mMotions[i]->Play(1, 0);
        GetParallel()->AddTail(mMotions[i]);
    }
    mTasks = new MParallelTaskSet;
    GetParallel()->AddTail(mTasks, mOam);
}

MMotion *DinoBattleViewCreatureInfo::GetMotion(int index) {
    return mMotions[index];
}

void DinoBattleViewCreatureInfo::SetPosition(MPoint pos) {
    mPos.x = pos.x;
    mPos.y = pos.y;
    mOam->SetOffset(MPoint(-pos.x, -pos.y));
}

void DinoBattleViewCreatureInfo::Show() {
    mWindow->mPicture->Show();
    mOam->mHidden = FALSE;
}

void InfoPanelWindow::Idle() {
    mPicture->SetPosition(MPoint(-(mOam->mOffset.x + mUnk7C * 256), -mOam->mOffset.y));
}

void DinoBattleViewCreatureInfo::Idle() {
}

void DinoBattleViewCreatureInfo::PlayMotion(int index) {
    mMotions[index]->Play(2, 1);
}

#pragma auto_inline on
