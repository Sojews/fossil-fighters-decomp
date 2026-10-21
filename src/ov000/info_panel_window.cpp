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
#include "game/dino_static_init.h"
#include "game/dino_resource.h"
#include "m/console.h"

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

extern "C" MPoint data_ov000_02194dbc;      // the element text's position
extern "C" MPoint data_ov000_02194ea4[6];   // the number texts' positions
extern "C" MPoint data_ov000_02194e7c[5];   // the label texts' positions

// Builds the panel's page for a kind (0 the stats, 1 a text, 2 the attacks) unless it is already shown; the page
// arrows show or hide either way.
extern "C" void func_ov000_0217c9f0(DinoBattleViewCreatureInfo *info, u32 kind, u8 arrows) {
    info->mUnkEC = arrows;
    for (int i = 0; i < 2; i++) {
        if (arrows) {
            info->mMotions[i]->Show();
        } else {
            info->mMotions[i]->Hide();
        }
    }
    if (info->mUnkE8 == kind) {
        return;
    }
    info->mUnkE8 = kind;
    info->mTasks->Clear();
    switch (info->mUnkE8) {
    case 0: {
        MFont *font = gDinoResource->CreateFont12(0, 1, 10);
        info->mUnkFC = new MMotion(std::string(data_ov000_02194d8c.elementPath), info->mOam, 2, 1);
        info->mUnkFC->SetPriority(0);
        info->mUnkFC->SetAnchor(1);
        info->mUnkFC->Show();
        info->mTasks->AddTail(info->mUnkFC);
        info->mUnk84 = new MIndicator(new MSmartObjCanvas(info->mOam->mDisplay, info->mOam, MPoint(0x70, 0x10),
                                                          gDinoResource->GetUnkTable(), TRUE),
                                      font, 0);
        info->mUnk84->SetPosition(data_ov000_02194dbc);
        info->mUnk84->SetPriority(0);
        info->mUnk84->Show();
        info->mTasks->AddTail(info->mUnk84);
        MFont *numbers = ((DinoBattleView *) info->mUnk80)->GetUnk120();
        for (int i = 0; i < 6; i++) {
            info->mUnk88[i] = new MIndicator(
                new MSmartObjCanvas(info->mOam->mDisplay, info->mOam, MPoint(0x20, 0x10),
                                    ((DinoBattleView *) info->mUnk80)->GetUnk11CPalette(), TRUE),
                numbers, 0);
            info->mUnk88[i]->SetPosition(data_ov000_02194ea4[i]);
            info->mUnk88[i]->SetPriority(0);
            info->mUnk88[i]->Show();
            info->mTasks->AddTail(info->mUnk88[i]);
        }
        MFont *first = ((DinoBattleView *) info->mUnk80)->GetUnk11C();
        MFont *labels = ((DinoBattleView *) info->mUnk80)->GetUnk118();
        for (int i = 0; i < 5; i++) {
            info->mUnkA0[i] = new MIndicator(
                new MSmartObjCanvas(info->mOam->mDisplay, info->mOam, MPoint(0x20, 0x10),
                                    ((DinoBattleView *) info->mUnk80)->GetUnk11CPalette(), TRUE),
                i != 0 ? labels : first, 0);
            info->mUnkA0[i]->SetPosition(data_ov000_02194e7c[i]);
            info->mUnkA0[i]->SetPriority(0);
            info->mUnkA0[i]->Show();
            info->mTasks->AddTail(info->mUnkA0[i]);
        }
        break;
    }
    case 1: {
        MFont *font = gDinoResource->CreateFont12(0, 1, 10);
        info->mUnkC4 = new MConsole(new MSmartObjCanvas(info->mOam->mDisplay, info->mOam, MPoint(0x6c, 0x20),
                                                        gDinoResource->GetUnkTable(), TRUE),
                                    font, MPoint(0, 3), MFx32(0));
        info->mUnkC4->SetPosition(MPoint(10, 23));
        info->mUnkC4->SetPriority(0);
        info->mUnkC4->Show();
        info->mTasks->AddTail(info->mUnkC4);
        MFont *numbers = ((DinoBattleView *) info->mUnk80)->GetUnk120();
        for (int i = 0; i < 4; i++) {
            info->mUnk88[i] = new MIndicator(
                new MSmartObjCanvas(info->mOam->mDisplay, info->mOam, MPoint(0x20, 0x10),
                                    ((DinoBattleView *) info->mUnk80)->GetUnk11CPalette(), TRUE),
                numbers, 0);
            info->mUnk88[i]->SetPosition(data_ov000_02194ea4[i + 2]);
            info->mUnk88[i]->SetPriority(0);
            info->mUnk88[i]->Show();
            info->mTasks->AddTail(info->mUnk88[i]);
        }
        break;
    }
    case 2: {
        MFont *font = gDinoResource->CreateFont12(0, 10, 1);
        MFont *numbers = ((DinoBattleView *) info->mUnk80)->GetUnk120();
        for (int i = 0; i < 2; i++) {
            info->mUnkB4[i] = new MMotion(std::string(data_ov000_02194d8c.attackPath), info->mOam, 2, 1);
            info->mUnkB4[i]->SetPosition(MPoint(-3, i * 38));
            info->mUnkB4[i]->SetPriority(0);
            info->mUnkB4[i]->SetAnchor(2);
            info->mUnkB4[i]->Show();
            info->mTasks->AddTail(info->mUnkB4[i]);
            info->mUnkBC[i] = new MMotion(std::string(data_ov000_02194d8c.effectPath), info->mOam, 2, 1);
            info->mUnkBC[i]->SetPosition(MPoint(72, i * 38 + 30));
            info->mUnkBC[i]->SetPriority(0);
            info->mUnkBC[i]->Show();
            info->mTasks->AddTail(info->mUnkBC[i]);
            info->mUnk88[i] = new MIndicator(new MSmartObjCanvas(info->mOam->mDisplay, info->mOam, MPoint(0x80, 0x10),
                                                                 gDinoResource->GetUnkTable(), TRUE),
                                             font, 0);
            info->mUnk88[i]->SetPosition(MPoint(4, i * 38 + 6));
            info->mUnk88[i]->SetPriority(0);
            info->mUnk88[i]->Show();
            info->mTasks->AddTail(info->mUnk88[i]);
            int j = i + 2;
            info->mUnk88[j] = new MIndicator(
                new MSmartObjCanvas(info->mOam->mDisplay, info->mOam, MPoint(0x20, 0x10),
                                    ((DinoBattleView *) info->mUnk80)->GetUnk11CPalette(), TRUE),
                numbers, 0);
            info->mUnk88[j]->SetPosition(MPoint(81, i * 38 + 22));
            info->mUnk88[j]->SetPriority(0);
            info->mUnk88[j]->Show();
            info->mTasks->AddTail(info->mUnk88[j]);
            int k = i + 4;
            info->mUnk88[k] = new MIndicator(
                new MSmartObjCanvas(info->mOam->mDisplay, info->mOam, MPoint(0x20, 0x10),
                                    ((DinoBattleView *) info->mUnk80)->GetUnk11CPalette(), TRUE),
                numbers, 0);
            info->mUnk88[k]->SetPosition(MPoint(37, i * 38 + 22));
            info->mUnk88[k]->SetPriority(0);
            info->mUnk88[k]->Show();
            info->mTasks->AddTail(info->mUnk88[k]);
        }
        break;
    }
    }
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

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_02194e0c[];
extern "C" u32 data_ov000_02194e64[];
extern "C" u32 data_ov000_02194e6c[];
extern "C" u32 data_ov000_02194e74[];
extern "C" u32 data_ov000_02194e8c[];
extern "C" u32 data_ov000_02194eac[];
extern "C" u32 data_ov000_02194eb4[];
extern "C" u32 data_ov000_02194ebc[];
extern "C" u32 data_ov000_02194ec4[];
extern "C" u32 data_ov000_02194ecc[];
extern "C" u32 data_ov000_02194e84[];
extern "C" u32 data_ov000_02194e94[];
extern "C" u32 data_ov000_02194e9c[];
static struct InfoPanelWindowStaticInit {
    InfoPanelWindowStaticInit() {
        DINO_POOL_WORD(data_ov000_02194d8c, 0x68) = 128;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x6c) = 176;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x38) = 8;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x3c) = 150;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x50) = 64;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x54) = 112;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x18) = 96;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x1c) = 32;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x40) = 0;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x44) = 0;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x20) = 32;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x24) = 16;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x48) = 22;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x4c) = 29;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x60) = 84;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x64) = 29;
        DINO_POOL_WORD(data_ov000_02194e0c, 0x50) = 32;
        DINO_POOL_WORD(data_ov000_02194e0c, 0x54) = 48;
        DINO_POOL_WORD(data_ov000_02194e64, 0x0) = 96;
        DINO_POOL_WORD(data_ov000_02194e64, 0x4) = 48;
        DINO_POOL_WORD(data_ov000_02194e6c, 0x0) = 32;
        DINO_POOL_WORD(data_ov000_02194e6c, 0x4) = 66;
        DINO_POOL_WORD(data_ov000_02194e74, 0x0) = 96;
        DINO_POOL_WORD(data_ov000_02194e74, 0x4) = 66;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x30) = 24;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x34) = 6;
        DINO_POOL_WORD(data_ov000_02194e8c, 0x18) = 34;
        DINO_POOL_WORD(data_ov000_02194e8c, 0x1c) = 22;
        DINO_POOL_WORD(data_ov000_02194eac, 0x0) = 105;
        DINO_POOL_WORD(data_ov000_02194eac, 0x4) = 22;
        DINO_POOL_WORD(data_ov000_02194eb4, 0x0) = 21;
        DINO_POOL_WORD(data_ov000_02194eb4, 0x4) = 41;
        DINO_POOL_WORD(data_ov000_02194ebc, 0x0) = 81;
        DINO_POOL_WORD(data_ov000_02194ebc, 0x4) = 41;
        DINO_POOL_WORD(data_ov000_02194ec4, 0x0) = 21;
        DINO_POOL_WORD(data_ov000_02194ec4, 0x4) = 59;
        DINO_POOL_WORD(data_ov000_02194ecc, 0x0) = 81;
        DINO_POOL_WORD(data_ov000_02194ecc, 0x4) = 59;
        DINO_POOL_WORD(data_ov000_02194e0c, 0x70) = 51;
        DINO_POOL_WORD(data_ov000_02194e0c, 0x74) = 30;
        DINO_POOL_WORD(data_ov000_02194e84, 0x0) = 43;
        DINO_POOL_WORD(data_ov000_02194e84, 0x4) = 49;
        DINO_POOL_WORD(data_ov000_02194e8c, 0x0) = 103;
        DINO_POOL_WORD(data_ov000_02194e8c, 0x4) = 49;
        DINO_POOL_WORD(data_ov000_02194e94, 0x0) = 43;
        DINO_POOL_WORD(data_ov000_02194e94, 0x4) = 67;
        DINO_POOL_WORD(data_ov000_02194e9c, 0x0) = 103;
        DINO_POOL_WORD(data_ov000_02194e9c, 0x4) = 67;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x28) = 64;
        DINO_POOL_WORD(data_ov000_02194d8c, 0x2c) = -16;
    }
} sInfoPanelWindowStaticInit;
