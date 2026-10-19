// DinoUi2FieldMenuCursor.
// Only the destructor is decompiled so far.

#include "game/dino_ui2_field_menu_cursor.h"
#include "m/motion.h"

extern const char *data_ov004_02176220[];   // motion files

DinoUi2FieldMenuCursor::DinoUi2FieldMenuCursor(MSmartOam *oam) : mFrame(0, 0, 0, 0), mMode(1), mHidden(1), mTime(0) {
    mUnk10 = new MMotion(std::string(data_ov004_02176220[21]), oam, 0, 1);
    mUnk10->Play(3, 0);
    mUnk10->Hide();
    mUnk14 = new MMotion(std::string(data_ov004_02176220[2]), oam, 0, 1);
    mUnk14->Play(3, 0);
    mUnk14->Hide();
}

DinoUi2FieldMenuCursor::~DinoUi2FieldMenuCursor() {
    delete mUnk14;
    delete mUnk10;
}
