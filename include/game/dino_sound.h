// The game's sound interface: the sound system behind a mutex, with BGM fading and tasks that play and stop
// sounds when they're run.
#ifndef GAME_DINO_SOUND_H
#define GAME_DINO_SOUND_H

#include "m/task.h"
#include "m/looper.h"

// (The argument, from the excavate defs, is unused.)
void DinoSoundInit(u8 arg);
void DinoSoundPlayBgm(int bgm, int arg);
void DinoSoundStopBgm(int bgm, int fadeFrames);
void DinoSoundStopBgmAll(int fadeFrames);
void DinoSoundPlaySe(int se, int arg);
void DinoSoundStopSe(int se, int fadeFrames);
void DinoSoundFunc_0201feb4(int fadeFrames);
void DinoSoundFunc_0201feec(int id);
void DinoSoundLoadScene(int scene);
int DinoSoundGetScene();
void DinoSoundSetVolume(int volume);
void DinoSoundSetTargetVolume(int volume);
void DinoSoundPause();
void DinoSoundResume();
void DinoSoundFunc_0202016c(int type, int id, int a, int b);

MTask *DinoSoundCreatePlayBgmTask(int bgm, int arg);
MTask *DinoSoundCreateStopBgmTask(int bgm, int fadeFrames);
MTask *DinoSoundCreateStopBgmAllTask(int fadeFrames);
MTask *DinoSoundCreatePlaySeTask(int se, int arg);
MTask *DinoSoundCreateStopSeTask(int se, int fadeFrames);
MTask *DinoSoundCreateLoadSceneTask(int scene);

// Updates the sound system each frame and fades the BGM volume towards its target.
class DinoSoundMainTask : public MLooperPostLoopHook, public MLooperVSyncHook {
public:
    DinoSoundMainTask() : MLooperPostLoopHook(FALSE, FALSE), MLooperVSyncHook(FALSE, FALSE) {
        MLooperPostLoopHook::Enable();
        MLooperVSyncHook::Enable();
    }
    virtual ~DinoSoundMainTask() {}
    virtual void OnPostLoop();
    virtual void OnVSync();
};

class DinoSoundPlayBgmTask : public MTransfer {
public:
    DinoSoundPlayBgmTask(int bgm, int arg) {
        mBgm = bgm;
        mArg = arg;
    }
    virtual ~DinoSoundPlayBgmTask() {}
    virtual void Transfer();

    int mBgm;   // 0x10
    int mArg;   // 0x14
};

class DinoSoundStopBgmTask : public MTransfer {
public:
    DinoSoundStopBgmTask(int bgm, int fadeFrames) {
        mBgm = bgm;
        mFadeFrames = fadeFrames;
    }
    virtual ~DinoSoundStopBgmTask() {}
    virtual void Transfer();

    int mBgm;           // 0x10
    int mFadeFrames;    // 0x14
};

class DinoSoundStopBgmAllTask : public MTransfer {
public:
    DinoSoundStopBgmAllTask(int fadeFrames) { mFadeFrames = fadeFrames; }
    virtual ~DinoSoundStopBgmAllTask() {}
    virtual void Transfer();

    int mFadeFrames;    // 0x10
};

class DinoSoundPlaySeTask : public MTransfer {
public:
    DinoSoundPlaySeTask(int se, int arg) {
        mSe = se;
        mArg = arg;
    }
    virtual ~DinoSoundPlaySeTask() {}
    virtual void Transfer();

    int mSe;    // 0x10
    int mArg;   // 0x14
};

class DinoSoundStopSeTask : public MTransfer {
public:
    DinoSoundStopSeTask(int se, int fadeFrames) {
        mSe = se;
        mFadeFrames = fadeFrames;
    }
    virtual ~DinoSoundStopSeTask() {}
    virtual void Transfer();

    int mSe;            // 0x10
    int mFadeFrames;    // 0x14
};

class DinoSoundLoadSceneTask : public MTransfer {
public:
    DinoSoundLoadSceneTask(int scene) { mScene = scene; }
    virtual ~DinoSoundLoadSceneTask() {}
    virtual void Transfer();

    int mScene;     // 0x10
};

#endif
