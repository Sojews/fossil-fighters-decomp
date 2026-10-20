#ifndef GAME_DINO_UI_KEYBOARD_MENU_H
#define GAME_DINO_UI_KEYBOARD_MENU_H

#include <string>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MTextPicture;
class KasekiPictCache;
class MMotion;
class MIndicator;

class MSmartOam;

class DinoUiKeyboardMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiKeyboardMenu(int mode, u32 arg, int flag);
    virtual ~DinoUiKeyboardMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    MTask *CreateSetFocusTask(MMenuItem *item);
    MTask *CreatePageChangeTask(u32 page);
    void SetPage(int page);
    // Keeps the name (a default one if it's empty) and fades out.
    void OnBackDecided();
    // Keeps the name (without trailing blanks) and fades out.
    void Finish();
    // Shows the name (each character over its reading) and moves the cursor to its place.
    void RefreshName();
    // B deletes the character before the cursor (or, on a blank name, goes to the back button), Y and X move the
    // cursor, START goes to the OK button.
    void CheckKeys();

    MSmartOam *mOam;            // 0xB4
    u8 unkB8[0xE4 - 0xB8];
    MMenuItem *mOkButton;       // 0xE4: START goes to it
    MMenuItem *mUnkE8;          // 0xE8: focused after a page change
    MMenuItem *mBackButton;     // 0xEC: B goes to it while the name is blank
    MIndicator *mChars[19];     // 0xF0: the name's characters
    MTextPicture *mPagePicture; // 0x13C
    u32 unk140;
    MMotion *mFocusFrame;       // 0x144: around the focused function key
    MMotion *mCursor;           // 0x148
    MTaskSet *mFocusTasks;      // 0x14C
    MTaskSet *mUnk150;          // 0x150
    MTaskSet *mUnk154;          // 0x154
    MTask *mUnk158;             // 0x158
    DinoHelpDummyItem *mHelp;   // 0x15C: the help for the name (the player's or the team's)
    u32 unk160;
    DinoHelpDummyItem *mKeysHelp;   // 0x164: the help for the keys
    u8 unk168[0x180 - 0x168];
    MPoint mFocusPos;           // 0x180
    int mMode;                  // 0x188
    u32 mArg;                   // 0x18C
    int mPage;                  // 0x190
    KasekiPictCache *mPictCache;    // 0x194: stopped and waited on before exiting
    u8 unk198[0x19C - 0x198];
    std::wstring mUnk19C;       // 0x19C
    std::wstring mUnk1A8;       // 0x1A8
    std::wstring mUnk1B4[3];    // 0x1B4
    std::wstring mUnk1D8;       // 0x1D8
    u8 unk1E4[0x1E8 - 0x1E4];
    int mCursorPos;             // 0x1E8
    int mMaxLength;             // 0x1EC
    u8 unk1F0[0x1F4 - 0x1F0];
    u8 mUnk1F4;                 // 0x1F4
    u8 mFlag;                   // 0x1F5
    u8 unk1F6[2];
};

#endif
