// MFont, MRawFont, M1BitFont, M4BitFont, MCompositeFont and the glyph transfers. Not decompiled yet: text
// measuring (std::wstring), MFontSet (resources) and the glyph transfers' Transfer functions.

#include "m/font.h"
#include <algorithm>
#include "m/looper.h"
#include "m/text_codec.h"

static int sMeasureMin[3] = {0, 1, 1};

MFont::MFont() {
    mRefCount = 0;
    mColor = 0;
    mShadowColor = 0;
    mBackColor = 0;
}

MFont::~MFont() {
}

void MFont::AddRef() {
    mRefCount++;
}

void MFont::Release() {
    if (--mRefCount <= 0) {
        delete this;
    }
}

const void *MFont::GetPalette() {
    return NULL;
}

void MFont::SetColor(int color) {
    mColor = color;
}

void MFont::SetShadowColor(int color) {
    mShadowColor = color;
}

void MFont::SetBackColor(int color) {
    mBackColor = color;
}

int MFont::GetColor() {
    return mColor;
}

int MFont::GetShadowColor() {
    return mShadowColor;
}

int MFont::GetBackColor() {
    return mBackColor;
}

void MFont::SetColors(int color, int shadow, int back) {
    SetColor(color);
    SetShadowColor(shadow);
    SetBackColor(back);
}

MPoint MFont::MeasureText(std::string text, int spacing, int lineSpacing) {
    return MeasureText(MDecodeText(text, GetEncoding()), spacing, lineSpacing);
}

// The size of text: the widest line by the lines' total height (lines end at '\n').
MPoint MFont::MeasureText(const std::wstring &text, int spacing, int lineSpacing) {
    int lines = 0;
    int height = 0;
    int width = 0;
    const wchar_t *p = text.begin();
    while (p != text.end()) {
        int count = 0;
        MPoint line(0, 0);
        while (*p != 0 && *p != '\n') {
            MPoint size = GetGlyphSize(*p);
            line.x += size.x;
            line.y = std::max(line.y, size.y);
            p++;
            count++;
        }
        if (count != 0) {
            line.x += (count - 1) * spacing;
        }
        width = std::max(width, line.x);
        height += line.y;
        if (p == text.end()) {
            break;
        }
        lines++;
        p++;
    }
    height += lines * lineSpacing;
    return MPoint(std::max(sMeasureMin[2], width), std::max(sMeasureMin[1], height));
}

MRawFont::MRawFont(const MFontData *data) {
    mData = data;
    mCachedCode = 0;
    mCachedGlyph = NULL;
}

int MRawFont::GetEncoding() {
    return mData->encoding;
}

MPoint MRawFont::GetMaxSize() {
    const MFontData *data = mData;
    MPoint size;
    size.x = data->maxSize.x;
    size.y = data->maxSize.y;
    return size;
}

MPoint MRawFont::GetMinSize() {
    const MFontData *data = mData;
    MPoint size;
    size.x = data->maxSize.x;
    size.y = data->maxSize.y;
    return size;
}

MPoint MRawFont::GetGlyphSize(u32 code) {
    const u8 *glyph = FindGlyph(code);
    if (glyph == NULL) {
        return MPoint(0, 0);
    }
    return MPoint(glyph[0], glyph[1]);
}

const MFontData *MRawFont::GetData() {
    return mData;
}

u32 MRawFont::GetFirstCode() {
    const u8 *table = (const u8 *)mData + mData->tableOffset;
    return (table[0] << 8) | table[1];
}

u32 MRawFont::GetLastCode() {
    const u8 *table = (const u8 *)mData + mData->tableOffset;
    const u8 *entry = table + mData->entrySize * (mData->count - 1);
    return (entry[0] << 8) | entry[1];
}

BOOL MRawFont::HasGlyph(u32 code) {
    if (FindGlyph(code) != NULL) {
        return TRUE;
    }
    return FALSE;
}

