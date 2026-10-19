// Save data checks run before the game reads or writes its save.

#pragma ipa file

#define MSL_STRING_BASICS
#define MSL_STRING_REPLACE_DEFINITION
#include <string>
#include "game/backup_validater.h"
#include "game/common_window_message.h"
#include "game/help.h"
#include "m/lcd.h"
#include "m/multi_play.h"
#include "m/bright.h"

extern "C" {
extern void *gDinoBackup;
extern void *data_020cb978;
extern const int data_0208ad0c;
extern const int data_0208ad08;
BOOL func_02013ab8(void);

void func_02050a3c(void);
void func_02050ba8(void);
void func_0201ffe8(void);
const char *func_02011dc8(void *table, int id);
}

DinoBackupValidater::DinoBackupValidater(int messageId, MTask *caller, void *arg) : MProcess(NULL) {
    mMessageId = messageId;
    mCaller = caller;
    mUnk7C = arg;
}

void DinoBackupValidater::Idle() {
    if (gDinoBackup != NULL) {
        if (func_02013ab8()) {
            Exit();
            return;
        }
        if (mCaller != NULL) {
            mCaller->Exit();
            mSerial.AddTail(new MTimer(1));
        }
        if (mUnk7C != NULL) {
            mSerial.AddTail(((MMultiPlay *) mUnk7C)->CreateFinishTask());
        }
        func_02050a3c();
        func_02050ba8();
        func_0201ffe8();
        new DinoHelpDisabler();
        gLcd->GetBottomDisplay()->mBright->Reset();
        MTask *window = new DinoCommonWindowMessage(gLcd->GetBottomDisplay(), MPoint(0x80, 0x60),
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

// String members the game emits in this file.
// (Calling __replace here first instantiates it with the name the game's callers use.)
#pragma dont_inline on
void DinoBackupValidaterReplaceAnchor(std::string *s, const char *first, const char *last) {
    s->__replace(0, 0, first, last, std::random_access_iterator_tag());
}
#pragma dont_inline reset
template void std::string::reserve(std::string::size_type n);
template std::string::basic_string(const char *s);
template std::string::~basic_string();
template std::string &std::string::assign(const char *first, const char *last);
