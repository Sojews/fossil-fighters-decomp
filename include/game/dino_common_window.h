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

    MDisplay *mDisplay;     // 0x18
    MText *mText;           // 0x1C
    MChr *mChr;             // 0x20
    MChrBlock *mChrBlock;   // 0x24
    MPalUnit *mPalBlock;   // 0x28
    MPictureFile mFile;     // 0x2C
    MPoint mPos;            // 0x60
    MPoint mTileSize;       // 0x68: of the window graphics
    MPoint mUnk70;          // 0x70
    MPoint mUnk78;          // 0x78
    u8 mBattle;             // 0x80
};

#endif
