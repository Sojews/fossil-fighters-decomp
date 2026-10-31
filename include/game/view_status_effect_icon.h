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

// The status effects' motion paths: {effect, path} pairs, ended by a NULL path; the static initialization fills in
// the paths. (data_ov000_021952b4's word 0x30 is the first path.)
struct StatusEffectPath {
    u32 mEffect;
    const char *mPath;
};
extern "C" struct {
    u8 unk00[0x30];
    u32 firstIcon;  // 0x30
} data_ov000_021952b4;
extern "C" StatusEffectPath data_ov000_021952e0[];

struct StatusEffectEntry {
    StatusEffectEntry() {}

    u32 mId;        // 0x00
    int mCount;     // 0x04
    u8 mFlag;       // 0x08

    int GetAnime();
};

// (A user constructor, but the game copies it with vector's trivial paths.)
namespace Metrowerks {
template <> struct is_trivial<StatusEffectEntry> {
    static const bool value = true;
};
} // namespace Metrowerks

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
    // Updates the icons to the effects (low nibble: the effect, bits 5-6: the count, bit 7: the flag).
    void SetEffects(int count, const u8 *effects);

    u32 mUnk74;                     // 0x74
    MSmartOam *mOam;                // 0x78
    MParallelTaskSet *mIcons;       // 0x7C
    StatusEffectSlot mSlots[2];     // 0x80
    u32 unkA8;
    u32 mShown[2];                  // 0xAC: the entry each icon shows
    u32 unkB4[2];
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
        mPos.x = 0;
        mPos.y = 0;
        mTasks = new MSerialTaskSet();
        mParallel.AddTail(mTasks);
    }
    virtual ~ViewStatusEffectIcon();
    virtual void Idle();

    class PlayEraseMotionTask : public MTask {
    public:
        PlayEraseMotionTask(ViewStatusEffectIcon *icon) { mIcon = icon; }
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
    MPoint mPos;                // 0x7C
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
    // Replaces the motion (keeping whether it was shown) and plays the anime.
    void LoadMotion(const char *path, int anime) {
        BOOL hidden = TRUE;
        if (mMotion != NULL) {
            mTasks->Clear();
            hidden = mMotion->IsHidden();
            mParallel.Remove(mMotion);
            delete mMotion;
        }
        mMotion = new MMotion(std::string(path), mOam, 2, 1);
        mMotion->SetPosition(mPos);
        mMotion->SetPriority(2);
        mMotion->SetAnchor(1);
        if (!hidden) {
            mMotion->Show();
        }
        mParallel.AddTail(mMotion);
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
            MTask *priority = new SetPriorityTask(this, 2);
            mTasks->AddTail(new MTimer(1), mMotion->CreateWaitStopTask(), priority);
        }
    }
    void SetPosition(MPoint pos) {
        mPos = pos;
        if (mMotion != NULL) {
            mMotion->SetPosition(pos);
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
    ViewStatusEffectIconLoadMotionTask(ViewStatusEffectIcon *icon, const StatusEffectEntry *entry, int anime)
        : mUnk10((u32)icon) {
        mAnime = anime;
        int i = 0;
        if (data_ov000_021952b4.firstIcon != 0) {
            const StatusEffectPath *path = data_ov000_021952e0;
            do {
                if (entry->mId == path->mEffect) {
                    mUnk14 = data_ov000_021952e0[i].mPath;
                    break;
                }
                path++;
                i++;
            } while (path->mPath != NULL);
        }
    }
    virtual ~ViewStatusEffectIconLoadMotionTask();
    virtual void Execute();

    u32 mUnk10;             // 0x10: the ViewStatusEffectIcon
    std::string mUnk14;     // 0x14: the motion's path
    int mAnime;             // 0x20: to play (-1 for none)
};

class ViewStatusEffectIconShowTask : public MTask {
public:
    ViewStatusEffectIconShowTask(ViewStatusEffectIcon *icon, u8 show) {
        mIcon = icon;
        mShow = show;
    }
    virtual ~ViewStatusEffectIconShowTask();
    virtual void Execute();

    ViewStatusEffectIcon *mIcon;    // 0x10
    u8 mShow;                       // 0x14
};

#endif
