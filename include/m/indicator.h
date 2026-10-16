// Indicators: a line of text on a canvas that's redrawn only where it changes.
#ifndef M_INDICATOR_H
#define M_INDICATOR_H

#include <string>
#include <vector>
#include "m/canvas.h"

class MIndicator : public MFixedSizeView {
public:
    // A run of the line: one character (or blank space, code 0) from x to end.
    struct Entry {
        u32 code;
        int x;
        int end;
    };

    MIndicator(MCanvas *canvas, MFont *font, int spacing);
    virtual ~MIndicator();
    virtual void Execute();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();

    MFont *GetFont();
    void SetPosition(int x, int y);
    void SetText(const std::string &text);
    void SetText(const std::wstring &text);

    MCanvas *mCanvas;               // 0x18
    MFont *mFont;                   // 0x1C
    int mSpacing;                   // 0x20
    std::vector<Entry> mShown;      // 0x24
    std::vector<Entry> mPending;    // 0x30
    std::wstring mText;             // 0x3C
};

#endif
