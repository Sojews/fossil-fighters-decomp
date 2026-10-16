// MPictureFile and the views that show pictures.

#include "m/picture.h"
#include "m/bg.h"
#include "m/ext_affine.h"
#include "m/chr.h"
#include "m/pal.h"
#include "m/text.h"
#include "m/smart_text.h"

MPictureFile::MPictureFile(std::string path, int lock) : mResource(path) {
    Open(lock);
}

MPictureFile::MPictureFile() {
}

void MPictureFile::Open(int lock) {
    switch (GetData()->type) {
    case 0:
        mPalette = GetData()->GetPaletteFile(mResource.mArchive);
        mChr = GetData()->GetChrFile(mResource.mArchive);
        mScreen = GetData()->GetScreenFile(mResource.mArchive);
        lock &= LOCK_CHR | LOCK_PALETTE | LOCK_SCREEN;
        break;
    case 3:
    case 4:
        mPalette = GetData()->GetPaletteFile(mResource.mArchive);
        mChr = GetData()->GetChrFile(mResource.mArchive);
        lock &= LOCK_CHR | LOCK_PALETTE;
        break;
    case 1:
    case 2:
        mChr = GetData()->GetChrFile(mResource.mArchive);
        lock &= LOCK_CHR;
        break;
    }
    if (lock & LOCK_CHR) {
        mChr.Lock();
    }
    if (lock & LOCK_PALETTE) {
        mPalette.Lock();
    }
    if (lock & LOCK_SCREEN) {
        mScreen.Lock();
    }
}

const void *MPictureFile::GetChr() {
    mChr.Lock();
    return mChr.GetData();
}

const void *MPictureFile::GetPalette() {
    mPalette.Lock();
    return mPalette.GetData();
}

const void *MPictureFile::GetScreen() {
    mScreen.Lock();
    return mScreen.GetData();
}

MArchiveFile MPictureFile::GetChrFile() {
    return mChr;
}

bool MPictureFile::IsUnk04() {
    return GetData()->flags & 1;
}

u32 MPictureFile::GetPaletteSize() {
    return GetData()->paletteSize;
}

u32 MPictureFile::GetChrSize() {
    return GetData()->chrSize;
}

MPoint MPictureFile::CalcSize() {
    return MPoint(GetData()->width, GetData()->height);
}

int MPictureFile::GetWidth() {
    return GetData()->width;
}

int MPictureFile::GetHeight() {
    return GetData()->height;
}

MPoint MPictureFile::CalcTileSize() {
    MPoint size(GetData()->width, GetData()->height);
    return MPoint(size.x >> 3, size.y >> 3);
}

MPoint MPictureFile::GetSize() {
    return CalcSize();
}

MPoint MPictureFile::GetTileSize() {
    return CalcTileSize();
}

MExtAffinePicture::MExtAffinePicture(std::string path, BOOL fromEnd, bool loadChr)
    : MFixedSizeView(MPoint(5, 5)), mFile(path, 0) {
    mDisplay = gCurrentDisplay;
    Init(fromEnd, loadChr);
}

MExtAffinePicture::MExtAffinePicture(MDisplay *display, std::string path, BOOL fromEnd, bool loadChr)
    : MFixedSizeView(MPoint(5, 5)), mDisplay(display), mFile(path, 0) {
    Init(fromEnd, loadChr);
}

void MExtAffinePicture::Init(BOOL fromEnd, bool loadChr) {
    SetFixedSize(mFile.GetSize());
    MBg *bg = mDisplay->mBg;
    mChr = bg->CreateChr(mFile.IsUnk04(), mFile.GetChrSize() + 1, fromEnd);
    mScreen = bg->CreateExtAffine(1, fromEnd);
    mScreen->SetChr(mChr);
    mPal = mScreen->GetPalette();
    if (mScreen->GetPalette() == NULL) {
        mPal = bg->mPal;
        mPalBlock = mPal->Alloc(mFile.GetPaletteSize(), mFile.GetPalette(), fromEnd);
        if (!loadChr) {
            mChrBlock = mChr->AllocShared(mFile.GetChr(), mFile.GetChrSize(), mPalBlock->index, TRUE);
        } else {
            mChrBlock = mChr->Alloc(mFile.GetChrSize(), TRUE);
            MArchiveFile file = mFile.GetChrFile();
            mChrBlock->Load(&file, 0, 0, 0, -1, mPalBlock->index);
        }
    } else {
        mPalBlock = mPal->Alloc(1, mFile.GetPalette(), fromEnd);
        if (!loadChr) {
            mChrBlock = mChr->AllocShared(mFile.GetChr(), mFile.GetChrSize(), 0, TRUE);
        } else {
            mChrBlock = mChr->Alloc(mFile.GetChrSize(), TRUE);
            MArchiveFile file = mFile.GetChrFile();
            mChrBlock->Load(&file, 0, 0, 0, -1, 0);
        }
    }
    MChrBlock *blank = mChr->Alloc(1, TRUE);
    blank->Fill(0, 0, -1);
    mScreen->Fill(MRect(0, 0, 32, 32), blank->index, mPalBlock->index, false, false);
    mScreen->Transfer((const u16 *)mFile.GetScreen(), mFile.CalcTileSize(), MPoint(0, 0), mChrBlock->index,
                      mPalBlock->index, false, false);
}

MExtAffinePicture::~MExtAffinePicture() {
    MBg *bg = mDisplay->mBg;
    bg->DeleteChr(mChr);
    mPal->Free(mPalBlock);
    bg->DeleteExtAffine(mScreen);
}

void MExtAffinePicture::Execute() {
}

MExtAffine *MExtAffinePicture::GetScreen() {
    return mScreen;
}

