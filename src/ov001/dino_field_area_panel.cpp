// DinoFieldAreaPanel: the panel with the area's name.

#include "game/dino_field_area_panel.h"
#include "game/dino_resource.h"
#include "m/lcd.h"
#include "m/motion.h"
#include "m/smart_oam.h"
#include "m/oam_canvas.h"
#include "m/indicator.h"
#include "m/font.h"
#include <algorithm>

// This file's data.
extern "C" struct {
    u32 unk00;
    int minCells;           // 0x04: the frame's narrowest animation
    const char *motionPath; // 0x08: the frame's
    int maxCells;           // 0x0C: the frame's widest animation
    MPoint pos;             // 0x10: where the frame stops
} data_ov001_0216aa14;

DinoFieldAreaPanel::DinoFieldAreaPanel() : MProcess(NULL) {
    mArea = 0;
    MSmartOam *oam = new MSmartOam(gLcd->mMain, 0x10, 0, 1);
    mFrame = new MMotion(std::string(data_ov001_0216aa14.motionPath), oam, 0, 1);
    mFrame->Show();
    mFont = gDinoResource->CreateFont12(0, 1, 10);
    mName = new MIndicator(new MSmartObjCanvas(oam, MPoint(0xA0, 0x10), gDinoResource->GetUnkTable(), TRUE), mFont, 0);
    mName->Show();
    mTasks = new MSerialTaskSet;
    GetParallel()->AddTail(mTasks, mFrame, mName, oam);
}

void DinoFieldAreaPanel::SetMode(int area, int show) {
    mTasks->Clear();
    mFrame->Hide();
    mName->Hide();
    if (area == 0) {
        mArea = 0;
        return;
    }
    if (area == mArea) {
        return;
    }
    mArea = area;
    if (!show) {
        return;
    }
    std::string text(gDinoResource->GetText(area));
    MPoint size = mFont->MeasureText(text, 0, 0);
    int cells = (size.x + 11) / 12;
    mFrame->Play(std::min(data_ov001_0216aa14.maxCells, std::max(data_ov001_0216aa14.minCells, (size.x + 11) / 12)), 0);
    mFrame->SetPosition(MPoint(0, 0));
    int x = (cells * 12 - size.x) / 2 + 8;
    mName->SetPosition(MPoint(x, 2));
    mName->SetText(std::string(text));
    MPoint hidden(-mFrame->GetFrame().right, data_ov001_0216aa14.pos.y);
    mTasks->AddTail(new MTimer(30),
                    (new MParallelTaskSet(TRUE))
                        ->Add(mFrame->CreateShowTransfer())
                        ->Add(mName->CreateShowTransfer())
                        ->Add(mFrame->CreateCoordAnime(hidden, data_ov001_0216aa14.pos, 30, -2))
                        ->Add(mName->CreateCoordAnime(hidden + MPoint(x, 2),
                                                      MPoint(x + data_ov001_0216aa14.pos.x, data_ov001_0216aa14.pos.y + 2),
                                                      30, -2)),
                    new MTimer(180),
                    (new MParallelTaskSet(TRUE))
                        ->Add(mFrame->CreateCoordAnime(data_ov001_0216aa14.pos, hidden, 60, 1))
                        ->Add(mName->CreateCoordAnime(MPoint(x + data_ov001_0216aa14.pos.x, data_ov001_0216aa14.pos.y + 2),
                                                      hidden + MPoint(x, 2), 60, 1)));
}

void DinoFieldAreaPanel::Idle() {
}

DinoFieldAreaPanel::~DinoFieldAreaPanel() {
}
