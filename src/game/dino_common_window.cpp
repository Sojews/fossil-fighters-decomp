// DinoCommonWindow.

#include "game/dino_common_window.h"
#include "m/bg.h"
#include "m/text.h"
#include "m/chr.h"
#include "m/pal.h"
#include "m/window.h"
#include "game/dino_static_init.h"
#include <algorithm>

// The file's statics, in its size-sorted .data pool (see dino_static_init.h).
struct DinoCommonWindowPool {
    const char *null;           // 0x00
    const char *battleBgPath;   // 0x04
    const char *bgPath;         // 0x08
    MPoint unk0C;               // 0x0C
    MPoint drawPos;             // 0x14
    MPoint bottom;              // 0x1C: the frame tiles, relative to frame
    MPoint topLeft;             // 0x24
    MPoint top;                 // 0x2C
    MPoint frame;               // 0x34
    MPoint bottomRight;         // 0x3C
    MPoint topRight;            // 0x44
    MPoint center;              // 0x4C
    MPoint right;               // 0x54
    MPoint bottomLeft;          // 0x5C
    MPoint left;                // 0x64
    u8 unk6C[8];                // 0x6C: a typeinfo
    MPoint flush;               // 0x74: the flash tiles (three per phase)
};
extern "C" DinoCommonWindowPool data_020ba548;
extern "C" const MPoint data_020ba55c;  // drawPos
extern "C" const MPoint data_020ba57c;  // frame
extern "C" u32 data_020ba5c8[];
extern "C" const MRect data_020ba5e8;   // the inside tile

DinoCommonWindow::DinoCommonWindow(u32 extraTiles, bool battle, bool large) : MFixedSizeView(MPoint(0, 0)) {
    mPos = MPoint(0, 0);
    mFrame = MRect(0, 0, 0, 0);
    mBattle = battle;
    mDisplay = gCurrentDisplay;
    MBg *bg = mDisplay->mBg;
    mFile = MPictureFile(battle ? data_020ba548.battleBgPath : data_020ba548.bgPath, 0);
    SetFixedSize(mFile.GetSize());
    mTileSize = mFile.GetTileSize();
    MPal *pal = bg->mPal;
    mPalBlock = pal->Alloc(mFile.GetPaletteSize(), mFile.GetPalette(), TRUE);
    mChr = bg->CreateChr(mFile.IsUnk04(), extraTiles + mFile.GetChrSize(), TRUE);
    mChrBlock = mChr->Alloc(mFile.GetChrSize(), TRUE);
    mChrBlock->Load(&mFile.GetChrFile(), 0, 0, 0, 0x7FFF, 0);
    mText = bg->CreateText(large ? 3 : 0, TRUE);
    mText->SetChr(mChr);
    mText->mTextCntBits.colors256 = mFile.IsUnk04();
    Draw(data_020ba55c, MRect(0, 0, 64, 64));
}

// Draws the rect's top rows (three when the rect is three tall) in one of the flash phases.
void DinoCommonWindow::Flush(MRect rect, u8 phase) {
    Draw(MPoint(data_020ba548.flush.x + phase * 3, data_020ba548.flush.y), MRect(rect.left, rect.top, rect.left + 1, rect.top + 1));
    Draw(MPoint(data_020ba548.flush.x + phase * 3, data_020ba548.flush.y + 1), MRect(rect.left, rect.top + 1, rect.left + 1, rect.top + 2));
    Draw(MPoint(data_020ba548.flush.x + phase * 3 + 1, data_020ba548.flush.y), MRect(rect.left + 1, rect.top, rect.right - 1, rect.top + 1));
    Draw(MPoint(data_020ba548.flush.x + phase * 3 + 1, data_020ba548.flush.y + 1), MRect(rect.left + 1, rect.top + 1, rect.right - 1, rect.top + 2));
    Draw(MPoint(data_020ba548.flush.x + phase * 3 + 2, data_020ba548.flush.y), MRect(rect.right - 1, rect.top, rect.right, rect.top + 1));
    Draw(MPoint(data_020ba548.flush.x + phase * 3 + 2, data_020ba548.flush.y + 1), MRect(rect.right - 1, rect.top + 1, rect.right, rect.top + 2));
    if (rect.bottom - rect.top == 3) {
        // (phase's column spelled differently: mwcc recomputes it here)
        Draw(MPoint(data_020ba548.flush.x + (phase + phase * 2), data_020ba548.flush.y + 2), MRect(rect.left, rect.top + 2, rect.left + 1, rect.top + 3));
        Draw(MPoint(data_020ba548.flush.x + (phase + phase * 2) + 1, data_020ba548.flush.y + 2), MRect(rect.left + 1, rect.top + 2, rect.right - 1, rect.top + 3));
        Draw(MPoint(data_020ba548.flush.x + (phase + phase * 2) + 2, data_020ba548.flush.y + 2), MRect(rect.right - 1, rect.top + 2, rect.right, rect.top + 3));
    }
}

