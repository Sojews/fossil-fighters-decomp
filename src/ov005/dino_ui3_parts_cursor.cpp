// DinoUi3PartsCursor.
// Only the destructor is decompiled so far.

#include "game/dino_ui3_parts_cursor.h"
#include "m/motion.h"

extern const char *data_ov005_0216561c[];   // motion files

DinoUi3PartsCursor::DinoUi3PartsCursor(MSmartOam *oam) : mFrame(0, 0, 0, 0), mUnk24(1), mHidden(1), mTime(0) {
    mUnk10 = new MMotion(std::string(data_ov005_0216561c[16]), oam, 0, 1);
    mUnk10->Play(3, 0);
    mUnk10->Hide();
    mUnk10->GetUnit()->SetMode(1);
}

DinoUi3PartsCursor::~DinoUi3PartsCursor() {
    delete mUnk10;
}
