// DinoUi2FieldMenuCursor.
// Only the destructor is decompiled so far.

#include "game/dino_ui2_field_menu_cursor.h"
#include "m/motion.h"
#include "game/dino_static_init.h"

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

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov004_021762a0[];
extern "C" u32 data_ov004_02176320[];
extern "C" u32 data_ov004_021763a0[];
// NONMATCHING: register choice/scheduling of the pooled stores.
static struct DinoUi2FieldMenuCursorStaticInit {
    DinoUi2FieldMenuCursorStaticInit() {
        DINO_POOL_WORD(data_ov004_021762a0, 0x54) = 128;
        DINO_POOL_WORD(data_ov004_021762a0, 0x58) = 176;
        DINO_POOL_WORD(data_ov004_021762a0, 0x3c) = 8;
        DINO_POOL_WORD(data_ov004_021762a0, 0x40) = 150;
        DINO_POOL_WORD(data_ov004_021762a0, 0x14) = 64;
        DINO_POOL_WORD(data_ov004_021762a0, 0x18) = 112;
        DINO_POOL_WORD(data_ov004_02176320, 0x68) = DINO_POOL_WORD(data_ov004_02176220, 0x20);
        DINO_POOL_WORD(data_ov004_02176320, 0x6c) = DINO_POOL_WORD(data_ov004_02176220, 0x20);
        DINO_POOL_WORD(data_ov004_02176320, 0x70) = DINO_POOL_WORD(data_ov004_02176220, 0x78);
        DINO_POOL_WORD(data_ov004_02176320, 0x74) = DINO_POOL_WORD(data_ov004_02176220, 0x10);
        DINO_POOL_WORD(data_ov004_02176320, 0x78) = DINO_POOL_WORD(data_ov004_02176220, 0x3c);
        DINO_POOL_WORD(data_ov004_02176320, 0x7c) = DINO_POOL_WORD(data_ov004_02176220, 0x50);
        DINO_POOL_WORD(data_ov004_021763a0, 0x0) = DINO_POOL_WORD(data_ov004_02176220, 0x4);
        DINO_POOL_WORD(data_ov004_021763a0, 0x4) = DINO_POOL_WORD(data_ov004_021762a0, 0x0);
        DINO_POOL_WORD(data_ov004_021763a0, 0x8) = DINO_POOL_WORD(data_ov004_02176220, 0x68);
        DINO_POOL_WORD(data_ov004_021763a0, 0xc) = DINO_POOL_WORD(data_ov004_02176220, 0x38);
        DINO_POOL_WORD(data_ov004_021763a0, 0x10) = DINO_POOL_WORD(data_ov004_02176220, 0x74);
        DINO_POOL_WORD(data_ov004_021763a0, 0x14) = DINO_POOL_WORD(data_ov004_02176220, 0x70);
        DINO_POOL_WORD(data_ov004_021763a0, 0x18) = DINO_POOL_WORD(data_ov004_02176220, 0x64);
        DINO_POOL_WORD(data_ov004_021763a0, 0x1c) = DINO_POOL_WORD(data_ov004_02176220, 0x6c);
        DINO_POOL_WORD(data_ov004_021763a0, 0x20) = DINO_POOL_WORD(data_ov004_02176220, 0x60);
        DINO_POOL_WORD(data_ov004_021763a0, 0x24) = DINO_POOL_WORD(data_ov004_02176220, 0xc);
        DINO_POOL_WORD(data_ov004_021763a0, 0x28) = DINO_POOL_WORD(data_ov004_021762a0, 0x4);
        DINO_POOL_WORD(data_ov004_021763a0, 0x2c) = DINO_POOL_WORD(data_ov004_02176220, 0x2c);
        DINO_POOL_WORD(data_ov004_021763a0, 0x30) = DINO_POOL_WORD(data_ov004_02176220, 0x24);
        DINO_POOL_WORD(data_ov004_021763a0, 0x34) = DINO_POOL_WORD(data_ov004_02176220, 0x34);
        DINO_POOL_WORD(data_ov004_021763a0, 0x38) = DINO_POOL_WORD(data_ov004_021762a0, 0x8);
        DINO_POOL_WORD(data_ov004_021763a0, 0x3c) = DINO_POOL_WORD(data_ov004_02176220, 0x14);
        DINO_POOL_WORD(data_ov004_021763a0, 0x40) = DINO_POOL_WORD(data_ov004_02176220, 0x4c);
        DINO_POOL_WORD(data_ov004_021763a0, 0x44) = DINO_POOL_WORD(data_ov004_02176220, 0x30);
        DINO_POOL_WORD(data_ov004_021763a0, 0x48) = DINO_POOL_WORD(data_ov004_02176220, 0x48);
        DINO_POOL_WORD(data_ov004_021763a0, 0x4c) = DINO_POOL_WORD(data_ov004_02176220, 0x28);
        DINO_POOL_WORD(data_ov004_021763a0, 0x50) = DINO_POOL_WORD(data_ov004_02176220, 0x18);
        DINO_POOL_WORD(data_ov004_021763a0, 0x54) = DINO_POOL_WORD(data_ov004_02176220, 0x40);
        DINO_POOL_WORD(data_ov004_021763a0, 0x58) = DINO_POOL_WORD(data_ov004_02176220, 0x44);
        DINO_POOL_WORD(data_ov004_021763a0, 0x5c) = DINO_POOL_WORD(data_ov004_02176220, 0x58);
        DINO_POOL_WORD(data_ov004_021762a0, 0x2c) = 0;
        DINO_POOL_WORD(data_ov004_021762a0, 0x30) = 176;
        DINO_POOL_WORD(data_ov004_021762a0, 0x34) = 0;
        DINO_POOL_WORD(data_ov004_021762a0, 0x38) = 178;
        DINO_POOL_WORD(data_ov004_021762a0, 0xc) = 156;
        DINO_POOL_WORD(data_ov004_021762a0, 0x10) = 144;
        DINO_POOL_WORD(data_ov004_021762a0, 0x44) = 36;
        DINO_POOL_WORD(data_ov004_021762a0, 0x48) = 174;
        DINO_POOL_WORD(data_ov004_021762a0, 0x4c) = 136;
        DINO_POOL_WORD(data_ov004_021762a0, 0x50) = 174;
    }
} sDinoUi2FieldMenuCursorStaticInit;
