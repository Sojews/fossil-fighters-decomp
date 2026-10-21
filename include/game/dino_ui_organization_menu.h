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
#include "m/resource.h"
#include "game/help.h"

class DinoUiFieldMenuFraction;
class DinoUiFieldMenuCursor;
class DinoUiRvInformation;
class MTextPicture;
class DinoUiOrganizationMenuItem;
class DinoUiPartsReviverName;
class DinoUiPartsTeamTech;
class DinoUiPartsReviverMedalButton;
class MSmartOam;
class MView;
class MIndicator;
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
    MPoint numberOffset;    // 0x58: of a slot's number
    u8 unk60[0x78 - 0x60];
    MPoint sortPos;     // 0x78: the first sort button
};

// More of the screen's positions (filled in at start).
struct DinoUiOrganizationLayout2 {
    MPoint sortStep;            // 0x00: to the next sort button
    MPoint buttonPos;           // 0x08: the back and OK buttons' (one at a time)
    MPoint pageLPos;            // 0x10
    MPoint pageRPos;            // 0x18
    u8 unk20[0x30 - 0x20];
    MPoint memberMenuOffset;    // 0x30: from the member's medal
    MPoint nameOffset;          // 0x38: of the creature's name from its medal or slot
    MPoint cursorOffset;        // 0x40: of the cursor's frame on a medal or slot
};

class DinoUiOrganizationMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    DinoUiOrganizationMenu(u32 team);
    virtual ~DinoUiOrganizationMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    void CreateSlots();
    void CreateSlot(int index);
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
    // Lists the creatures with any part, in each sort's order, and counts the pages.
    void LoadCreatures();
    // Puts the list's creature at index into the team as member.
    void SetMember(u8 index, u8 member);
    // Swaps the selected member with member, flying their medals across.
    void SwapMember(int member);
    // Picks up the page's slot's creature (if it's revived), to put into a member.
    void PickSlot(int slot);
    // Puts the page's slot's creature into the selected member (if it's revived).
    void PlaceSlot(int slot);
    // Puts the selected slot's creature into the member.
    void PlaceMember(int member);
    void RemoveListLinks();
    void UpdateDetails(u8 index);
    void SetHelpMode(int mode);
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
    void OpenMemberMenu(int member);
    void Back();
    void Confirm();
    void Leave();
    void SelectMember(int index);
    void RemoveMember();
    MTask *CreateRevertTasks(DinoTeam *saved);
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
    MIndicator *mNumbers[15];           // 0x154: the slots' creature numbers
    int mSlotCount;                     // 0x190
    u32 mUnk194;                        // 0x194
    MTaskSet *mCursorTasks;             // 0x198: the cursor's moves
    DinoUiFieldMenuCursor *mCursor;     // 0x19C: hidden while the member menu is open
    DinoUiFieldMenuFraction *mFraction; // 0x1A0: "page n/m"
    u32 mTeamIndex;                     // 0x1A4: the saved team being edited
    int mResult;                        // 0x1A8
    std::vector<u32> mCreatures;        // 0x1AC: the ids listed, a page of 15 at a time
    // The creatures in each sort's order.
    std::vector<u32> mByVoice;          // 0x1B8
    std::vector<u32> mByUnk88;          // 0x1C4
    std::vector<u32> mByLevel;          // 0x1D0
    std::vector<u32> mByUnk02031104;    // 0x1DC
    std::vector<u32> mByUnk0D;          // 0x1E8
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
    u8 mUnk23C;                         // 0x23C: a member is being moved
    u8 mUnk23D;                         // 0x23D
    u8 mUnk23E;                         // 0x23E
    u8 unk23F;
    std::vector<MMotionFile> mMotions;  // 0x240
    MTaskSet *mUnk24C;                  // 0x24C: the details' help areas
    MTaskSet *mUnk250;                  // 0x250: the buttons' help areas
    u8 mUnk254;                         // 0x254
    u8 mUnk255;                         // 0x255
    u8 unk256[2];
    DinoUiPartsTeamTech *mTeamTech;     // 0x258
    MResource<DinoKasekiDefs> mKasekiDefs;  // 0x25C
    DinoHelpDummyItem *mHelp26C;        // 0x26C: help areas on the top screen
    DinoHelpDummyItem *mHelp270;        // 0x270
    DinoHelpDummyItem *mPageHelps[9];   // 0x274: the details page's help areas
    DinoHelpDummyItem *mHelps[6];       // 0x298
    u8 mListLinked;                     // 0x2B0
    u8 mSlotsLinked;                    // 0x2B1
};

class DinoUiOrganizationMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoUiOrganizationMenuItem(DinoUiOrganizationMenu *owner, const std::string &path, MPoint pos, u32 textId);
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
    DinoUiOrganizationOKButton(DinoUiOrganizationMenu *owner);
    virtual ~DinoUiOrganizationOKButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOrganizationBackButton : public DinoUiOrganizationMenuItem {
public:
    DinoUiOrganizationBackButton(DinoUiOrganizationMenu *owner);
    virtual ~DinoUiOrganizationBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOrganizationPageLButton : public DinoUiOrganizationMenuItem {
public:
    DinoUiOrganizationPageLButton(DinoUiOrganizationMenu *owner);
    virtual ~DinoUiOrganizationPageLButton();
    virtual void OnDecide();
};

class DinoUiOrganizationPageRButton : public DinoUiOrganizationMenuItem {
public:
    DinoUiOrganizationPageRButton(DinoUiOrganizationMenu *owner);
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
