#ifndef GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_MENU_H
#define GAME_DINO_UI_CONTAINER_BASE_MENU_ITEM_MENU_H

#include "game/dino_ui3_parts_fraction.h"
#include "m/archive.h"
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

class DinoUiContainerBaseMenu : public DinoHelpBase, public MMenu {
public:
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

    DinoUi3PartsFraction *mFraction; // 0xB4: "page n/m"
    u8 mFlags[0x40];            // 0xB8
    int mPage;                  // 0xF8
    int mLastPage;              // 0xFC
    u8 mCount;                  // 0x100: how many are marked
    u8 unk101[3];
    u32 mUnk104[1];
    MArchivePtr mArchive;       // 0x108
    MArchiveFile mFile;         // 0x10C
    u32 mUnk118[2];             // 0x118
    std::vector<void *> mItems; // 0x120: this page's items
    u32 mUnk12C[2];             // 0x12C
    MMenuItem *mBack;           // 0x134
    MMenuItem *mLinkFrom;       // 0x138: linked to the first page item
    u32 mUnk13C[3];
    std::vector<u32> mUnk148;   // 0x148: the entries
    u8 mSelected;               // 0x154: 0xFF for none
    u8 mIdled;                  // 0x155: set once the menu has idled (decides only count after)
};

#endif
