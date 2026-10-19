#ifndef GAME_DINO_UI_MUSEUM_H
#define GAME_DINO_UI_MUSEUM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/picture.h"
#include "m/lcd.h"
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

class MSmartOam;
class DinoUiMuseum : public MProcess {
public:
    DinoUiMuseum();
    virtual ~DinoUiMuseum();
    virtual void Idle();
};

class DinoUiFieldMenuFraction;
class DinoUiFieldMenuCursor;
class DinoUiPartsReviverName;
class DinoUiMuseum3DModel;

class DinoUiMuseumMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    DinoUiMuseumMenu();
    virtual ~DinoUiMuseumMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    void CreateSlots();
    void LinkSlots();
    void OnBackDecided();
    void CreateSlot(u32 index);
    void ClearSlots();
    void LoadPageMotions();
    // Lists the creatures revived (with a head) and sorts them.
    void LoadCreatures();
    void PageRight();
    void FinishPageRight();
    void PageLeft();
    void FinishPageLeft();
    void SelectTab(u8 tab);
    void LoadTab(u8 tab);
    void ShowInformation();
    void OpenInformation();
    void CloseInformation();

    u8 unkB3[1];            // members start in MMenu's tail padding
    MSmartOam *mOam;        // 0xB4: the items' sprites
    MMenuItem *mLinkFrom;   // 0xB8: linked down to the slots
    u8 unkBC[0xC4 - 0xBC];
    MMenuItem *mTabs[5];    // 0xC4
    MPictureFile *mPicture; // 0xD8
    MTextPicture *mInformation; // 0xDC: the fossil information shown
    MExtAffinePicture *mTabPicture; // 0xE0: scrolled to the tab's part
    MMenuItem *mSlots[15];  // 0xE4
    u8 unk120[0x15C - 0x120];
    int mSlotCount;     // 0x15C
    u8 unk160[0x164 - 0x160];
    MTaskSet *mCursorTasks;             // 0x164
    DinoUiFieldMenuCursor *mCursor;     // 0x168
    DinoUiFieldMenuFraction *mFraction; // 0x16C: "page n/m"
    DinoUiPartsReviverName *mName;      // 0x170: the focused slot's creature
    MTaskSet *mHelpTasks;               // 0x174
    std::vector<u32> mCreatures;        // 0x178: one slot each
    std::vector<u32> mByVoice;          // 0x184: mCreatures by their cries
    std::vector<u32> mByUnk88;          // 0x190
    u32 mPage;                          // 0x19C
    u32 mPageCount;                     // 0x1A0: the last page
    u8 mTab;                            // 0x1A4
    int mSelected;  // 0x1A8: the fossil whose information is shown
    u8 mUnk1AC;     // 0x1AC: whether the slots can be chosen
    u8 unk1AD[3];
    DinoUiMuseum3DModel *mModel;        // 0x1B0: waited on before exiting
    std::vector<MMotionFile> mMotions;  // 0x1B4: the page's slots' motions
    MTaskSet *mUnk1C0;                  // 0x1C0
};

class MExtAffinePicture;

// The path of the light's picture.
extern "C" const char data_ov006_02196648[];

class DinoUiMuseumEffectLight : public MProcess {
public:
    // On the top screen, translucent, below its place.
    DinoUiMuseumEffectLight() : MProcess(gLcd->GetTopDisplay()), mAngle(0), mTimer(0) {
        MDisplayScope scope(GetDisplay());
        mPicture = new MTextPicture(std::string(data_ov006_02196648), TRUE, false);
        mPicture->SetPriority(1);
        mPicture->SetPosition(0, -0x60);
        mParallel.AddTail(mPicture);
        gCurrentDisplay->mBlend->SetMode(1);
        gCurrentDisplay->mBlend->SetAlpha(16, 16);
        MBlend *blend = gCurrentDisplay->mBlend;
        blend->SetTarget1(mPicture->GetPlaneMask());
        gCurrentDisplay->mBlend->SetTarget2(0x3F);
        mHeight = 0;
    }
    virtual ~DinoUiMuseumEffectLight();
    virtual void Idle();

    MTextPicture *mPicture;         // 0x74
    u16 mAngle;                     // 0x78: of the bobbing
    int mHeight;                    // 0x7C: risen so far
    u32 mTimer;                     // 0x80: frames left to rise
};

class DinoUiMuseumMenu;

class DinoUiMuseumMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoUiMuseumMenuItem(DinoUiMuseumMenu *owner, const std::string &path, MPoint pos, u32 textId);
    virtual ~DinoUiMuseumMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiMuseumMenu *mOwner;  // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiMuseumBackButton : public DinoUiMuseumMenuItem {
public:
    virtual ~DinoUiMuseumBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMuseumPageLButton : public DinoUiMuseumMenuItem {
public:
    virtual ~DinoUiMuseumPageLButton();
    virtual void OnDecide();
};

class DinoUiMuseumPageRButton : public DinoUiMuseumMenuItem {
public:
    virtual ~DinoUiMuseumPageRButton();
    virtual void OnDecide();
};

class DinoUiMuseumSlotCreateTask : public MProcess {
public:
    DinoUiMuseumSlotCreateTask(DinoUiMuseumMenu *menu);
    virtual ~DinoUiMuseumSlotCreateTask();
    virtual void Idle();

    DinoUiMuseumMenu *mMenu;    // 0x74
    u32 mIndex;         // 0x78
};

class DinoUiMuseumInformationMenu;

class DinoUiMuseumInformationMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoUiMuseumInformationMenuItem(DinoUiMuseumInformationMenu *owner, const std::string &path, MPoint pos, u32 textId);
    virtual ~DinoUiMuseumInformationMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiMuseumInformationMenu *mOwner; // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiMuseumInformationBackButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMuseumInformationPageLButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationPageLButton();
    virtual void OnDecide();
};

class DinoUiMuseumInformationPageRButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationPageRButton();
    virtual void OnDecide();
};

class DinoUiMuseumInformationVoiceButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationVoiceButton();
    virtual void OnDecide();
};

class DinoUiMuseumReviverButton : public MMenuItem {
public:
    virtual ~DinoUiMuseumReviverButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    MMotion *mMotion;   // 0x88
    u32 mUnk8C;         // 0x8C
    u8 mBaseAnime;      // 0x90
};

#endif
