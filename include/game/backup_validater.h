#ifndef GAME_BACKUP_VALIDATER_H
#define GAME_BACKUP_VALIDATER_H

#include "m/task.h"

// Checks the save data before it's read or written, and shows a message if it can't be.
class DinoBackupValidater : public MProcess {
public:
    DinoBackupValidater(int messageId, MTask *caller, void *arg);
    virtual void Idle();

    int mMessageId;     // 0x74
    MTask *mCaller;     // 0x78: exits if the check fails
    void *mUnk7C;       // 0x7C
};

class DinoBackupReadValidater : public DinoBackupValidater {
public:
    DinoBackupReadValidater(MTask *caller, void *arg);
};

class DinoBackupWriteValidater : public DinoBackupValidater {
public:
    DinoBackupWriteValidater(MTask *caller, void *arg);
};

#endif
