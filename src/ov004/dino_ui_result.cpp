// DinoUiResult.
// Only the destructor is decompiled so far.

#include "game/dino_ui_result.h"
#include "m/lcd.h"
#include "game/dino_static_init.h"

DinoUiResult::DinoUiResult(u32 arg0, u8 arg1, u32 count, u32 count2, const std::string &text)
    : MProcess(gLcd->mSub), mUnk74(arg0), mUnk78(arg1), mCount(count), mCount2(count2), mText(text) {
    if (mCount > 99) {
        mCount = 99;
    }
    if (mCount2 > 99) {
        mCount2 = 99;
    }
}

DinoUiResult::~DinoUiResult() {
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov004_02177148[];
static struct DinoUiResultStaticInit {
    DinoUiResultStaticInit() {
        DINO_POOL_WORD(data_ov004_02177148, 0x20) = 60;
        DINO_POOL_WORD(data_ov004_02177148, 0x24) = 9;
        DINO_POOL_WORD(data_ov004_02177148, 0x10) = 148;
        DINO_POOL_WORD(data_ov004_02177148, 0x14) = 9;
    }
} sDinoUiResultStaticInit;
