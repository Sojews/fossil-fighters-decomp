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
    mUnk70 = MPoint(0, 0);
    mUnk78 = MPoint(0, 0);
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
