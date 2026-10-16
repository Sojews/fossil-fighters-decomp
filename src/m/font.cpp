// MFont, MRawFont, M1BitFont, M4BitFont, MCompositeFont and the glyph transfers. Not decompiled yet: text
// measuring (std::wstring), MFontSet (resources) and the glyph transfers' Transfer functions.

#include "m/font.h"
#include <algorithm>
#include "m/looper.h"

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

MRawFont::MRawFont(const MFontData *data) {
    mData = data;
    mCachedCode = 0;
    mCachedGlyph = NULL;
}

int MRawFont::GetHeight() {
    return mData->height;
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
void M1BitFont::Draw(MChrBlock *chr, u32 tile, MPoint pos, MPoint size, u32 code) {
    const u8 *glyph = FindGlyph(code);
    const u8 *bits = glyph + 2;
    if (bits != NULL) {
        gLooper->AddTransferTask(new MChr16BinFontTransfer(bits, GetGlyphSize(code), pos, size, GetColor(),
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

void M4BitFont::Draw(MChrBlock *chr, u32 tile, MPoint pos, MPoint size, u32 code) {
    const u8 *glyph = FindGlyph(code);
    const u8 *bits = glyph + 2;
    if (bits != NULL) {
        gLooper->AddTransferTask(new MChr16FontTransfer(bits, GetGlyphSize(code), pos, size, mOwnColors,
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

int MCompositeFont::GetHeight() {
    return mFonts[0]->GetHeight();
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

void MCompositeFont::Draw(MChrBlock *chr, u32 tile, MPoint pos, MPoint size, u32 code) {
    for (std::vector<MFont *>::iterator it = mFonts.begin(); it != mFonts.end(); it++) {
        if ((*it)->HasGlyph(code)) {
            (*it)->Draw(chr, tile, pos, size, code);
            return;
        }
    }
}