void MExtAffinePicture::SetPosition(int x, int y) {
    SetPosition(MPoint(x, y));
}

void MExtAffinePicture::SetPosition(MPoint pos) {
    mScreen->SetOffset(MPoint(-pos.x, -pos.y));
}

MPoint MExtAffinePicture::GetPosition() {
    return MPoint(-mScreen->mOffset.x, -mScreen->mOffset.y);
}

void MExtAffinePicture::SetPriority(int priority) {
    mScreen->mBgCntBits.priority = priority;
}

int MExtAffinePicture::GetPriority() {
    return mScreen->mBgCntBits.priority;
}

void MExtAffinePicture::Show() {
    mScreen->Show();
}

void MExtAffinePicture::Hide() {
    mScreen->Hide();
}

BOOL MExtAffinePicture::IsHidden() {
    return mScreen->IsHidden();
}

u32 MExtAffinePicture::GetPlaneMask() {
    return 1 << mScreen->mBg;
}

MTextPicture::MTextPicture(std::string path, BOOL fromEnd, bool loadChr)
    : MFixedSizeView(MPoint(5, 5)), mFile(path, 0) {
    mDisplay = gCurrentDisplay;
    Init(fromEnd, loadChr);
}

MTextPicture::MTextPicture(MDisplay *display, std::string path, BOOL fromEnd, bool loadChr)
    : MFixedSizeView(MPoint(5, 5)), mDisplay(display), mFile(path, 0) {
    Init(fromEnd, loadChr);
}

MTextPicture::MTextPicture(const MPictureFile &file, BOOL fromEnd, bool loadChr)
    : MFixedSizeView(MPoint(5, 5)), mFile(file) {
    mDisplay = gCurrentDisplay;
    Init(fromEnd, loadChr);
}

MTextPicture::MTextPicture(MDisplay *display, const MPictureFile &file, BOOL fromEnd, bool loadChr)
    : MFixedSizeView(MPoint(5, 5)), mDisplay(display), mFile(file) {
    Init(fromEnd, loadChr);
}

void MTextPicture::Init(BOOL fromEnd, bool loadChr) {
    SetFixedSize(mFile.GetSize());
    MBg *bg = mDisplay->mBg;
    mChr = bg->CreateChr(mFile.IsUnk04(), mFile.GetChrSize() + 1, fromEnd);
    mSmartText = new MSmartText(mDisplay, fromEnd);
    mSmartText->mText->SetChr(mChr);
    mSmartText->mText->mTextCntBits.colors256 = mFile.IsUnk04();
    if (!mFile.IsUnk04() || mSmartText->mText->mUnk30 == 0) {
        mPal = bg->mPal;
        mPalBlock = mPal->Alloc(mFile.GetPaletteSize(), mFile.GetPalette(), fromEnd);
        if (!loadChr) {
            mChrBlock = mChr->AllocShared(mFile.GetChr(), mFile.GetChrSize(), mPalBlock->index, TRUE);
        } else {
            mChrBlock = mChr->Alloc(mFile.GetChrSize(), TRUE);
            MArchiveFile file = mFile.GetChrFile();
            mChrBlock->Load(&file, 0, 0, 0, -1, mPalBlock->index);
        }
    } else {
        mPal = (MPal *)mSmartText->mText->mUnk30;
        mPalBlock = mPal->Alloc(1, mFile.GetPalette(), fromEnd);
        if (!loadChr) {
            mChrBlock = mChr->AllocShared(mFile.GetChr(), mFile.GetChrSize(), 0, TRUE);
        } else {
            mChrBlock = mChr->Alloc(mFile.GetChrSize(), TRUE);
            MArchiveFile file = mFile.GetChrFile();
            mChrBlock->Load(&file, 0, 0, 0, -1, 0);
        }
    }
    MChrBlock *blank = mChr->Alloc(1, TRUE);
    mBlank = blank;
    blank->Fill(0, 0, -1);
    mSmartText->Fill(MRect(-0x8000, -0x8000, 0x7FFF, 0x7FFF), blank->index, mPalBlock->index, false, false);
    mSmartText->Transfer((const u16 *)mFile.GetScreen(), mFile.CalcTileSize(), MPoint(0, 0), mChrBlock->index,
                         mPalBlock->index, false, false);
}

MTextPicture::~MTextPicture() {
    mDisplay->mBg->DeleteChr(mChr);
    mPal->Free(mPalBlock);
    delete mSmartText;
}

void MTextPicture::Execute() {
    if (mSmartText->mState == STATE_RUN) {
        mSmartText->Execute();
    }
}

MPalBlock *MTextPicture::GetPalBlock() {
    return mPalBlock;
}

void MTextPicture::SetPosition(int x, int y) {
    SetPosition(MPoint(x, y));
}

void MTextPicture::SetPosition(MPoint pos) {
    mSmartText->SetScroll(MPoint(-pos.x, -pos.y));
}

MPoint MTextPicture::GetPosition() {
    return MPoint(-mSmartText->mScroll.x, -mSmartText->mScroll.y);
}

void MTextPicture::SetPriority(int priority) {
    mSmartText->mText->mBgCntBits.priority = priority;
}

int MTextPicture::GetPriority() {
    return mSmartText->mText->mBgCntBits.priority;
}

void MTextPicture::Show() {
    mSmartText->mText->Show();
}

void MTextPicture::Hide() {
    mSmartText->mText->Hide();
}

BOOL MTextPicture::IsHidden() {
    return mSmartText->mText->IsHidden();
}

u32 MTextPicture::GetPlaneMask() {
    return 1 << mSmartText->mText->mBg;
}
