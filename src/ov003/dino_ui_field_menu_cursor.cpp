// DinoUiFieldMenuCursor.
// Only the destructor is decompiled so far.

#include "game/dino_ui_field_menu_cursor.h"
#include "m/motion.h"

extern const char *data_ov003_02160ce0[];   // motion files
extern const char *data_ov003_02160c60[];

DinoUiFieldMenuCursor::DinoUiFieldMenuCursor(MSmartOam *oam) : mFrame(0, 0, 0, 0), mMode(1), mHidden(1), mUnk2C(0) {
    mUnk10 = new MMotion(std::string(data_ov003_02160ce0[0]), oam, 0, 1);
    mUnk10->Play(3, 0);
    mUnk10->Hide();
    mUnk14 = new MMotion(std::string(data_ov003_02160c60[16]), oam, 0, 1);
    mUnk14->Play(3, 0);
    mUnk14->Hide();
    mUnk10->GetUnit()->SetMode(1);
    mUnk14->GetUnit()->SetMode(1);
}

DinoUiFieldMenuCursor::~DinoUiFieldMenuCursor() {
    delete mUnk14;
    delete mUnk10;
}
