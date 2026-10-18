#ifndef GAME_DINO_UI_TITLE_SKIP_H
#define GAME_DINO_UI_TITLE_SKIP_H

#include "m/task.h"

class MInput;

// Shows a screen for some frames, or until skipped, then fades out.
class DinoUiTitleSkip : public MProcess {
public:
    DinoUiTitleSkip(u8 *skipped, MInput *input, int frames, int fadeFrames, u8 unk84, u8 unk85)
        : MProcess(NULL), mSkipped(skipped), mInput(input), mFrames(frames), mFadeFrames(fadeFrames), mUnk84(unk84),
          mUnk85(unk85) {}
    virtual ~DinoUiTitleSkip();
    virtual void Idle();

    u8 *mSkipped;       // 0x74
    MInput *mInput;     // 0x78
    int mFrames;        // 0x7C
    int mFadeFrames;    // 0x80
    u8 mUnk84;          // 0x84
    u8 mUnk85;          // 0x85
};

#endif
