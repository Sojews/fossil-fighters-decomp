// SetEpisodeTask, DinoEventFieldMenu, DinoEventMedalList, DinoEventDebugMenu, DinoEventSaveOption, DinoEventSave, DinoEventATM, DinoEventNameEntry, DinoEventSenryuEntry, DinoEventSenryuListEntry, DinoEventShopBuy, DinoEventShopSell, DinoEventShopMask, DinoEventMaskEquip, DinoEventLicenseCard, DinoEventEnding, DinoEventMuseum, DinoEventReviveEffectCaller.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#define DINO_EVENT_UI_TASK_OWNER
#define DINO_EVENT_CAPTURE_FADE_UI_TASK_OWNER
#include "game/set_episode_task.h"
#include "game/dino_event_field_menu.h"
#include "game/dino_event_medal_list.h"
#include "game/dino_event_debug_menu.h"
#include "game/dino_event_save_option.h"
#include "game/dino_event_save.h"
#include "game/dino_event_atm.h"
#include "game/dino_event_name_entry.h"
#include "game/dino_event_senryu_entry.h"
#include "game/dino_event_senryu_list_entry.h"
#include "game/dino_event_shop_buy.h"
#include "game/dino_event_canon_putin.h"
#include "game/dino_event_canon_takeout.h"
#include "game/dino_backup.h"
#include "game/dino_event_shop_sell.h"
#include "game/dino_event_shop_mask.h"
#include "game/dino_event_mask_equip.h"
#include "game/dino_event_license_card.h"
#include "game/dino_event_ending.h"
#include "game/dino_event_museum.h"
#include "game/dino_event_revive_effect_caller.h"
#include "game/dino_event_capture_fade_ui_task.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/overlay.h"
#include "m/lcd.h"
#include "m/bright.h"
#include "m/screen.h"
#include "game/dino_event_ui_task.h"
#include "game/dino_ui_shop_buy.h"
#include "game/dino_ui_shop_sell.h"
#include "game/dino_ui_mask_buy.h"
#include "game/dino_ui_mask_equip.h"
#include "game/dino_ui_canon_takeout.h"
#include "game/dino_ui_atm.h"
#include "game/dino_ui_ending.h"
#include "game/dino_ui_license_card.h"
#include "game/dino_ui_canon_putin.h"
#include "game/dino_ui_save_menu.h"
#include "game/dino_ui_save_option.h"
#include "game/dino_ui_debug_menu.h"
#include "game/dino_ui_field_menu.h"
#include "game/dino_ui_museum.h"
#include "game/dino_ui_keyboard.h"
#include "game/dino_ui_senryu.h"
#include "game/dino_ui_medal_menu.h"

DinoEventUiTask::~DinoEventUiTask() {
}

DinoEventCaptureFadeUiTask::~DinoEventCaptureFadeUiTask() {
}

SetEpisodeTask::~SetEpisodeTask() {
}

DinoEventFieldMenu::~DinoEventFieldMenu() {
}

DinoEventMedalList::~DinoEventMedalList() {
}

DinoEventDebugMenu::~DinoEventDebugMenu() {
}

DinoEventSaveOption::~DinoEventSaveOption() {
}

DinoEventSave::~DinoEventSave() {
}

DinoEventATM::~DinoEventATM() {
}

DinoEventNameEntry::~DinoEventNameEntry() {
}

DinoEventSenryuEntry::~DinoEventSenryuEntry() {
}

DinoEventSenryuListEntry::~DinoEventSenryuListEntry() {
}

DinoEventShopBuy::~DinoEventShopBuy() {
}

DinoEventShopSell::~DinoEventShopSell() {
}

DinoEventShopMask::~DinoEventShopMask() {
}

DinoEventMaskEquip::~DinoEventMaskEquip() {
}

DinoEventLicenseCard::~DinoEventLicenseCard() {
}

DinoEventEnding::~DinoEventEnding() {
}

DinoEventMuseum::~DinoEventMuseum() {
}

DinoEventReviveEffectCaller::~DinoEventReviveEffectCaller() {
}

