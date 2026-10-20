// The talk window and its name panel.
#ifndef GAME_DINO_TALK_WINDOW_H
#define GAME_DINO_TALK_WINDOW_H

#include <string>
#include "m/task.h"
#include "m/fx32.h"
#include "m/geometry.h"

class MConsole;
class MView;
class MSmartOam;
class MMotion;
class MFontSet;

class DinoTalkWindow : public MProcess {
public:
    // oam: the sprites to draw the text on (a text layer canvas if 0); flip: the name panel on the other side.
    DinoTalkWindow(MDisplay *display, std::string speaker, int oam, int priority, u8 flip);
    void SetUnkF4();
    void ClearUnkF4();
    void SetRate(MFx32 rate);
    void ResetRate();
    void Print(const std::string &text);
    BOOL IsEmpty();
    void NextPage();
    void SetUnkAC(u32 value);
    void SetUnkB0(u8 value);
    u8 GetUnkB0();
    // While the text is still typing, stops it there and carries on after the frames.
    void Pause(int frames);
    BOOL IsPageDone();
    void Skip();
    BOOL IsAnimating();
    MTask *CreateCloseTask();
    // Waits until the window has started showing its text.
    MTask *CreateWaitStartTask();
    virtual ~DinoTalkWindow();
    virtual void Idle();

    // Shows the next-page mark while a page waits, and shakes the window (and its name panel) while mUnkAC lasts.
    void StateShow();

    void (DinoTalkWindow::*mState)(); // 0x74
    std::wstring mText; // 0x7C
    MView *mFrame;      // 0x88: an MTextPicture
    u32 mUnk8C;         // 0x8C
    class DinoTalkWindowNamePanel *mNamePanel;  // 0x90
    MConsole *mConsole; // 0x94
    u8 mStarted;        // 0x98
    MPoint mConsolePos; // 0x9C
    MPoint mPos;        // 0xA4
    u32 mUnkAC;         // 0xAC: frames left shaking
    u8 mUnkB0;          // 0xB0
    MTaskSet *mAnims;   // 0xB4: the window's own animations
    MTaskSet *mConsoleTasks;    // 0xB8
    int mOam;           // 0xBC: (an MSmartOam *)
    u8 mFlip;           // 0xC0
    MPoint mOffset;     // 0xC4: where the window is moved to (when flipped)
    MFx32 mRate;        // 0xCC
    u16 mColors[3];     // 0xD0: the speaker's name colors
    u8 unkD6[0xF0 - 0xD6];
    MView *mNextMark;   // 0xF0
    u8 mUnkF4;          // 0xF4: the next-page mark is wanted
};

class DinoTalkWindowNamePanel : public MProcess {
public:
    DinoTalkWindowNamePanel(DinoTalkWindow *owner, std::wstring text, int priority, u8 flip, const void *palette);
    virtual ~DinoTalkWindowNamePanel();
    virtual void Idle();

    DinoTalkWindow *mOwner; // 0x74
    std::wstring mText; // 0x78
    MSmartOam *mOam;    // 0x84
    MConsole *mName;    // 0x88
    MMotion *mPanel;    // 0x8C
    u32 mWidth;         // 0x90: of the name's text
    u32 mPanelWidth;    // 0x94: mWidth rounded up to tiles
    MPoint mNameOffset; // 0x98: of the name in the panel
    MFontSet *mFont;    // 0xA0
    MPoint mPos;        // 0xA4
    u32 mUnkAC;         // 0xAC
    u32 mTiles;         // 0xB0: mPanelWidth in tiles
    MPoint mShake;      // 0xB4
    u8 mFlip;           // 0xBC: on the other side
};

#endif
