// Canvases: views that can be drawn into pixel by pixel (4 bits a pixel), by character and by rectangle.
#ifndef M_CANVAS_H
#define M_CANVAS_H

#include "m/view.h"
#include "m/font.h"
#include "m/smart_text.h"

struct MPalUnit;
#include <string>

class MCanvasWindow;

class MCanvas : public MFixedSizeView {
public:
    MCanvas(MPoint size);
    virtual ~MCanvas() = 0;
    virtual void DrawChar(MFont *font, MPoint pos, u32 code) = 0;
    virtual void Clear() = 0;
    virtual void ClearRect(MRect rect) = 0;
    virtual void FillRect(MRect rect, int color) = 0;
    virtual int GetPixel(int x, int y) = 0;
    virtual void SetPixel(int x, int y, int color) = 0;
    virtual void GetLine(int y, void *dest) = 0;
    virtual void SetLine(int y, const void *src) = 0;

    // Reads or writes one pixel row of a block of 16-color tiles laid out tilesWide to a row.
    void ReadRow(MChrBlock *chr, int start, int tilesWide, int y, u32 *dest);
    void WriteRow(MChrBlock *chr, int start, int tilesWide, int y, const u32 *src);

    void DrawText(MFont *font, MPoint pos, int spacing, const std::string &text);
    void DrawText(MFont *font, MPoint pos, int spacing, const std::wstring &text);
    MCanvasWindow *CreateWindow(MRect rect);
};

// A rectangle of another canvas.
class MCanvasWindow : public MCanvas {
public:
    MCanvasWindow(MCanvas *canvas, MRect rect);
    virtual ~MCanvasWindow() {}
    virtual void Execute();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();
    virtual void DrawChar(MFont *font, MPoint pos, u32 code);
    virtual void Clear();
    virtual void ClearRect(MRect rect);
    virtual void FillRect(MRect rect, int color);
    virtual int GetPixel(int x, int y);
    virtual void SetPixel(int x, int y, int color);
    virtual void GetLine(int y, void *dest);
    virtual void SetLine(int y, const void *src);

    MCanvas *mCanvas;   // 0x18
    MRect mRect;        // 0x1C
};

// A canvas that draws nothing.
class MDummyCanvas : public MCanvas {
public:
    MDummyCanvas(MPoint size);
    virtual ~MDummyCanvas() {}
    virtual void Execute();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();
    virtual void DrawChar(MFont *font, MPoint pos, u32 code);
    virtual void Clear();
    virtual void ClearRect(MRect rect);
    virtual void FillRect(MRect rect, int color);
    virtual int GetPixel(int x, int y);
    virtual void SetPixel(int x, int y, int color);
    virtual void GetLine(int y, void *dest);
    virtual void SetLine(int y, const void *src);

    MPoint mPos;        // 0x18
    u8 mHidden;         // 0x20
    int mPriority;      // 0x24
};

// A canvas on a text BG, each tile of the screen its own character.
class MTextCanvas : public MCanvas {
public:
    // A canvas on the current display's text layer: palette (data) and tiles allocated from the end if fromEnd.
    MTextCanvas(MPoint size, const void *palette, int backColor, BOOL fromEnd);
    MTextCanvas(MDisplay *display, MPoint size, const void *palette, int backColor, BOOL fromEnd);
    virtual ~MTextCanvas();
    virtual void Execute();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();
    virtual void DrawChar(MFont *font, MPoint pos, u32 code);
    virtual void Clear();
    virtual void ClearRect(MRect rect);
    virtual void FillRect(MRect rect, int color);
    virtual int GetPixel(int x, int y);
    virtual void SetPixel(int x, int y, int color);
    virtual void GetLine(int y, void *dest);
    virtual void SetLine(int y, const void *src);

    void Init(const void *palette, BOOL fromEnd);

    MDisplay *mDisplay;     // 0x18
    MSmartText *mText;      // 0x1C
    MChr *mChr;             // 0x20
    MChrBlock *mBlank;      // 0x24
    MChrBlock *mTiles;      // 0x28
    MPalUnit *mPal;        // 0x2C
    MPoint mTileSize;       // 0x30
    int mBackColor;         // 0x38
};

// Fills a rectangle of 16-color tiles laid out row by row.
class MChr16Fill : public MTransfer {
public:
    MChr16Fill(int color, u32 base, MPoint tiles, MRect rect) {
        mPattern = color | (color << 4);
        mBase = base;
        mTiles = tiles;
        mSize = rect.GetSize();
        mOrigin = rect.GetTopLeft();
    }
    virtual ~MChr16Fill();
    virtual void Transfer();

    u32 mPattern;       // 0x10: the color in both nibbles
    u32 mBase;          // 0x14
    MPoint mTiles;      // 0x18: tiles across and down
    MPoint mSize;       // 0x20: pixels
    MPoint mOrigin;     // 0x28
};

#endif
