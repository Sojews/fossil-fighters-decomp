// DinoBattleDebugLog.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#pragma ipa file

#include "game/dino_static_init.h"
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
                                MPoint(0, 2), MFx32Const(0.3));
        mConsole->SetUnk3C(MFx32(4));
        mConsole->Show();
        mParallel.AddTail(mConsole);
        mInputHub = new MSingleInputHub(1);
        mParallel.AddTail(mInputHub);
    }
}

// The file's static initialization (see dino_static_init.h): the battle help areas' rects and texts.
// NONMATCHING: 97.7%, scheduling only (the game sets up the next constant after the truncated colour's first store).
extern "C" u32 data_ov000_0219569c[], data_ov000_0219581c[], data_ov000_0219589c[], data_ov000_021958dc[], data_ov000_0219591c[], data_ov000_0219595c[], data_ov000_0219599c[];
extern "C" const u32 data_0208a370, data_0208a378, data_0208a37c, data_0208a380, data_0208a384, data_0208a38c, data_0208a390, data_0208a394, data_0208a3b8, data_0208a3bc, data_0208a3c0, data_0208a3c4;
static struct DinoBattleDebugLogStaticInit {
    DinoBattleDebugLogStaticInit() {
        DINO_POOL_WORD(data_ov000_0219569c, 0x44) = 128;
        DINO_POOL_WORD(data_ov000_0219569c, 0x48) = 176;
        DINO_POOL_WORD(data_ov000_0219569c, 0x24) = 8;
        DINO_POOL_WORD(data_ov000_0219569c, 0x28) = 150;
        DINO_POOL_WORD(data_ov000_0219569c, 0x54) = 64;
        DINO_POOL_WORD(data_ov000_0219569c, 0x58) = 112;
        DINO_POOL_WORD(data_ov000_0219569c, 0x2c) = 128;
        DINO_POOL_WORD(data_ov000_0219569c, 0x30) = 152;
        DINO_POOL_WORD(data_ov000_0219581c, 0x68) = 0;
        DINO_POOL_WORD(data_ov000_0219581c, 0x6c) = 20;
        DINO_POOL_WORD(data_ov000_0219581c, 0x70) = data_0208a3c4;
        DINO_POOL_WORD(data_ov000_0219589c, 0x8) = 0;
        DINO_POOL_WORD(data_ov000_0219589c, 0xc) = -20;
        DINO_POOL_WORD(data_ov000_0219589c, 0x10) = data_0208a3c0;
        DINO_POOL_WORD(data_ov000_0219589c, 0x28) = -36;
        DINO_POOL_WORD(data_ov000_0219589c, 0x2c) = 0;
        DINO_POOL_WORD(data_ov000_0219589c, 0x30) = data_0208a3bc;
        DINO_POOL_WORD(data_ov000_0219589c, 0x48) = 36;
        DINO_POOL_WORD(data_ov000_0219589c, 0x4c) = 0;
        DINO_POOL_WORD(data_ov000_0219589c, 0x50) = data_0208a3b8;
        DINO_POOL_WORD(data_ov000_0219581c, 0x28) = 36;
        DINO_POOL_WORD(data_ov000_0219581c, 0x2c) = 0;
        DINO_POOL_WORD(data_ov000_0219581c, 0x30) = data_0208a3b8;
        ((volatile u16 *)data_ov000_021958dc)[0x16 / 2] = data_0208a394;
        DINO_POOL_WORD(data_ov000_0219589c, 0x58) = 65;
        DINO_POOL_WORD(data_ov000_0219589c, 0x5c) = 76;
        DINO_POOL_WORD(data_ov000_0219589c, 0x60) = 100;
        DINO_POOL_WORD(data_ov000_0219589c, 0x64) = 111;
        ((volatile u16 *)data_ov000_021958dc)[0x2a / 2] = (u16)data_0208a390;
        DINO_POOL_WORD(data_ov000_0219589c, 0x6c) = 35;
        DINO_POOL_WORD(data_ov000_0219589c, 0x70) = 38;
        DINO_POOL_WORD(data_ov000_0219589c, 0x74) = 70;
        DINO_POOL_WORD(data_ov000_0219589c, 0x78) = 73;
        ((volatile u16 *)data_ov000_021958dc)[0x3e / 2] = (u16)data_0208a390;
        DINO_POOL_WORD(data_ov000_0219591c, 0x0) = 30;
        DINO_POOL_WORD(data_ov000_0219591c, 0x4) = 116;
        DINO_POOL_WORD(data_ov000_0219591c, 0x8) = 65;
        DINO_POOL_WORD(data_ov000_0219591c, 0xc) = 151;
        ((volatile u16 *)data_ov000_0219591c)[0x12 / 2] = data_0208a38c;
        DINO_POOL_WORD(data_ov000_0219591c, 0x14) = 0;
        DINO_POOL_WORD(data_ov000_0219591c, 0x18) = 77;
        DINO_POOL_WORD(data_ov000_0219591c, 0x1c) = 30;
        DINO_POOL_WORD(data_ov000_0219591c, 0x20) = 112;
        ((volatile u16 *)data_ov000_0219591c)[0x26 / 2] = data_0208a384;
        DINO_POOL_WORD(data_ov000_0219591c, 0x28) = 0;
        DINO_POOL_WORD(data_ov000_0219591c, 0x2c) = 161;
        DINO_POOL_WORD(data_ov000_0219591c, 0x30) = 75;
        DINO_POOL_WORD(data_ov000_0219591c, 0x34) = 191;
        ((volatile u16 *)data_ov000_0219591c)[0x3a / 2] = data_0208a380;
        DINO_POOL_WORD(data_ov000_0219591c, 0x3c) = 155;
        DINO_POOL_WORD(data_ov000_0219591c, 0x40) = 76;
        DINO_POOL_WORD(data_ov000_0219591c, 0x44) = 190;
        DINO_POOL_WORD(data_ov000_0219591c, 0x48) = 111;
        ((volatile u16 *)data_ov000_0219595c)[0xe / 2] = (u16)data_0208a37c;
        DINO_POOL_WORD(data_ov000_0219591c, 0x50) = 185;
        DINO_POOL_WORD(data_ov000_0219591c, 0x54) = 38;
        DINO_POOL_WORD(data_ov000_0219591c, 0x58) = 220;
        DINO_POOL_WORD(data_ov000_0219591c, 0x5c) = 73;
        ((volatile u16 *)data_ov000_0219595c)[0x22 / 2] = (u16)data_0208a37c;
        DINO_POOL_WORD(data_ov000_0219591c, 0x64) = 190;
        DINO_POOL_WORD(data_ov000_0219591c, 0x68) = 116;
        DINO_POOL_WORD(data_ov000_0219591c, 0x6c) = 225;
        DINO_POOL_WORD(data_ov000_0219591c, 0x70) = 151;
        ((volatile u16 *)data_ov000_0219595c)[0x36 / 2] = data_0208a378;
        DINO_POOL_WORD(data_ov000_0219591c, 0x78) = 220;
        DINO_POOL_WORD(data_ov000_0219591c, 0x7c) = 77;
        DINO_POOL_WORD(data_ov000_0219599c, 0x0) = 255;
        DINO_POOL_WORD(data_ov000_0219599c, 0x4) = 112;
        ((volatile u16 *)data_ov000_0219599c)[0xa / 2] = data_0208a370;
        DINO_POOL_WORD(data_ov000_0219599c, 0xc) = 180;
        DINO_POOL_WORD(data_ov000_0219599c, 0x10) = 161;
        DINO_POOL_WORD(data_ov000_0219599c, 0x14) = 255;
        DINO_POOL_WORD(data_ov000_0219599c, 0x18) = 191;
    }
} sDinoBattleDebugLogStaticInit;

#pragma auto_inline off
// The status effect icon's vector members the game emits in this file.
template size_t std::__vec_deleter<StatusEffectEntry, std::allocator<StatusEffectEntry> >::grow_by(size_t n);
template void std::__vec_deleter<StatusEffectEntry, std::allocator<StatusEffectEntry> >::append_realloc(size_t n, const StatusEffectEntry &x);
template std::__vec_deleter_helper<StatusEffectEntry, std::allocator<StatusEffectEntry> &>::~__vec_deleter_helper();
template void std::__vec_deleter<StatusEffectEntry, std::allocator<StatusEffectEntry> &>::allocate(size_t n, Metrowerks::int2type<true>);
