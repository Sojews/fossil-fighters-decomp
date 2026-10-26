// The game's sound interface and its tasks.

#include "m/archive.h"
#include <algorithm>
#include <string>
#include "game/dino_sound.h"
#include "sound_api.h"
#include "m/heap.h"

// mwcc lays out a file's statics in an order that depends on the whole file; this order gives the game's
// (fade-up step, volume, BGM, target volume, fade-down step).
static u32 sVolume = 101;
static u32 sFadeDownStep = 8;
static u32 sTargetVolume = 101;
static int sBgm = -1;
static u32 sFadeUpStep = 8;

static u8 sInitialized;
static int sPauseCount;
static MMutex sMutex;
static int sScene;
static u8 sHeap[0x80000];

// Locks the sound mutex for the rest of the scope, if asked to. The functions here leave a word unused below
// the lock, as if it had a field before the mutex.
class DinoSoundLock {
public:
    DinoSoundLock(OSMutex *mutex, u8 lock) {
        mMutex = mutex;
        mLocked = lock;
        if (mLocked) {
            OS_LockMutex(mMutex);
        }
    }
    DinoSoundLock(OSMutex *mutex) {
        mMutex = mutex;
        mLocked = TRUE;
        OS_LockMutex(mMutex);
    }
    ~DinoSoundLock() {
        if (mLocked) {
            OS_UnlockMutex(mMutex);
        }
    }

    u32 mUnk00;
    OSMutex *mMutex;
    u8 mLocked;
};

static void DinoSoundNothing() {
}

void DinoSoundInit(u8 arg) {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    if (sInitialized) {
        return;
    }
    sInitialized = TRUE;
    DinoSoundNothing();
    func_020031d0();
    func_020031dc(sHeap, sizeof(sHeap), "/etc/snd_data.sdat");
    func_02003234(101);
    func_02003228(101);
    new DinoSoundMainTask();
}

void DinoSoundPlayBgm(int bgm, int arg) {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    if (sBgm == bgm && arg == 0) {
        return;
    }
    sBgm = bgm;
    func_02003200(bgm, arg, 0);
}

// bgm -1 stops the BGM that was last played.
void DinoSoundStopBgm(int bgm, int fadeFrames) {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    if (bgm == -1) {
        bgm = sBgm;
        sBgm = -1;
        if (bgm == -1) {
            return;
        }
    }
    func_0200320c(bgm, fadeFrames);
}

void DinoSoundStopBgmAll(int fadeFrames) {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    func_02003264(fadeFrames);
}

void DinoSoundPlaySe(int se, int arg) {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    func_02003200(se, arg, 0);
}

void DinoSoundStopSe(int se, int fadeFrames) {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    func_0200320c(se, fadeFrames);
}

void DinoSoundFunc_0201feb4(int fadeFrames) {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    func_02003258(fadeFrames);
}

void DinoSoundFunc_0201feec(int id) {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    func_0200321c(id, 0);
}

void DinoSoundLoadScene(int scene) {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    if (sScene == scene) {
        return;
    }
    sScene = scene;
    if (scene != 0) {
        func_02003284(scene);
    }
}

int DinoSoundGetScene() {
    return sScene;
}

void DinoSoundSetVolume(int volume) {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    func_02003228(volume);
}

void DinoSoundSetTargetVolume(int volume) {
    DinoSoundLock lock(&sMutex.mutex);
    sTargetVolume = volume;
}

void DinoSoundPause() {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    if (sPauseCount++ == 0) {
        func_02003240();
    }
}

void DinoSoundResume() {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    if (--sPauseCount == 0) {
        func_0200324c();
    }
}

MTask *DinoSoundCreatePlayBgmTask(int bgm, int arg) {
    return new DinoSoundPlayBgmTask(bgm, arg);
}

MTask *DinoSoundCreateStopBgmTask(int bgm, int fadeFrames) {
    return new DinoSoundStopBgmTask(bgm, fadeFrames);
}

MTask *DinoSoundCreateStopBgmAllTask(int fadeFrames) {
    return new DinoSoundStopBgmAllTask(fadeFrames);
}

MTask *DinoSoundCreatePlaySeTask(int se, int arg) {
    return new DinoSoundPlaySeTask(se, arg);
}

MTask *DinoSoundCreateStopSeTask(int se, int fadeFrames) {
    return new DinoSoundStopSeTask(se, fadeFrames);
}

void DinoSoundFunc_0202016c(int type, int id, int a, int b) {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    func_02003270(type, id, a, b);
}

MTask *DinoSoundCreateLoadSceneTask(int scene) {
    return new DinoSoundLoadSceneTask(scene);
}

void DinoSoundLoadSceneTask::Transfer() {
    if (mScene != sScene) {
        DinoSoundLoadScene(mScene);
    }
}

void DinoSoundStopSeTask::Transfer() {
    DinoSoundStopSe(mSe, mFadeFrames);
}

void DinoSoundPlaySeTask::Transfer() {
    DinoSoundPlaySe(mSe, mArg);
}

void DinoSoundStopBgmAllTask::Transfer() {
    DinoSoundStopBgmAll(mFadeFrames);
}

void DinoSoundStopBgmTask::Transfer() {
    DinoSoundStopBgm(mBgm, mFadeFrames);
}

void DinoSoundPlayBgmTask::Transfer() {
    DinoSoundPlayBgm(mBgm, mArg);
}

void DinoSoundMainTask::OnVSync() {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    func_020031f4();
}

// NONMATCHING: the game's frame has the unused word below the std::min temporaries (the up one is a slot higher).
void DinoSoundMainTask::OnPostLoop() {
    DinoSoundLock lock(&sMutex.mutex, TRUE);
    if (sVolume != sTargetVolume) {
        if (sVolume < sTargetVolume) {
            sVolume += std::min(sFadeUpStep, sTargetVolume - sVolume);
        } else {
            u32 diff = sVolume - sTargetVolume;
            sVolume -= std::min(sFadeDownStep, diff);
        }
        func_02003234(sVolume);
    }
    func_020031e8();
}

// (It calls the replace in dino_resource.cpp.)
template <> std::string &std::string::replace(std::string::size_type pos, std::string::size_type n, const char *s);
// A string member the game emits at the end of this file.
template std::string &std::string::operator=(const char *s);