// Fills rect of the text screen with the window graphics' tile at pos.
void DinoCommonWindow::Draw(MPoint pos, MRect rect) {
    if (rect.left <= rect.right && rect.top <= rect.bottom) {
        u32 base = mChrBlock->index;
        mText->Fill(rect, base + ((const u16 *) mFile.GetScreen())[pos.y * mTileSize.x + pos.x], mPalBlock->index,
                    FALSE, FALSE);
    }
}

void DinoCommonWindow::SetFrame(MRect rect) {
    mFrame.left = rect.left;
    mFrame.top = rect.top;
    mFrame.right = rect.right;
    mFrame.bottom = rect.bottom;
    Unk0201be50(data_020ba57c, rect);
    Unk0201c098(data_020ba5e8, rect.Inflate(-1));
}

void DinoCommonWindow::Unk0201bb5c() {
    Unk0201c098(data_020ba5e8, mFrame.Inflate(-1));
}

DinoCommonWindow::~DinoCommonWindow() {
    MBg *bg = mDisplay->mBg;
    bg->DeleteText(mText);
    bg->DeleteChr(mChr);
    bg->mPal->Free(mPalBlock);
}

void DinoCommonWindow::Execute() {
}

void DinoCommonWindow::SetPosition(int x, int y) {
    SetPosition(MPoint(x, y));
}

void DinoCommonWindow::SetPosition(MPoint pos) {
    mPos = pos;
    mText->SetOffset(MPoint(-mPos.x, -mPos.y));
}

MPoint DinoCommonWindow::GetPosition() {
    return mPos;
}

void DinoCommonWindow::SetPriority(int priority) {
    mText->mBgCntBits.priority = priority;
}

int DinoCommonWindow::GetPriority() {
    return mText->mBgCntBits.priority;
}

void DinoCommonWindow::Show() {
    mText->Show();
}

void DinoCommonWindow::Hide() {
    mText->Hide();
}

BOOL DinoCommonWindow::IsHidden() {
    return mText->IsHidden();
}

u32 DinoCommonWindow::GetPlaneMask() {
    return 1 << mText->mBg;
}

// A one-tile rect at (x, y).
static inline MRect Cell(int x, int y) {
    MRect r(0, 0, 1, 1);
    r.right += x - r.left;
    r.bottom += y - r.top;
    r.left = x;
    r.top = y;
    return r;
}

// Draws rect's border with the frame tiles at pos.
void DinoCommonWindow::Unk0201be50(MPoint pos, MRect rect) {
    Draw(pos + data_020ba548.topLeft, Cell(rect.left, rect.top));
    Draw(pos + data_020ba548.topRight, Cell(rect.right - 1, rect.top));
    Draw(pos + data_020ba548.bottomLeft, Cell(rect.left, rect.bottom - 1));
    Draw(pos + data_020ba548.bottomRight, Cell(rect.right - 1, rect.bottom - 1));
    Draw(pos + data_020ba548.left, MRect(rect.left, rect.top + 1, rect.left + 1, rect.bottom - 1));
    Draw(pos + data_020ba548.top, MRect(rect.left + 1, rect.top, rect.right - 1, rect.top + 1));
    Draw(pos + data_020ba548.right, MRect(rect.right - 1, rect.top + 1, rect.right, rect.bottom - 1));
    Draw(pos + data_020ba548.bottom, MRect(rect.left + 1, rect.bottom - 1, rect.right - 1, rect.bottom));
}

// Tiles dst with the src rect of the window graphics.
static inline bool IsValid(const MRect &r) { return r.left <= r.right && r.top <= r.bottom; }

void DinoCommonWindow::Unk0201c098(MRect src, MRect dst) {
    int x, x0, w, y0, x1, y1, h;
    if (!IsValid(dst)) {
        return;
    }
    w = src.right - src.left;
    h = src.bottom - src.top;
    x0 = dst.left / w;
    y0 = dst.top / h;
    x1 = (dst.right - 1) / w + 1;
    y1 = (dst.bottom - 1) / h + 1;
    for (x = x0; x < x1; x++) {
        for (int y = y0; y < y1; y++) {
            int l = x == x0 ? dst.left % w : 0;
            int r = x == x1 - 1 ? (dst.right - 1) % w + 1 : w;
            int t = y == y0 ? dst.top % h : 0;
            int b = y == y1 - 1 ? (dst.bottom - 1) % h + 1 : h;
            mText->Transfer((const u16 *) mFile.GetScreen(), mTileSize, MRect(l, t, r, b).Moved(src.GetTopLeft()),
                            MPoint(l + x * w, t + y * h), mChrBlock->index, mPalBlock->index, FALSE, FALSE);
        }
    }
}

