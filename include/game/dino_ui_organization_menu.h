// The team organization menu.
#ifndef GAME_DINO_UI_ORGANIZATION_MENU_H
#define GAME_DINO_UI_ORGANIZATION_MENU_H

#include "game/dino_resource.h"
#include "game/dino_backup.h"
#include <vector>
#include "m/menu.h"
#include "m/input.h"
#include "m/motion.h"
#include "m/archive.h"
#include "game/help.h"

class DinoUiFieldMenuFraction;
class DinoUiRvInformation;
class MTextPicture;
class DinoUiOrganizationMenuItem;
class DinoUiPartsReviverName;
class DinoUiPartsTeamTech;
class DinoUiPartsReviverMedalButton;
class MSmartOam;
class DinoCommonWindowMessage;

// Where things go on the screen (filled in at start).
struct DinoUiOrganizationLayout {
    const char *unk00[16];
    int slotX;          // 0x40: the first slot
    int slotY;          // 0x44
    int slotColumnX;    // 0x48: to the next slot in a row
    int slotColumnY;    // 0x4C
    int slotRowX;       // 0x50: to the next row
    int slotRowY;       // 0x54
};

class DinoUiOrganizationMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiOrganizationMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    void CreateSlots();
    void CreateSlot(u32 index);
    void ClearSlots();
    void RemoveSlotLinks();
    void FinishPageRight();
    void FinishPageLeft();
    void SetTab(u8 tab);
    void LoadPage();
    void PageRight();
    void PageLeft();
    void ShowTabInfo(u8 tab);
    void SetSort(u8 sort);
    void Sort(u8 sort);
    void RemoveListLinks();
    void UpdateDetails(u8 index);
    void TabLeft();
    void TabRight();
    BOOL InTeam(u32 id);
    void PlaceSlots();
    void LinkList();
    void UpdateTeamTech();
    void Refresh();
    void CreateList();
    void HandleResult();
    void HandleMemberResult();
    void EnableAll();
    void SelectMember(int index);
    void RemoveMember();
    // Where the creature is in the list (the list's size if it isn't there).
    int FindCreature(u8 id) {
        int i;
        for (i = 0; i < mCreatures.size(); i++) {
            if (id == mCreatures[i]) {
                break;
            }
        }
        return i;
    }
    void ClearList();
    void LinkSlots();
    void RestoreFocus();

    MSmartOam *mOam;                    // 0xB4
    MMenuItem *mUnkB8;                  // 0xB8
    DinoUiOrganizationMenuItem *mUnkBC; // 0xBC
    MMenuItem *mUnkC0;                  // 0xC0
    MMenuItem *mUnkC4;                  // 0xC4
    MMenuItem *mTabs[4];                // 0xC8
    MMenuItem *mButtons[5];             // 0xD8: the first three are linked down to the current tab
    u32 mUnkEC[4];                      // 0xEC
    MTextPicture *mTabPicture;          // 0xFC
    MTextPicture *mSortCursor;          // 0x100
    DinoUiPartsReviverMedalButton *mListItems[5]; // 0x104
    DinoUiPartsReviverMedalButton *mSlots[15]; // 0x118
    u32 mUnk154[15];                    // 0x154
    int mSlotCount;                     // 0x190
    u32 mUnk194[3];                     // 0x194
    DinoUiFieldMenuFraction *mFraction; // 0x1A0: "page n/m"
    u32 mUnk1A4;                        // 0x1A4
    int mResult;                        // 0x1A8
    std::vector<u32> mCreatures;        // 0x1AC: the ids listed, a page of 15 at a time
    std::vector<void *> mUnk1B8;        // 0x1B8
    std::vector<void *> mUnk1C4;        // 0x1C4
    std::vector<void *> mUnk1D0;        // 0x1D0
    std::vector<void *> mUnk1DC;        // 0x1DC
    std::vector<void *> mUnk1E8;        // 0x1E8
    int mPage;                          // 0x1F4
    int mLastPage;                      // 0x1F8
    u8 mTab;                            // 0x1FC
    u8 mSort;                           // 0x1FD
    u8 mFocusTabs;                      // 0x1FE: focus the new tab's item when paging
    u8 unk1FF;
    DinoTeam mTeam;                     // 0x200
    DinoUiRvInformation *mInfo;         // 0x228
    DinoUiPartsReviverName *mName;      // 0x22C
    MTaskSet *mUnk230;                  // 0x230
    DinoCommonWindowMessage *mMessage;  // 0x234
    int mSelected;                      // 0x238: a team member (0-4) or slot (5-)
    u8 mUnk23C;                         // 0x23C
    u8 unk23D;
    u8 mUnk23E;                         // 0x23E
    u8 unk23F;
    std::vector<MMotionFile> mMotions;  // 0x240
    u32 mUnk24C[3];                     // 0x24C
    DinoUiPartsTeamTech *mTeamTech;     // 0x258
    MArchivePtr mArchive;               // 0x25C
    MArchiveFile mFile;                 // 0x260
    u8 unk26C[0x2B0 - 0x26C];
    u8 mListLinked;                     // 0x2B0
    u8 mSlotsLinked;                    // 0x2B1
};

class DinoUiOrganizationMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiOrganizationMenuItem();
    virtual MRect GetFrame();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiOrganizationMenu *mOwner; // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiOrganizationOKButton : public DinoUiOrganizationMenuItem {
public:
    virtual ~DinoUiOrganizationOKButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOrganizationBackButton : public DinoUiOrganizationMenuItem {
public:
    virtual ~DinoUiOrganizationBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOrganizationPageLButton : public DinoUiOrganizationMenuItem {
public:
    virtual ~DinoUiOrganizationPageLButton();
    virtual void OnDecide();
};

class DinoUiOrganizationPageRButton : public DinoUiOrganizationMenuItem {
public:
    virtual ~DinoUiOrganizationPageRButton();
    virtual void OnDecide();
};

class DinoUiOrganizationSlotCreateTask : public MProcess {
public:
    DinoUiOrganizationSlotCreateTask(DinoUiOrganizationMenu *menu);
    virtual ~DinoUiOrganizationSlotCreateTask();
    virtual void Idle();

    DinoUiOrganizationMenu *mMenu;  // 0x74
    u32 mIndex;                     // 0x78
};

class DinoUiOrganizationMenuBlendResetTask : public MTask {
public:
    virtual ~DinoUiOrganizationMenuBlendResetTask();
    virtual void Execute();
};

#endif
