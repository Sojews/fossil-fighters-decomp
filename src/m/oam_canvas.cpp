// MOamCanvas, a template over the two kinds of OAM allocator, and the classes made of it.
// NONMATCHING (several functions): mostly register allocation and loop strength reduction; the game reloads the
// piece count every iteration of the piece loops.

#pragma ipa file

#include "m/oam_canvas.h"
#include "m/display.h"
#include "m/looper.h"

template <class TOam, class TUnit>
MOamCanvas<TOam, TUnit>::MOamCanvas(MDisplay *display, TOam *oam, MPoint size, const void *palette, BOOL fromEnd)
    : MCanvas(size), mDisplay(display), mOam(oam) {
    mPalBlock = mDisplay->mObj->mPal->Alloc(1, palette, TRUE);
    Layout(MRect(0, 0, (size.x + 7) / 8 * 8, (size.y + 7) / 8 * 8));
    Alloc(fromEnd);
    SetPosition(0, 0);
    Clear();
}

template <class TOam, class TUnit>
MOamCanvas<TOam, TUnit>::~MOamCanvas() {
    mOam->Free(mUnit);
    mDisplay->mObj->mChr->FreeShared(mChrBlock);
    mDisplay->mObj->mPal->Free(mPalBlock);
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::DrawChar(MFont *font, MPoint pos, u32 code) {
    MRect glyph(pos, pos + font->GetGlyphSize(code));
    for (u32 i = 0; i < mPieces.size(); i++) {
        if (glyph.Intersects(mPieces[i].rect)) {
            font->Draw(mChrBlock, mPieces[i].chrOffset, mPieces[i].rect.GetSize() / 8,
                       pos - mPieces[i].rect.GetTopLeft(), code);
        }
    }
}

template <class TOam, class TUnit>
MPalUnit *MOamCanvas<TOam, TUnit>::GetPalBlock() {
    return mPalBlock;
}

template <class TOam, class TUnit>
TUnit *MOamCanvas<TOam, TUnit>::GetOamUnit() {
    return mUnit;
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::SetPosition(int x, int y) {
    SetPosition(MPoint(x, y));
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::SetPosition(MPoint pos) {
    mPos = pos;
    for (u32 i = 0; i < mPieces.size(); i++) {
        mUnit->GetEntries()[i].SetCoord(pos + mPieces[i].rect.GetTopLeft());
    }
}

template <class TOam, class TUnit>
MPoint MOamCanvas<TOam, TUnit>::GetPosition() {
    return mPos;
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::SetPriority(int priority) {
    mUnit->SetPriority(priority);
}

template <class TOam, class TUnit>
int MOamCanvas<TOam, TUnit>::GetPriority() {
    return mUnit->GetEntries()->priority;
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::Hide() {
    mUnit->Hide();
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::Show() {
    mUnit->Show();
}

template <class TOam, class TUnit>
BOOL MOamCanvas<TOam, TUnit>::IsHidden() {
    if (mUnit->GetEntries()->objMode == 2) {
        return TRUE;
    }
    return FALSE;
}

template <class TOam, class TUnit>
u32 MOamCanvas<TOam, TUnit>::GetPlaneMask() {
    return 0x10;
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::Clear() {
    mChrBlock->Fill(0, 0, -1);
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::ClearRect(MRect rect) {
    FillRect(rect, 0);
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::FillRect(MRect rect, int color) {
    for (u32 i = 0; i < mPieces.size(); i++) {
        if (rect.Intersects(mPieces[i].rect)) {
            gLooper->AddTransferTask(new MChr16Fill(
                color, mChrBlock->GetTileAddr(mPieces[i].chrOffset),
                mPieces[i].rect.GetSize() / 8,
                rect.Intersection(mPieces[i].rect).Moved(MPoint(0, 0) - mPieces[i].rect.GetTopLeft())));
        }
    }
}

template <class TOam, class TUnit>
int MOamCanvas<TOam, TUnit>::GetPixel(int x, int y) {
    for (u32 i = 0; i < mPieces.size(); i++) {
        MRect &r = mPieces[i].rect;
        if (r.left <= x && x < r.right && r.top <= y && y < r.bottom) {
            int py = y - mPieces[i].rect.top;
            int px = x - mPieces[i].rect.left;
            u32 tile = mPieces[i].chrOffset + ((px >> 3) + (py >> 3) * ((mPieces[i].rect.right - mPieces[i].rect.left) >> 3));
            u32 row = ((u32 *)mChrBlock->GetTileAddr(tile))[py & 7];
            return (row >> ((px & 7) * 4)) & 0xF;
        }
    }
    return 0;
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::SetPixel(int x, int y, int color) {
    for (u32 i = 0; i < mPieces.size(); i++) {
        Piece &piece = mPieces[i];
        if (piece.rect.left <= x && x < piece.rect.right && piece.rect.top <= y && y < piece.rect.bottom) {
            MPoint tl = piece.rect.GetTopLeft();
            int py = y - tl.y;
            int px = x - tl.x;
            u32 tile = piece.chrOffset + (px >> 3) + (py >> 3) * ((piece.rect.right - piece.rect.left) >> 3);
            u32 *row = (u32 *)mChrBlock->GetTileAddr(tile) + (py & 7);
            int shift = (px & 7) * 4;
            *row = (*row & ~(0xF << shift)) | (color << shift);
        }
    }
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::GetLine(int y, void *dest) {
    for (u32 i = 0; i < mPieces.size(); i++) {
        Piece &piece = mPieces[i];
        if (y >= piece.rect.top && y < piece.rect.bottom) {
            ReadRow(mChrBlock, piece.chrOffset, piece.rect.GetWidth() >> 3, y - piece.rect.top,
                    (u32 *)((u8 *)dest + piece.rect.left));
        }
    }
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::SetLine(int y, const void *src) {
    for (u32 i = 0; i < mPieces.size(); i++) {
        Piece &piece = mPieces[i];
        if (y >= piece.rect.top && y < piece.rect.bottom) {
            WriteRow(mChrBlock, piece.chrOffset, piece.rect.GetWidth() >> 3, y - piece.rect.top,
                     (const u32 *)((const u8 *)src + piece.rect.left));
        }
    }
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::Execute() {
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::AddPiece(MRect rect, int shape) {
    int h = rect.GetHeight();
    s32 w = rect.GetWidth();
    MPoint size = gObjSizes[0][shape];
    int sw = size.x;
    int sh = size.y;
    mPieces.push_back(Piece(MRect(rect.GetTopLeft(), rect.GetTopLeft() + size), shape));
    if (w <= sw && h > sh) {
        mRects.push_back(MRect(rect.left, rect.top + sh, rect.right, rect.bottom));
    } else if (w > sw && h <= sh) {
        mRects.push_back(MRect(rect.left + sw, rect.top, rect.right, rect.bottom));
    } else if (w > sw && h > sh) {
        mRects.push_back(MRect(rect.left, rect.top + sh, rect.right, rect.bottom));
        mRects.push_back(MRect(rect.left + sw, rect.top, rect.right, rect.top + sh));
    }
}
template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::Layout(MRect rect) {
    mRects.push_back(rect);
    while (mRects.size() != 0) {
        rect = *--mRects.end();
        mRects.erase(mRects.end() - 1);
        int w = rect.right - rect.left;
        int h = rect.bottom - rect.top;
        int fittingArea = 0x80000000;
        int bestArea = 0x80000000;
        int fitting = -1;
        int best = -1;
        MPoint sz(w, h);
        if (sz == MPoint(8, 8)) {
            AddPiece(rect, 0);
        } else {
            int i;
            const MPoint *size = &gObjSizes[3][2];
            for (i = 14; i > 0; i--, size--) {
                const MPoint s = *size;
                if (s == MPoint(0, 0)) {
                    continue;
                }
                if (s.x == w && s.y == h) {
                    AddPiece(rect, i);
                    goto next;
                }
                int area = s.x * s.y;
                if (s.x <= w && s.y <= h) {
                    if (area > fittingArea) {
                        fittingArea = area;
                        fitting = i;
                    }
                } else if (area > bestArea && area < std::min(s.y, h) * std::min(s.x, w) * 2) {
                    bestArea = area;
                    best = i;
                }
            }
            if (best == -1) {
                AddPiece(rect, fitting);
            } else {
                AddPiece(rect, best);
            }
        }
    next:;
    }
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::Alloc(BOOL fromEnd) {
    u32 i;
    u32 offset = 0;
    MChr *chr = mDisplay->mObj->mChr;
    for (i = 0; i < mPieces.size(); i++) {
        Piece &piece = mPieces[i];
        u32 tiles = gObjTileSizes[0][piece.shape].x * gObjTileSizes[0][piece.shape].y;
        u32 size;
        if (chr->mMode == 0) {
            size = (tiles + chr->mBoundary - 1) / chr->mBoundary * chr->mBoundary;
        } else {
            size = (tiles * 2 + chr->mBoundary - 1) / chr->mBoundary * chr->mBoundary / 2;
        }
        piece.chrSize = size;
        mPieces[i].chrOffset = offset;
        offset += mPieces[i].chrSize;
    }
    mUnit = mOam->Alloc(mPieces.size(), fromEnd);
    mChrBlock = chr->Alloc(0, offset, fromEnd);
    for (i = 0; i < mPieces.size(); i++) {
        int shape = mPieces[i].shape;
        mUnit->GetEntries()[i].SetShape(shape);
        mUnit->GetEntries()[i].SetPalette(mPalBlock);
        u32 tile = (mChrBlock->index + mPieces[i].chrOffset) / chr->mBoundary;
        if (mUnit->GetEntries()[i].colors256 == 1) {
            tile *= 2;
        }
        mUnit->GetEntries()[i].tile = tile;
    }
}
template class MOamCanvas<MOam, MOamUnit>;
template class MOamCanvas<MSmartOam, MSmartOamUnit>;

void MSmartObjCanvas::SetAnchor(int anchor) {
    GetOamUnit()->SetAnchor(anchor);
}