MTask *DinoCommonWindow::CreateOpenTask(MRect from) {
    MRect frame = MRect(mFrame.left * 8, mFrame.top * 8, mFrame.right * 8, mFrame.bottom * 8).Moved(GetPosition());
    from.left = std::max(frame.left, std::min(frame.right, from.left));
    from.right = std::max(frame.left, std::min(frame.right, from.right));
    from.top = std::max(frame.top, std::min(frame.bottom, from.top));
    from.bottom = std::max(frame.top, std::min(frame.bottom, from.bottom));
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    set->AddTail(mDisplay->mWin->CreateClearTransfer(), mDisplay->mWin->GetWindow1()->CreateShowTransfer(),
                 mDisplay->mWin->GetWindow1()->CreateBlendEnabledTransfer(TRUE),
                 mDisplay->mWin->GetObjWindow()->CreateBlendEnabledTransfer(TRUE),
                 mDisplay->mWin->GetWindow1()->CreateTargetTransfer(0x3F),
                 mDisplay->mWin->GetObjWindow()->CreateTargetTransfer(GetPlaneMask() ^ 0x3F),
                 mDisplay->mWin->GetWindow1()->CreateShapeAnime(from, frame, 8, 1),
                 mDisplay->mWin->CreateClearTransfer());
    return set;
}

MTask *DinoCommonWindow::CreateCloseTask(MRect to) {
    MRect frame = MRect(mFrame.left * 8, mFrame.top * 8, mFrame.right * 8, mFrame.bottom * 8).Moved(GetPosition());
    to.left = std::max(frame.left, std::min(frame.right, to.left));
    to.right = std::max(frame.left, std::min(frame.right, to.right));
    to.top = std::max(frame.top, std::min(frame.bottom, to.top));
    to.bottom = std::max(frame.top, std::min(frame.bottom, to.bottom));
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    set->AddTail(mDisplay->mWin->CreateClearTransfer(), mDisplay->mWin->GetWindow1()->CreateShowTransfer(),
                 mDisplay->mWin->GetWindow1()->CreateBlendEnabledTransfer(TRUE),
                 mDisplay->mWin->GetObjWindow()->CreateBlendEnabledTransfer(TRUE),
                 mDisplay->mWin->GetWindow1()->CreateTargetTransfer(0x3F),
                 mDisplay->mWin->GetObjWindow()->CreateTargetTransfer(GetPlaneMask() ^ 0x3F),
                 mDisplay->mWin->GetWindow1()->CreateShapeTransfer(frame),
                 mDisplay->mWin->GetWindow1()->CreateShapeAnime(frame, to, 8, 1),
                 mDisplay->mWin->CreateClearTransfer());
    return set;
}

// The file's static initialization (see dino_static_init.h).
static struct DinoCommonWindowStaticInit {
    DinoCommonWindowStaticInit() {
        DINO_POOL_WORD(data_020ba5c8, 0x20) = 0;
        DINO_POOL_WORD(data_020ba5c8, 0x24) = 0;
        DINO_POOL_WORD(data_020ba5c8, 0x28) = 1;
        DINO_POOL_WORD(data_020ba5c8, 0x2c) = 1;
        DINO_POOL_WORD(data_020ba5c8, 0x30) = 8;
        DINO_POOL_WORD(data_020ba5c8, 0x34) = 0;
        DINO_POOL_WORD(data_020ba5c8, 0x38) = 9;
        DINO_POOL_WORD(data_020ba5c8, 0x3c) = 1;
        DINO_POOL_WORD(data_020ba548, 0x34) = 0;
        DINO_POOL_WORD(data_020ba548, 0x38) = 8;
        DINO_POOL_WORD(data_020ba548, 0x0c) = 3;
        DINO_POOL_WORD(data_020ba548, 0x10) = 8;
        DINO_POOL_WORD(data_020ba548, 0x14) = 6;
        DINO_POOL_WORD(data_020ba548, 0x18) = 8;
        DINO_POOL_WORD(data_020ba548, 0x74) = 0;
        DINO_POOL_WORD(data_020ba548, 0x78) = 5;
        DINO_POOL_WORD(data_020ba548, 0x24) = 0;
        DINO_POOL_WORD(data_020ba548, 0x28) = 0;
        DINO_POOL_WORD(data_020ba548, 0x2c) = 1;
        DINO_POOL_WORD(data_020ba548, 0x30) = 0;
        DINO_POOL_WORD(data_020ba548, 0x44) = 2;
        DINO_POOL_WORD(data_020ba548, 0x48) = 0;
        DINO_POOL_WORD(data_020ba548, 0x64) = 0;
        DINO_POOL_WORD(data_020ba548, 0x68) = 1;
        DINO_POOL_WORD(data_020ba548, 0x4c) = 1;
        DINO_POOL_WORD(data_020ba548, 0x50) = 1;
        DINO_POOL_WORD(data_020ba548, 0x54) = 2;
        DINO_POOL_WORD(data_020ba548, 0x58) = 1;
        DINO_POOL_WORD(data_020ba548, 0x5c) = 0;
        DINO_POOL_WORD(data_020ba548, 0x60) = 2;
        DINO_POOL_WORD(data_020ba548, 0x1c) = 1;
        DINO_POOL_WORD(data_020ba548, 0x20) = 2;
        DINO_POOL_WORD(data_020ba548, 0x3c) = 2;
        DINO_POOL_WORD(data_020ba548, 0x40) = 2;
    }
} sDinoCommonWindowStaticInit;
