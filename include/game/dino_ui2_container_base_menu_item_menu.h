#ifndef GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_MENU_H
#define GAME_DINO_UI2_CONTAINER_BASE_MENU_ITEM_MENU_H

#include "m/smart_oam.h"
#include "m/blend.h"
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

class DinoUi2FieldMenuCursor;
class DinoUi2PartsRockName;

class DinoUi2ContainerBaseMenuItem;

class DinoUi2ContainerBaseMenu : public DinoHelpBase, public MMenu {
public:
    DinoUi2ContainerBaseMenu();
    void CreateDecideButton(const char *motion, u32 textId);
    MSmartOam *GetOam() { return mOam; }
    virtual ~DinoUi2ContainerBaseMenu();
    virtual BOOL TryDecide() = 0;   // the decide button was pressed; FALSE if nothing happens
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    virtual void Setup() = 0;
    virtual void GetEntry(u32 index, DinoBackupEntry *entry) = 0;
    virtual BOOL CanMark() = 0;     // FALSE once the most entries are marked
    virtual BOOL IsMarkable(DinoBackupEntry *entry) = 0;
    virtual MTask *CreateUnk2C() = 0;

    u8 mFlags[0x41];                // 0xB3: marked entries (members start in MMenu's tail padding)
    int mPage; // 0xF4
    int mLastPage; // 0xF8
    u8 mUnkFC;                      // 0xFC: how many are marked
    u8 mUnkFD;                      // 0xFD: the most that can be
    u32 mUnk100;                    // 0x100
    MSmartOam *mOam;                // 0x104
    u32 mUnk108;                    // 0x108
    std::vector<void *> mItems;     // 0x10C: this page's items
    u32 mUnk118[2];                 // 0x118
    DinoUi2ContainerBaseMenuItem *mBack; // 0x120
    DinoUi2ContainerBaseMenuItem *mLinkFrom; // 0x124: the decide button, linked to the first page item
    MTaskSet *mCursorTasks;         // 0x128
    DinoUi2FieldMenuCursor *mCursor; // 0x12C
    DinoUi2PartsRockName *mRockName; // 0x130
    std::vector<unsigned short> mUnk134; // 0x134: (the element type is a guess, distinct from other overlays')
    u8 mSelected;                   // 0x140: 0xFF for none
    u8 mIdled;                      // 0x141: set once the menu has idled (decides only count after)
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    void ClearItems();
    void RefreshPage2();
    u8 GetFlag(int index);
    void SetFlag(int index, u8 value);
    void RefreshMarks();
    void ResetDecideButton();
    void AddHelpEntries(u32 backText);
};

class DinoUi2CannonShootMenu : public DinoUi2ContainerBaseMenu {
public:
    DinoUi2CannonShootMenu(std::vector<void *> *chosen);
    virtual ~DinoUi2CannonShootMenu();
    virtual BOOL TryDecide();
    virtual void Setup();
    virtual void GetEntry(u32 index, DinoBackupEntry *entry);
    virtual BOOL CanMark();
    virtual BOOL IsMarkable(DinoBackupEntry *entry);
    virtual MTask *CreateUnk2C();
    void FinishDecide();

    BOOL mConfirmed;                // 0x144: the yes/no answer
    std::vector<void *> *mChosen;   // 0x148: gets the indices of the marked entries
};

#endif
