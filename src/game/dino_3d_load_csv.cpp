// Dino3DLoadCsv: plays a battle scene script (CSV) and takes commands from the battle code.
// Not decompiled yet: the script reader, Idle, the sound name loader and the scene loaders.

#include "game/dino_3d_load_csv.h"
#include "m/3d_resource.h"
#include "m/file.h"
#include <string.h>
#include <printf.h>
#include "m/anm_model.h"
#include "m/3d_model.h"
#include "m/process_pause.h"
#include "nitro/mi.h"

extern "C" {
char *ReadLine(char **p);
fx32 ParseFx32(char **p);
void ReadField(char **p, char *out);
BOOL SkipComma(char **p);
}

// The sound name tables.
static u8 sSoundNamesLoaded;
static std::vector<Dino3DSoundName> *sSoundData;
static std::vector<Dino3DSoundName> *sSongCommands;

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
    mRot.x = 0;
    mRot.y = 0;
    mRot.z = 0;
    mPos.x = 0;
    mPos.y = 0;
    mPos.z = 0;
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

#pragma opt_common_subs off
void Dino3DLoadCsv::LoadScene(u32 index, u32 arg) {
    MResource<Dino3DSceneFiles> scenes("/model/battle/arcscenecsv");
    MArchiveFile file = scenes.Get()->GetFile(scenes.mArchive, index);
    mPath = "";
    LoadSoundNames("/model/battle/");
    Reset();
    char *text = new char[file.GetSize()];
    file.Read(text, file.GetSize());
    ParseScript(text, mUnk84, "/model/battle/", arg, 0);
    delete[] text;
}
#pragma opt_common_subs reset

void Dino3DLoadCsv::Load(const char *path, u32 arg) {
    MFile file;
    int last = 0;
    char dir[0x80];
    strcpy(dir, path);
    for (u32 i = 0; i < strlen(dir); i++) {
        if (dir[i] == '/') {
            last = i;
        }
    }
    int name = last + 1;
    dir[name] = 0;
    LoadSoundNames(dir);
    mPath = path;
    Reset();
    if (file.Open(mPath)) {
        char *text = new char[file.GetLength() + 1];
        text[file.GetLength()] = 0;
        file.Read(text, file.GetLength());
        file.Close();
        ParseScript(text, mUnk84, dir, arg, 0);
        delete[] text;
    }
    // The camera ("cm_") and sound ("snd_") scripts next to it.
    char other[0x80];
    strcpy(other, dir);
    strcat(other, "cm_");
    strcat(other, path + name);
    mPath = other;
    if (MFileExists(mPath)) {
        if (file.Open(mPath)) {
            char *text = new char[file.GetLength() + 1];
            text[file.GetLength()] = 0;
            file.Read(text, file.GetLength());
            file.Close();
            ParseScript(text, (void *) ((u32) mUnk84 | 4), dir, arg, 0);
            delete[] text;
        }
    }
    strcpy(other, dir);
    strcat(other, "snd_");
    strcat(other, path + name);
    mPath = other;
    if (MFileExists(mPath)) {
        if (file.Open(mPath)) {
            char *text = new char[file.GetLength() + 1];
            text[file.GetLength()] = 0;
            file.Read(text, file.GetLength());
            file.Close();
            ParseScript(text, mUnk84, dir, arg, 0);
            delete[] text;
        }
    }
}

void Dino3DLoadCsv::LoadFile(const char *path, void *unk84, u32 arg) {
    MFile file;
    char dir[0x80];
    int last = 0;
    strcpy(dir, path);
    for (u32 i = 0; i < strlen(dir); i++) {
        if (dir[i] == '/') {
            last = i;
        }
    }
    dir[last + 1] = 0;
    mPath = path;
    if (file.Open(mPath)) {
        char *text = new char[file.GetLength() + 1];
        text[file.GetLength()] = 0;
        file.Read(text, file.GetLength());
        file.Close();
        ParseScript(text, unk84, dir, 0, arg);
        delete[] text;
    }
}

void Dino3DLoadCsv::LoadByName(const char *name, BOOL loop, u32 variant) {
    char *text = new char[0x200];
    mPath = name;
    Reset();
    sprintf(text, "\nscript\nModel,%s,1,%s\nname,din\nAnimSpeed,1\nscale_x,1\nscale_y,1\nscale_z,1\n", name,
            loop ? "loop" : "", variant);
    char path[0x80];
    strcpy(path, "/model/battle/");
    strcat(path, name);
    if (M3DModelExists(path)) {
        ParseScript(text, mUnk84, "/model/battle/", 0, 0);
    }
    delete[] text;
}

Dino3DLoadCsv::~Dino3DLoadCsv() {
}

