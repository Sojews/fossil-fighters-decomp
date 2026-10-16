// MView, MFixedSizeView and the transfers and animations that change a view.

#include "m/view.h"

MView::MView() {
}

MView::~MView() {
}

void MView::SetPosition(int x, int y) {
    SetPosition(MPoint(x, y));
}

int MView::GetWidth() {
    return GetSize().x;
}

int MView::GetHeight() {
    return GetSize().y;
}

void MView::SetFrame(int left, int top, int right, int bottom) {
    SetFrame(MRect(left, top, right, bottom));
}

void MView::SetFrame(MRect frame) {
    SetPosition(MPoint(frame.left, frame.top));
    SetSize(MPoint(frame.right - frame.left, frame.bottom - frame.top));
}

MRect MView::GetFrame() {
    MPoint pos = GetPosition();
    MPoint size = GetSize();
    return MRect(pos, MPoint(pos.x + size.x, pos.y + size.y));
}

MFixedSizeView::MFixedSizeView(MPoint size) {
    mSize = size;
}

MFixedSizeView::MFixedSizeView() {
    mSize.x = 0;
    mSize.y = 0;
}

void MFixedSizeView::SetSize(MPoint size) {
    OS_Terminate();
}

MPoint MFixedSizeView::GetSize() {
    return mSize;
}

void MFixedSizeView::SetFixedSize(MPoint size) {
    mSize = size;
}

MTask *MView::CreateCoordTransfer(int x, int y) {
    return new MViewCoordTransfer(this, MPoint(x, y));
}

MTask *MView::CreateCoordTransfer(MPoint pos) {
    return new MViewCoordTransfer(this, pos);
}

MTask *MView::CreatePriorityTransfer(int priority) {
    return new MViewPriorityTransfer(this, priority);
}

MTask *MView::CreateShowTransfer() {
    return new MViewShowTransfer(this);
}

MTask *MView::CreateHideTransfer() {
    return new MViewHideTransfer(this);
}

MTask *MView::CreateCoordAnime(MPoint from, MPoint to, int duration, int ease) {
    return new MViewCoordAnime(this, from, to, duration, ease);
}

MTask *MView::CreateFrameAnime(MRect from, MRect to, int duration, int ease) {
    return new MViewFrameAnime(this, from, to, duration, ease);
}

void MViewFrameAnime::Apply(int step) {
    MRectAnime::Apply(step);
    mView->SetFrame(MRect(mValue.left, mValue.top, mValue.right, mValue.bottom));
}

void MViewCoordAnime::Apply(int step) {
    MPointAnime::Apply(step);
    mView->SetPosition(MPoint(mValue.x, mValue.y));
}

void MViewHideTransfer::Transfer() {
    mView->Hide();
}

void MViewShowTransfer::Transfer() {
    mView->Show();
}

void MViewPriorityTransfer::Transfer() {
    mView->SetPriority(mPriority);
}

void MViewCoordTransfer::Transfer() {
    mView->SetPosition(mPos);
}
