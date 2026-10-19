// DinoCleaningScore.
// Only the destructors are decompiled so far.

#include "m/picture.h"
#include "game/dino_cleaning_score.h"

DinoCleaningScore::~DinoCleaningScore() {
}

#pragma auto_inline off

MFx32 DinoCleaningScore::GetUnk80() {
    return mUnk80;
}

MFx32 DinoCleaningScore::GetUnk84() {
    return mUnk84;
}

u8 DinoCleaningScore::GetUnk90() {
    return mUnk90;
}

u8 DinoCleaningScore::GetUnk91() {
    return mUnk91;
}

void DinoCleaningScore::SetUnk9CC() {
    mUnk9CC = TRUE;
}

// Shows the score's gauge, rising to the score.
void DinoCleaningScore::Show() {
    mGauge->SetPosition(1, 0);
    mGauge->Show();
    mSerial.AddTail(mGauge->CreateCoordAnime(MPoint(0x80, 0x92), MPoint(0x80, (100 - mScore) * 100 / 100 + 0x2E), 90, -1));
}
