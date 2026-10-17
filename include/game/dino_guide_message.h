// A guide message window.
#ifndef GAME_DINO_GUIDE_MESSAGE_H
#define GAME_DINO_GUIDE_MESSAGE_H

#include <string>
#include "m/task.h"
#include "m/font.h"
#include "m/motion.h"
#include "game/help.h"

class DinoGuideMessage : public MProcess, public DinoHelpHook {
public:
    virtual ~DinoGuideMessage();
    virtual void Idle();
    virtual void Unk08();
    virtual void Unk0C();

    void (DinoGuideMessage::*mState)(); // 0x7C
    std::string mText;      // 0x84
    u32 mUnk90;             // 0x90
    MFont *mFont;           // 0x94
    u32 mUnk98;             // 0x98
    MMotionFile mMotion;    // 0x9C
};

#endif
