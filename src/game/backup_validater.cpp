// Save data checks run before the game reads or writes its save.

#include <string>
#include "game/backup_validater.h"
#include "game/common_window_message.h"
#include "game/help.h"

extern "C" {
extern void *data_020cc00c;
extern void *data_0214d1b8;
extern void *data_020cb978;
extern const int data_0208ad0c;
extern const int data_0208ad08;
BOOL func_02013ab8(void);
MTask *func_02069ed8(void *arg);
void func_02050a3c(void);
void func_02050ba8(void);
void func_0201ffe8(void);
void *func_0204c084(void *mgr);
void func_0205d134(void *arg);
const char *func_02011dc8(void *table, int id);
}

DinoBackupValidater::DinoBackupValidater(int messageId, MTask *caller, void *arg) : MProcess(NULL) {
    mMessageId = messageId;
    mCaller = caller;
    mUnk7C = arg;
}

void DinoBackupValidater::Idle() {
    if (data_020cc00c != NULL) {
        if (func_02013ab8()) {
            Exit();
            return;
        }
        if (mCaller != NULL) {
            mCaller->Exit();
            mSerial.AddTail(new MTimer(1));
        }
        if (mUnk7C != NULL) {
            mSerial.AddTail(func_02069ed8(mUnk7C));
        }
        func_02050a3c();
        func_02050ba8();
        func_0201ffe8();
        new DinoHelpDisabler();
        func_0205d134(*(void **)((u8 *)func_0204c084(data_0214d1b8) + 0x24));
        MTask *window = new DinoCommonWindowMessage(func_0204c084(data_0214d1b8), MPoint(0x80, 0x60),
                                                    func_02011dc8(data_020cb978, mMessageId), 4, 0);
        mSerial.AddTail(window);
    }
}

DinoBackupReadValidater::DinoBackupReadValidater(MTask *caller, void *arg)
    : DinoBackupValidater(data_0208ad0c, caller, arg) {
}

DinoBackupWriteValidater::DinoBackupWriteValidater(MTask *caller, void *arg)
    : DinoBackupValidater(data_0208ad08, caller, arg) {
}
