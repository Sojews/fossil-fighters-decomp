// DinoBattleViewField.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_field.h"
#include "game/dino_resource.h"
#include "types.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_view_player_field.h"
#include "game/dino_battle_view_field_player_bg.h"
#include "game/motion_advantage_arrow.h"
#include "game/dino_battle_tutorial.h"
#include "game/dino_battle2.h"
#include "game/dino_battle_view_kp_up_down.h"
#include "game/dino_battle_view_backdrop.h"
#include "m/smart_oam.h"
#include "m/lcd.h"

static inline const MPoint *DinoFieldPoints() {
    const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
    return (const MPoint *)((const u8 *)defs + defs->fieldPoints);
}

static inline MPoint DinoFieldPoint(int i) {
    return DinoFieldPoints()[i];
}

// Where a side's view sits for a slot kind (mirrored for the second side).
extern "C" MPoint func_ov000_0215f22c(int kind, int side) {
    MPoint pos = DinoFieldPoint(1);
    switch (kind) {
    case 0:
        pos.x = -128;
        break;
    case 1:
        pos = DinoFieldPoint(0);
        break;
    case 2:
        break;
    case 3:
        pos = DinoFieldPoint(2);
        break;
    case 4:
        pos = DinoFieldPoint(3);
        break;
    case 5:
        pos = DinoFieldPoint(4);
        break;
    }
    if (side != 0) {
        pos.x = 256 - pos.x;
    }
    return pos;
}

extern "C" const char data_ov000_021924a4[];    // the field's picture
// The TU's pooled motion paths.
extern "C" struct {
    u8 unk00[0xC];
    const char *mPath0C;    // 0x0C
    const char *mPath10;    // 0x10
    const char *mPath14;    // 0x14
    u8 unk18[4];
    const char *mPath1C;    // 0x1C
    u8 unk20[4];
    const char *mPath24;    // 0x24
} data_ov000_02191b1c;

// Sets up the field: the two sides' slots sliding in, its picture and motions, the player BG, the advantage arrow,
// the tutorial marks, the backdrop (unless turned off), the fader, the tutorial or talk window and the KP marks.
// NONMATCHING: call sequence identical; the push_backs' tag bytes take other stack slots (and the frame is 4 bytes
// bigger).
DinoBattleViewField::DinoBattleViewField(DinoBattleView *view, MSmartOam *oam, u32 tutorial)
    : MProcess(NULL), mUnkA8(0), mView(view), mOam(oam), mTask128(NULL), mTask12C(NULL), mTalkWindow(NULL),
      mTutorialWindow(NULL), mUnk150(0) {
    for (int side = 0; side < 2; side++) {
        mSlotViewsC0[side] = new DinoBattleViewPlayerField(side);
        mParallel.AddTail(mSlotViewsC0[side]);
        mSlotViewsC0[side]->Show();
        mSlotViewsC0[side]->SetPriority(2);
        mSlotViewsC0[side]->SetPosition(func_ov000_0215f22c(0, side));
        mParallel.AddTail(mSlotViewsC0[side]->CreateCoordAnime(mSlotViewsC0[side]->GetPosition(),
                                                                func_ov000_0215f22c(1, side), 60, -1));
    }
    mPictures.push_back(MPictureFile(std::string(data_ov000_021924a4), 7));
    mMotionFiles.push_back(MMotionFile(std::string(data_ov000_02191b1c.mPath10), 7));
    mMotionFiles.push_back(MMotionFile(std::string(data_ov000_02191b1c.mPath14), 7));
    mMotionFiles.push_back(MMotionFile(std::string(data_ov000_02191b1c.mPath0C), 7));
    mMotionFiles.push_back(MMotionFile(std::string(data_ov000_02191b1c.mPath1C), 7));
    mMotionFiles.push_back(MMotionFile(std::string(data_ov000_02191b1c.mPath24), 7));
    mMotionFiles.push_back(MMotionFile(std::string(data_ov000_02191b1c.mPath1C), 7));
    mPlayerBG = new DinoBattleViewFieldPlayerBG(mView);
    mParallel.AddTail(mPlayerBG);
    mView->SetUnk128(mSlotViewsC0[0]->GetPlaneMask() | mSlotViewsC0[1]->GetPlaneMask());
    mArrow = new DinoBattleViewAdvantageArrow((u32) view, oam);
    mArrow->Suspend();
    mParallel.AddTail(mArrow);
    mTutorialMotion = new DinoBattleViewTutorialMotion(mOam);
    mParallel.AddTail(mTutorialMotion);
    if (gDinoResource->GetBattleAdjustDefs()->fixedFieldBg) {
        mBackdrop = NULL;
    } else {
        mBackdrop = new DinoBattleViewBackdrop();
        mParallel.AddTail(mBackdrop);
    }
    mFader = new DinoBattleViewLCDFader((u32) mView);
    mParallel.AddTail(mFader);
    if (tutorial) {
        mTutorialWindow = new DinoBattleViewTutorialWindow();
        mParallel.AddTail(mTutorialWindow);
    } else {
        mTalkWindow = new DinoBattleTalkWindow((u32) mView);
        mParallel.AddTail(mTalkWindow);
    }
    mKPUpDown = new DinoBattleViewKPUpDown();
    mParallel.AddTail(mKPUpDown);
    mTasks144 = new MParallelTaskSet();
    mOam14C = new MSmartOam(gLcd->mMain, 0x10, 8, TRUE);
    mParallel.AddTail(mTasks144, mOam14C);
    mUnk148 = 0;
    mTasksB4 = new MParallelTaskSet();
    mTasksB8 = new MParallelTaskSet();
    mTasksBC = new MParallelTaskSet();
    mParallel.AddTail(mTasksB4, mTasksB8, mTasksBC);
}
