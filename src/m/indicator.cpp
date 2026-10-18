// MIndicator. Not decompiled yet: the template functions.

#pragma ipa file

#include "m/indicator.h"
#include "m/text_codec.h"

// NONMATCHING (also SetText): push_back's two byte temporaries are in swapped stack slots.
MIndicator::MIndicator(MCanvas *canvas, MFont *font, int spacing)
    : MFixedSizeView(canvas->GetSize()), mCanvas(canvas), mFont(font), mSpacing(spacing) {
    mFont->AddRef();
    mCanvas->Clear();
    Entry blank;
    blank.code = 0;
    blank.x = 0;
    blank.end = mCanvas->GetWidth();
    mShown.push_back(blank);
}

MIndicator::~MIndicator() {
    delete mCanvas;
    mFont->Release();
}

// Draws the runs of the pending line that differ from the shown one, then shows it.
void MIndicator::Execute() {
    if (mCanvas->mState == STATE_RUN) {
        mCanvas->Execute();
    }
    if (!mPending.empty()) {
        std::vector<Entry>::iterator shown = mShown.begin();
        for (std::vector<Entry>::iterator it = mPending.begin(); it != mPending.end(); it++) {
            while (shown->end < it->x) {
                shown++;
            }
            if (shown->code == it->code && shown->x == it->x && shown->end == it->end) {
                shown++;
            } else if (it->code != 0) {
                mCanvas->DrawChar(mFont, MPoint(it->x, 0), it->code);
            } else {
                mCanvas->ClearRect(MRect(it->x, 0, it->end, mCanvas->GetHeight()));
            }
        }
        mShown.swap(mPending);
        mPending.clear();
    }
}

MFont *MIndicator::GetFont() {
    return mFont;
}

void MIndicator::SetPosition(int x, int y) {
    SetPosition(MPoint(x, y));
}

void MIndicator::SetPosition(MPoint pos) {
    mCanvas->SetPosition(pos);
}

MPoint MIndicator::GetPosition() {
    return mCanvas->GetPosition();
}

void MIndicator::SetPriority(int priority) {
    mCanvas->SetPriority(priority);
}

int MIndicator::GetPriority() {
    return mCanvas->GetPriority();
}

void MIndicator::Hide() {
    mCanvas->Hide();
}

void MIndicator::Show() {
    mCanvas->Show();
}

BOOL MIndicator::IsHidden() {
    return mCanvas->IsHidden();
}

u32 MIndicator::GetPlaneMask() {
    return mCanvas->GetPlaneMask();
}

void MIndicator::SetText(const std::string &text) {
    SetText(MDecodeText(text, mFont->GetEncoding()));
}

// Lays the text out into runs; Execute draws whatever differs from what's shown.
void MIndicator::SetText(const std::wstring &text) {
    if (mText == text) {
        return;
    }
    mText = text;
    mPending.clear();
    int x = 0;
    for (const wchar_t *p = text.begin(); p != text.end(); p++) {
        u32 code = *p;
        MPoint size = mFont->GetGlyphSize(code);
        Entry entry;
        entry.code = code;
        entry.x = x;
        x = x + size.x + mSpacing;
        entry.end = x;
        mPending.push_back(entry);
        if (x >= mCanvas->GetWidth()) {
            break;
        }
    }
    if (x < mCanvas->GetWidth()) {
        Entry blank;
        blank.code = 0;
        blank.x = x;
        blank.end = mCanvas->GetWidth();
        mPending.push_back(blank);
    }
}
