// DinoBattleDebugLog.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#pragma ipa file

#include "game/dino_battle_debug_log.h"
#include "types.h"
#include "game/dino_resource.h"
#include "m/canvas.h"
#include "m/console.h"
#include "m/display.h"
#include "m/input.h"
#include "game/view_status_effect_icon.h"

DinoBattleDebugLog::~DinoBattleDebugLog() {
}

#pragma auto_inline off

// A console of the battle's log, when the adjustment data asks for one.
// NONMATCHING: the game loads the speed argument straight from its stack slot (ours through its address).
DinoBattleDebugLog::DinoBattleDebugLog(MDisplay *display) : MProcess(display) {
    if (gDinoResource->GetBattleAdjustDefs()->debugLog) {
        MDisplayScope scope(display);
        MFont *font = gDinoResource->CreateFont12(0, 10, 1);
        mConsole = new MConsole(new MTextCanvas(MPoint(0x100, 0xCC), gDinoResource->GetUnkTable(), 0, TRUE), font,
                                MPoint(0, 2), MFx32::FromRaw(0x4CD));
        mConsole->SetUnk3C(MFx32(4));
        mConsole->Show();
        mParallel.AddTail(mConsole);
        mInputHub = new MSingleInputHub(1);
        mParallel.AddTail(mInputHub);
    }
}

#pragma auto_inline off
// The status effect icon's vector members the game emits in this file.
template size_t std::__vec_deleter<StatusEffectEntry, std::allocator<StatusEffectEntry> >::grow_by(size_t n);
template void std::__vec_deleter<StatusEffectEntry, std::allocator<StatusEffectEntry> >::append_realloc(size_t n, const StatusEffectEntry &x);
