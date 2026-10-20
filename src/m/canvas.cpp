// MCanvas, MCanvasWindow, MDummyCanvas and MTextCanvas. Not decompiled yet: MCanvas's text drawing
// (std::wstring) and MTextCanvas's construction and destruction (they set up the BG).

#include "m/canvas.h"
#include "m/text.h"
#include "m/looper.h"
#include "m/text_codec.h"
#include "nitro/mi.h"
#include "m/bg.h"
#include "m/pal.h"
#include "m/chr.h"

MCanvasWindow::MCanvasWindow(MCanvas *canvas, MRect rect)
    : MCanvas(MPoint(rect.right - rect.left, rect.bottom - rect.top)) {
    mCanvas = canvas;
    mRect.left = rect.left;
    mRect.top = rect.top;
    mRect.right = rect.right;
    mRect.bottom = rect.bottom;
}

void MCanvasWindow::DrawChar(MFont *font, MPoint pos, u32 code) {
    mCanvas->DrawChar(font, pos + MPoint(mRect.left, mRect.top), code);
}

void MCanvasWindow::SetPosition(MPoint pos) {
    mRect.MoveTo(pos);
}

MPoint MCanvasWindow::GetPosition() {
    return MPoint(mRect.left, mRect.top);
}

void MCanvasWindow::SetPriority(int priority) {
}

int MCanvasWindow::GetPriority() {
    return mCanvas->GetPriority();
}

void MCanvasWindow::Show() {
}

void MCanvasWindow::Hide() {
}

BOOL MCanvasWindow::IsHidden() {
    return FALSE;
}

u32 MCanvasWindow::GetPlaneMask() {
    return mCanvas->GetPlaneMask();
}

void MCanvasWindow::Clear() {
    mCanvas->ClearRect(mRect);
}

void MCanvasWindow::ClearRect(MRect rect) {
    MRect r = rect;
    r.Offset(MPoint(mRect.left, mRect.top));
    mCanvas->ClearRect(r);
}

void MCanvasWindow::FillRect(MRect rect, int color) {
    MRect r = rect;
    r.Offset(MPoint(mRect.left, mRect.top));
    mCanvas->FillRect(r, color);
}

int MCanvasWindow::GetPixel(int x, int y) {
    return mCanvas->GetPixel(x + mRect.left, y + mRect.top);
}

void MCanvasWindow::SetPixel(int x, int y, int color) {
    mCanvas->SetPixel(x + mRect.left, y + mRect.top, color);
}

void MCanvasWindow::GetLine(int y, void *dest) {
    u8 line[256];
    mCanvas->GetLine(y - mRect.top, line);
    MI_CpuCopy8(line + mRect.left, dest, mRect.right - mRect.left);
}

void MCanvasWindow::SetLine(int y, const void *src) {
    u8 line[256];
    mCanvas->GetLine(y - mRect.top, line);
    MI_CpuCopy8(src, line + mRect.left, mRect.right - mRect.left);
    mCanvas->SetLine(y - mRect.top, line);
}

void MCanvasWindow::Execute() {
}

MCanvas::MCanvas(MPoint size) : MFixedSizeView(size) {
}

MCanvas::~MCanvas() {
}

void MCanvas::DrawText(MFont *font, MPoint pos, int spacing, const std::string &text) {
    DrawText(font, pos, spacing, MDecodeText(text, font->GetEncoding()));
}

// Draws characters left to right until one would go past the right edge.
void MCanvas::DrawText(MFont *font, MPoint pos, int spacing, const std::wstring &text) {
    for (const wchar_t *p = text.begin(); p != text.end(); p++) {
        wchar_t c = *p;
        int width = font->GetGlyphSize(c).x;
        if (pos.x + width > GetWidth()) {
            break;
        }
        if (pos.y < GetHeight()) {
            DrawChar(font, pos, c);
        }
        pos.x += width + spacing;
    }
}

MCanvasWindow *MCanvas::CreateWindow(MRect rect) {
    return new MCanvasWindow(this, rect);
}

MDummyCanvas::MDummyCanvas(MPoint size) : MCanvas(size) {
    mPos.x = 0;
    mPos.y = 0;
    mHidden = FALSE;
    mPriority = 0;
}

void MDummyCanvas::DrawChar(MFont *font, MPoint pos, u32 code) {
}

void MDummyCanvas::SetPosition(MPoint pos) {
    mPos = pos;
}

MPoint MDummyCanvas::GetPosition() {
    return mPos;
}

void MDummyCanvas::SetPriority(int priority) {
    mPriority = priority;
}

int MDummyCanvas::GetPriority() {
    return mPriority;
}

void MDummyCanvas::Show() {
    mHidden = FALSE;
}

void MDummyCanvas::Hide() {
    mHidden = TRUE;
}

BOOL MDummyCanvas::IsHidden() {
    return mHidden;
}

u32 MDummyCanvas::GetPlaneMask() {
    return 0;
}

void MDummyCanvas::Clear() {
}

void MDummyCanvas::ClearRect(MRect rect) {
}

void MDummyCanvas::FillRect(MRect rect, int color) {
}

int MDummyCanvas::GetPixel(int x, int y) {
    return 0;
}

void MDummyCanvas::SetPixel(int x, int y, int color) {
}

void MDummyCanvas::GetLine(int y, void *dest) {
}

void MDummyCanvas::SetLine(int y, const void *src) {
}

