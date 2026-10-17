// DinoCleaningPaper: sheets of paper that drift down the screen while cleaning.

#include "game/dino_cleaning_paper.h"
#include "m/motion.h"
#include "m/seed_generator.h"

extern const char *data_ov002_021645cc[];   // the sheets' motion files

DinoCleaningPaper::DinoCleaningPaper(MSmartOam *oam0, MSmartOam *oam1, MRandom *random) : MProcess(NULL) {
    mRandom = random;
    mDone = FALSE;
    mOams[0] = oam0;
    mOams[1] = oam1;
}

void DinoCleaningPaper::SetDone() {
    mDone = TRUE;
}

DinoCleaningPaper::~DinoCleaningPaper() {
}

void DinoCleaningPaper::Idle() {
    int x;
    const char *name;
    u32 i;
    MTask *anime;
    MTask *exit;
    int frames;

    if (mDone) {
        return;
    }
    name = data_ov002_021645cc[mRandom->Next(6)];
    x = mRandom->Next(0x100);
    frames = mRandom->Next(60) + 120;
    for (i = 0; i < 2; i++) {
        MMotion *motion = new MMotion(std::string(name), mOams[i], 0, 1);
        motion->Play(1, 0);
        motion->Show();
        exit = MTaskExitTransfer::Create(motion);
        MPoint from(x, -200);
        MPoint to(x, 200);
        anime = motion->CreateCoordAnime(from, to, frames, 0);
        MParallelTaskSet *set = new MParallelTaskSet(TRUE);
        set->AddTail(anime);
        set->AddTail(exit);
        MParallelTaskSet *parallel = &mParallel;
        parallel->AddTail(set);
        parallel->AddTail(motion);
    }
    MTask *timer = new MTimer(5);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
}
