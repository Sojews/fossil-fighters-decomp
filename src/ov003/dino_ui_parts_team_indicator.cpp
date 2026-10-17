// DinoUiPartsTeamIndicator.
// Only the destructor is decompiled so far.

#include "game/dino_ui_parts_team_indicator.h"

DinoUiPartsTeamIndicator::~DinoUiPartsTeamIndicator() {
    mFont->Release();
    delete mView;
}