void MDummyCanvas::Execute() {
}

// Reads one row of pixels (4 bytes a tile) across a canvas's tiles.
void MCanvas::ReadRow(MChrBlock *chr, int start, int tilesWide, int y, u32 *dest) {
    int row = y >> 3;
    int index = start + row * tilesWide;
    u8 *p = (u8 *)chr->addr + index * chr->unitSize + ((y & 7) << 2);
    while (tilesWide > 0) {
        *dest++ = *(u32 *)p;
        p += 32;
        tilesWide--;
    }
}

void MCanvas::WriteRow(MChrBlock *chr, int start, int tilesWide, int y, const u32 *src) {
    u8 *base = (u8 *)chr->addr;
    int row = y >> 3;
    int col = y & 7;
    int index = start + row * tilesWide;
    u8 *p = base + index * chr->unitSize + col * 4;
    while (tilesWide > 0) {
        *(u32 *)p = *src;
        p += 0x20;
        src++;
        tilesWide--;
    }
}

MTextCanvas::MTextCanvas(MPoint size, const void *palette, int backColor, BOOL fromEnd) : MCanvas(size) {
    mDisplay = gCurrentDisplay;
    mBackColor = backColor;
    Init(palette, fromEnd);
}

MTextCanvas::MTextCanvas(MDisplay *display, MPoint size, const void *palette, int backColor, BOOL fromEnd)
    : MCanvas(size) {
    mDisplay = display;
    mBackColor = backColor;
    Init(palette, fromEnd);
}

void MTextCanvas::Init(const void *palette, BOOL fromEnd) {
    u32 count = (GetHeight() + 7) >> 3;
    mTileSize.x = (GetWidth() + 7) >> 3;
    mTileSize.y = count;
    count *= mTileSize.x;
    MBg *bg = mDisplay->mBg;
    mPal = bg->mPal->Alloc(1, palette, fromEnd);
    mChr = bg->CreateChr(0, count + 1, fromEnd);
    mBlank = mChr->Alloc(1, 1);
    mTiles = mChr->Alloc(count, 1);
    mBlank->Fill(0, 0, 0xFFFFFFFF);
    mText = new MSmartText(mDisplay, fromEnd);
    mText->mText->SetChr(mChr);
    mText->Fill(MRect(0x80000000, 0x80000000, 0x7FFFFFFF, 0x7FFFFFFF), mBlank->index, mPal->index, FALSE, FALSE);
    mText->TransferSequence(mTileSize, MPoint(0, 0), mTiles->index, mPal->index, FALSE, FALSE);
    Clear();
}

MTextCanvas::~MTextCanvas() {
    delete mText;
    MBg *bg = mDisplay->mBg;
    bg->DeleteChr(mChr);
    bg->mPal->Free(mPal);
}

void MTextCanvas::Execute() {
    if (mText->mState == STATE_RUN) {
        mText->Execute();
    }
}

void MTextCanvas::DrawChar(MFont *font, MPoint pos, u32 code) {
    font->Draw(mTiles, 0, mTileSize, pos, code);
}

void MTextCanvas::SetPosition(MPoint pos) {
    mText->SetScroll(MPoint(-pos.x, -pos.y));
}

MPoint MTextCanvas::GetPosition() {
    return MPoint(-mText->mScroll.x, -mText->mScroll.y);
}

void MTextCanvas::SetPriority(int priority) {
    mText->mText->mBgCntBits.priority = priority;
}

int MTextCanvas::GetPriority() {
    return mText->mText->mBgCntBits.priority;
}

void MTextCanvas::Clear() {
    mTiles->Fill(mBackColor, 0, -1);
}

void MTextCanvas::ClearRect(MRect rect) {
    gLooper->AddTransferTask(new MChr16Fill(mBackColor, mTiles->addr, mTileSize, rect));
}

void MTextCanvas::FillRect(MRect rect, int color) {
    gLooper->AddTransferTask(new MChr16Fill(color, mTiles->addr, mTileSize, rect));
}

int MTextCanvas::GetPixel(int x, int y) {
    MChrBlock *tiles = mTiles;
    u32 row = *(u32 *)(tiles->addr + tiles->unitSize * ((x >> 3) + mTileSize.x * (y >> 3)) + (y & 7) * 4);
    return (row >> ((x & 7) * 4)) & 0xF;
}

// NONMATCHING: the game loads the tile block's fields later (scheduling).
void MTextCanvas::SetPixel(int x, int y, int color) {
    MChrBlock *tiles = mTiles;
    u32 *row = (u32 *)(tiles->addr + tiles->unitSize * ((x >> 3) + mTileSize.x * (y >> 3))) + (y & 7);
    int shift = (x & 7) * 4;
    *row = (*row & ~(0xF << shift)) | (color << shift);
}

void MTextCanvas::GetLine(int y, void *dest) {
    ReadRow(mTiles, 0, mTileSize.x, y, (u32 *)dest);
}

void MTextCanvas::SetLine(int y, const void *src) {
    WriteRow(mTiles, 0, mTileSize.x, y, (const u32 *)src);
}

void MTextCanvas::Hide() {
    mText->mText->Hide();
}

void MTextCanvas::Show() {
    mText->mText->Show();
}

BOOL MTextCanvas::IsHidden() {
    return mText->mText->IsHidden();
}

u32 MTextCanvas::GetPlaneMask() {
    return 1 << mText->mText->mBg;
}
