// DinoCommonLabel.

#include <vector>
#include "game/dino_common_label.h"
#include "game/dino_resource.h"
#include "m/canvas.h"
#include "m/display.h"
#include "m/font.h"
#include "m/obj.h"
#include "m/oam_canvas.h"
#include "m/smart_oam.h"
#include "m/text_codec.h"

DinoCommonLabel::DinoCommonLabel(MSmartOam *oam, MFont *font, const void *palette, u32 textId, MPoint spacing)
    : MFixedSizeView(MPoint(0, 0)), mFont(font), mText(gDinoResource->GetText(textId)), mSpacing(spacing.x),
      mLineSpacing(spacing.y) {
    mCanvas = mSmartCanvas = new MSmartObjCanvas(
        oam->mDisplay, oam, font->MeasureText(gDinoResource->GetText(textId), mSpacing, mLineSpacing), palette, TRUE);
    Init();
}

DinoCommonLabel::DinoCommonLabel(MDisplay *display, MFont *font, const void *palette, const std::string &text,
                                 MPoint spacing)
    : MFixedSizeView(MPoint(0, 0)), mFont(font), mText(text), mSpacing(spacing.x), mLineSpacing(spacing.y) {
    mCanvas = new MObjCanvas(display, display->mObj->mOam, font->MeasureText(text, mSpacing, mLineSpacing), palette,
                             TRUE);
    mSmartCanvas = NULL;
    Init();
}

void DinoCommonLabel::Init() {
    mFont->AddRef();
    SetFixedSize(mCanvas->GetSize());
    Draw();
}

// Draws the text, each line centered.
void DinoCommonLabel::Draw() {
    int y = 0;
    std::vector<std::string> lines = MSplitString(mText, '\n');
    for (u32 i = 0; i < lines.size(); i++) {
        MPoint size = mFont->MeasureText(lines[i].c_str(), mSpacing, mLineSpacing);
        mCanvas->DrawText(mFont, MPoint((mCanvas->GetSize().x - size.x) / 2, y), mSpacing, lines[i].c_str());
        y += size.y + mLineSpacing;
    }
}

void DinoCommonLabel::SetColors(int color, int shadow, int back) {
    mFont->SetColors(color, shadow, back);
    Draw();
}

DinoCommonLabel::~DinoCommonLabel() {
    mFont->Release();
    delete mCanvas;
}

void DinoCommonLabel::Execute() {
    if (mCanvas->mState == STATE_RUN) {
        mCanvas->Execute();
    }
}

void DinoCommonLabel::SetPosition(MPoint pos) {
    mCanvas->SetPosition(pos);
}

MPoint DinoCommonLabel::GetPosition() {
    return mCanvas->GetPosition();
}

void DinoCommonLabel::SetPriority(int priority) {
    mCanvas->SetPriority(priority);
}

int DinoCommonLabel::GetPriority() {
    return mCanvas->GetPriority();
}

void DinoCommonLabel::Show() {
    mCanvas->Show();
}

void DinoCommonLabel::Hide() {
    mCanvas->Hide();
}

BOOL DinoCommonLabel::IsHidden() {
    return mCanvas->IsHidden();
}

u32 DinoCommonLabel::GetPlaneMask() {
    return mCanvas->GetPlaneMask();
}
