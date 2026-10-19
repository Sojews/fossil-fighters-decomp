#ifndef GAME_DINO_UI_DEBUG_REVIVER_MENU_MENU_H
#define GAME_DINO_UI_DEBUG_REVIVER_MENU_MENU_H

#include "m/font.h"
#include "m/canvas.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include <stdio.h>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MTextCanvas;
class MSmartOam;

class DinoUiDebugReviverMenu : public MMenu {
public:
    DinoUiDebugReviverMenu();
    virtual ~DinoUiDebugReviverMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);

    // Lists the creature's parts.
    void Refresh();

    MTextCanvas *mCanvas;   // 0xA8
    MFont *mFont;       // 0xAC
    MSmartOam *mOam;    // 0xB0
    MTaskSet *mTasks;   // 0xB4: the cursor's moves
    MView *mCursor;     // 0xB8
    u32 mUnkBC;         // 0xBC
};

// A kaseki rock's record (only the fields the debug menu sets are known).
struct DinoDebugKasekiRock {
    u32 unk0 : 10;
    u32 present : 1;
    u32 level : 10;
    u32 unk21 : 11;
};

// The part names' first text id.
extern "C" const u32 data_0208a34c;

class DinoUiDebugReviverMenuItem : public MMenuItem {
public:
    // Shows the creature's part and its level, and gives the player a kaseki rock for each part it has.
    DinoUiDebugReviverMenuItem(MCanvas *canvas, MFont *font, MPoint pos, u32 part, u32 creature)
        : mCanvas(canvas), mFont(font), mPos(pos), mCreature(creature), mPart(part) {
        mFont->AddRef();
        mCanvas->DrawText(mFont, MPoint(mPos.x + 6, mPos.y - 16), 0,
                          std::string(gDinoResource->GetText(data_0208a34c + mPart)));
        DinoCreatureState state;
        gDinoBackup->GetCreature(mCreature, &state);
        char text[256];
        sprintf(text, "%03d%%", state.parts[mPart].level);
        mCanvas->DrawText(mFont, mPos, 0, std::string(text));
        for (u32 i = 0; i < 4; i++) {
            u32 index = i + (mCreature - 1) * 4;
            DinoDebugKasekiRock rock;
            MI_CpuFill8(&rock, 0, sizeof(rock));
            if (state.parts[i].level) {
                rock.present = 1;
                rock.level = state.parts[i].level;
            }
            gDinoBackup->SetKasekiRock(index + 1, (const u32 *)&rock);
        }
    }
    virtual ~DinoUiDebugReviverMenuItem();
    virtual MRect GetFrame();
    virtual void OnDecide();

    MCanvas *mCanvas;   // 0x88
    MFont *mFont;       // 0x8C
    MPoint mPos;        // 0x90
    u32 mCreature;      // 0x98
    u32 mPart;          // 0x9C
};

#endif
