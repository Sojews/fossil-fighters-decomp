// MOamCanvas, a template over the two kinds of OAM allocator, and the classes made of it.
// NONMATCHING (several functions): mostly register allocation and loop strength reduction; the game reloads the
// piece count every iteration of the piece loops.

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
    MPoint size = font->GetGlyphSize(code);
    MRect glyph(pos, pos + size);
    for (u32 i = 0; i < mPieces.size(); i++) {
        Piece &piece = mPieces[i];
        MRect rect = piece.rect;
        if (rect.right > glyph.left && glyph.right > rect.left && rect.bottom > glyph.top &&
            glyph.bottom > rect.top) {
            font->Draw(mChrBlock, piece.chrOffset,
                       MPoint((piece.rect.right - piece.rect.left) / 8, (piece.rect.bottom - piece.rect.top) / 8),
                       MPoint(pos.x - piece.rect.left, pos.y - piece.rect.top), code);
        }
    }
}

template <class TOam, class TUnit>
MPalBlock *MOamCanvas<TOam, TUnit>::GetPalBlock() {
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
        MPoint p(pos.x + mPieces[i].rect.left, pos.y + mPieces[i].rect.top);
        typename TUnit::Entry *entry = &mUnit->GetEntries()[i];
        entry->SetCoord(p);
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
        MRect r = mPieces[i].rect;
        if (r.right > rect.left && rect.right > r.left && r.bottom > rect.top && rect.bottom > r.top) {
            Piece &piece = mPieces[i];
            MPoint tiles((piece.rect.right - piece.rect.left) / 8, (piece.rect.bottom - piece.rect.top) / 8);
            MPoint origin(-piece.rect.left, -piece.rect.top);
            MRect p = piece.rect;
            MRect clip(rect.left > p.left ? rect.left : p.left, rect.top > p.top ? rect.top : p.top,
                       rect.right < p.right ? rect.right : p.right, rect.bottom < p.bottom ? rect.bottom : p.bottom);
            clip.Offset(origin);
            gLooper->AddTransferTask(
                new MChr16Fill(color, mChrBlock->addr + mChrBlock->unitSize * piece.chrOffset, tiles, clip));
        }
    }
}

template <class TOam, class TUnit>
int MOamCanvas<TOam, TUnit>::GetPixel(int x, int y) {
    for (u32 i = 0; i < mPieces.size(); i++) {
        if (mPieces[i].rect.Contains(MPoint(x, y))) {
            Piece &piece = mPieces[i];
            int px = x - piece.rect.left;
            int py = y - piece.rect.top;
            u32 tile = piece.chrOffset + ((px >> 3) + (py >> 3) * ((piece.rect.right - piece.rect.left) >> 3));
            u32 row = *(u32 *)(mChrBlock->addr + mChrBlock->unitSize * tile + (py & 7) * 4);
            return (row >> ((px & 7) * 4)) & 0xF;
        }
    }
    return 0;
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::SetPixel(int x, int y, int color) {
    for (u32 i = 0; i < mPieces.size(); i++) {
        Piece &piece = mPieces[i];
        int left = piece.rect.left;
        int right = piece.rect.right;
        if (left <= x && x < right && piece.rect.top <= y && y < piece.rect.bottom) {
            int px = x - piece.rect.left;
            int py = y - piece.rect.top;
            u32 tile = piece.chrOffset + (px >> 3) + ((right - left) >> 3) * (py >> 3);
            u32 *row = (u32 *)(mChrBlock->addr + mChrBlock->unitSize * tile + (py & 7) * 4);
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
    int h = rect.bottom - rect.top;
    int w = rect.right - rect.left;
    int sw = gObjSizes[0][shape].x;
    int sh = gObjSizes[0][shape].y;
    Piece piece;
    piece.rect = MRect(MPoint(rect.left, rect.top), MPoint(rect.left + sw, rect.top + sh));
    piece.shape = shape;
    mPieces.push_back(piece);
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
        MRect r = mRects.back();
        mRects.erase(std::vector<MRect>::iterator(&mRects.back()));
        int h = r.bottom - r.top;
        int w = r.right - r.left;
        int best = -1;
        int fitting = -1;
        int bestArea = 0x80000000;
        int fittingArea = 0x80000000;
        if (MPoint(w, h) == MPoint(8, 8)) {
            AddPiece(r, 0);
        } else {
            int i;
            const MPoint *size = &gObjSizes[3][2];
            for (i = 14; i > 0; i--, size--) {
                MPoint s = *size;
                if (s == MPoint(0, 0)) {
                    continue;
                }
                if (s.x == w && s.y == h) {
                    AddPiece(r, i);
                    goto next;
                }
                int area = s.x * s.y;
                if (s.x <= w && s.y <= h) {
                    if (area > fittingArea) {
                        fittingArea = area;
                        fitting = i;
                    }
                } else if (area > bestArea && area < (h < s.y ? h : s.y) * (w < s.x ? w : s.x) * 2) {
                    bestArea = area;
                    best = i;
                }
            }
            if (best == -1) {
                AddPiece(r, fitting);
            } else {
                AddPiece(r, best);
            }
        }
    next:;
    }
}

template <class TOam, class TUnit>
void MOamCanvas<TOam, TUnit>::Alloc(BOOL fromEnd) {
    u32 offset = 0;
    MChr *chr = mDisplay->mObj->mChr;
    for (u32 i = 0; i < mPieces.size(); i++) {
        Piece &piece = mPieces[i];
        u32 boundary = chr->mBoundary;
        u32 tiles = gObjTileSizes[0][piece.shape].x * gObjTileSizes[0][piece.shape].y;
        u32 size;
        if (chr->mMode == 0) {
            size = (tiles + boundary - 1) / boundary * boundary;
        } else {
            size = (tiles * 2 + boundary - 1) / boundary * boundary / 2;
        }
        piece.chrSize = size;
        mPieces[i].chrOffset = offset;
        offset += mPieces[i].chrSize;
    }
    mUnit = mOam->Alloc(mPieces.size(), fromEnd);
    mChrBlock = chr->Alloc(0, offset, fromEnd);
    for (u32 i = 0; i < mPieces.size(); i++) {
        int shape = mPieces[i].shape;
        typename TUnit::Entry *entry = &mUnit->GetEntries()[i];
        entry->SetShape(shape);
        entry->SetPalette(mPalBlock);
        u32 tile = (mChrBlock->index + mPieces[i].chrOffset) / chr->mBoundary;
        if (entry->colors256 == 1) {
            tile *= 2;
        }
        entry->tile = tile;
    }
}

template class MOamCanvas<MOam, MOamUnit>;
template class MOamCanvas<MSmartOam, MSmartOamUnit>;

void MSmartObjCanvas::SetAnchor(int anchor) {
    GetOamUnit()->SetAnchor(anchor);
}
