// The game's standard window: a frame drawn on a text layer from the common window graphics.
#ifndef GAME_DINO_COMMON_WINDOW_H
#define GAME_DINO_COMMON_WINDOW_H

#include "m/view.h"
#include "m/picture.h"

class MText;

class DinoCommonWindow : public MFixedSizeView {
public:
    DinoCommonWindow(u32 extraTiles, bool battle, bool large);
    virtual ~DinoCommonWindow();
    virtual void Execute();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();
    virtual void SetPosition(int x, int y);

    void Draw(MPoint pos, MRect rect);
    // Tiles b with the a rect of the window graphics.
    void Unk0201c098(MRect a, MRect b);
    // Draws rect's border with the frame tiles at pos.
    void Unk0201be50(MPoint pos, MRect rect);
    // Sets the window's rect and draws it.
    void SetFrame(MRect rect);
    // Unk0201c098 with the frame inside the window's rect.
    void Unk0201bb5c();
    // A task that opens the window out of the rect.
    MTask *CreateOpenTask(MRect from);
    // Draws the rect's rows in one of the flash phases.
    void Flush(MRect rect, u8 phase);
    // A task that closes the window into the rect.
    MTask *CreateCloseTask(MRect to);

    MDisplay *mDisplay;     // 0x18
    MText *mText;           // 0x1C
    MChr *mChr;             // 0x20
    MChrBlock *mChrBlock;   // 0x24
    MPalUnit *mPalBlock;   // 0x28
    MPictureFile mFile;     // 0x2C
    MPoint mPos;            // 0x60
    MPoint mTileSize;       // 0x68: of the window graphics
    MRect mFrame;           // 0x70
    u8 mBattle;             // 0x80
};

#endif
