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

DinoCommonItem::DinoCommonItem(u32 type, MSmartOam *oam, MPoint pos, u32 priority, u32 label, u32 helpText) {
    mOam = oam;
    mTextId = helpText;
    mFrame = data_020ba7d0[type].Moved(pos);
    if (label) {
        MFont *font = gDinoResource->CreateFont12(0, 1, 10);
        mView = new DinoCommonLabel(oam, font, gDinoResource->GetUnkTable(), label, MPoint(0, 2));
        MPoint half = mView->GetSize() / 2;
        MPoint p;
        p.y = pos.y - half.y;
        p.x = pos.x - half.x;
        mPos = p;
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
// The game's frame table is a pooled `static const MRect[4]` built element by element: the first element sits within
// reach of the pool's anchor (0x6c bytes before the table, which the trailing copies also use), the others through
// their own addresses. Every pointer below is derived from the table itself so mwcc sees one object: the table
// stores keep the address load of each later element below the previous element's stores (separate anchor externs
// let it hoist), and the second element's top is read back from the first's bottom so its -8 (16 - 0x18) is
// computed before the left store.
static struct DinoCommonItemStaticInit {
    DinoCommonItemStaticInit() {
        s32 *pool = (s32 *) &data_020ba7d0[-7].top;
        pool[0x6c / 4] = -32;
        pool[0x70 / 4] = -16;
        pool[0x74 / 4] = 32;
        pool[0x78 / 4] = 16;
        s32 *r = (s32 *) &data_020ba7d0[1];
        s32 small = pool[0x78 / 4] - 0x18;
        r[0] = -32;
        r[1] = small;
        r[2] = 32;
        r[3] = 8;
        volatile s32 *q = (volatile s32 *) &data_020ba7d0[2];
        q[0] = small;
        q[1] = small;
        q[2] = 8;
        q[3] = 8;
        q = (volatile s32 *) &data_020ba7d0[3];
        q[0] = small;
        q[1] = small;
        q[2] = 8;
        q[3] = 8;
        volatile s32 *p = (volatile s32 *) pool;
        p[0x3c / 4] = p[0x10 / 4];
        p[0x40 / 4] = p[0x0c / 4];
        p[0x44 / 4] = p[0x08 / 4];
        p[0x48 / 4] = p[0x04 / 4];
    }
} sDinoCommonItemStaticInit;
