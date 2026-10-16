// Fonts: glyph sets that draw characters into OBJ or BG character memory. A raw font is one file of glyphs
// sorted by character code (1 or 4 bits a pixel); a composite font searches several in turn.
#ifndef M_FONT_H
#define M_FONT_H

#include <vector>
#include <string>
#include "m/resource.h"
#include "m/task.h"
#include "m/geometry.h"
#include "m/chr.h"

class MFont {
public:
    MFont();
    virtual int GetEncoding() = 0;
    virtual MPoint GetMaxSize() = 0;
    virtual MPoint GetMinSize() = 0;
    virtual MPoint GetGlyphSize(u32 code) = 0;
    virtual BOOL HasGlyph(u32 code) = 0;
    virtual void SetColor(int color);
    virtual void SetShadowColor(int color);
    virtual void SetBackColor(int color);
    virtual const void *GetPalette();
    virtual void Draw(MChrBlock *chr, u32 tile, MPoint pos, MPoint size, u32 code) = 0;
    virtual ~MFont();

    void AddRef();
    void Release();
    int GetColor();
    int GetShadowColor();
    int GetBackColor();
    void SetColors(int color, int shadow, int back);
    MPoint MeasureText(const std::string &text, int spacing, int lineSpacing);
    MPoint MeasureText(const std::wstring &text, int spacing, int lineSpacing);

    int mRefCount;      // 0x04
    int mColor;         // 0x08
    int mShadowColor;   // 0x0C
    int mBackColor;     // 0x10
};

// The header of a font file.
struct MFontData {
    u32 type;           // 0x00: 0: 1 bit per pixel, 1: 4 bits per pixel
    u32 flags;          // 0x04: bit 0: (1-bit fonts) ...
    int encoding;       // 0x08: the text encoding the font is indexed by
    u32 entrySize;      // 0x0C: bytes per glyph entry
    u32 unk10;
    u32 count;          // 0x14: glyph entries
    u32 unk18;
    u32 unk1C;
    MPoint maxSize;     // 0x20
    u32 unk28;
    u32 paletteOffset;  // 0x2C
    u32 unk30;
    u32 tableOffset;    // 0x34: glyph entries (big-endian code, width, height, bits), sorted by code
};

class MRawFont : public MFont {
public:
    MRawFont(const MFontData *data);
    virtual int GetEncoding();
    virtual MPoint GetMaxSize();
    virtual MPoint GetMinSize();
    virtual MPoint GetGlyphSize(u32 code);
    virtual BOOL HasGlyph(u32 code);
    virtual ~MRawFont() {}

    const MFontData *GetData();
    u32 GetFirstCode();
    u32 GetLastCode();
    const u8 *FindGlyph(u32 code);

    const MFontData *mData;     // 0x14
    u32 mCachedCode;            // 0x18
    const u8 *mCachedGlyph;     // 0x1C
};

class M1BitFont : public MRawFont {
public:
    M1BitFont(const MFontData *data);
    virtual void Draw(MChrBlock *chr, u32 tile, MPoint pos, MPoint size, u32 code);
    virtual ~M1BitFont() {}

    BOOL mUnk20;    // 0x20
};

class M4BitFont : public MRawFont {
public:
    M4BitFont(const MFontData *data);
    virtual void SetColor(int color);
    virtual const void *GetPalette();
    virtual void Draw(MChrBlock *chr, u32 tile, MPoint pos, MPoint size, u32 code);
    virtual ~M4BitFont() {}

    u8 mOwnColors;  // 0x20: draw with the font's own colors (SetColor(-1))
};

class MCompositeFont : public MFont {
public:
    MCompositeFont();
    virtual int GetEncoding();
    virtual MPoint GetMaxSize();
    virtual MPoint GetMinSize();
    virtual MPoint GetGlyphSize(u32 code);
    virtual BOOL HasGlyph(u32 code);
    virtual void SetColor(int color);
    virtual void SetShadowColor(int color);
    virtual void SetBackColor(int color);
    virtual const void *GetPalette();
    virtual void Draw(MChrBlock *chr, u32 tile, MPoint pos, MPoint size, u32 code);
    virtual ~MCompositeFont();

    void AddFont(MFont *font);

    std::vector<MFont *> mFonts;    // 0x14
    MPoint mMaxSize;                // 0x20
    MPoint mMinSize;                // 0x28
};

// The fonts of a font file (a count, then the offsets of the fonts' data), shown as one font.
class MFontSet : public MCompositeFont {
public:
    struct Header {
        u32 unk00;
        u32 count;          // 0x04
        u32 tableOffset;    // 0x08: offsets of the fonts' data
    };

    MFontSet(std::string path, int color, int shadow, int back);

    const Header *GetHeader() { return mResource.Get(); }
    const MFontData *GetFontData(u32 i) {
        const Header *header = GetHeader();
        return (const MFontData *)((const u8 *)header + ((const u32 *)((const u8 *)header + header->tableOffset))[i]);
    }

    MResource<Header> mResource;    // 0x30
};

// Draws a 1-bit glyph into 16-color tiles.
class MChr16BinFontTransfer : public MTransfer {
public:
    MChr16BinFontTransfer(const u8 *src, MPoint glyphSize, MPoint pos, MPoint size, int color, int shadow,
                          int back, BOOL unk, void *dest) {
        mSrc = src;
        mGlyphSize = glyphSize;
        mDest = dest;
        mPos = pos;
        mSize = size;
        mColor = color;
        mShadowColor = shadow;
        mBackColor = back;
        mUnk3C = unk;
    }
    virtual ~MChr16BinFontTransfer() {}
    virtual void Transfer();

    const u8 *mSrc;     // 0x10
    MPoint mGlyphSize;  // 0x14
    void *mDest;        // 0x1C
    MPoint mPos;        // 0x20: where in the destination tiles, in pixels
    MPoint mSize;       // 0x28: the destination's size, in pixels
    int mColor;         // 0x30
    int mShadowColor;   // 0x34
    int mBackColor;     // 0x38
    BOOL mUnk3C;        // 0x3C
};

// Draws a 4-bit glyph into 16-color tiles.
class MChr16FontTransfer : public MTransfer {
public:
    MChr16FontTransfer(const u8 *src, MPoint glyphSize, MPoint pos, MPoint size, u8 ownColors, void *dest) {
        mSrc = src;
        mGlyphSize = glyphSize;
        mDest = dest;
        mPos = pos;
        mSize = size;
        mOwnColors = ownColors;
    }
    virtual ~MChr16FontTransfer() {}
    virtual void Transfer();

    const u8 *mSrc;     // 0x10
    MPoint mGlyphSize;  // 0x14
    void *mDest;        // 0x1C
    MPoint mPos;        // 0x20: where in the destination tiles, in pixels
    MPoint mSize;       // 0x28: the destination's size, in pixels
    u8 mOwnColors;      // 0x30
};

#endif
