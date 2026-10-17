// The help sample: a practice screen for the help feature (touch an item while help is on to read about it).
#ifndef GAME_DINO_HELP_SAMPLE_H
#define GAME_DINO_HELP_SAMPLE_H

#include "m/task.h"
#include "m/menu.h"
#include "m/menu_item.h"
#include "game/help.h"

class MMotion;
class MSmartOam;
class MConsole;
class MTextPicture;

// The "yes"/"no" buttons of the quit question.
class DinoHelpSampleYesNoItem : public MMenuItem, public DinoHelpItem {
public:
    DinoHelpSampleYesNoItem(MSmartOam *oam, MPoint pos, const char *path, u32 textId);
    virtual ~DinoHelpSampleYesNoItem() {}
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    MMotion *mMotion;   // 0x94
    u32 mTextId;        // 0x98
};

// Asks whether to leave the sample.
class DinoHelpSampleYesNoMenu : public DinoHelpBase, public MMenu {
public:
    DinoHelpSampleYesNoMenu(MTask *sample);
    virtual ~DinoHelpSampleYesNoMenu() {}
    virtual void OnStart();
    virtual void OnDecide(MMenuItem *item);

    MTask *mSample;                 // 0xB4: exited too on "yes"
    MSmartOam *mOam;                // 0xB8
    DinoHelpSampleYesNoItem *mYes;  // 0xBC
    DinoHelpSampleYesNoItem *mNo;   // 0xC0
    MTextPicture *mBg;              // 0xC4
};

// The text box on the top screen that describes the button last pressed.
class DinoHelpSampleConsole : public MProcess, public DinoHelpItem {
public:
    DinoHelpSampleConsole(MDisplay *display);
    virtual ~DinoHelpSampleConsole() {}
    virtual void Idle();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    void SetText(const char *text);

    MConsole *mConsole;     // 0x80
    MTextPicture *mBg;      // 0x84
};

// One of the four sample buttons.
class DinoHelpSampleMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoHelpSampleMenuItem(MSmartOam *oam, MPoint pos, const char *path, DinoHelpSampleConsole *console,
                           u32 descTextId, u32 helpTextId);
    virtual ~DinoHelpSampleMenuItem() {}
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    MMotion *mMotion;                   // 0x94
    DinoHelpSampleConsole *mConsole;    // 0x98
    u32 mDescTextId;                    // 0x9C: shown in the console when pressed
    u32 mHelpTextId;                    // 0xA0
};

class DinoHelpSampleMenu : public DinoHelpBase, public MMenu {
public:
    DinoHelpSampleMenu();
    virtual ~DinoHelpSampleMenu() {}
    virtual void OnStart();
    virtual void OnIdle();

    MSmartOam *mOam;    // 0xB4
};

class DinoHelpSample : public MProcess {
public:
    DinoHelpSample();
    virtual ~DinoHelpSample() {}
    virtual void Idle();
};

#endif
