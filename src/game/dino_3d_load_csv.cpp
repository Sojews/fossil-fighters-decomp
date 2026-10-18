// Dino3DLoadCsv: plays a battle scene script (CSV) and takes commands from the battle code.
// Not decompiled yet: the script reader, Idle, the sound name loader and the scene loaders.

#include "game/dino_3d_load_csv.h"
#include "m/anm_model.h"
#include "m/3d_model.h"
#include "m/process_pause.h"
#include "nitro/mi.h"

// The sound name tables.
static u8 sSoundNamesLoaded;
static std::vector<Dino3DSoundName> *sSongCommands;
static std::vector<Dino3DSoundName> *sSoundData;

// A script keyword and its value; tables end with -1.
struct Dino3DName {
    const char *mName;
    int mValue;
};

// (Kept until the script reader, which uses them, is decompiled.)
#pragma force_active on

// Makes a key from a script line's values if it has a value (flags[1]), and a maximum (flags[2]).
static BOOL Dino3DMakeKey(MAnmKey *key, int frame, const u8 *flags, const int *values) {
    if (flags[1] == 1) {
        key->mFrame = frame;
        key->mMin = values[1];
        key->mMax = values[1];
        key->mUnkC = 0;
        if (flags[2] == 1) {
            key->mMax = values[2];
        }
        return TRUE;
    }
    return FALSE;
}

