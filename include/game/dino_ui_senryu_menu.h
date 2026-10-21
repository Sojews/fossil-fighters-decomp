#ifndef GAME_DINO_UI_SENRYU_MENU_H
#define GAME_DINO_UI_SENRYU_MENU_H

#include <string>
#include "m/menu_item.h"
#include "m/font.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"

class MSmartOam;
class MIndicator;
class MMotion;
#include "game/dino_field_camera_hook.h"

class DinoUiSenryuMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiSenryuMenu();
    virtual ~DinoUiSenryuMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    MTask *CreateCurChangeTask(u32 cur);
    // B focuses the back button, or presses it once it's focused.
    void CheckKeys();
    void OnBackDecided();
    // The sub menu's choice for the entry: 1 sets the record's flag, 2 clears it.
    void OnSubMenuChoice(u32 choice, u32 index);

    MSmartOam *mOam;            // 0xB4
    MIndicator *mLines[3][3];   // 0xB8: three lines per poem on the page
    MIndicator *mNames[3];      // 0xDC: each poem's author
    MIndicator *mPageText;      // 0xE8
    MView *mCursor;     // 0xEC
    MMotion *mFrames[3];        // 0xF0
    MMotion *mMarks[3];         // 0xFC
    u32 mUnk108;        // 0x108
    u32 mUnk10C;        // 0x10C
    MTaskSet *mUnk110;  // 0x110
    MTaskSet *mUnk114;  // 0x114
    MTaskSet *mUnk118;  // 0x118
    MTask *mUnk11C;     // 0x11C
    MFont *mFont120;    // 0x120
    MFont *mFont124;    // 0x124
    MFont *mFont128;    // 0x128
    MMenuItem *mBackButton; // 0x12C
    u32 mPage;          // 0x130
    u32 mUnk134;        // 0x134
    u32 mChoice;        // 0x138: the sub menu's answer
    u8 mUnk13C;         // 0x13C
    u8 mSilentFocus;    // 0x13D
    std::wstring mUnk140[126];  // 0x140
    std::wstring mUnk728[42];   // 0x728
    s32 mEntries[42];   // 0x920: the record index << 8, and flags
    u32 mCount;         // 0x9C8: how many entries
    u32 unk9CC;
};

#endif
