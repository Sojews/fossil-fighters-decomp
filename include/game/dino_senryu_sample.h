#ifndef GAME_DINO_SENRYU_SAMPLE_H
#define GAME_DINO_SENRYU_SAMPLE_H

#include <string>
#include "m/task.h"
#include "m/archive.h"
#include "m/resource.h"
#include "game/dino_resource.h"

class DinoSenryuSample : public MProcess {
public:
    DinoSenryuSample();
    virtual ~DinoSenryuSample();
    virtual void Idle();

    void StateStart();
    void StatePages();
    void StateEnd();
    void StateMenu();
    void StateFillRecords();

    void (DinoSenryuSample::*mState)(); // 0x74
    std::wstring mText;     // 0x7C
    MResource<DinoSenryuDefs> mDefs;    // 0x88
    int mResult;            // 0x98: the page chosen, -2 to leave
    u32 mPageCount;         // 0x9C
};

#endif
