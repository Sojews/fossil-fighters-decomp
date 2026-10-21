#ifndef GAME_DINO_BATTLE_VIEW_FIELD_H
#define GAME_DINO_BATTLE_VIEW_FIELD_H

#include "m/picture.h"
#include "m/motion.h"
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleView;
class MSmartOam;

class DinoBattleViewField : public MProcess, public MMessageObserver {
public:
    DinoBattleViewField(DinoBattleView *view, MSmartOam *oam, u32 tutorial);
    virtual ~DinoBattleViewField();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    // (Also laid out as DinoBattleViewUnkE0, dino_battle_view_class.h, which OnMessage uses.)
    u32 mUnkA8;                 // 0xA8
    DinoBattleView *mView;      // 0xAC
    MSmartOam *mOam;            // 0xB0
    MTaskSet *mTasksB4;         // 0xB4
    MTaskSet *mTasksB8;         // 0xB8
    MTaskSet *mTasksBC;         // 0xBC
    class DinoBattleViewPlayerField *mSlotViewsC0[2];   // 0xC0: by side
    u32 unkC8;
    MPictureFile mPictureCC;    // 0xCC
    u8 unk100[0x118 - 0x100];
    class DinoBattleViewFieldPlayerBG *mPlayerBG;   // 0x118
    class DinoBattleViewAdvantageArrow *mArrow;     // 0x11C
    class DinoBattleViewBackdrop *mBackdrop;        // 0x120
    class DinoBattleViewLCDFader *mFader;           // 0x124
    MTask *mTask128;            // 0x128
    MTask *mTask12C;            // 0x12C
    u32 mUnk130;                // 0x130
    class DinoBattleViewTutorialMotion *mTutorialMotion;    // 0x134
    class DinoBattleTalkWindow *mTalkWindow;        // 0x138
    class DinoBattleViewTutorialWindow *mTutorialWindow;    // 0x13C
    class DinoBattleViewKPUpDown *mKPUpDown;        // 0x140
    MParallelTaskSet *mTasks144;    // 0x144
    u32 mUnk148;                // 0x148
    MSmartOam *mOam14C;         // 0x14C
    int mUnk150;                // 0x150
    std::vector<MPictureFile> mPictures;    // 0x154
    std::vector<MMotionFile> mMotionFiles;  // 0x160
};

#endif
