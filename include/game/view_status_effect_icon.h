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
    void RemoveEntry(u32 id);
    void ResetIconModes();
    // Waits for both icons' motions and the queue.
    MTask *CreateWaitTask();

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

        ViewStatusEffectIcon *mIcon;    // 0x10
    };

    class SetPriorityTask : public MTask {
    public:
        SetPriorityTask(ViewStatusEffectIcon *icon, int priority) {
            mIcon = icon;
            mPriority = priority;
        }
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
    u32 GetMotionAnime() {
        return mMotion != NULL ? mMotion->GetAnime() : 0;
    }
    // Plays the anime (none for -1) in front, except the low ones, then drops behind again when it stops.
    void PlayAnime(int anime) {
        if (mMotion != NULL && anime != -1) {
            mTasks->Clear();
            mMotion->Play(anime, 1);
            BOOL front = TRUE;
            switch (anime) {
            case 1:
            case 2:
            case 3:
            case 13:
            case 14:
            case 15:
                front = FALSE;
                break;
            }
            mMotion->SetPriority(front ? 1 : 2);
            mTasks->AddTail(new MTimer(1), mMotion->CreateWaitStopTask(), new SetPriorityTask(this, 2));
        }
    }
};

class ViewStatusEffectIconWaitStop : public MTask {
public:
    ViewStatusEffectIconWaitStop(ViewStatusEffectIcon *icon) { mIcon = icon; }
    virtual ~ViewStatusEffectIconWaitStop();
    virtual void Execute();

    ViewStatusEffectIcon *mIcon;    // 0x10
};

class ViewStatusEffectIconLoadMotionTask : public MTask {
public:
    virtual ~ViewStatusEffectIconLoadMotionTask();
    virtual void Execute();

    u32 mUnk10;             // 0x10: the ViewStatusEffectIcon
    std::string mUnk14;     // 0x14: the motion's path
    int mAnime;             // 0x20: to play (-1 for none)
};

class ViewStatusEffectIconShowTask : public MTask {
public:
    virtual ~ViewStatusEffectIconShowTask();
    virtual void Execute();

    ViewStatusEffectIcon *mIcon;    // 0x10
    u8 mShow;                       // 0x14
};

#endif
