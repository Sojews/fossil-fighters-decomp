#ifndef GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_MENU_H
#define GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_MENU_H

#include "game/dino_ui3_parts_fraction.h"
#include "m/resource.h"
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

struct DinoBackupEntry;

class DinoUi3PartsCursor;
class DinoUi3PartsRockName;

class DinoUiContainerBaseMenuItem;

class DinoUiContainerBaseMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiContainerBaseMenu();
    virtual ~DinoUiContainerBaseMenu();
    virtual BOOL TryDecide() = 0;   // the decide button was pressed; FALSE if nothing happens
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    virtual void Setup() = 0;
    virtual void GetEntry(u32 index, DinoBackupEntry *entry) = 0;
    virtual BOOL CanMark() = 0;     // FALSE when no more entries can be marked
    virtual BOOL IsMarkable(DinoBackupEntry *entry) = 0;
    virtual MTask *CreateUnk2C() = 0;
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    void ClearItems();
    void RefreshPage2();
    u8 GetFlag(int index);
    void SetFlag(int index, u8 flag);
    void RefreshMarks();
    void ResetDecideButton();
    void HidePageButtons();
    void AddHelpEntries(u32 backText);
    void CreateDecideButton(const char *motion, u32 textId);
    MSmartOam *GetOam() { return mOam; }

    DinoUi3PartsFraction *mFraction; // 0xB4: "page n/m"
    u8 mFlags[0x40];            // 0xB8
    int mPage;                  // 0xF8
    int mLastPage;              // 0xFC
    u8 mCount;                  // 0x100: how many are marked
    u8 mUnk101;                 // 0x101
    u8 unk102[2];
    u32 mUnk104;                // 0x104
    MResource<u8> mDefs;        // 0x108: the definitions the entries are looked up in
    MSmartOam *mOam;            // 0x118
    u32 mUnk11C;                // 0x11C
    std::vector<void *> mItems; // 0x120: this page's items
    DinoUiContainerBaseMenuItem *mPageL; // 0x12C
    DinoUiContainerBaseMenuItem *mPageR; // 0x130
    DinoUiContainerBaseMenuItem *mBack;  // 0x134
    DinoUiContainerBaseMenuItem *mLinkFrom; // 0x138: the decide button, linked to the first page item
    MTaskSet *mCursorTasks;     // 0x13C
    DinoUi3PartsCursor *mCursor; // 0x140
    DinoUi3PartsRockName *mRockName; // 0x144
    std::vector<u32> mUnk148;   // 0x148: the entries
    u8 mSelected;               // 0x154: 0xFF for none
    u8 mIdled;                  // 0x155: set once the menu has idled (decides only count after)
};

#endif
