// DinoEventKasekiRockGet: the fossil rock get screen, and its container select.
// Also the string vector instances the game emits here.

#pragma ipa file

#include "game/dino_ui_container_drop_menu_page_inc_task_menu.h"
#include "game/dino_event_exec.h"
#include "game/dino_backup.h"
#include "m/lcd.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_resource.h"
#include "m/text_codec.h"

DinoEventKasekiRockGet::~DinoEventKasekiRockGet() {
}

DinoEventKasekiRockGetContainerSelect::DinoEventKasekiRockGetContainerSelect(DinoEventLoop *event, u32 arg0,
                                                                             u32 arg1,
                                                                             const std::vector<std::string> &names)
    : DinoEventCaptureFadeUiTask(event, 3), mArg0(arg0), mArg1(arg1), mNames(names) {
}

DinoEventKasekiRockGetContainerSelect::~DinoEventKasekiRockGetContainerSelect() {
}

void DinoEventKasekiRockGet::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoEventKasekiRockGet::*StateEnd_04)();
    void (DinoEventKasekiRockGet::*StateSwap_0C)();
    u8 unk14[0x1C - 0x14];
    void (DinoEventKasekiRockGet::*StateReplace_1C)();
    void (DinoEventKasekiRockGet::*Unk02034ed8_24)();
    void (DinoEventKasekiRockGet::*Unk02034ed8_2C)();
    u8 unk34[0x3C - 0x34];
    void (DinoEventKasekiRockGet::*StateEnd_3C)();
    u8 unk44[0x4C - 0x44];
    void (DinoEventKasekiRockGet::*Unk02034ed8_4C)();
    void (DinoEventKasekiRockGet::*StateEnd_54)();
    void (DinoEventKasekiRockGet::*StateStart_5C)();
    void (DinoEventKasekiRockGet::*StateStore_64)();
    u8 unk6C[0x7C - 0x6C];
    void (DinoEventKasekiRockGet::*StateAskSwap_7C)();
} data_020bd1f8;

// "/msg/msg_dig" (the TU's string literals are pooled data too).
extern "C" const char data_020bd360[];

DinoEventKasekiRockGet::DinoEventKasekiRockGet(DinoEventLoop *event, u32 mode, const DinoBackupEntry *entry, u8 *result)
    : MProcess(gLcd->GetBottomDisplay()), mEvent(event), mMode(mode), mEntry(*entry), mMessages(data_020bd360),
      mResult(result) {
    mState = data_020bd1f8.StateStart_5C;
}

// "%new" and "%old", the tags the messages name the rocks by.
extern "C" const char data_020bd370[], data_020bd378[];
// Each mode's messages: stored, ask to swap, swapped, left, and two more; -1 for none.
extern "C" const u32 data_0208b260[][6];

std::string DinoReplaceTag(std::string text, std::string tag, std::string value);

// The message box saying the mode's message (or a one-frame wait if it has none), naming the rocks.
MTask *DinoEventKasekiRockGet::CreateMessageBox(u32 message) {
    u32 id = data_0208b260[mMode][message];
    if (id == (u32)-1) {
        return new MTimer(1);
    }
    const u8 *data = (const u8 *)mMessages.Get();
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[id];
    std::string text((const char *)entry + ((const u32 *)entry)[1]);
    text = DinoReplaceTag(text, data_020bd370, mName);
    text = DinoReplaceTag(text, data_020bd378, mSwapName);
    return new DinoCommonWindowMessageBox(MPoint(128, 96), std::string(text), 4);
}

// The dialog's second pair of labels.
extern "C" const u32 data_0208a67c, data_0208a678;

// The mode's message as a yes/no question (or a one-frame wait if it has none), naming the rocks: its last two lines are
// the answers.
// NONMATCHING: the game copies the size before scaling it for the answers' addresses (as KasekiRockFound's).
MTask *DinoEventKasekiRockGet::CreateYesNo(u32 message) {
    u32 id = data_0208b260[mMode][message];
    if (id == (u32)-1) {
        return new MTimer(1);
    }
    const u8 *data = (const u8 *)mMessages.Get();
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[id];
    std::string text((const char *)entry + ((const u32 *)entry)[1]);
    text = DinoReplaceTag(text, data_020bd370, mName);
    text = DinoReplaceTag(text, data_020bd378, mSwapName);
    std::vector<std::string> lines = MSplitString(text, '\n');
    std::string yes(*(lines.end() - 2));
    std::string no(*(lines.end() - 1));
    lines.erase(lines.end() - 2, lines.end());
    text = MJoinStrings(lines, '\n');
    DinoCommonWindowYesNoDialog *dialog = new DinoCommonWindowYesNoDialog(text, &mAnswer);
    dialog->SetLabels(yes, no);
    dialog->SetLabels2(std::string(gDinoResource->GetText(data_0208a67c)), std::string(gDinoResource->GetText(data_0208a678)));
    return dialog;
}

