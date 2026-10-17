// The view showing the cleaning image on a BG screen.
#ifndef GAME_DINO_CLEANING_IMAGE_VIEW_H
#define GAME_DINO_CLEANING_IMAGE_VIEW_H

#include "m/view.h"
#include "m/screen.h"

class DinoCleaningImageView : public MFixedSizeView {
public:
    virtual ~DinoCleaningImageView();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();

    u32 unk18[2];
    MPoint mPos;        // 0x20
    MScreen *mScreen;   // 0x28
};

#endif
