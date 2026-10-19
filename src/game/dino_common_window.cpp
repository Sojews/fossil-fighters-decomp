// DinoCommonWindow.

#include "game/dino_common_window.h"
#include "m/bg.h"
#include "m/text.h"
#include "m/chr.h"
#include "m/pal.h"

static const char *sNull;
static const char *sBattleBgPath = "/image/image_common_window_bg_btl";
static const char *sBgPath = "/image/image_common_window_bg";
static MPoint sDrawPos;

// NONMATCHING: the file's statics aren't all here yet, so they're pooled at different offsets.
DinoCommonWindow::DinoCommonWindow(u32 extraTiles, bool battle, bool large) : MFixedSizeView(MPoint(0, 0)) {
    mPos = MPoint(0, 0);
    mFrame = MRect(0, 0, 0, 0);
    mBattle = battle;
    mDisplay = gCurrentDisplay;
    MBg *bg = mDisplay->mBg;
    mFile = MPictureFile(battle ? sBattleBgPath : sBgPath, 0);
    SetFixedSize(mFile.GetSize());
    mTileSize = mFile.GetTileSize();
    mPalBlock = bg->mPal->Alloc(mFile.GetPaletteSize(), mFile.GetPalette(), TRUE);
    mChr = bg->CreateChr(mFile.IsUnk04(), extraTiles + mFile.GetChrSize(), TRUE);
    mChrBlock = mChr->Alloc(mFile.GetChrSize(), TRUE);
    {
        MArchiveFile file = mFile.GetChrFile();
        mChrBlock->Load(&file, 0, 0, 0, 0x7FFF, 0);
    }
    mText = bg->CreateText(large ? 3 : 0, TRUE);
    mText->SetChr(mChr);
    mText->mTextCntBits.colors256 = mFile.IsUnk04();
    Draw(sDrawPos, MRect(0, 0, 64, 64));
}

// Fills rect of the text screen with the window graphics' tile at pos.
void DinoCommonWindow::Draw(MPoint pos, MRect rect) {
    if (rect.left <= rect.right && rect.top <= rect.bottom) {
        u32 base = mChrBlock->index;
        mText->Fill(rect, base + ((const u16 *) mFile.GetScreen())[pos.y * mTileSize.x + pos.x], mPalBlock->index,
                    FALSE, FALSE);
    }
}

extern "C" const MPoint data_020ba57c;
extern "C" const MRect data_020ba5e8;

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