void Dino3DLoadCsv::Idle() {
    MScopedLock pauseLock(&mPauseMutex, TRUE);
    MScopedLock commandLock(&mCommandMutex, TRUE);
    if (mPauseCount != 0) {
        return;
    }
    for (std::list<Dino3DCommand>::iterator it = mCommands.begin(); it != mCommands.end(); ++it) {
        switch (it->mType) {
        case 2:
            mRot.x = it->mArgs[0];
            mRot.y = it->mArgs[1];
            mRot.z = it->mArgs[2];
            if (mUnk74 != NULL) {
                mUnk74->SetRotRef(&mRot);
            }
            break;
        case 3:
            mPos.x = it->mArgs[0];
            mPos.y = it->mArgs[1];
            mPos.z = it->mArgs[2];
            if (mUnk74 != NULL) {
                mUnk74->SetPosRef(&mPos);
            }
            break;
        case 4:
            for (MAnmModelBase *anm = (MAnmModelBase *) mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
                anm->SetFrame(it->mArgs[0]);
            }
            break;
        case 6:
            mUnkA6 = it->mArgs[0];
            if (mUnk74 != NULL) {
                mUnk74->SetUnk138(it->mArgs[0]);
            }
            break;
        case 7:
            mUnkB0 = it->mArgs[0];
            for (MAnmModelBase *anm = (MAnmModelBase *) mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
                anm->SetUnk134(it->mArgs[0]);
            }
            break;
        case 8:
            if (mUnk74 != NULL) {
                mUnk74->SetUnk117(it->mArgs[0] != 0 ? TRUE : FALSE);
            }
            break;
        case 9:
            for (MAnmModelBase *anm = (MAnmModelBase *) mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
                anm->mActive = FALSE;
            }
            break;
        case 10:
            for (MAnmModelBase *anm = (MAnmModelBase *) mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
                anm->mActive = TRUE;
            }
            break;
        case 11:
            mUnkAC = 0;
            for (MAnmModelBase *anm = (MAnmModelBase *) mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
                anm->mFollowParent = FALSE;
            }
            break;
        case 12:
            mUnkAC = 1;
            for (MAnmModelBase *anm = (MAnmModelBase *) mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
                anm->mFollowParent = TRUE;
            }
            break;
        case 13:
            for (MAnmModelBase *anm = (MAnmModelBase *) mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
                anm->Unk10();
                anm->mFrame = 0;
                anm->mLoopEnd = 0;
                anm->mSpeed = 0x10;
                anm->Run();
                anm->mActive = TRUE;
                anm->mFollowParent = TRUE;
            }
            break;
        case 14:
            for (MAnmModelBase *anm = (MAnmModelBase *) mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
                anm->Run();
            }
            break;
        case 15:
            for (MAnmModelBase *anm = (MAnmModelBase *) mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
                anm->Suspend();
            }
            break;
        case 17:
            if (mUnk78 != NULL) {
                mUnk78->Run();
            }
            break;
        case 18:
            if (mUnk78 != NULL) {
                mUnk78->Suspend();
            }
            break;
        case 19:
            if (mUnk78 != NULL) {
                mUnk78->mEndFrame = it->mArgs[0];
            }
            break;
        case 0:
            if (mUnk78 != NULL) {
                fx32 y = it->mArgs[1];
                fx32 z = it->mArgs[2];
                MAnmTargetCamera *camera = (MAnmTargetCamera *) mUnk78;
                camera->mTarget.x = it->mArgs[0];
                camera->mTarget.y = y;
                camera->mTarget.z = z;
            }
            break;
        case 20:
            if (mUnk74 != NULL && mUnk74->mModel != NULL) {
                mUnk74->mModel->SetUnk2F4(it->mArgs[0]);
                mUnk74->mModel->SetUnk1C(it->mArgs[1]);
            }
            break;
        case 22:
            if (mUnk74 != NULL) {
                mUnk74->TintPalette(it->mArgs[0], it->mArgs[1]);
            }
            break;
        case 21:
            for (MAnmModelBase *anm = (MAnmModelBase *) mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
                if (anm->mModel != NULL) {
                    anm->mModel->SetFlag100(it->mArgs[0] != 0 ? TRUE : FALSE);
                }
            }
            break;
        }
    }
    mCommands.clear();
    if (mFrame >= mEndFrame) {
        Reset();
        return;
    }
    mFrame++;
    MAnmModelBase *anm = mUnk74;
    if (anm != NULL && mUnkAC == 1 && mUnkB0 > 0x10) {
        BOOL looped;
        if (anm->mModel != NULL) {
            int frames = anm->mModel->GetFrameCount();
            looped = (anm->mFrame - anm->mStartFrame) % (frames << 4) == 0 ? TRUE : FALSE;
        } else {
            looped = FALSE;
        }
        if (looped) {
            MAnmModelBase *restart = mUnk7C;
            if (restart != NULL) {
                restart->Unk10();
                restart->mFrame = 0;
                restart->mLoopEnd = 0;
                restart->mSpeed = 0x10;
                mUnk7C->Run();
            }
        }
    }
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

void Dino3DLoadCsv::SetParent(Dino3DLoadCsv *parent) {
    if (mUnk78 == NULL) {
        return;
    }
    mUnk464 = parent;
    mUnk78->SetParent(NULL, -1);
    if (mParentName[0] == 0) {
        return;
    }
    Dino3DLoadCsv *p = mUnk464;
    int paused;
    if (p != NULL) {
        MMutexLock lock(&p->mPauseMutex);
        paused = p->mPauseCount;
    } else {
        paused = 0;
    }
    if (paused != 0) {
        return;
    }
    p = mUnk464;
    if (p == NULL) {
        return;
    }
    for (MAnmModelBase *anm = (MAnmModelBase *) p->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
        if (Dino3DStrCaseCmp(anm->mName.c_str(), mParentName) == 0) {
            int joint = -1;
            if (strlen(mJointName) != 0) {
                joint = anm->mModel->FindJoint(mJointName);
            }
            mUnk78->SetParent(anm, joint);
            return;
        }
    }
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
    sSongCommands = new std::vector<Dino3DSoundName>();
    sSoundData = new std::vector<Dino3DSoundName>();
}

void Dino3DDestroySoundNames() {
    Dino3DClearSoundNames();
    delete sSongCommands;
    delete sSoundData;
}

void Dino3DClearSoundNames() {
    sSoundNamesLoaded = FALSE;
    sSongCommands->clear();
    sSoundData->clear();
}

// Loads the sound name tables (song commands, then sound data: an id and a name a line) from the directory, once.
void Dino3DLoadCsv::LoadSoundNames(const char *dir) {
    MProcessPause pause(this, &mPauseMutex, &mPauseCount);
    if (sSoundNamesLoaded) {
        return;
    }
    sSoundNamesLoaded = TRUE;
    sSongCommands->clear();
    sSoundData->clear();
    char path[0x80];
    MFile file;
    strcpy(path, dir);
    strcat(path, "songcmd_id.csv");
    if (file.Open(path)) {
        char *text = new char[file.GetLength() + 1];
        text[file.GetLength()] = 0;
        file.Read(text, file.GetLength());
        file.Close();
        char *cursor = text;
        for (char *line = ReadLine(&cursor); line != NULL; line = ReadLine(&cursor)) {
            for (int i = 0; i < 16; i++) {
                mFieldPresent[i] = mFieldNumeric[i] = 0;
            }
            int n = 0;
            do {
                ReadField(&line, mFields[n]);
                mFieldPresent[n] = 1;
                n++;
            } while (n < 16 && SkipComma(&line));
            for (int i = 0; i < 16; i++) {
                if (mFieldPresent[i]) {
                    char *field = mFields[i];
                    mFieldValues[i] = ParseFx32(&field);
                    if (mFieldValues[i] != 0x7fffffff) {
                        mFieldNumeric[i] = 1;
                    }
                }
            }
            if (mFieldNumeric[0] && mFieldPresent[1] && strlen(mFields[1]) != 0) {
                Dino3DSoundName name;
                name.mId = mFieldValues[0] >> FX32_SHIFT;
                name.mName = mFields[1];
                sSongCommands->push_back(name);
            }
        }
        delete[] text;
    }
    strcpy(path, dir);
    strcat(path, "sounddata_id.csv");
    if (file.Open(path)) {
        char *text = new char[file.GetLength() + 1];
        text[file.GetLength()] = 0;
        file.Read(text, file.GetLength());
        file.Close();
        char *cursor = text;
        for (char *line = ReadLine(&cursor); line != NULL; line = ReadLine(&cursor)) {
            for (int i = 0; i < 16; i++) {
                mFieldPresent[i] = mFieldNumeric[i] = 0;
            }
            int n = 0;
            do {
                ReadField(&line, mFields[n]);
                mFieldPresent[n] = 1;
                n++;
            } while (n < 16 && SkipComma(&line));
            for (int i = 0; i < 16; i++) {
                if (mFieldPresent[i]) {
                    char *field = mFields[i];
                    mFieldValues[i] = ParseFx32(&field);
                    if (mFieldValues[i] != 0x7fffffff) {
                        mFieldNumeric[i] = 1;
                    }
                }
            }
            if (mFieldNumeric[0] && mFieldPresent[1] && strlen(mFields[1]) != 0) {
                Dino3DSoundName name;
                name.mId = mFieldValues[0] >> FX32_SHIFT;
                name.mName = mFields[1];
                sSoundData->push_back(name);
            }
        }
        delete[] text;
    }
}

#pragma auto_inline off
template class std::__vec_deleter<Dino3DSoundName, std::allocator<Dino3DSoundName> >;
template std::__vec_deleter<Dino3DSoundName, std::allocator<Dino3DSoundName> &>::~__vec_deleter();
