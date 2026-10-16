// DinoCommonFont.

#include "game/dino_common_font.h"
#include "game/dino_resource.h"

DinoCommonFont::DinoCommonFont(int color, int shadow, int back) {
    mFonts[0] = gDinoResource->CreateFont12(color, shadow, back);
    mFonts[1] = gDinoResource->CreateFont24(color, shadow, back);
    for (u32 i = 0; i < 2; i++) {
        mFonts[i]->AddRef();
    }
    mCurrent = mFonts[0];
}

DinoCommonFont::~DinoCommonFont() {
    for (u32 i = 0; i < 2; i++) {
        mFonts[i]->Release();
    }
}

void DinoCommonFont::Select(int size) {
    mCurrent = mFonts[size];
}

int DinoCommonFont::GetEncoding() {
    return mCurrent->GetEncoding();
}

BOOL DinoCommonFont::HasGlyph(u32 code) {
    return mCurrent->HasGlyph(code);
}

MPoint DinoCommonFont::GetMaxSize() {
    return mCurrent->GetMaxSize();
}

MPoint DinoCommonFont::GetMinSize() {
    return mCurrent->GetMinSize();
}

MPoint DinoCommonFont::GetGlyphSize(u32 code) {
    return mCurrent->GetGlyphSize(code);
}

void DinoCommonFont::SetColor(int color) {
    MFont::SetColor(color);
    for (u32 i = 0; i < 2; i++) {
        mFonts[i]->SetColor(color);
    }
}

void DinoCommonFont::SetShadowColor(int color) {
    MFont::SetShadowColor(color);
    for (u32 i = 0; i < 2; i++) {
        mFonts[i]->SetShadowColor(color);
    }
}

void DinoCommonFont::SetBackColor(int color) {
    MFont::SetBackColor(color);
    for (u32 i = 0; i < 2; i++) {
        mFonts[i]->SetBackColor(color);
    }
}

void DinoCommonFont::Draw(MChrBlock *chr, u32 tile, MPoint pos, MPoint size, u32 code) {
    mCurrent->Draw(chr, tile, pos, size, code);
}
