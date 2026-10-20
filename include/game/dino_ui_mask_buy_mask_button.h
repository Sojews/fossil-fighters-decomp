#ifndef GAME_DINO_UI_MASK_BUY_MASK_BUTTON_H
#define GAME_DINO_UI_MASK_BUY_MASK_BUTTON_H

#include "m/view.h"
#include "m/menu_item.h"
#include "m/motion.h"
#include <string>

// The masks' motions (by mask, 20 of them), and the file's other motion paths (the details panel is [22]).
extern "C" const char *data_ov005_0216b320[];
extern "C" const char *data_ov005_0216b180[];

class DinoUiMaskBuyMaskButton : public MMenuItem {
public:
    // The mask's button at pos (centered), with its details panel (hidden) and its mark (shown if flag); mask 0 is an
    // empty slot.
    DinoUiMaskBuyMaskButton(MSmartOam *oam, MPoint pos, u32 mask, u8 flag) : mPos(pos) {
        SetMode(2);
        mMotion = new MMotion(std::string(data_ov005_0216b320[mask % 20]), oam, 2, 1);
        mMotion->Play(1, 0);
        mMotion->SetPosition(pos);
        mMotion->SetPriority(1);
        mMotion->Show();
        mMotion->GetUnit()->SetMode(1);
        mParallel.AddTail(mMotion);
        mView = new MMotion(std::string(data_ov005_0216b180[22]), oam, 2, 1);
        mView->Play(1, 0);
        mView->SetPosition(pos);
        mView->SetPriority(1);
        mView->Hide();
        mView->GetUnit()->SetMode(1);
        mParallel.AddTail(mView);
        mMark = new MMotion(std::string(data_ov005_0216b180[22]), oam, 2, 1);
        mMark->Play(2, 0);
        mMark->SetPosition(pos);
        mMark->SetPriority(1);
        mMark->GetUnit()->SetMode(1);
        mParallel.AddTail(mMark);
        if (flag) {
            mMark->Show();
        } else {
            mMark->Hide();
        }
        if (mask == 0) {
            mMotion->Hide();
            mMark->Hide();
        }
    }
    // An empty slot (mask 0, no mark); its path is read through the file's data pool.
    DinoUiMaskBuyMaskButton(MSmartOam *oam, MPoint pos);
    virtual ~DinoUiMaskBuyMaskButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);

    MPoint mPos;  // 0x88: the center
    MMotion *mMotion; // 0x90
    MMotion *mView; // 0x94: the mask's details
    MMotion *mMark; // 0x98: shown while the mask is chosen
};

#endif
