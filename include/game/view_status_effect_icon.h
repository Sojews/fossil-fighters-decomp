#ifndef GAME_VIEW_STATUS_EFFECT_ICON_H
#define GAME_VIEW_STATUS_EFFECT_ICON_H

#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class ViewStatusEffectIcon;

struct StatusEffectEntry {
    u32 mId;        // 0x00
    int mCount;     // 0x04
    u8 mFlag;       // 0x08

    int GetAnime();
};

struct StatusEffectSlot {
    std::vector<StatusEffectEntry> mEntries;    // 0x00
    ViewStatusEffectIcon *mIcon;                // 0x0C
    u32 mUnk10;                                 // 0x10
};

class MSmartOam;

class DinoBattleViewStatusEffectIcon : public MProcess {
public:
    DinoBattleViewStatusEffectIcon(MSmartOam *oam, u32 side, u32 unk74);
    virtual ~DinoBattleViewStatusEffectIcon();
    virtual void Idle();

    void HideIcons();
    void ResetIconModes();

    u32 mUnk74;                     // 0x74
    MSmartOam *mOam;                // 0x78
    MParallelTaskSet *mIcons;       // 0x7C
    StatusEffectSlot mSlots[2];     // 0x80
    u32 unkA8[5];
    u32 mSide;                      // 0xBC
    int mTimer;                     // 0xC0
    MPoint mOffsets[2];             // 0xC4
    MSerialTaskSet *mQueue;         // 0xD4
};

class ViewStatusEffectIcon : public MProcess {
public:
    ViewStatusEffectIcon(MSmartOam *oam) : MProcess(NULL) {
        mMotion = NULL;
        mOam = oam;
        mUnk7C = 0;
        mUnk80 = 0;
        mTasks = new MSerialTaskSet();
        mParallel.AddTail(mTasks);
    }
    virtual ~ViewStatusEffectIcon();
    virtual void Idle();

    class PlayEraseMotionTask : public MTask {
    public:
        virtual ~PlayEraseMotionTask();
        virtual void Execute();
    };

    class SetPriorityTask : public MTask {
    public:
        virtual ~SetPriorityTask();
        virtual void Execute();

        ViewStatusEffectIcon *mIcon;    // 0x10
        int mPriority;                  // 0x14
    };

    MMotion *mMotion;           // 0x74
    MSmartOam *mOam;            // 0x78
    u32 mUnk7C;                 // 0x7C
    u32 mUnk80;                 // 0x80
    MSerialTaskSet *mTasks;     // 0x84

    BOOL IsPlaying() {
        if (mMotion != NULL && mMotion->IsPlaying()) {
            return TRUE;
        }
        return FALSE;
    }
    void ShowMotion() {
        if (mMotion != NULL) {
            mMotion->Show();
        }
    }
    void HideMotion() {
        if (mMotion != NULL) {
            mMotion->Hide();
        }
    }
    void SetMotionPriority(int priority) {
        if (mMotion != NULL) {
            mMotion->SetPriority(priority);
        }
    }
};

class ViewStatusEffectIconWaitStop : public MTask {
public:
    virtual ~ViewStatusEffectIconWaitStop();
    virtual void Execute();

    ViewStatusEffectIcon *mIcon;    // 0x10
};

class ViewStatusEffectIconLoadMotionTask : public MTask {
public:
    virtual ~ViewStatusEffectIconLoadMotionTask();
    virtual void Execute();

    u32 mUnk10;             // 0x10
    std::string mUnk14;     // 0x14
};

class ViewStatusEffectIconShowTask : public MTask {
public:
    virtual ~ViewStatusEffectIconShowTask();
    virtual void Execute();

    ViewStatusEffectIcon *mIcon;    // 0x10
    u8 mShow;                       // 0x14
};

#endif
