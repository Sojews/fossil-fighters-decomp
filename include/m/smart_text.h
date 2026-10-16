// A text screen layer that remembers what was drawn on it: each transfer or fill is kept as an item, and every
// frame the items are drawn again where the screen has scrolled into view (or where they were drawn over).
#ifndef M_SMART_TEXT_H
#define M_SMART_TEXT_H

#include <vector>
#include "m/task.h"
#include "m/geometry.h"
#include "m/region.h"

class MText;
class MDisplay;

class MSmartText : public MTask {
public:
    // Something drawn on the layer, covering a rectangle of tiles.
    class Item {
    public:
        Item(MRect rect) : mRect(rect) {}
        virtual ~Item() {}
        // Draws the part of the item in clip (tiles, in layer coordinates).
        virtual void Draw(MText *text, MRect clip) = 0;
        MRect GetRect() const { return mRect; }
        MRect mRect;    // 0x04
    };

    MSmartText(MDisplay *display, BOOL fromEnd);
    virtual ~MSmartText();
    virtual void Execute();

    void Add(Item *item);
    void Erase(MRect rect);
    void Transfer(const u16 *src, MPoint srcSize, MPoint dst, int tile, int palette, bool hflip, bool vflip);
    void TransferSequence(MPoint srcSize, MPoint dst, int tile, int palette, bool hflip, bool vflip);
    void Fill(MRect rect, int tile, int palette, bool hflip, bool vflip);

    void SetScroll(MPoint scroll) { mScroll = scroll; }

    MDisplay *mDisplay;         // 0x10
    MText *mText;               // 0x14
    MPoint mScroll;             // 0x18: pixels
    MRect mPrevView;            // 0x20: the tiles in view last frame
    MRect mView;                // 0x30: the tiles in view
    std::vector<Item *> mItems; // 0x40: oldest first
    MRegion mUpdate;            // 0x4C: what's still to draw this frame
    MRegion mDirty;             // 0x64: tiles erased or drawn over since the last frame
};

// A rectangle of tile map entries copied from memory.
class TextPattern : public MSmartText::Item {
public:
    TextPattern(const u16 *src, MPoint srcSize, MPoint dst, int tile, int palette, bool hflip, bool vflip);
    virtual void Draw(MText *text, MRect clip);

    const u16 *mSrc;    // 0x14
    MPoint mSrcSize;    // 0x18
    MPoint mOffset;     // 0x20: layer position -> source position
    int mTile;          // 0x28
    int mPalette;       // 0x2C
    bool mHFlip;        // 0x30
    bool mVFlip;        // 0x31
};

// A rectangle of consecutive tiles.
class TextSequence : public MSmartText::Item {
public:
    TextSequence(MPoint srcSize, MPoint dst, int tile, int palette, bool hflip, bool vflip);
    virtual void Draw(MText *text, MRect clip);

    MPoint mSrcSize;    // 0x14
    MPoint mOffset;     // 0x1C
    int mTile;          // 0x24
    int mPalette;       // 0x28
    bool mHFlip;        // 0x2C
    bool mVFlip;        // 0x2D
};

// A rectangle filled with one entry.
class TextFill : public MSmartText::Item {
public:
    TextFill(MRect rect, int tile, int palette, bool hflip, bool vflip);
    virtual void Draw(MText *text, MRect clip);

    int mTile;          // 0x14
    int mPalette;       // 0x18
    bool mHFlip;        // 0x1C
    bool mVFlip;        // 0x1D
};

#endif
