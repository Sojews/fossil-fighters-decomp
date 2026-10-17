#ifndef GAME_DINO_SENRYU_SAMPLE_H
#define GAME_DINO_SENRYU_SAMPLE_H

#include <string>
#include "m/task.h"
#include "m/archive.h"

class DinoSenryuSample : public MProcess {
public:
    virtual ~DinoSenryuSample();
    virtual void Idle();

    u32 mUnk74[2];          // 0x74
    std::wstring mText;     // 0x7C
    MArchivePtr mArchive;   // 0x88
    MArchiveFile mFile;     // 0x8C
};

#endif
