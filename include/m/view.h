// Views: anything placed on screen with a position, a size and a draw priority, which can be shown and hidden.
#ifndef M_VIEW_H
#define M_VIEW_H

#include "m/task.h"
#include "m/anime.h"
#include "m/geometry.h"

class MView : public MTask {
public:
    MView();
    virtual ~MView() = 0;
    virtual void SetPosition(MPoint pos) = 0;
    virtual MPoint GetPosition() = 0;
    virtual void SetSize(MPoint size) = 0;
    virtual MPoint GetSize() = 0;
    virtual MRect GetFrame();
    virtual void SetPriority(int priority) = 0;
    virtual int GetPriority() = 0;
    virtual void Show() = 0;
    virtual void Hide() = 0;
    virtual BOOL Unk30() = 0;
    virtual BOOL Unk34() = 0;

    void SetPosition(int x, int y);
    int GetWidth();
    int GetHeight();
    void SetFrame(int left, int top, int right, int bottom);
    void SetFrame(MRect frame);

    MTask *CreateCoordTransfer(int x, int y);
    MTask *CreateCoordTransfer(MPoint pos);
    MTask *CreatePriorityTransfer(int priority);
    MTask *CreateShowTransfer();
    MTask *CreateHideTransfer();
    MTask *CreateCoordAnime(MPoint from, MPoint to, int duration, int ease);
    MTask *CreateFrameAnime(MRect from, MRect to, int duration, int ease);
};

// A view whose size is set when it's made; SetSize() is an error.
class MFixedSizeView : public MView {
public:
    MFixedSizeView(MPoint size);
    MFixedSizeView();
    virtual ~MFixedSizeView() {}
    virtual void SetSize(MPoint size);
    virtual MPoint GetSize();

    void SetFixedSize(MPoint size);

    MPoint mSize;   // 0x10
};

class MViewCoordTransfer : public MTransfer {
public:
    MViewCoordTransfer(MView *view, MPoint pos) {
        mView = view;
        mPos = pos;
    }
    virtual ~MViewCoordTransfer() {}
    virtual void Transfer();

    MView *mView;   // 0x10
    MPoint mPos;    // 0x14
};

class MViewPriorityTransfer : public MTransfer {
public:
    MViewPriorityTransfer(MView *view, int priority) {
        mView = view;
        mPriority = priority;
    }
    virtual ~MViewPriorityTransfer() {}
    virtual void Transfer();

    MView *mView;   // 0x10
    int mPriority;  // 0x14
};

class MViewShowTransfer : public MTransfer {
public:
    MViewShowTransfer(MView *view) { mView = view; }
    virtual ~MViewShowTransfer() {}
    virtual void Transfer();

    MView *mView;   // 0x10
};

class MViewHideTransfer : public MTransfer {
public:
    MViewHideTransfer(MView *view) { mView = view; }
    virtual ~MViewHideTransfer() {}
    virtual void Transfer();

    MView *mView;   // 0x10
};

// Moves a view.
class MViewCoordAnime : public MPointAnime {
public:
    MViewCoordAnime(MView *view, MPoint from, MPoint to, int duration, int ease) : MPointAnime(from, to) {
        mView = view;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~MViewCoordAnime() {}
    virtual void Apply(int step);

    MView *mView;   // 0x50
};

// Moves and resizes a view.
class MViewFrameAnime : public MRectAnime {
public:
    MViewFrameAnime(MView *view, MRect from, MRect to, int duration, int ease) : MRectAnime(from, to) {
        mView = view;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~MViewFrameAnime() {}
    virtual void Apply(int step);

    MView *mView;   // 0x70
};

#endif
