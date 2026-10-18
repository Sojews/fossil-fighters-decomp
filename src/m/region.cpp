// MRegion.

#pragma ipa file

#include "m/region.h"

MRegion::MRegion() {
}

MRegion::MRegion(MRect rect) {
    mRects.push_back(rect);
}

MRegion::MRegion(const MRegion &other) : mRects(other.mRects) {
}

MRegion &MRegion::operator=(const MRegion &other) {
    mRects = other.mRects;
    return *this;
}

u32 MRegion::GetCount() {
    return mRects.size();
}

MRect MRegion::GetRect(u32 i) {
    return mRects[i];
}

void MRegion::Set(MRect rect) {
    mRects.clear();
    mRects.push_back(rect);
}

void MRegion::Add(MRect rect) {
    MRegion add(rect);
    add.Subtract(*this);
    // (The game builds these iterators from the vector's fields; using begin() and end() makes mwcc fold the
    // first comparison.)
    typedef std::vector<MRect>::iterator Iterator;
    for (Iterator it(add.mRects.data_); it != Iterator(add.mRects.data_ + add.mRects.size_); it++) {
        mRects.push_back(*it);
    }
}

void MRegion::Add(const MRegion &other) {
    if (other.IsEmpty()) {
        return;
    }
    MRegion add(other);
    add.Subtract(*this);
    // (The game builds these iterators from the vector's fields; using begin() and end() makes mwcc fold the
    // first comparison.)
    typedef std::vector<MRect>::iterator Iterator;
    for (Iterator it(add.mRects.data_); it != Iterator(add.mRects.data_ + add.mRects.size_); it++) {
        mRects.push_back(*it);
    }
}

void MRegion::Subtract(MRect rect) {
    mTemp.clear();
    for (std::vector<MRect>::iterator it = mRects.begin(); it != mRects.end(); it++) {
        MRect r = *it;
        if (!r.Intersects(rect)) {
            mTemp.push_back(r);
            continue;
        }
        if (rect.top > r.top) {
            mTemp.push_back(MRect(r.left, r.top, r.right, rect.top));
            r.top = rect.top;
        }
        if (rect.bottom < r.bottom) {
            mTemp.push_back(MRect(r.left, rect.bottom, r.right, r.bottom));
            r.bottom = rect.bottom;
        }
        if (rect.left > r.left) {
            mTemp.push_back(MRect(r.left, r.top, rect.left, r.bottom));
        }
        if (rect.right < r.right) {
            mTemp.push_back(MRect(rect.right, r.top, r.right, r.bottom));
        }
    }
    std::swap(mRects.cap(), mTemp.cap());
    std::swap(mRects.data_, mTemp.data_);
    std::swap(mRects.size_, mTemp.size_);
}

void MRegion::Subtract(const MRegion &other) {
    for (MRect *it = other.mRects.data_; it != other.mRects.data_ + other.mRects.size_; it++) {
        Subtract(*it);
    }
}

void MRegion::Intersect(MRect rect) {
    mTemp.clear();
    for (std::vector<MRect>::iterator it = mRects.begin(); it != mRects.end(); it++) {
        MRect clip = it->Intersection(rect);
        if (clip.left <= clip.right && clip.top <= clip.bottom) {
            mTemp.push_back(clip);
        }
    }
    std::swap(mRects.cap(), mTemp.cap());
    std::swap(mRects.data_, mTemp.data_);
    std::swap(mRects.size_, mTemp.size_);
}

bool MRegion::IsEmpty() const {
    return mRects.size_ == 0;
}

void MRegion::Clear() {
    mRects.clear();
}
