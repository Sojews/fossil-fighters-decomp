// Canvases drawn with sprites: the canvas is cut into OBJ-sized pieces, each with its own OAM entry and tiles.
#ifndef M_OAM_CANVAS_H
#define M_OAM_CANVAS_H

#include <vector>
#include "m/canvas.h"
#include "m/display.h"
#include "m/obj.h"
#include "m/smart_oam.h"

// TOam hands out the OAM entries (MOam, or MSmartOam for sorted entries) in TUnits.
template <class TOam, class TUnit>
class MOamCanvas : public MCanvas {
public:
    // A sprite of the canvas.
    struct Piece {
        MRect rect;     // 0x00: in the canvas
        int shape;      // 0x10: size * 4 + shape (an index into gObjSizes)
        u32 chrSize;    // 0x14: tiles
        u32 chrOffset;  // 0x18: tiles into the canvas's characters
    };

    MOamCanvas(MDisplay *display, TOam *oam, MPoint size, const void *palette, BOOL fromEnd);
    virtual ~MOamCanvas();
    virtual void DrawChar(MFont *font, MPoint pos, u32 code);
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Hide();
    virtual void Show();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();
    virtual void Clear();
    virtual void ClearRect(MRect rect);
    virtual void FillRect(MRect rect, int color);
    virtual int GetPixel(int x, int y);
    virtual void SetPixel(int x, int y, int color);
    virtual void GetLine(int y, void *dest);
    virtual void SetLine(int y, const void *src);
    virtual void Execute();

    MPalUnit *GetPalBlock();
    TUnit *GetOamUnit();
    void SetPosition(int x, int y);
    // Cuts a rectangle into pieces, biggest sprites first.
    void Layout(MRect rect);
    void AddPiece(MRect rect, int shape);
    void Alloc(BOOL fromEnd);

    MDisplay *mDisplay;             // 0x18
    TOam *mOam;                     // 0x1C
    MPoint mPos;                    // 0x20
    std::vector<Piece> mPieces;     // 0x28
    std::vector<MRect> mRects;      // 0x34: still to cut
    MPalUnit *mPalBlock;           // 0x40
    MChrBlock *mChrBlock;           // 0x44
    TUnit *mUnit;                   // 0x48
};

class MObjCanvas : public MOamCanvas<MOam, MOamUnit> {
public:
    MObjCanvas(MDisplay *display, MOam *oam, MPoint size, const void *palette, BOOL fromEnd)
        : MOamCanvas<MOam, MOamUnit>(display, oam, size, palette, fromEnd) {}
    // With the display's sprites.
    MObjCanvas(MDisplay *display, const MPoint &size, const void *palette, BOOL fromEnd)
        : MOamCanvas<MOam, MOamUnit>(display, display->mObj->mOam, size, palette, fromEnd) {}
    virtual ~MObjCanvas() {}
};

class MSmartObjCanvas : public MOamCanvas<MSmartOam, MSmartOamUnit> {
public:
    MSmartObjCanvas(MDisplay *display, MSmartOam *oam, MPoint size, const void *palette, BOOL fromEnd)
        : MOamCanvas<MSmartOam, MSmartOamUnit>(display, oam, size, palette, fromEnd) {}
    // On the sprites' display.
    MSmartObjCanvas(MSmartOam *oam, MPoint size, const void *palette, BOOL fromEnd)
        : MOamCanvas<MSmartOam, MSmartOamUnit>(oam->mDisplay, oam, size, palette, fromEnd) {}
    virtual ~MSmartObjCanvas() {}

    void SetAnchor(int anchor);
};

#endif
