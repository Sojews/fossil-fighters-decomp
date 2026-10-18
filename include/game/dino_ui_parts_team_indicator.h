#ifndef GAME_DINO_UI_PARTS_TEAM_INDICATOR_H
#define GAME_DINO_UI_PARTS_TEAM_INDICATOR_H

#include "m/task.h"
#include "m/font.h"
#include "m/oam_canvas.h"
#include "m/view.h"

// A line of outlined text.
class DinoUiPartsTeamIndicator : public MProcess {
public:
    DinoUiPartsTeamIndicator(MPoint pos, MFont *font);
    virtual ~DinoUiPartsTeamIndicator();
    virtual void Idle();
    void SetText(const std::wstring &text);
    void Show();
    void Hide();

    MFont *mFont;           // 0x74
    MObjCanvas *mCanvas;    // 0x78
};

#endif