void DinoEventKasekiRockGet::StateStart() {
    gDinoBackup->SetPlayerUnk58((const DinoPlayerUnk58 *)&mEntry);
    mName = gDinoBackup->GetEntryName(&mEntry);
    if (!gDinoBackup->IsUnk98Full()) {
        mState = data_020bd1f8.StateStore_64;
    } else {
        mState = data_020bd1f8.StateAskSwap_7C;
    }
}

void DinoEventKasekiRockGet::StateAskSwap() {
    mAnswer = 1;
    if (mMode != 0) {
        mSerial.AddTail(CreateMessageBox(0));
    }
    mSerial.AddTail(CreateYesNo(1));
    mState = data_020bd1f8.Unk02034ed8_24;
}

void DinoEventKasekiRockGet::StateAnswered() {
    if (mAnswer != 0) {
        mState = data_020bd1f8.Unk02034ed8_4C;
    } else {
        mState = data_020bd1f8.StateEnd_54;
    }
}

void DinoEventKasekiRockGet::StateChosen() {
    if (mSwapIndex == -1) {
        mState = data_020bd1f8.StateEnd_3C;
    } else {
        mState = data_020bd1f8.StateSwap_0C;
    }
}

void DinoEventKasekiRockGet::StateConfirmed() {
    if (mAnswer != 0) {
        mState = data_020bd1f8.StateEnd_04;
    } else {
        mState = data_020bd1f8.Unk02034ed8_2C;
    }
}

void DinoEventKasekiRockGet::StateSwap() {
    DinoBackupEntry entry;
    gDinoBackup->GetUnk98(mSwapIndex, &entry);
    mSwapName = gDinoBackup->GetEntryName(&entry);
    mState = data_020bd1f8.StateReplace_1C;
}

// Stores the rock in a free slot.
void DinoEventKasekiRockGet::StateStore() {
    gDinoBackup->SetUnk98(gDinoBackup->FindFreeUnk98(), &mEntry);
    if (mResult != NULL) {
        *mResult = TRUE;
    }
    GetSerial()->AddTail(CreateMessageBox(0), MTaskExitTransfer::Create(this));
}

// Stores it over the one chosen.
void DinoEventKasekiRockGet::StateReplace() {
    gDinoBackup->SetUnk98(mSwapIndex, &mEntry);
    if (mResult != NULL) {
        *mResult = TRUE;
    }
    GetSerial()->AddTail(CreateMessageBox(2), MTaskExitTransfer::Create(this));
}

// Leaves the rock.
void DinoEventKasekiRockGet::StateEnd() {
    if (mResult != NULL) {
        *mResult = FALSE;
    }
    GetSerial()->AddTail(CreateMessageBox(3), MTaskExitTransfer::Create(this));
}

template class std::__vec_deleter<std::string, std::allocator<std::string> >;
// The game keeps this std::string member's one copy here (other files call it).
namespace std {
template <class C, class Traits, class Alloc>
typename basic_string<C, Traits, Alloc>::size_type basic_string<C, Traits, Alloc>::find(const basic_string &str,
                                                                                        size_type pos) const {
    const C *s;
    size_type n;
    if (!str.is_long()) {
        s = str.mShort.buf;
        n = str.mShort.size;
    } else {
        s = str.mLong.data;
        n = str.mLong.size;
    }
    return find(s, pos, n);
}
template string::size_type string::find(const string &str, string::size_type pos) const;
}

// NONMATCHING: the game puts the reallocating branch's tail at the end.
template void std::__vec_deleter<std::string, std::allocator<std::string> >::assign<std::string *>(
    std::string *first, std::string *last, Metrowerks::int2type<true>);

MTask *DinoEventKasekiRockGetContainerSelect::CreateUi() {
    return new DinoUiContainerDropMenu(mArg0, mArg1, std::vector<std::string>(mNames));
}
