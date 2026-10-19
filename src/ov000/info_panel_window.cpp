// InfoPanelWindow and DinoBattleViewCreatureInfo.
// NONMATCHING: the DinoBattleViewCreatureInfo constructor (the game keeps the address of the motion slot on the stack
// across the path string's destructor), the DinoBattleViewPopNumber constructor (the game keeps the bounce's top and
// frame count from the first anime for the second).

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/info_panel_window.h"
#include "types.h"
#include "game/dino_battle_view_creature_info.h"
#include "m/picture.h"
#include "m/smart_oam.h"
#include "m/motion.h"
#include "m/font.h"
#include "m/indicator.h"
#include "m/oam_canvas.h"
#include "game/dino_battle_view_class.h"

inline InfoPanelWindow::InfoPanelWindow(MPictureFile *file, MSmartOam *oam) : MProcess(NULL), mOam(oam), mUnk7C(0) {
    mPicture = new MTextPicture(*file, 1, false);
    mParallel.AddTail(mPicture);
}

InfoPanelWindow::~InfoPanelWindow() {
}

// Pooled file statics.
struct PopNumberStatics {
    u8 mUnk00;      // 0x00
    int mCount;     // 0x04: live pop numbers
};
extern "C" PopNumberStatics data_ov000_021978e0;

extern "C" void func_ov000_0217dafc(u8 value) {
    data_ov000_021978e0.mUnk00 = value;
}

DinoBattleViewPopNumber::~DinoBattleViewPopNumber() {
    data_ov000_021978e0.mCount--;
}

DinoBattleViewCreatureInfo::~DinoBattleViewCreatureInfo() {
    delete mPicture;
}

#pragma auto_inline off

// The TU's pooled constants.
extern "C" struct {
    u8 unk00[4];
    const char *attackPath;     // 0x04
    const char *effectPath;     // 0x08
    const char *leftPath;       // 0x0C: the page arrows
    const char *elementPath;    // 0x10
    const char *rightPath;      // 0x14
    u8 unk18[0x10];
    MPoint arrowPos;            // 0x28
} data_ov000_02194d8c;

DinoBattleViewCreatureInfo::DinoBattleViewCreatureInfo(DinoBattleView *view, MPoint pos)
    : MProcess(NULL), mUnk80((u32)view), mUnkE8(3), mCreature(NULL) {
    mPicture = new MPictureFile(std::string("image/btl_info_panel"), 7);
    mOam = new MSmartOam(0x22, 0, 1);
    mOam->SetUnk1C(1);
    mOam->mHidden = TRUE;
    SetPosition(pos);
    mWindow = new InfoPanelWindow(mPicture, mOam);
    mWindow->mPicture->SetPriority(0);
    mParallel.AddTail(mWindow);
    for (int i = 0; i < 2; i++) {
        mMotions[i] = new MMotion(std::string(i != 0 ? data_ov000_02194d8c.rightPath : data_ov000_02194d8c.leftPath),
                                  mOam, 2, 1);
        mMotions[i]->SetPriority(0);
        mMotions[i]->SetAnchor(0);
        mMotions[i]->Show();
        mMotions[i]->SetPosition(data_ov000_02194d8c.arrowPos);
        mMotions[i]->Play(1, 0);
        GetParallel()->AddTail(mMotions[i]);
    }
    mTasks = new MParallelTaskSet;
    GetParallel()->AddTail(mTasks, mOam);
}

MMotion *DinoBattleViewCreatureInfo::GetMotion(int index) {
    return mMotions[index];
}

void DinoBattleViewCreatureInfo::SetPosition(MPoint pos) {
    mPos.x = pos.x;
    mPos.y = pos.y;
    mOam->SetOffset(MPoint(-pos.x, -pos.y));
}

// Recolors a text: the font has each color's glyphs 0x20 codes apart. Spaces and question marks stay.
extern "C" void func_ov000_0217d220(DinoBattleViewCreatureInfo *info, char *text, int color) {
    char shift = (s8)color << 5;
    while (*text != '\0') {
        if (*text != '?' && *text != ' ') {
            *text++ += shift;
        } else {
            text++;
        }
    }
}

// The same for the second color only, everything included.
extern "C" void func_ov000_0217d254(DinoBattleViewCreatureInfo *info, char *text, int color) {
    if (color == 2 && *text != '\0') {
        do {
            *text++ += 0x20;
        } while (*text != '\0');
    }
}

void DinoBattleViewCreatureInfo::Show() {
    mWindow->mPicture->Show();
    mOam->mHidden = FALSE;
}

void InfoPanelWindow::Idle() {
    mPicture->SetPosition(MPoint(-(mOam->mOffset.x + mUnk7C * 256), -mOam->mOffset.y));
}

void DinoBattleViewCreatureInfo::Idle() {
}

void DinoBattleViewCreatureInfo::PlayMotion(int index) {
    mMotions[index]->Play(2, 1);
}

#pragma auto_inline on

// The TU's pooled constants.
extern "C" struct {
    u8 unk00[0x14];
    MPoint popOffset;   // 0x14: of a pop number from its position
} data_ov000_0219504c;

DinoBattleViewPopNumber::DinoBattleViewPopNumber(DinoBattleView *view, MSmartOam *oam, MPoint pos, int value, int arg4,
                                                 int kind)
    : MProcess(NULL) {
    data_ov000_021978e0.mCount++;
    MSmartOam *own = new MSmartOam(1, 0, 1);
    own->SetOffset(oam->GetOffset());
    MFont *font = view->GetUnk120();
    int number = value < 0 ? -value : value;
    int shift;
    if (kind == 0) {
        shift = value <= 0 ? 0x20 : 0x40;
    } else if (kind == 3) {
        shift = 0;
    } else {
        if (data_ov000_021978e0.mUnk00 && kind == 2) {
            kind = 1;
        }
        shift = kind << 5;
    }
    char text[20];
    OS_SPrintf(text, "%4d", number);
    if (shift != 0) {
        char *c = text;
        while (*c != '\0') {
            *c += shift;
            c++;
        }
    }
    MPoint size = font->MeasureText(std::string(text), 0, 0);
    size.y = 0x10;
    MSmartObjCanvas *canvas = new MSmartObjCanvas(own->mDisplay, own, size, view->GetUnk11CPalette(), TRUE);
    canvas->SetPriority(1);
    canvas->SetAnchor(0);
    canvas->Show();
    canvas->GetOamUnit()->SetMode(1);
    mIndicator = new MIndicator(canvas, font, 0);
    mIndicator->SetPosition(pos);
    mIndicator->SetText(std::string(text));
    GetParallel()->AddTail(mIndicator, own);
    int half = -size.x / 2;
    MPoint from = mIndicator->GetPosition() + MPoint(half, 0) + data_ov000_0219504c.popOffset;
    if (kind == 3) {
        mSerial.AddTail(mIndicator->CreateCoordAnime(from, from + MPoint(0, -10), 20, -2));
    } else {
        for (int height = 10; height > 0; height -= 3) {
            GetSerial()->AddTail(mIndicator->CreateCoordAnime(from, from + MPoint(0, -height), height + 2, -1),
                                 mIndicator->CreateCoordAnime(from + MPoint(0, -height), from, height + 2, 1));
        }
    }
    GetSerial()->AddTail(new MTimer(60), MTaskExitTransfer::Create(this));
}
