// Dino3DLoadCsv: plays a battle scene script (CSV) as animated 3D objects, like MPolygonChar, and takes commands
// from the battle code through a queue.
#ifndef GAME_DINO_3D_LOAD_CSV_H
#define GAME_DINO_3D_LOAD_CSV_H

#include <list>
#include <string>
#include <vector>
#include "m/task.h"
#include "m/archive.h"
#include "m/resource.h"
#include "nitro/fx.h"

class MAnmModelBase;

// A sound (song command or sound data) name from the ID tables.
struct Dino3DSoundName {
    std::string mName;  // 0x0
    u32 mId;            // 0xC
};

// The scene script archive's header: file indices, then the files' paths.
struct Dino3DSceneFiles {
    MArchiveFile GetFile(const MArchivePtr &archive, u32 index) const {
        return archive.GetFile((const char *) this + mPaths, ((const u32 *) ((const u8 *) this + mIndices))[index]);
    }

    u32 unk0[2];
    u32 mIndices;   // 0x8
    u32 mPaths;     // 0xC
};

// A queued command.
struct Dino3DCommand {
    int mType;      // 0x00
    u32 mArgs[4];   // 0x04
};

class Dino3DLoadCsv : public MProcess {
public:
    Dino3DLoadCsv(void *unk80, u16 unkA4, void *unk84, void *unk88);
    Dino3DLoadCsv(const char *path, void *unk80, u16 unkA4, void *unk84, void *unk88, u32 unk468);
    Dino3DLoadCsv(u32 index, void *unk80, u16 unkA4, void *unk84, void *unk88);
    virtual ~Dino3DLoadCsv();
    virtual void Idle();

    void Reset();
    void Load(const char *path, u32 arg);
    void LoadScene(u32 index, u32 arg);
    void LoadFile(const char *path, void *unk84, u32 arg);
    void LoadByName(const char *name, BOOL alt, u32 variant);
    void LoadSoundNames(const char *dir);
    void ParseScript(char *text, void *unk84, const char *dir, u32 arg, u32 arg2);
    BOOL IsStopped();
    int GetFrameCount();
    int GetUnkB0();
    u16 GetUnk78State();
    BOOL IsUnk74Done();
    void SetUnk464(Dino3DLoadCsv *parent);
    void SetParent(Dino3DLoadCsv *parent);
    void PushVec2(const VecFx32 *v);
    void PushVec3(const VecFx32 *v);
    void Push4(u32 arg);
    void Push7(u32 arg);
    void Push8(u32 arg);
    void Push11();
    void Push12();
    void Push15();
    void Push17();
    void Push18();
    void Push19(u32 arg);
    void PushVec0(const VecFx32 *v);
    void Push20(u32 a, u32 b);
    void Push21(u32 arg);
    void Push22(u32 a, u32 b);

    MAnmModelBase *mUnk74;          // 0x74
    MAnmModelBase *mUnk78;          // 0x78
    MAnmModelBase *mUnk7C;          // 0x7C
    void *mUnk80;                   // 0x80
    void *mUnk84;                   // 0x84
    void *mUnk88;                   // 0x88
    u32 mUnk8C;                     // 0x8C
    u32 mUnk90;                     // 0x90
    u32 mUnk94;                     // 0x94
    u32 mUnk98;                     // 0x98
    u32 mUnk9C;                     // 0x9C
    u32 mUnkA0;                     // 0xA0
    u16 mUnkA4;                     // 0xA4
    u16 mUnkA6;                     // 0xA6
    u32 mUnkA8;                     // 0xA8
    u8 mUnkAC;                      // 0xAC
    int mUnkB0;                     // 0xB0
    int mEndFrame;                  // 0xB4
    int mFrame;                     // 0xB8
    const char *mPath;              // 0xBC
    u8 unkC0[0x440 - 0xC0];
    u8 mUnk440;                     // 0x440
    char mParentName[16];           // 0x441
    char mJointName[16];            // 0x451
    Dino3DLoadCsv *mUnk464;         // 0x464
    u32 mUnk468;                    // 0x468
    std::list<Dino3DCommand> mCommands; // 0x46C
    OSMutex mPauseMutex;            // 0x478
    OSMutex mCommandMutex;          // 0x490
    int mPauseCount;                // 0x4A8
};

// The sound name tables, loaded with the first scene.
void Dino3DInitSoundNames();
void Dino3DDestroySoundNames();
void Dino3DClearSoundNames();

#endif
