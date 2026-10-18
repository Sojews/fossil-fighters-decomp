// A character drawn as animated 3D objects played from a scene script (MAnmModelBase and the other MAnm kinds).
#ifndef M_POLYGON_CHAR_H
#define M_POLYGON_CHAR_H

#include "m/task.h"

class MAnmModelBase;

class MPolygonChar : public MProcess {
public:
    MPolygonChar(void *unk80, u16 unkA4, void *unk84, void *unk88);
    virtual ~MPolygonChar();
    virtual void Idle();

    void Reset();
    // Loads a scene script and the model it plays.
    void Load(const char *script, const char *model);

    MAnmModelBase *mUnk74;      // 0x74
    MAnmModelBase *mUnk78;      // 0x78
    MAnmModelBase *mUnk7C;      // 0x7C: restarted whenever mUnk74 loops
    void *mUnk80;               // 0x80
    void *mUnk84;               // 0x84
    void *mUnk88;               // 0x88
    u32 mUnk8C;                 // 0x8C
    u32 mUnk90;                 // 0x90
    u32 mUnk94;                 // 0x94
    u32 mUnk98;                 // 0x98
    u32 mUnk9C;                 // 0x9C
    u32 mUnkA0;                 // 0xA0
    u16 mUnkA4;                 // 0xA4
    u16 mUnkA6;                 // 0xA6
    u32 mUnkA8;                 // 0xA8
    u8 mUnkAC;                  // 0xAC
    int mUnkB0;                 // 0xB0
    int mEndFrame;              // 0xB4
    int mFrame;                 // 0xB8
    u8 unkBC[0x43C - 0xBC];
    u32 mUnk43C;                // 0x43C
    OSMutex mMutex;             // 0x440
    int mPauseCount;            // 0x458: the task sets stay suspended while this isn't 0
};

#endif