// Finds a glyph by binary search, remembering the last one found.
const u8 *MRawFont::FindGlyph(u32 code) {
    u32 &cachedCode = mCachedCode;
    const u8 *&cachedGlyph = mCachedGlyph;

    if (mCachedCode == code) {
        return cachedGlyph;
    }
    if (code < GetFirstCode() || code > GetLastCode()) {
        return NULL;
    }
    const u8 *table = (const u8 *)mData + mData->tableOffset;
    u32 entrySize = mData->entrySize;
    u32 lo = 0;
    u32 hi = mData->count;
    while (lo < hi) {
        u32 mid = (lo + hi) >> 1;
        const u8 *entry = table + mid * entrySize;
        u32 entryCode = (entry[0] << 8) | entry[1];
        if (entryCode == code) {
            cachedCode = code;
            cachedGlyph = entry + 2;
            return entry + 2;
        }
        if (entryCode < code) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    cachedCode = 0;
    cachedGlyph = NULL;
    return NULL;
}

M1BitFont::M1BitFont(const MFontData *data) : MRawFont(data) {
    mUnk20 = (GetData()->flags & 1) ? 1 : 0;
}

// NONMATCHING: the destination address gets r6 instead of r7 (also M4BitFont::Draw).
void M1BitFont::Draw(MChrBlock *chr, u32 tile, MPoint size, MPoint pos, u32 code) {
    const u8 *glyph = FindGlyph(code);
    const u8 *bits = glyph + 2;
    if (bits != NULL) {
        gLooper->AddTransferTask(new MChr16BinFontTransfer(bits, GetGlyphSize(code), size, pos, GetColor(),
                                                           GetShadowColor(), GetBackColor(), mUnk20,
                                                           (void *)(chr->addr + tile * chr->unitSize)));
    }
}

M4BitFont::M4BitFont(const MFontData *data) : MRawFont(data) {
    mOwnColors = FALSE;
}

const void *M4BitFont::GetPalette() {
    const MFontData *data = GetData();
    return (const u8 *)data + data->paletteOffset;
}

void M4BitFont::SetColor(int color) {
    mOwnColors = color == -1 ? TRUE : FALSE;
}

void M4BitFont::Draw(MChrBlock *chr, u32 tile, MPoint size, MPoint pos, u32 code) {
    const u8 *glyph = FindGlyph(code);
    const u8 *bits = glyph + 2;
    if (bits != NULL) {
        gLooper->AddTransferTask(new MChr16FontTransfer(bits, GetGlyphSize(code), size, pos, mOwnColors,
                                                        (void *)(chr->addr + tile * chr->unitSize)));
    }
}

MCompositeFont::MCompositeFont() {
    mMaxSize.x = 0;
    mMaxSize.y = 0;
    mMinSize.x = 0x7FFFFFFF;
    mMinSize.y = 0x7FFFFFFF;
}

MCompositeFont::~MCompositeFont() {
    for (std::vector<MFont *>::iterator it = mFonts.begin(); it != mFonts.end(); it++) {
        (*it)->Release();
    }
}

int MCompositeFont::GetEncoding() {
    return mFonts[0]->GetEncoding();
}

void MCompositeFont::AddFont(MFont *font) {
    font->AddRef();
    mFonts.push_back(font);
    mMaxSize.x = std::max(mMaxSize.x, font->GetMaxSize().x);
    mMaxSize.y = std::max(mMaxSize.y, font->GetMaxSize().y);
    mMinSize.x = std::min(mMinSize.x, font->GetMinSize().x);
    mMinSize.y = std::min(mMinSize.y, font->GetMinSize().y);
}

MPoint MCompositeFont::GetMaxSize() {
    return mMaxSize;
}

MPoint MCompositeFont::GetMinSize() {
    return mMinSize;
}

MPoint MCompositeFont::GetGlyphSize(u32 code) {
    for (std::vector<MFont *>::iterator it = mFonts.begin(); it != mFonts.end(); it++) {
        if ((*it)->HasGlyph(code)) {
            return (*it)->GetGlyphSize(code);
        }
    }
    return MPoint(0, 0);
}

BOOL MCompositeFont::HasGlyph(u32 code) {
    for (std::vector<MFont *>::iterator it = mFonts.begin(); it != mFonts.end(); it++) {
        if ((*it)->HasGlyph(code)) {
            return TRUE;
        }
    }
    return FALSE;
}

void MCompositeFont::SetColor(int color) {
    MFont::SetColor(color);
    for (std::vector<MFont *>::iterator it = mFonts.begin(); it != mFonts.end(); it++) {
        (*it)->SetColor(color);
    }
}

void MCompositeFont::SetShadowColor(int color) {
    MFont::SetShadowColor(color);
    for (std::vector<MFont *>::iterator it = mFonts.begin(); it != mFonts.end(); it++) {
        (*it)->SetShadowColor(color);
    }
}

void MCompositeFont::SetBackColor(int color) {
    MFont::SetBackColor(color);
    for (std::vector<MFont *>::iterator it = mFonts.begin(); it != mFonts.end(); it++) {
        (*it)->SetBackColor(color);
    }
}

const void *MCompositeFont::GetPalette() {
    for (std::vector<MFont *>::iterator it = mFonts.begin(); it != mFonts.end(); it++) {
        if ((*it)->GetPalette() != NULL) {
            return (*it)->GetPalette();
        }
    }
    return NULL;
}

void MCompositeFont::Draw(MChrBlock *chr, u32 tile, MPoint size, MPoint pos, u32 code) {
    for (std::vector<MFont *>::iterator it = mFonts.begin(); it != mFonts.end(); it++) {
        if ((*it)->HasGlyph(code)) {
            (*it)->Draw(chr, tile, size, pos, code);
            return;
        }
    }
}

MFontSet::MFontSet(std::string path, int color, int shadow, int back) : mResource(path) {
    for (u32 i = 0; i < GetHeader()->count; i++) {
        if (GetFontData(i)->count != 0) {
            switch (GetFontData(i)->type) {
            default:
                OS_Terminate();
                break;
            case 0:
                AddFont(new M1BitFont(GetFontData(i)));
                break;
            case 1:
                AddFont(new M4BitFont(GetFontData(i)));
                break;
            }
        }
    }
    SetColors(color, shadow, back);
}

// A glyph row's four pixels per halfword, read as a word (the glyph data is only halfword aligned).
static inline u32 MReadGlyphWord(const u32 *src) {
    return ((const u16 *) src)[0] | (((const u16 *) src)[1] << 16);
}

// Draws a 4-bit glyph into 4-bit tiles at a pixel position, clipped to the destination; with its own colors, the
// glyph's zero pixels are transparent.
// NONMATCHING: the game hoists `words - 1` and the masks' complements out of the row loop (ours recomputes them each
// row), which also shifts its register and stack choices.
void MChr16FontTransfer::Transfer() {
    const u32 *src = (const u32 *) mSrc;
    int srcWords = (mGlyphSize.x + 7) >> 3;
    if (mPos.y < 0) {
        src += -mPos.y * srcWords;
        mGlyphSize.y += mPos.y;
        mPos.y = 0;
    }
    int over = mPos.y + mGlyphSize.y - mSize.y * 8;
    if (over > 0) {
        mGlyphSize.y -= over;
    }
    int rows = mGlyphSize.y;
    int width = mSize.x;
    u32 *dst = (u32 *) ((u8 *) mDest + ((mPos.x >> 3) + (mPos.y >> 3) * width) * 32 + (mPos.y & 7) * 4);
    int shift = mPos.x & 7;
    u32 firstMask = ~((1 << (shift * 4)) - 1);
    int end = shift + mGlyphSize.x;
    int endShift = end & 7;
    u32 lastMask = (1 << (endShift * 4)) - 1;
    if (endShift == 0) {
        lastMask = 1 - 2;
    }
    u32 words = (end + 7) >> 3;
    int nextRow = -words * 8 + 1;
    int nextTileRow = nextRow + (width - 1) * 8;
    u32 first = mPos.x < 0 ? -(mPos.x >> 3) : 0;
    int overX = mPos.x + mGlyphSize.x - width * 8;
    u32 last = overX > 0 ? words - ((overX + 7) >> 3) : words;
    shift *= 4;
    if (!mOwnColors) {
        while (rows != 0) {
            u32 carry = 0;
            for (u32 i = 0; i < words; i++) {
                u32 word = MReadGlyphWord(src);
                u32 out = (word << shift) | (carry >> (32 - shift));
                if (i >= first && i < last) {
                    if (i == 0 && i == words - 1) {
                        *dst = (*dst & ~(lastMask & firstMask)) | (out & (lastMask & firstMask));
                    } else if (i == 0) {
                        *dst = (*dst & ~firstMask) | (out & firstMask);
                    } else if (i == words - 1) {
                        *dst = (*dst & ~lastMask) | (out & lastMask);
                    } else {
                        *dst = out;
                    }
                }
                carry = word;
                src++;
                dst += 8;
            }
            src -= words - srcWords;
            dst = (u32 *) ((u8 *) dst + ((((u8 *) dst - (u8 *) mDest) & 0x1F) == 0x1C ? nextTileRow * 4 : nextRow * 4));
            rows--;
        }
    } else {
        while (rows != 0) {
            u32 carry = 0;
            for (u32 i = 0; i < words; i++) {
                u32 word = MReadGlyphWord(src);
                u32 out = (word << shift) | (carry >> (32 - shift));
                // A nibble mask of the nonzero pixels.
                u32 mask = (((((~out >> 1) & 0x77777777) + out) & 0x88888888) >> 3) + 0x77777777;
                mask ^= 0x77777777;
                if (i >= first && i < last) {
                    if (i == 0 && i == words - 1) {
                        mask &= lastMask & firstMask;
                        *dst = (*dst & ~mask) | (out & mask);
                    } else if (i == 0) {
                        *dst = (*dst & ~(firstMask & mask)) | (firstMask & out & mask);
                    } else if (i == words - 1) {
                        *dst = (*dst & ~(lastMask & mask)) | (lastMask & out & mask);
                    } else {
                        *dst = (*dst & ~mask) | (out & mask);
                    }
                }
                carry = word;
                src++;
                dst += 8;
            }
            src -= words - srcWords;
            dst = (u32 *) ((u8 *) dst + ((((u8 *) dst - (u8 *) mDest) & 0x1F) == 0x1C ? nextTileRow * 4 : nextRow * 4));
            rows--;
        }
    }
}

MChr16BinFontTransfer::~MChr16BinFontTransfer() {
}

MChr16FontTransfer::~MChr16FontTransfer() {
}
