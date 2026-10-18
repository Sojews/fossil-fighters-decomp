#ifndef GAME_DINO_SENRYU_SAMPLE_H
#define GAME_DINO_SENRYU_SAMPLE_H

#include <string>
#include "m/task.h"
#include "m/archive.h"
#include "m/resource.h"

class DinoSenryuSample : public MProcess {
public:
    DinoSenryuSample();
    virtual ~DinoSenryuSample();
    virtual void Idle();

    void StateEnd();
    void StateMenu();

    void (DinoSenryuSample::*mState)(); // 0x74
    std::wstring mText;     // 0x7C
    MResource<u8> mData;    // 0x88
    s32 mResult;            // 0x98: -2 to leave
};

#endif
