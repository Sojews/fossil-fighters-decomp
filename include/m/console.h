// Consoles: views that type text out onto a canvas a few characters a frame, wrapping lines.
#ifndef M_CONSOLE_H
#define M_CONSOLE_H

#include <string>
#include "m/canvas.h"
#include "m/console_filter.h"
#include "m/fx32.h"

class MConsole : public MFixedSizeView {
public:
    MConsole(MCanvas *canvas, MFont *font, MPoint spacing, MFx32 speed);  // spacing: between letters, lines
    virtual ~MConsole();
    virtual void Execute();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();

    void SetSpeed(MFx32 speed);
    MFx32 GetSpeed();
    void SetRate(MFx32 rate);
    void SetUnk3C(MFx32 value);
    MPoint GetCursor();
    void SetPosition(int x, int y);
    void Clear();
    BOOL IsEmpty();
    void ClearText();
    void SetUnk44(int value);
    void SetFilter(MConsoleFilter *filter);
    void Print(const std::string &text);
    void Print(const std::wstring &text);

    MCanvas *mCanvas;           // 0x18
    MFont *mFont;               // 0x1C
    int mSpacing;               // 0x20
    int mLineSpacing;           // 0x24
    int mUnk28;                 // 0x28
    int mUnk2C;                 // 0x2C
    int mUnk30;                 // 0x30
    MFx32 mRate;                // 0x34: characters a frame
    MFx32 mProgress;            // 0x38
    MFx32 mUnk3C;               // 0x3C
    int mUnk40;                 // 0x40
    int mUnk44;                 // 0x44
    std::wstring mText;         // 0x48: what's left to type
    MConsoleFilter *mFilter;    // 0x54
    MPoint mCursor;             // 0x58
    int mUnk60;                 // 0x60
    int mHyphenWidth;           // 0x64
    u8 mUnk68;                  // 0x68
    u8 mUnk69;                  // 0x69
};

#endif