static int Dino3DStrCaseCmp(const char *s1, const char *s2) {
    while (*s1 != 0 && *s2 != 0) {
        char c1 = *s1;
        char c2 = *s2;
        if (c1 >= 'A' && c1 <= 'Z') {
            c1 += 0x20;
        }
        if (c2 >= 'A' && c2 <= 'Z') {
            c2 += 0x20;
        }
        if (c1 != c2) {
            break;
        }
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

static int Dino3DLookup(const char *name, const Dino3DName *table) {
    while (table->mValue != -1) {
        if (Dino3DStrCaseCmp(table->mName, name) == 0) {
            break;
        }
        table++;
    }
    return table->mValue;
}

#pragma force_active reset

Dino3DLoadCsv::Dino3DLoadCsv(void *unk80, u16 unkA4, void *unk84, void *unk88)
    : MProcess(NULL), mUnk74(NULL), mUnk80(unk80), mUnk84(unk84), mUnk88(unk88), mUnkA4(unkA4), mUnk468(0) {
    OS_InitMutex(&mPauseMutex);
    OS_InitMutex(&mCommandMutex);
    mParallel.SetUseMutex(TRUE);
    mUnk440 = 0;
    mPauseCount = 0;
    MI_CpuFill8(mParentName, 0, sizeof(mParentName) + sizeof(mJointName));
    Reset();
}

Dino3DLoadCsv::Dino3DLoadCsv(const char *path, void *unk80, u16 unkA4, void *unk84, void *unk88, u32 unk468)
    : MProcess(NULL), mUnk74(NULL), mUnk80(unk80), mUnk84(unk84), mUnk88(unk88), mUnkA4(unkA4), mUnk468(unk468) {
    OS_InitMutex(&mPauseMutex);
    OS_InitMutex(&mCommandMutex);
    mParallel.SetUseMutex(TRUE);
    mUnk440 = 0;
    mPauseCount = 0;
    MI_CpuFill8(mParentName, 0, sizeof(mParentName) + sizeof(mJointName));
    Reset();
    Load(path, 0);
}

Dino3DLoadCsv::Dino3DLoadCsv(u32 index, void *unk80, u16 unkA4, void *unk84, void *unk88)
    : MProcess(NULL), mUnk74(NULL), mUnk80(unk80), mUnk84(unk84), mUnk88(unk88), mUnkA4(unkA4), mUnk468(0) {
    OS_InitMutex(&mPauseMutex);
    OS_InitMutex(&mCommandMutex);
    mParallel.SetUseMutex(TRUE);
    mUnk440 = 0;
    mPauseCount = 0;
    MI_CpuFill8(mParentName, 0, sizeof(mParentName) + sizeof(mJointName));
    Reset();
    LoadScene(index, 0);
}

void Dino3DLoadCsv::Reset() {
    MProcessPause pause(this, &mPauseMutex, &mPauseCount);
    MScopedLock lock(&mCommandMutex, TRUE);
    mUnk98 = 0;
    mUnk9C = 0;
    mUnkA0 = 0;
    mUnk8C = 0;
    mUnk90 = 0;
    mUnk94 = 0;
    mUnkB0 = 0x1000;
    mUnkAC = 1;
    mUnkA6 = 0x7FFF;
    mUnk464 = NULL;
    mUnkA8 = 0;
    mEndFrame = 0x0FFFFFFF;
    mFrame = 0;
    mUnk74 = NULL;
    mUnk78 = NULL;
    mUnk7C = NULL;
    mParallel.Clear();
    mCommands.clear();
}

Dino3DLoadCsv::~Dino3DLoadCsv() {
}

BOOL Dino3DLoadCsv::IsStopped() {
    BOOL stopped = TRUE;
    if (mUnk74 != NULL && mUnk74->mState == STATE_RUN) {
        stopped = FALSE;
    }
    return stopped;
}

int Dino3DLoadCsv::GetFrameCount() {
    if (mUnk74 != NULL) {
        if (mUnk74->mModel != NULL) {
            return mUnk74->mModel->GetFrameCount();
        }
        return 0;
    }
    return 0;
}

int Dino3DLoadCsv::GetUnkB0() {
    return mUnkB0;
}

u16 Dino3DLoadCsv::GetUnk78State() {
    int paused;
    {
        MMutexLock lock(&mPauseMutex);
        paused = mPauseCount;
    }
    if (paused != 0) {
        return 2;
    }
    if (mUnk78 != NULL) {
        return mUnk78->mState;
    }
    return 2;
}

BOOL Dino3DLoadCsv::IsUnk74Done() {
    int paused;
    {
        MMutexLock lock(&mPauseMutex);
        paused = mPauseCount;
    }
    if (paused != 0) {
        return TRUE;
    }
    if (mUnk74 != NULL) {
        if (mUnk74->mModel != NULL) {
            return mUnk74->mModel->IsAnimDone();
        }
        return FALSE;
    }
    return TRUE;
}

void Dino3DLoadCsv::SetUnk464(Dino3DLoadCsv *parent) {
    MMutexLock lock(&mCommandMutex);
    mUnk464 = parent;
}

void Dino3DLoadCsv::PushVec2(const VecFx32 *v) {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 2;
    command.mArgs[0] = v->x;
    command.mArgs[1] = v->y;
    command.mArgs[2] = v->z;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::PushVec3(const VecFx32 *v) {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 3;
    command.mArgs[0] = v->x;
    command.mArgs[1] = v->y;
    command.mArgs[2] = v->z;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push4(u32 arg) {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 4;
    command.mArgs[0] = arg;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push7(u32 arg) {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 7;
    command.mArgs[0] = arg;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push8(u32 arg) {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 8;
    command.mArgs[0] = arg;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push11() {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 11;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push12() {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 12;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push15() {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 15;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push17() {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 17;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push18() {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 18;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push19(u32 arg) {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 19;
    command.mArgs[0] = arg;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::PushVec0(const VecFx32 *v) {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 0;
    command.mArgs[0] = v->x;
    command.mArgs[1] = v->y;
    command.mArgs[2] = v->z;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push20(u32 a, u32 b) {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 20;
    command.mArgs[0] = a;
    command.mArgs[1] = b;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push21(u32 arg) {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 21;
    command.mArgs[0] = arg;
    mCommands.push_back(command);
}

void Dino3DLoadCsv::Push22(u32 a, u32 b) {
    MScopedLock lock(&mCommandMutex, TRUE);
    Dino3DCommand command;
    command.mType = 22;
    command.mArgs[0] = a;
    command.mArgs[1] = b;
    mCommands.push_back(command);
}

void Dino3DInitSoundNames() {
    sSoundData = new std::vector<Dino3DSoundName>();
    sSongCommands = new std::vector<Dino3DSoundName>();
}

void Dino3DDestroySoundNames() {
    Dino3DClearSoundNames();
    delete sSoundData;
    delete sSongCommands;
}

void Dino3DClearSoundNames() {
    sSoundNamesLoaded = FALSE;
    sSoundData->clear();
    sSongCommands->clear();
}
