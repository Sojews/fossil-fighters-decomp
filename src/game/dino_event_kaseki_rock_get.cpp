// DinoEventKasekiRockGet: the fossil rock get screen, and its container select.
// Also the string vector instances the game emits here.

#pragma ipa file

#include "game/dino_ui_container_drop_menu_page_inc_task_menu.h"
#include "game/dino_event_exec.h"
#include "game/dino_backup.h"
#include "m/lcd.h"

DinoEventKasekiRockGet::~DinoEventKasekiRockGet() {
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

MTask *DinoEventKasekiRockGetContainerSelect::CreateUi() {
    return new DinoUiContainerDropMenu(mUnk84[0], mUnk84[1], std::vector<std::string>(mUnk8C));
}
