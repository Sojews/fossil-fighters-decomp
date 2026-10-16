// The help system: the help button shows help for whichever registered help item the player picks. Help bases
// own help items; hooks are told when help opens and closes.
#ifndef GAME_HELP_H
#define GAME_HELP_H

#include <string>
#include "m/task.h"
#include "m/geometry.h"

class DinoHelpItem;

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
    virtual u32 GetHelpId() = 0;
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
    DinoHelpDummyItem(u32 id, MRect rect, std::string text);
    virtual u32 GetHelpId();
    virtual MRect GetRect();
    void SetText(const std::string &text);
    virtual const char *GetText();
    virtual void Execute();

    u32 mHelpId;            // 0x1C
    MRect mRect;            // 0x20
    std::string mText;      // 0x30
    const char *mTextPtr;   // 0x3C: mText's characters
};

// Told when help opens and closes.
class DinoHelpHook {
public:
    DinoHelpHook();
    virtual ~DinoHelpHook();
    virtual void Unk08() = 0;
    virtual void Unk0C() = 0;

    u8 mUnk4;
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
class DinoHelpDriver : public MProcess {
public:
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

    u32 mUnk74[(0xA4 - 0x74) / 4];
    int mEnableCount;       // 0xA4: help is available while this is 0 or more... (see Enable/Disable)
};

#endif
