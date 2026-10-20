// DinoCommonItem.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_common_item_menu.h"
#include "game/dino_common_item.h"
#include "game/dino_static_init.h"

DinoCommonItem::~DinoCommonItem() {
}

MRect DinoCommonItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoCommonItem::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoCommonItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoCommonItem::OnDecide() {
}

MRect DinoCommonItem::GetFrame() {
    MPoint d(-mOam->mOffset.x, -mOam->mOffset.y);
    MRect r = mFrame;
    r.Offset(d);
    return r;
}

void DinoCommonItem::OnSelectChanged(BOOL selected) {
    UpdateLook();
    if (mView) {
        if (selected) {
            mView->SetPosition(MPoint(mPos.x + 1, mPos.y + 1));
        } else {
            mView->SetPosition(mPos);
        }
    }
}


void DinoCommonItem::OnFocusChanged(BOOL focused) {
    UpdateLook();
}

void DinoCommonItem::UpdateLook() {
    if (IsSelected()) {
        mMotion->Play(3, 0);
    } else if (IsFocused()) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoCommonItem::OnEnableChanged(BOOL enabled) {
    if (mView) {
        if (enabled) {
            mView->SetColors(0, 1, 10);
        } else {
            mView->SetColors(0, 11, 10);
        }
    }
}

// Per look: the frame around the position (left, top, right, bottom; four words a look), and the motion.
extern "C" const MRect data_020ba7d0[];
extern "C" const char *data_020ba7a0[];

// NONMATCHING: the label's size is read through a pointer and swaps slots with the label's spacing.
DinoCommonItem::DinoCommonItem(u32 type, MSmartOam *oam, MPoint pos, u32 priority, u32 label, u32 helpText) {
    mOam = oam;
    mTextId = helpText;
    mFrame = data_020ba7d0[type].Moved(pos);
    if (label) {
        MFont *font = gDinoResource->CreateFont12(0, 1, 10);
        mView = new DinoCommonLabel(oam, font, gDinoResource->GetUnkTable(), label, MPoint(0, 2));
        MPoint size = mView->GetSize();
        mPos = MPoint(pos.x - size.x / 2, pos.y - size.y / 2);
        mView->SetPriority(priority);
        mView->SetPosition(mPos);
        mView->Show();
        mParallel.AddTail(mView);
    } else {
        mView = NULL;
    }
    mMotion = new MMotion(std::string(data_020ba7a0[type]), oam, 0, 1);
    mMotion->SetPriority(priority);
    mMotion->SetAnchor(1);
    mMotion->SetPosition(pos);
    mMotion->Play(1, 0);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    if (mTextId) {
        DinoHelpItem::Enable();
    }
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_020ba764[];
extern "C" u32 data_020ba7e0[];
extern "C" u32 data_020ba7f0[];
extern "C" u32 data_020ba800[];
// NONMATCHING: scheduling (mwcc hoists the second rect's address load above the first rect's last stores).
static struct DinoCommonItemStaticInit {
    DinoCommonItemStaticInit() {
        volatile s32 *p = (volatile s32 *) data_020ba764;
        p[0x6c / 4] = -32;
        p[0x70 / 4] = -16;
        p[0x74 / 4] = 32;
        p[0x78 / 4] = 16;
        volatile s32 *q = (volatile s32 *) data_020ba7e0;
        q[0] = -32;
        q[1] = -8;
        q[2] = 32;
        q[3] = 8;
        q = (volatile s32 *) data_020ba7f0;
        q[0] = -8;
        q[1] = -8;
        q[2] = 8;
        q[3] = 8;
        q = (volatile s32 *) data_020ba800;
        q[0] = -8;
        q[1] = -8;
        q[2] = 8;
        q[3] = 8;
        p[0x3c / 4] = p[0x10 / 4];
        p[0x40 / 4] = p[0x0c / 4];
        p[0x44 / 4] = p[0x08 / 4];
        p[0x48 / 4] = p[0x04 / 4];
    }
} sDinoCommonItemStaticInit;
