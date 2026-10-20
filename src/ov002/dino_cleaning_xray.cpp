// DinoCleaningXray.

#include "game/dino_cleaning_xray.h"
#include "game/dino_cleaning.h"
#include "game/dino_cleaning_kaseki_rock.h"
#include "m/input.h"
#include "game/dino_static_init.h"

// The X-ray button, in the file's data pool.
extern const u8 data_ov002_02164efc[];
#define XRAY_BUTTON_RECT (*(const MRect *) (data_ov002_02164efc + 0x64))

DinoCleaningXray::DinoCleaningXray(DinoCleaning *cleaning) : DinoCleaningTool(cleaning) {
}

void DinoCleaningXray::Unk18() {
}

void DinoCleaningXray::Unk1C() {
}

// Touching the X-ray button shows the fossil.
void DinoCleaningXray::Unk20() {
    MInput *input = GetCleaning()->GetUnkDC();
    if (input->GetPressed() & 0x1000) {
        MPoint pos = input->GetTouchPos();
        if (XRAY_BUTTON_RECT.Contains(pos)) {
            GetCleaning()->GetKasekiRock()->StartXray();
        }
    }
}

DinoCleaningXray::~DinoCleaningXray() {
}

// The file's static initialization (see dino_static_init.h).
#pragma always_inline on
static struct DinoCleaningXrayStaticInit {
    DinoCleaningXrayStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_02164efc, 0x0, 0x44, 0x8, 0x10, 0x54, 0x64, 0x6c, 0x74);
    }
} sDinoCleaningXrayStaticInit;
