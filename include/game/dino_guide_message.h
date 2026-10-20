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
    DinoGuideMessage(std::string text);
    virtual ~DinoGuideMessage();
    virtual void Idle();
    virtual void Unk08();
    virtual void Unk0C();
    void Restart();
    void Scroll();

    void (DinoGuideMessage::*mState)(); // 0x7C
    std::string mText;      // 0x84
    class MSmartObjCanvas *mCanvas; // 0x90: the text strip
    MFont *mFont;           // 0x94
    class MSmartOam *mOam;  // 0x98
    MMotionFile mMotion;    // 0x9C
    OSMutex mMutex;         // 0xA0
};

#endif
