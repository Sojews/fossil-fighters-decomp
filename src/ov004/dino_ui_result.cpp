// DinoUiResult.
// Only the destructor is decompiled so far.

#include "game/dino_ui_result.h"
#include "m/lcd.h"

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
