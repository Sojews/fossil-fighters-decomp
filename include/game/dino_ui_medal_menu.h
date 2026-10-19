// The medal list menu.
#ifndef GAME_DINO_UI_MEDAL_MENU_H
#define GAME_DINO_UI_MEDAL_MENU_H

#include "game/dino_resource.h"
#include <vector>
#include "m/menu.h"
#include "m/motion.h"
#include "game/help.h"

class MSmartOam;
class DinoUiFieldMenuFraction;
class MTextPicture;

class DinoUiMedalMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    DinoUiMedalMenu(u32 *mode, u32 *index, u8 *unk);
    virtual ~DinoUiMedalMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided(BOOL arg);
    void PageRight();
    void PageLeft();
    void LoadPage();
    void FinishPageRight();
    void FinishPageLeft();
    void ClearSlots();
    void CreateSlots();
    void CreateSlot(u32 index);
    BOOL HasUnk1EC(u8 id);
    void SetSort(u8 sort);
    void Sort(u8 sort);

    MSmartOam *mOam;                    // 0xB4: the items' sprites
    u32 mUnkB8[13];                     // 0xB8
    MTextPicture *mSortCursor;          // 0xEC
    u32 mUnkF0[5];                      // 0xF0
    MMenuItem *mSlots[15];              // 0x104
    u32 mUnk140[15];                    // 0x140
    u32 mSlotCount;                     // 0x17C
    u32 mUnk180[3];                     // 0x180
    DinoUiFieldMenuFraction *mFraction; // 0x18C: "page n/m"
    int *mResult;                       // 0x190
    u32 mUnk194[2];
    std::vector<void *> mUnk19C;        // 0x19C
    std::vector<void *> mUnk1A8;        // 0x1A8
    std::vector<void *> mUnk1B4;        // 0x1B4
    u32 mPage;                          // 0x1C0
    u32 mLastPage;                      // 0x1C4
    u8 mSort;                           // 0x1C8
    u8 unk1C9[3];
    u32 mUnk1CC[8];                     // 0x1CC
    u8 mUnk1EC[5];                      // 0x1EC
    u8 unk1F1[0x13];
    u8 mUnk204;                         // 0x204
    std::vector<MMotionFile> mMotions;  // 0x208
    u8 unk214[0x220 - 0x214];
    MTaskSet *mUnk220;                  // 0x220
};

class DinoUiSortMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiSortMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u32 mTextId;        // 0x94
    MPoint mPos;        // 0x98
};

class DinoUiMedalList : public MProcess {
public:
    DinoUiMedalList();
    virtual ~DinoUiMedalList();
    virtual void Idle();

    void StateMenu();

    void (DinoUiMedalList::*mState)(); // 0x74
    u32 mUnk7C;                         // 0x7C
    u32 mUnk80;                         // 0x80
    u8 mUnk84;                          // 0x84
    u8 mUnk85;                          // 0x85
};

class DinoUiMedalMenu;

class DinoUiMedalMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoUiMedalMenuItem(DinoUiMedalMenu *owner, const std::string &path, MPoint pos, u32 textId);
    virtual ~DinoUiMedalMenuItem();
    virtual MRect GetFrame();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiMedalMenu *mOwner; // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiMedalBackButton : public DinoUiMedalMenuItem {
public:
    virtual ~DinoUiMedalBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMedalPageLButton : public DinoUiMedalMenuItem {
public:
    virtual ~DinoUiMedalPageLButton();
    virtual void OnDecide();
};

class DinoUiMedalPageRButton : public DinoUiMedalMenuItem {
public:
    virtual ~DinoUiMedalPageRButton();
    virtual void OnDecide();
};

class DinoUiMedalSlotCreateTask : public MProcess {
public:
    DinoUiMedalSlotCreateTask(DinoUiMedalMenu *menu);
    virtual ~DinoUiMedalSlotCreateTask();
    virtual void Idle();

    DinoUiMedalMenu *mMenu; // 0x74
    u32 mIndex;             // 0x78
};

#endif
