// A label: a few centered lines of text drawn once onto a sprite canvas, which it stands in for as a view.
#ifndef GAME_DINO_COMMON_LABEL_H
#define GAME_DINO_COMMON_LABEL_H

#include <string>
#include "m/view.h"

class MCanvas;
class MDisplay;
class MFont;
class MSmartOam;
class MSmartObjCanvas;

class DinoCommonLabel : public MFixedSizeView {
public:
    // The text is game text number textId.
    DinoCommonLabel(MSmartOam *oam, MFont *font, const void *palette, u32 textId, int spacing, int lineSpacing);
    DinoCommonLabel(MDisplay *display, MFont *font, const void *palette, const std::string &text, int spacing,
                    int lineSpacing);
    virtual ~DinoCommonLabel();
    virtual void Execute();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();

    void Init();
    void Draw();
    void SetColors(int color, int shadow, int back);

    MCanvas *mCanvas;               // 0x18
    MFont *mFont;                   // 0x1C
    std::string mText;              // 0x20
    int mSpacing;                   // 0x2C
    int mLineSpacing;               // 0x30
    MSmartObjCanvas *mSmartCanvas;  // 0x34: the canvas, when it's on smart OAM
};

#endif
