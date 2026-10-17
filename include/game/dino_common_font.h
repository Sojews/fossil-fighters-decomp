// The game's text font: 12 and 24 pixel font sets, one of which draws at a time.
#ifndef GAME_DINO_COMMON_FONT_H
#define GAME_DINO_COMMON_FONT_H

#include "m/font.h"

class DinoCommonFont : public MFont {
public:
    DinoCommonFont(int color, int shadow, int back);
    virtual int GetEncoding();
    virtual MPoint GetMaxSize();
    virtual MPoint GetMinSize();
    virtual MPoint GetGlyphSize(u32 code);
    virtual BOOL HasGlyph(u32 code);
    virtual void SetColor(int color);
    virtual void SetShadowColor(int color);
    virtual void SetBackColor(int color);
    virtual void Draw(MChrBlock *chr, u32 tile, MPoint size, MPoint pos, u32 code);
    virtual ~DinoCommonFont();

    void Select(int size);

    MFont *mCurrent;    // 0x14
    MFont *mFonts[2];   // 0x18
};

#endif
