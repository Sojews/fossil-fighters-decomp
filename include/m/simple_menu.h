// A debug menu: a column of text items with an arrow either side of the chosen one, picked with the D-pad and A
// or Start (or by touching an item); B picks the item with id -2 if there is one.
#ifndef M_SIMPLE_MENU_H
#define M_SIMPLE_MENU_H

#include <string>
#include <vector>
#include "m/geometry.h"
#include "m/task.h"

class MFont;
class MInput;
class MParallelTaskSet;
class MView;

class MSimpleMenu : public MProcess {
public:
    // The items to make, up to one with id -1.
    struct Item {
        int id;
        const char *text;
    };
    struct Entry {
        int id;
        MView *view;
    };

    // The chosen id goes in *result.
    MSimpleMenu(int *result, MFont *font, const Item *items, int initial);
    virtual ~MSimpleMenu();
    virtual void Idle();

    void Init();
    void AddEntry(int id, MView *view);
    void AddItem(int id, std::string text);
    void Layout();
    MPoint GetArrowPos(int side, int index);
    void SetCursor(int id);
    int HitTest(MPoint pos);
    void Decide(int id);
    void MoveCursor(int index);
    void Start();
    void Update();

    void (MSimpleMenu::*mState)();  // 0x74
    int *mResult;                   // 0x7C
    MFont *mFont;                   // 0x80
    MInput *mInput;                 // 0x84
    const Item *mItems;             // 0x88
    int mInitial;                   // 0x8C
    u8 mHasCancel;                  // 0x90
    u32 mCursor;                    // 0x94
    std::vector<Entry> mEntries;    // 0x98
    MParallelTaskSet *mAnimTasks;   // 0xA4: the arrows moving
    MParallelTaskSet *mViews;       // 0xA8
    MView *mArrows[2];              // 0xAC
    u8 mLocked;                     // 0xB4: only touch works
};

#endif
