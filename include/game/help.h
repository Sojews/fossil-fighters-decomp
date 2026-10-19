// The help system: the help button shows help for whichever registered help item the player picks. Help bases
// own help items; hooks are told when help opens and closes.
#ifndef GAME_HELP_H
#define GAME_HELP_H

#include <list>
#include <map>
#include <string>
#include "m/task.h"
#include "m/geometry.h"
#include "m/motion.h"
#include "m/menu.h"

class DinoHelpItem;
class MDisplay;
class MInput;

// Something that owns help items (a menu, say).
class DinoHelpBase {
public:
    DinoHelpBase();
    virtual ~DinoHelpBase();

    void Enable();
    void Disable();
    BOOL IsEnabled();
    void SetSelectedItem(DinoHelpItem *item);

    void AddEntry(u32 a, u32 b, u32 c);
    void RemoveEntry(u32 a, u32 b);
    static MTask *CreateEnableTask(DinoHelpBase *base);

    u8 mEnabled;                    // 0x04
    DinoHelpItem *mSelectedItem;    // 0x08
};

// Something help can be shown for.
class DinoHelpItem {
public:
    DinoHelpItem();
    virtual ~DinoHelpItem();
    virtual MRect GetRect() = 0;        // the area the item's help is for
    virtual MDisplay *GetDisplay() = 0;   // where the help is shown
    virtual const char *GetText() = 0;

    void Enable();
    void Disable();
    BOOL IsEnabled();
    void Select();

    u8 mEnabled;            // 0x04
    DinoHelpBase *mBase;    // 0x08: the base that was current when the item was made
};

// A help item that's only an area and a text.
class DinoHelpDummyItem : public DinoHelpItem, public MTask {
public:
    DinoHelpDummyItem(MDisplay *display, MRect rect, std::string text);
    virtual MDisplay *GetDisplay();
    virtual MRect GetRect();
    void SetText(const std::string &text);
    virtual const char *GetText();
    virtual void Execute();

    MDisplay *mDisplay;     // 0x1C
    MRect mRect;            // 0x20
    std::string mText;      // 0x30
    const char *mTextPtr;   // 0x3C: mText's characters
};

// Told when help opens and closes.
class DinoHelpHook {
public:
    DinoHelpHook();
    virtual ~DinoHelpHook();
    virtual void Unk08() = 0;   // help opened
    virtual void Unk0C() = 0;   // help closed

    BOOL IsActive();
    void Activate();
    void Deactivate();

    u8 mActive; // 0x04
};

// While one exists, the help button is disabled.
class DinoHelpDisabler {
public:
    DinoHelpDisabler();
    virtual ~DinoHelpDisabler();
};

// Enables or disables a help base once.
class EnableHelpTask : public MTask {
public:
    EnableHelpTask(DinoHelpBase *base, bool enable) {
        mBase = base;
        mEnable = enable;
    }
    virtual ~EnableHelpTask();
    virtual void Execute();

    DinoHelpBase *mBase;    // 0x10
    bool mEnable;           // 0x14
};

// Keeps track of help bases, items and hooks.
// The help screen: a menu of the current base's help items.
class DinoHelp : public MMenu {
public:
    DinoHelp(MInput *input);
    virtual ~DinoHelp();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);

    u32 mUnkA8[15];     // 0xA8
    u8 mUnkE4;          // 0xE4
};

class MFont;
class MSmartObjCanvas;
class MSmartOam;

// The bar on the top screen showing the selected item's help text.
class DinoHelpStatus : public MProcess {
public:
    DinoHelpStatus(MSmartOam *oam);
    virtual ~DinoHelpStatus();
    virtual void Idle();

    void Start();
    void OpenStep();
    // Scrolls a text wider than the screen across it.
    void Scroll();
    void CloseStep();

    void (DinoHelpStatus::*mState)();   // 0x74
    MSmartOam *mOam;                    // 0x7C
    MFont *mFont;                       // 0x80
    MSmartObjCanvas *mCanvas;           // 0x84
    MMotionFile mMotion;                // 0x88
    MPoint mPositions[2];               // 0x8C
    u32 mPosition;                      // 0x9C
    u32 mLines;                         // 0xA0: open height, in lines of 2 pixels
};

// The help cursor: four corner motions around a frame.
class DinoHelpCursor : public MView {
public:
    DinoHelpCursor(MSmartOam *oam);
    virtual void Execute();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetSize(MPoint size);
    virtual MPoint GetSize();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();

    void Layout();
    void SetAnchor(int anchor);

    MParallelTaskSet mTasks;    // 0x10
    MMotion *mCorners[4];       // 0x40: top left, bottom left, top right, bottom right
    MRect mFrame;               // 0x50
};

class DinoHelpDriver : public MProcess {
public:
    DinoHelpDriver(MTask *cursor);
    virtual ~DinoHelpDriver();
    virtual void Idle();

    void Enable();
    void Disable();
    void AddBase(DinoHelpBase *base);
    void RemoveBase(DinoHelpBase *base);
    BOOL HasBase(DinoHelpBase *base);
    void AddItem(DinoHelpItem *item);
    void RemoveItem(DinoHelpItem *item);
    void AddHook(DinoHelpHook *hook);
    void RemoveHook(DinoHelpHook *hook);
    void AddEntry(u32 a, u32 b, u32 c);
    void RemoveEntry(u32 a, u32 b);
    BOOL CanShowHelp();
    void NotifyOpen();
    void NotifyClose();

    MParallelTaskSet *mTasks;                       // 0x74
    MTask *mCursor;                                 // 0x78
    MInput *mInput;                                 // 0x7C
    std::list<DinoHelpBase *> mBases;               // 0x80: the last is current
    std::list<DinoHelpItem *> mItems;               // 0x8C
    std::list<DinoHelpHook *> mHooks;               // 0x98
    int mEnableCount;                               // 0xA4: help is available while this is above 0
    std::multimap<u32, std::pair<u32, u32> > mEntries;  // 0xA8
};

#endif