void SetEpisodeTask::Execute() {
    if (mSeen) {
        if (!gDinoBackup->GetEpisodeSeen(mId)) {
            gDinoBackup->SetEpisodeSeen(mId, TRUE);
            mEvent->AddEpisode(mId);
            mEvent->SetEpisodesChanged();
        }
    } else {
        if (gDinoBackup->GetEpisodeSeen(mId)) {
            gDinoBackup->SetEpisodeSeen(mId, FALSE);
            mEvent->RemoveEpisode(mId);
            mEvent->ResetEpisode(mId);
            mEvent->SetEpisodesChanged();
        }
    }
    Exit();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoEventSaveOption>;
#pragma auto_inline off
template MTask *MCreateBgCallTask(DinoEventSaveOption *obj, void (DinoEventSaveOption::*func)(), int thread);
#pragma auto_inline reset
template class MBgCallTask1<DinoEventLoop, u32>;
// (Its creator is in dino_event.cpp; this copy makes mwcc emit the class's virtuals here.)
template MTask *MCreateBgCallTask(DinoEventLoop *obj, void (DinoEventLoop::*func)(u32), u32 a0, int thread);
template class MBgCallTask0<DinoEventCaptureFadeUiTask>;
template MTask *MCreateBgCallTask(DinoEventCaptureFadeUiTask *obj, void (DinoEventCaptureFadeUiTask::*func)(), int thread);

DinoEventCanonTakeout::~DinoEventCanonTakeout() {
    gDinoBackup->SetSystemFlag(0x26, mCount != gDinoBackup->CountUsedUnk14C());
}

DinoEventCanonPutin::~DinoEventCanonPutin() {
    gDinoBackup->SetSystemFlag(0x25, mCount != gDinoBackup->CountUsedUnk14C());
}

MTask *DinoEventCanonTakeout::CreateUi() {
    return new DinoUiCanonTakeout();
}

MTask *DinoEventMaskEquip::CreateUi() {
    return new DinoUiMaskEquip();
}

MTask *DinoEventShopMask::CreateUi() {
    return new DinoUiMaskBuy();
}

MTask *DinoEventShopSell::CreateUi() {
    return new DinoUiShopSell();
}

MTask *DinoEventShopBuy::CreateUi() {
    return new DinoUiShopBuy();
}

MTask *DinoEventATM::CreateUi() {
    return new DinoUiATM(0);
}

MTask *DinoEventEnding::CreateUi() {
    return new DinoUiEnding();
}

MTask *DinoEventLicenseCard::CreateUi() {
    return new DinoUiLicenseCard(1);
}

MTask *DinoEventCanonPutin::CreateUi() {
    return new DinoUiCanonPutin();
}

MTask *DinoEventSave::CreateUi() {
    return new DinoUiSaveMenu(&mResult, 1);
}

MTask *DinoEventDebugMenu::CreateUi() {
    return new DinoUiDebugMenu();
}

MTask *DinoEventFieldMenu::CreateUi() {
    MTask *ui = new DinoUiFieldMenu(mUnk84);
    if (ui->mState == MTask::STATE_RUN) {
        ui->Execute();
    }
    return ui;
}

MTask *DinoEventMuseum::CreateUi() {
    return new DinoUiMuseum();
}

MTask *DinoEventSenryuListEntry::CreateUi() {
    return new DinoUiSenryuList();
}

MTask *DinoEventSenryuEntry::CreateUi() {
    return new DinoUiKeyboard(2, 0);
}

MTask *DinoEventNameEntry::CreateUi() {
    return new DinoUiKeyboard(0, 0, mUnk81);
}

MTask *DinoEventMedalList::CreateUi() {
    MTask *ui = new DinoUiMedalList();
    if (ui->mState == MTask::STATE_RUN) {
        ui->Execute();
    }
    return ui;
}

#pragma auto_inline off
// Vector instances the game emits in this file (made explicitly until the code that uses them is decompiled).
template class std::__vec_deleter<u32, std::allocator<u32> >;

// The event loop's episode map's members the game emits in this file.
typedef std::__red_black_tree<u32, std::pair<const u32, DinoEventEpisode>,
                              std::__select_first<std::pair<const u32, DinoEventEpisode> >, std::less<u32>,
                              std::allocator<std::pair<const u32, DinoEventEpisode> > >
    DinoEventEpisodeTree;
template class std::__red_black_tree<u32, std::pair<const u32, DinoEventEpisode>,
                                     std::__select_first<std::pair<const u32, DinoEventEpisode> >, std::less<u32>,
                                     std::allocator<std::pair<const u32, DinoEventEpisode> > >;

#pragma auto_inline off

// The field's capture fades (in the field overlay).
extern "C" MProcess *func_ov001_02159cd0(DinoField *field);
extern "C" MProcess *func_ov001_02159d3c(DinoField *field);
M_EXTERN_OVERLAY(ov004);

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoEventSaveOption::*OpenUi_00)();
} data_020bd9d8;

// Fades the field out, slides the screen down while the save options load, runs them, then brings the field back
// (or ends the event when the game was quit).
void DinoEventSaveOption::Idle() {
    switch (mStep++) {
    case 0:
        mSerial.AddTail(func_ov001_02159cd0(mEvent->GetField()));
        break;
    case 1:
        gLcd->GetBottomDisplay()->mBright->SetMode(2);
        gLcd->GetBottomDisplay()->mBright->SetFactor(16);
        mParallel.AddTail(mEvent->mScreen->CreateOffsetAnime(MPoint(0, -92), MPoint(0, 0), 29, -2));
        mSerial.AddTail(MCreateBgCallTask(this, data_020bd9d8.OpenUi_00, 1));
        break;
    case 2:
        mSerial.AddTail(mUi);
        break;
    case 3:
        MUnloadOverlay(0, M_OVERLAY_ID(ov004));
        if (mResult == 0) {
            mEvent->CreateField();
            mSerial.AddTail(func_ov001_02159d3c(mEvent->GetField()));
        } else {
            mEvent->SetResult(0);
            mEvent->Exit();
            mSerial.AddTail(new MTimer(1));
        }
        break;
    case 4:
        Exit();
        break;
    }
}
