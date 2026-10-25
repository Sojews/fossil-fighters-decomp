// MSmartText: a text layer that redraws its contents as it scrolls.

#pragma ipa file

#include "m/smart_text.h"
#include "m/bg.h"
#include "m/display.h"
#include "m/text.h"

TextPattern::TextPattern(const u16 *src, MPoint srcSize, MPoint dst, int tile, int palette, bool hflip, bool vflip)
    : Item(MRect(dst.x, dst.y, dst.x + srcSize.x, dst.y + srcSize.y)), mSrc(src), mSrcSize(srcSize),
      mOffset(MPoint(0, 0) - dst), mTile(tile), mPalette(palette), mHFlip(hflip), mVFlip(vflip) {
}

void TextPattern::Draw(MText *text, MRect clip) {
    MPoint dst(clip.left, clip.top);
    int dy = mVFlip ? (mRect.bottom - clip.bottom) - (clip.top - mRect.top) : 0;
    int dx = mHFlip ? (mRect.right - clip.right) - (clip.left - mRect.left) : 0;
    MRect src(clip.left + dx, clip.top + dy, clip.right + dx, clip.bottom + dy);
    src.Offset(mOffset);
    text->Transfer(mSrc, mSrcSize, src, dst, mTile, mPalette, mHFlip, mVFlip);
}

TextSequence::TextSequence(MPoint srcSize, MPoint dst, int tile, int palette, bool hflip, bool vflip)
    : Item(MRect(dst.x, dst.y, dst.x + srcSize.x, dst.y + srcSize.y)), mSrcSize(srcSize), mOffset(MPoint(0, 0) - dst),
      mTile(tile), mPalette(palette), mHFlip(hflip), mVFlip(vflip) {
}

void TextSequence::Draw(MText *text, MRect clip) {
    MPoint dst(clip.left, clip.top);
    int dy = mVFlip ? (mRect.bottom - clip.bottom) - (clip.top - mRect.top) : 0;
    int dx = mHFlip ? (mRect.right - clip.right) - (clip.left - mRect.left) : 0;
    MRect src(clip.left + dx, clip.top + dy, clip.right + dx, clip.bottom + dy);
    src.Offset(mOffset);
    text->TransferSequence(mSrcSize, src, dst, mTile, mPalette, mHFlip, mVFlip);
}

TextFill::TextFill(MRect rect, int tile, int palette, bool hflip, bool vflip) : Item(rect) {
    mTile = tile;
    mPalette = palette;
    mHFlip = hflip;
    mVFlip = vflip;
}

void TextFill::Draw(MText *text, MRect clip) {
    text->Fill(clip, mTile, mPalette, mHFlip, mVFlip);
}

MSmartText::MSmartText(MDisplay *display, BOOL fromEnd)
    : mScroll(0, 0), mPrevView(0, 0, 0, 0), mView(0, 0, 0, 0) {
    if (display == NULL) {
        display = gCurrentDisplay;
    }
    mDisplay = display;
    mText = display->mBg->CreateText(1, fromEnd);
}

MSmartText::~MSmartText() {
    Erase(MRect(0x80000000, 0x80000000, 0x7fffffff, 0x7fffffff));
    mDisplay->mBg->DeleteText(mText);
}

void MSmartText::Add(Item *item) {
    mItems.push_back(item);
}

void MSmartText::Erase(MRect rect) {
    mDirty.Add(rect);
    for (std::vector<Item *>::iterator it = mItems.begin(); it != mItems.end();) {
        if (rect.Contains((*it)->GetRect())) {
            delete *it;
            it = mItems.erase(it);
        } else {
            it++;
        }
    }
}

void MSmartText::Transfer(const u16 *src, MPoint srcSize, MPoint dst, int tile, int palette, bool hflip,
                          bool vflip) {
    Erase(MRect(dst, dst + srcSize));
    Add(new TextPattern(src, srcSize, dst, tile, palette, hflip, vflip));
}

void MSmartText::TransferSequence(MPoint srcSize, MPoint dst, int tile, int palette, bool hflip, bool vflip) {
    Erase(MRect(dst, dst + srcSize));
    Add(new TextSequence(srcSize, dst, tile, palette, hflip, vflip));
}

void MSmartText::Fill(MRect rect, int tile, int palette, bool hflip, bool vflip) {
    Erase(rect);
    Add(new TextFill(rect, tile, palette, hflip, vflip));
}

void MSmartText::Execute() {
    mText->SetOffset(mScroll);
    mPrevView = mView;
    mView = MRect(mScroll.x >> 3, mScroll.y >> 3, (mScroll.x + 256 + 7) >> 3, (mScroll.y + 192 + 7) >> 3);
    mDirty.Intersect(mView);
    if (mView == mPrevView && mDirty.IsEmpty()) {
        return;
    }
    // What came into view, and what changed, drawn from the newest item down.
    mUpdate.Set(mView);
    mUpdate.Subtract(mPrevView);
    mUpdate.Add(mDirty);
    for (std::vector<Item *>::reverse_iterator it = mItems.rbegin(); it != mItems.rend(); it++) {
        mDirty = mUpdate;
        mDirty.Intersect((*it)->GetRect());
        if (!mDirty.IsEmpty()) {
            for (u32 i = 0; i < mDirty.GetCount(); i++) {
                (*it)->Draw(mText, mDirty.GetRect(i));
            }
            mUpdate.Subtract(mDirty);
            if (mUpdate.IsEmpty()) {
                break;
            }
        }
    }
    mDirty.Clear();
}
