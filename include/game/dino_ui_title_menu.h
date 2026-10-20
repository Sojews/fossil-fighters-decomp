#ifndef GAME_DINO_UI_TITLE_MENU_H
#define GAME_DINO_UI_TITLE_MENU_H

#include "m/input.h"
#include "m/smart_oam.h"
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/motion.h"
#include "game/help.h"

class DinoUiTitleMenu : public MMenu, public DinoHelpBase {
public:
    DinoUiTitleMenu(MInput *input, int *result) : MMenu(input, 0) {
        mResult = result;
    }
    virtual ~DinoUiTitleMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    int *mResult;           // 0xB4
    MSmartOam *mOam;        // 0xB8: scrolled into place first
    MMotion *mMotion;       // 0xBC
    MMenuItem *mStartItem;  // 0xC0
    u32 mUnkC4;
    u32 mIdleTime;   // 0xC8: frames without input; the demo starts after 1200
    u8 mSilentFocus; // 0xCC
};

class Dino3DLoadCsv;
class DinoUiOpeningChara;

class DinoUiOpening : public MProcess {
public:
    DinoUiOpening(MInput *input);
    virtual ~DinoUiOpening();
    virtual void Execute();
    virtual void Idle();

    void LoadScene();
    void StateNextScene();
    void StateShowScene();
    void StateWaitScene();
    // Slides the characters and the logo away, fades out to white and exits (quickly if skipped).
    void End();

    MInput *mInput;     // 0x74
    void (DinoUiOpening::*mState)(); // 0x78
    u8 unk80[0x88 - 0x80];
    MSmartOam *mOam;    // 0x88
    void *mUnk8C;       // 0x8C
    u32 unk90;
    Dino3DLoadCsv *mLoad;   // 0x94: the scene loaded next
    u32 mScene;         // 0x98
    MTaskSet *mLoadTasks;   // 0x9C
    MTaskSet *mShowTasks;   // 0xA0
    u8 mSkipped;        // 0xA4
    std::vector<void *> mUnkA8; // 0xA8
    u32 mSceneCount;    // 0xB4
    int mSceneFrames;   // 0xB8
    DinoUiOpeningChara *mChara1;    // 0xBC
    DinoUiOpeningChara *mChara2;    // 0xC0: (may be missing)
};

#endif
