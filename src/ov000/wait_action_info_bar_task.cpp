// WaitActionInfoBarTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/wait_action_info_bar_task.h"
#include "types.h"
#include "game/dino_resource.h"
#include "game/dino_battle_view_ability_menu_bg.h"
#include "game/dino_battle_view_ability_text_window.h"
#include "m/indicator.h"
#include "m/oam_canvas.h"
#include "game/dino_static_init.h"

WaitActionInfoBarTask::WaitActionInfoBarTask(DinoBattleViewActionInfoBar *bar) {
    mBar = bar;
}

WaitActionInfoBarTask::~WaitActionInfoBarTask() {
}

#pragma auto_inline off

extern "C" void func_ov000_0218b4dc(DinoBattleViewActionInfoBar *bar, const char *text);

// Lists the action's text in the bar (when the defs ask for it).
extern "C" void func_ov000_0218b4c0(DinoBattleViewActionInfoBar *bar, u32 textId) {
    func_ov000_0218b4dc(bar, gDinoResource->GetText(textId));
}

extern "C" void func_ov000_0218b4dc(DinoBattleViewActionInfoBar *bar, const char *text) {
    if (gDinoResource->GetBattleAdjustDefs()->actionLog) {
        bar->mLines.push_back(std::string(text));
    }
}

extern "C" DinoBattleViewAbilityTextWindow *func_ov000_0217c0b8(DinoBattleViewAbilityMenuBG *bg, int arg);

// The TU's pooled constants.
extern "C" struct {
    u8 unk00[8];
    int textY;      // 0x08
    u8 unk0C[0x18];
    int textX;      // 0x24: the text is centered on it
} data_ov000_02196f64;

// Shows a line (making the text and its window the first time).
// (opt_common_subs off: the game loads gCurrentDisplay for the canvas after copying the size.)
#pragma opt_common_subs off
extern "C" void func_ov000_0218b55c(DinoBattleViewActionInfoBar *bar, const char *text) {
    bar->mTimer = gDinoResource->GetBattleAdjustDefs()->actionLog;
    MPoint size = bar->mFont->MeasureText(std::string(text), 0, 0);
    if (bar->mText != NULL) {
        bar->mText->SetPosition(data_ov000_02196f64.textX - size.x / 2, data_ov000_02196f64.textY);
    } else {
        MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, MPoint(0x100, 0x10), gDinoResource->GetUnkTable(), TRUE);
        bar->mText = new MIndicator(canvas, bar->mFont, 0);
        canvas->GetOamUnit()->SetMode(1);
        bar->mText->SetPosition(data_ov000_02196f64.textX - size.x / 2, data_ov000_02196f64.textY);
        bar->mText->SetPriority(0);
        bar->mParallel.AddTail(bar->mText);
        bar->mBg = new DinoBattleViewAbilityMenuBG;
        bar->mWindow = func_ov000_0217c0b8(bar->mBg, 1);
        bar->mWindow->Show();
        bar->mWindow->SetUnk1C(0);
        bar->mParallel.AddTail(bar->mBg);
    }
    bar->mText->SetText(std::string(text));
}
#pragma opt_common_subs reset

// Shows the next line when none is up; fades the window in, and out once the line's time is over.
void DinoBattleViewActionInfoBar::Idle() {
    if (mText == NULL && !mLines.empty()) {
        func_ov000_0218b55c(this, mLines.front().c_str());
        mLines.erase(mLines.begin());
    }
    if (mText != NULL) {
        mTimer--;
        if (mTimer <= 0) {
            if (mFade > 0) {
                mFade--;
                if (mFade <= 0) {
                    mText->Exit();
                    mText = NULL;
                    mBg->Exit();
                    return;
                }
                mWindow->SetUnk1C(mFade);
            }
        } else if (mFade < 6) {
            mFade++;
            mWindow->SetUnk1C(mFade);
        }
        if (mFade == 6) {
            mText->Show();
        } else {
            mText->Hide();
        }
    }
}

BOOL DinoBattleViewActionInfoBar::IsIdle() {
    if (mText == NULL) {
        return TRUE;
    }
    return FALSE;
}

#pragma auto_inline on

void WaitActionInfoBarTask::Execute() {
    if (mBar->IsIdle() && mBar->mLines.empty()) {
        Exit();
    }
}

// The file's static initialization (see dino_static_init.h).
static struct WaitActionInfoBarTaskStaticInit {
    WaitActionInfoBarTaskStaticInit() {
        DINO_POOL_WORD(data_ov000_02196f64, 0x14) = 128;
        DINO_POOL_WORD(data_ov000_02196f64, 0x18) = 176;
        DINO_POOL_WORD(data_ov000_02196f64, 0x4) = 8;
        DINO_POOL_WORD(data_ov000_02196f64, 0x8) = 150;
        DINO_POOL_WORD(data_ov000_02196f64, 0xc) = 64;
        DINO_POOL_WORD(data_ov000_02196f64, 0x10) = 112;
        DINO_POOL_WORD(data_ov000_02196f64, 0x24) = 128;
        DINO_POOL_WORD(data_ov000_02196f64, 0x28) = 160;
    }
} sWaitActionInfoBarTaskStaticInit;
