// DinoUiRvInformation3DModel, DinoUiRvInformation3DModelRotate.
// Only some functions are decompiled so far.

#pragma ipa file

#include <typeinfo>
#include "game/dino_ui_rv_information3d_model.h"
#include "game/dino_ui_rv_information3d_model_rotate.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_ui_rv_information.h"
#include <stdio.h>
#include "m/indicator.h"
#include "m/motion.h"
#include "game/dino_ui_revive_status.h"
#include "game/dino_ui_field_menu_number3.h"
#include "game/dino_resource.h"
#include "m/picture.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/3d_model.h"
#include "m/3d_scene.h"
#include "m/3d.h"
#include "m/input.h"
#include "m/window.h"
#include <string>
#include "nitro/mi.h"
#include "m/font.h"
#include "m/smart_oam.h"
#include "m/oam_canvas.h"
#include "m/irq.h"
#include "m/anm_model.h"
#include "game/dino_static_init.h"
#include "game/dino_creature_sort.h"
#include "game/dino_backup.h"
#include "m/console.h"

extern "C" u8 data_ov003_021624d0[];  // the scene's heap

DinoUiRvInformation3DModel::DinoUiRvInformation3DModel() : MProcess(NULL) {
    mUnk80 = 0;
    mUnk84 = 0;
    mUnk88 = 0;
    mScene = new M3DScene((u32)data_ov003_021624d0, 0xF618);
    mScene->mUnk14 = 1;
    mScene->mUnk18 = 300 << FX32_SHIFT;
    mScene->mM3d->mBgCntBits.priority = 1;
    mScene->mM3d->Hide();
    gCurrentDisplay->mWin->GetWindow0()->Hide();
    gCurrentDisplay->mWin->GetWindow1()->Hide();
    VecFx32 v;
    v.x = 0;
    v.y = 0;
    v.z = 0;
    mScene->SetCameraTarget(&v);
    v.x = 0;
    v.y = 0;
    v.z = 19 << FX32_SHIFT;
    mScene->SetCameraPos(&v);
    mScene->SetUnk80(-80, 0);
    mUnk8C = mUnk90 = mUnk94 = 0;
    mTasks = new MParallelTaskSet(FALSE);
    mDragging = 0;
    MSingleInputHub *hub = new MSingleInputHub(1);
    hub->EnableTouch();
    mInput = hub->GetInput(0);
    mParallel.AddTail(hub);
    MParallelTaskSet *parallel = GetParallel();
    M3DScene *scene = mScene;
    parallel->AddTail(mTasks);
    parallel->AddTail(scene);
}

extern "C" void G3X_Reset();
#define DINO_TOUCH_BUTTON 0x1000   // the touch screen in MInput's buttons
#define REG_G3_SWAP_BUFFERS (*(volatile u32 *) 0x04000540)
// The TU's pooled statics start at the model's typeinfo (+0x6C: the model's touch area).
// The TU's pooled statics.
extern "C" struct DinoRvInformation3DModelStatics {
    u32 unk00;
    fx32 maxTurnY;      // 0x04: the Y turn speed's limits
    fx32 minTurnY;      // 0x08
    u32 unk0C[2];
    fx32 minTurnX;      // 0x14: the X turn speed's limits
    fx32 maxTurnX;      // 0x18
    u32 unk1C[(0x74 - 0x1C) / 4];
} data_ov003_02160660;
extern "C" const fx32 data_02089b04[];          // by creature: the camera's distance
extern "C" struct {
    void (DinoUiRvInformation3DModel::*LoadModel_00)();
} data_ov003_021606dc;

// Hides the scene's 3D screen when the value changes.
void DinoUiRvInformation3DModel::SetUnk84(u32 value) {
    if (mUnk84 != value) {
        mUnk84 = value;
        mScene->mM3d->Hide();
    }
}

// Turns the model with drags on it (easing off after), and loads the model again when the creature changes.
// NONMATCHING: register allocation around the touch checks (the game loads the buttons again for the release test).
#pragma opt_common_subs off
void DinoUiRvInformation3DModel::Idle() {
    if (!mTasks->IsEmpty()) {
        if (gCurrentDisplay == gLcd->GetBottomDisplay() && (mInput->GetPressed() & DINO_TOUCH_BUTTON) &&
            DINO_POOL(MRect, *(u32 *)&typeid(DinoUiRvInformation3DModel), 0x6C).Contains(mInput->GetTouchPos())) {
            mTouch = mInput->GetTouchPos();
            mDragging = TRUE;
            mUnkA8 = mUnkAC = 0;
        } else if ((mInput->mButtons & DINO_TOUCH_BUTTON) && mDragging) {
            MPoint touch = mInput->GetTouchPos();
            mUnkA8 += (touch.x - mTouch.x) << FX32_SHIFT;
            mUnkAC += (touch.y - mTouch.y) << FX32_SHIFT;
            mTouch.x = touch.x;
            mTouch.y = touch.y;
        } else if (mInput->mPrevButtons & (mInput->mButtons ^ mInput->mPrevButtons) & DINO_TOUCH_BUTTON) {
            mDragging = FALSE;
        }
        mUnkA8 = std::min(std::max(mUnkA8, data_ov003_02160660.minTurnY), data_ov003_02160660.maxTurnY);
        mUnkA8 = mUnkA8 * 4 / 5;
        mUnkAC = std::min(std::max(mUnkAC, data_ov003_02160660.minTurnX), data_ov003_02160660.maxTurnX);
        mUnkAC = mUnkAC * 2 / 3;
        mUnk90 += mUnkA8;
        mUnk90 = (mUnk90 + (360 << FX32_SHIFT)) % (360 << FX32_SHIFT);
        mUnk94 += mUnkAC;
        if (mUnk94 < -60 << FX32_SHIFT) {
            mUnk94 = -60 << FX32_SHIFT;
            mUnkAC = -mUnkAC / 3;
        }
        if (mUnk94 > 60 << FX32_SHIFT) {
            mUnk94 = 60 << FX32_SHIFT;
            mUnkAC = -mUnkAC / 3;
        }
        DinoUiRvInformation3DModelRotate *rotate = mRotate;
        rotate->mRotZ = mUnk8C;
        rotate->mRotY = mUnk90;
        rotate->mRotX = mUnk94;
    }
    if (mUnk84 != mUnk80) {
        mUnk80 = mUnk84;
        GetSerial()->AddTail(MTaskSetMakeEmptyTask::Create(mTasks));
        if (mUnk80 != 0) {
            VecFx32 pos;
            pos.x = 0;
            pos.y = 0;
            pos.z = data_02089b04[mUnk80];
            mScene->SetCameraPos(&pos);
            M3DScene *scene = mScene;
            G3X_Reset();
            REG_G3_SWAP_BUFFERS = scene->mUnk10 | (scene->mUnk14 << 1);
            GetSerial()->AddTail(scene->mM3d->CreateHideTransfer());
            GetSerial()->AddTail(mScene->CreateEnableTask(0), MCreateBgCallTask(this, data_ov003_021606dc.LoadModel_00, 1),
                                 mScene->CreateEnableTask(1));
            GetSerial()->AddTail(mScene->mM3d->CreateShowTransfer());
        }
    }
}
#pragma opt_common_subs reset

DinoUiRvInformation3DModel::~DinoUiRvInformation3DModel() {
}

DinoUiRvInformation3DModelRotate::~DinoUiRvInformation3DModelRotate() {
}

DinoUiRvInformation::~DinoUiRvInformation() {
    if (mHasTenPictures) {
        for (int i = 0; i < 10; i++) {
            delete mPictures[i];
        }
    } else {
        for (int i = 0; i < 7; i++) {
            delete mPictures[i];
        }
    }
}

extern "C" MPoint data_ov003_0216072c;        // the name's position
extern "C" MPoint data_ov003_02160734;        // the description's
extern "C" MPoint data_ov003_0216074c;
extern "C" MPoint data_ov003_02160874[4];     // the part levels', by part
extern "C" MPoint data_ov003_021608bc[6];     // the numbers', (page 0)
extern "C" MPoint data_ov003_0216085c[3];     // the attack rows', (pages 1 and 2)
extern "C" const char data_ov003_02160c0c[];  // the part levels' font
extern "C" u32 data_ov003_02160760[];

// Makes the page's parts, hidden.
#pragma opt_common_subs off
void DinoUiRvInformation::Unk02156800() {
    gCurrentDisplay = gLcd->mMain;
    switch (mPage) {
    case 0: {
        MFontSet *font = gDinoResource->CreateFont12(0, 1, 0);
        mViewD4 = new MIndicator(new MObjCanvas(gCurrentDisplay, MPoint(0x90, 0x10), gDinoResource->GetUnkTable(), TRUE),
                                 font, 0);
        mViewD4->SetPosition(data_ov003_0216072c);
        mViewD4->SetPriority(3);
        mViewD4->Hide();
        mPictureTasks->AddTail(mViewD4);
        mViewEC = new MConsole(new MObjCanvas(gCurrentDisplay, MPoint(0xF8, 0x30), gDinoResource->GetUnkTable(), TRUE),
                               font, MPoint(0, 3), 0);
        mViewEC->SetUnk44(6);
        mViewEC->SetPosition(data_ov003_02160734);
        mViewEC->SetPriority(3);
        mViewEC->Hide();
        mPictureTasks->AddTail(mViewEC);
        mViewD8 = new MIndicator(new MObjCanvas(gCurrentDisplay, MPoint(0x80, 0x10), gDinoResource->GetUnkTable(), TRUE),
                                 font, 0);
        mPictureTasks->AddTail(mViewD8);
        mViewD8->SetPosition(data_ov003_0216074c);
        mViewD8->SetPriority(3);
        mViewD8->Hide();
        MFontSet *levelFont = new MFontSet(std::string(data_ov003_02160c0c), 0, 0, 0);
        for (int i = 0; i < 4; i++) {
            mViewsDC[i] = new MIndicator(new MObjCanvas(gCurrentDisplay, MPoint(0x40, 0x10), levelFont->GetPalette(), TRUE),
                                         levelFont, 0);
            mPictureTasks->AddTail(mViewsDC[i]);
            mViewsDC[i]->SetPosition(data_ov003_02160874[i]);
            mViewsDC[i]->SetPriority(3);
            mViewsDC[i]->Hide();
        }
        for (int i = 0; i < 6; i++) {
            mNumbersBC[i] = new DinoUiFieldMenuNumber3(data_ov003_021608bc[i]);
            mNumbersBC[i]->SetPriority(3);
            mPictureTasks->AddTail(mNumbersBC[i]);
        }
        break;
    }
    case 1:
    case 2: {
        MSmartOam *oam = new MSmartOam(0xC, 0, TRUE);
        MFontSet *font = gDinoResource->CreateFont12(0, 10, 1);
        for (int i = 0; i < 3; i++) {
            mViewsF0[i] = new MMotion(std::string(((const char **)&data_ov003_02160660)[4]), oam, 2, 1);
            mViewsF0[i]->SetPosition(data_ov003_0216085c[i]);
            mViewsF0[i]->SetPriority(3);
            ((MMotion *)mViewsF0[i])->Play(1, 0);
            mViewsF0[i]->Hide();
            mPictureTasks->AddTail(mViewsF0[i]);
            mViews104[i] = new MIndicator(
                new MObjCanvas(gCurrentDisplay, MPoint(0x80, 0x20), gDinoResource->GetUnkTable(), TRUE), font, 0);
            mPictureTasks->AddTail(mViews104[i]);
            mViews104[i]->SetPosition(data_ov003_0216085c[i] + DINO_POOL(MPoint, data_ov003_02160660, 0x10c));
            mViews104[i]->SetPriority(3);
            mViews104[i]->Hide();
            mViews118[i] = new MConsole(
                new MObjCanvas(gCurrentDisplay, MPoint(0x80, 0x20), gDinoResource->GetUnkTable(), TRUE), font,
                MPoint(0, 3), 0);
            mViews118[i]->SetUnk44(6);
            mViews118[i]->SetPosition(data_ov003_0216085c[i] + DINO_POOL(MPoint, data_ov003_02160660, 0x104));
            mViews118[i]->SetPriority(3);
            mViews118[i]->Hide();
            mPictureTasks->AddTail(mViews118[i]);
            mNumbers12C[i] =
                new DinoUiFieldMenuNumber3(data_ov003_0216085c[i] + DINO_POOL(MPoint, data_ov003_02160660, 0x6C));
            mNumbers12C[i]->SetPriority(3);
            mPictureTasks->AddTail(mNumbers12C[i]);
            mNumbers140[i] =
                new DinoUiFieldMenuNumber3(data_ov003_0216085c[i] + DINO_POOL(MPoint, data_ov003_02160660, 0x64));
            mNumbers140[i]->SetPriority(3);
            mPictureTasks->AddTail(mNumbers140[i]);
            mNumbers12C[i]->Hide();
            mNumbers140[i]->Hide();
        }
        mPictureTasks->AddTail(oam);
        break;
    }
    case 3:
        break;
    }
}
#pragma opt_common_subs reset

void DinoUiRvInformation::SetUnk1B1(u8 value) {
    mUnk1B1 = value;
}

void DinoUiRvInformation::SetUnk1AC(u32 value) {
    mUnk1AC = value;
}

MTask *DinoUiRvInformation::CreateWaitModelTask() {
    return MTaskSetWaitEmptyTask::Create(&mModel->mSerial);
}

void DinoUiRvInformation::Idle() {
}


// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoUiRvInformation::*CreateSubPicture_00)();
} data_ov003_021606d4;
extern "C" struct {
    void (DinoUiRvInformation::*Unk02156800_00)();
} data_ov003_02160784;
extern "C" struct {
    void (DinoUiRvInformation::*Unk021571bc_00)();
} data_ov003_0216069c;
extern "C" struct {
    void (DinoUiRvInformation::*Unk02156ee8_00)(DinoCreatureInfo info);
} data_ov003_02160774;

extern "C" const char data_ov003_02160c2c[];
extern "C" const char data_ov003_02160c3c[];
extern "C" const char data_ov003_02160c40[];
extern "C" const char data_ov003_02160c44[];
extern "C" const char data_ov003_02160c48[];  // a one-digit value's format
extern "C" const char data_ov003_02160c50[];  // a two-digit value's format
// A zero value's text (copied as a whole, from a non-const object).
struct DinoUiRvBlankValue {
    char text[5];
};
extern "C" DinoUiRvBlankValue data_ov003_02160c58;

void DinoUiRvInformation::ShowSupportEffects() {
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(mInfo.state.index);
    if (mInfo.state.unk14 & 2) {
        ((MIndicator *)mViewD8)->SetText(std::string(data_ov003_02160c2c));
        for (int i = 0; i < 4; i++) {
            ((MIndicator *)mViewsDC[i])->SetText(std::string(data_ov003_02160c3c));
        }
        return;
    }
    ((MIndicator *)mViewD8)->SetText(std::string(data_ov003_02160c40));
    for (int i = 0; i < 4; i++) {
        ((MIndicator *)mViewsDC[i])->SetText(std::string(data_ov003_02160c44));
    }
    for (int side = 0; side < 2; side++) {
        const s8 *effects;
        if (side == 0) {
            effects = def->unk54 == 0 ? NULL : (const s8 *)def + def->unk54;
        } else {
            effects = def->unk58 == 0 ? NULL : (const s8 *)def + def->unk58;
        }
        if (effects[0] == 0 && effects[1] == 0 && effects[2] == 0 && effects[3] == 0) {
            continue;
        }
        ((MIndicator *)mViewD8)->SetText(std::string(gDinoResource->GetCreatureText0C(side)));
        int values[4] = {effects[0], effects[1], effects[2], effects[3]};
        for (int i = 0; i < 4; i++) {
            char text[16];
            int value = values[i];
            if (value != 0) {
                int sign = 0x40;
                if (value < 0) {
                    sign = 0x20;
                    value = -value;
                }
                if (value < 10) {
                    sprintf(text, data_ov003_02160c48, value);
                    text[1] += sign;
                    text[2] += sign;
                    text[3] += sign;
                } else {
                    sprintf(text, data_ov003_02160c50, value);
                    text[0] += sign;
                    text[1] += sign;
                    text[2] += sign;
                    text[3] += sign;
                }
            } else {
                *(DinoUiRvBlankValue *)text = data_ov003_02160c58;
            }
            ((MIndicator *)mViewsDC[i])->SetText(std::string(text));
        }
    }
}

// What the creature stats are worked out from, and the stats (main's func_02030f38).
struct DinoCreatureStatsInput {
    u32 id;         // 0x00
    int rank;       // 0x04
    int bonus[4];   // 0x08: by part
    int levels[5];  // 0x18: the parts', then the creature's own
};
struct DinoCreatureStats {
    int hp;         // 0x00
    int stats[5];   // 0x04
};
extern "C" DinoCreatureStats func_02030f38(const DinoCreatureStatsInput *info, BOOL fixed, MFx32 fraction);
extern "C" const char data_ov003_02160c38[];

void DinoUiRvInformation::ShowStatusPage() {
    u32 index = mInfo.state.index;
    ((MConsole *)mViewEC)->Clear();
    if (mInfo.state.unk14 & 2) {
        ((MIndicator *)mViewD4)->SetText(std::string(data_ov003_02160c2c));
        ((MConsole *)mViewEC)->Print(std::string(data_ov003_02160c38));
        mNumbersBC[0]->SetValue(0xFFFD);
        mNumbersBC[1]->SetValue(0xFFFC);
        mNumbersBC[2]->SetValue(0xFFFC);
        mNumbersBC[3]->SetValue(0xFFFC);
        mNumbersBC[4]->SetValue(0xFFFC);
        mNumbersBC[5]->SetValue(0xFFFC);
    } else {
        ((MIndicator *)mViewD4)->SetText(std::string(gDinoResource->GetCreatureText24(index)));
        ((MConsole *)mViewEC)->Print(std::string(gDinoResource->GetCreatureText20(index)));
        DinoCreatureStatsInput info;
        info.levels[0] = info.levels[1] = info.levels[2] = info.levels[3] = 0;
        MI_CpuFill8(&info, 0, sizeof(info));
        info.id = index;
        info.rank = GetCreatureLevel((*(const DinoCreatureState *)&mInfo));
        info.levels[4] = (*(const DinoCreatureState *)&mInfo).unk0_8;
        const DinoCreatureBody *body = (const DinoCreatureBody *)&mInfo.unk00[1];
        for (int i = 0; i < 4; i++) {
            info.bonus[i] = body->parts[i].unk12;
            info.levels[i] = body->parts[i].level;
        }
        DinoCreatureStats stats = func_02030f38(&info, mUnk1B1, mUnk1B4);
        mNumbersBC[0]->SetValue((u16)stats.hp);
        mNumbersBC[1]->SetValue((u16)GetCreatureLevel(*(const DinoCreatureState *)&mInfo));
        mNumbersBC[2]->SetValue((u16)stats.stats[0]);
        mNumbersBC[3]->SetValue((u16)stats.stats[1]);
        mNumbersBC[4]->SetValue((u16)stats.stats[2]);
        mNumbersBC[5]->SetValue((u16)stats.stats[3]);
    }
    for (int i = 0; i < 6; i++) {
        mNumbersBC[i]->Show();
    }
    ShowSupportEffects();
    mViewD4->Show();
    mViewEC->Show();
    mViewD8->Show();
    for (int i = 0; i < 4; i++) {
        mViewsDC[i]->Show();
    }
    if (mInfo.state.parts[0].level) {
        mModel->SetUnk84(index);
    } else {
        mModel->SetUnk84(0);
    }
}

BOOL DinoUiRvAttackFlag(u32 id);

// How many of the creature's parts have a level.
static inline u32 DinoCountLevelledParts(const DinoCreatureBody *body) {
    u32 n = 0;
    for (u32 j = 0; j < 4; j++) {
        if (body->parts[j].level != 0) {
            n++;
        }
    }
    return n;
}

// NONMATCHING: the attack loops are strength-reduced here and the row search keeps n on the stack.
void DinoUiRvInformation::ShowAttackPage() {
    if (mInfo.state.unk14 & 2) {
        for (int i = 0; i < 3; i++) {
            mViewsF0[i]->Show();
            mViews104[i]->Show();
            mViews118[i]->Show();
            mNumbers12C[i]->Show();
            mNumbers140[i]->Show();
            ((MIndicator *)mViews104[i])->SetText(std::string(data_ov003_02160c2c));
            ((MConsole *)mViews118[i])->Clear();
            ((MConsole *)mViews118[i])->Print(std::string(data_ov003_02160c38));
            mNumbers140[i]->SetValue(0xFFFD);
            mNumbers12C[i]->SetValue(0xFFFD);
            ((MMotion *)mViewsF0[i])->Play(2, 0);
        }
        return;
    }
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(mInfo.state.index);
    const DinoCreatureBody *body = (const DinoCreatureBody *)&mInfo.unk00[1];
    u32 count = 0;
    for (u32 i = 0; i < def->attackCount; i++) {
        if (((const u32 *)((const u8 *)def + def->attackLevels))[i] <= DinoCountLevelledParts(body)) {
            count++;
        }
    }
    BOOL special = FALSE;
    u32 specialAttack = def->specialAttack;
    if ((mUnk1AC == 0 && gDinoBackup->GetSystemFlag(0x28)) || mUnk1AC == 1 || mUnk1AC == 3) {
        if (specialAttack != 0 && def->specialAttackLevel <= DinoCountLevelledParts(body)) {
            count++;
            special = TRUE;
        }
    }
    for (int row = 0; row < 3; row++) {
        u32 n = (mPage - 1) * 3 + row;
        if (n < count) {
            mViewsF0[row]->Show();
            mViews104[row]->Show();
            mViews118[row]->Show();
            mNumbers12C[row]->Show();
            mNumbers140[row]->Show();
            u32 attack;
            if (special && n == count - 1) {
                attack = specialAttack;
            } else {
                attack = 0;
                u32 attacks = def->attackCount;
                for (u32 i = 0; i < attacks; i++) {
                    if (((const u32 *)((const u8 *)def + def->attackLevels))[i] <= DinoCountLevelledParts(body)) {
                        if (n-- == 0) {
                            attack = ((const u32 *)((const u8 *)def + def->attackIds))[i];
                            break;
                        }
                    }
                }
            }
            ((MIndicator *)mViews104[row])->SetText(std::string(gDinoResource->GetAttackName(attack)));
            ((MConsole *)mViews118[row])->Clear();
            ((MConsole *)mViews118[row])->Print(std::string(gDinoResource->GetCreatureText1C(attack)));
            const DinoAttackDef *attackDef = gDinoResource->GetAttackDef(attack);
            mNumbers140[row]->SetValue(DinoOffsetPtr<u16>(attackDef, attackDef->cost)[0]);
            u16 power = DinoOffsetPtr<u16>(attackDef, attackDef->cost)[1];
            if (power != 0) {
                DinoCreatureStatsInput info;
                info.levels[0] = info.levels[1] = info.levels[2] = info.levels[3] = 0;
                MI_CpuFill8(&info, 0, sizeof(info));
                info.id = mInfo.state.index;
                info.rank = GetCreatureLevel(*(const DinoCreatureState *)&mInfo);
                info.levels[4] = (*(const DinoCreatureState *)&mInfo).unk0_8;
                const DinoCreatureBody *parts = (const DinoCreatureBody *)&mInfo.unk00[1];
                for (int i = 0; i < 4; i++) {
                    info.bonus[i] = parts->parts[i].unk12;
                    info.levels[i] = parts->parts[i].level;
                }
                DinoCreatureStats stats = func_02030f38(&info, FALSE, MFx32::FromRaw(0));
                mNumbers12C[row]->SetValue((u16)(power + stats.stats[0]));
            } else {
                mNumbers12C[row]->SetValue(0xFFFF);
            }
            if (attack == specialAttack) {
                ((MMotion *)mViewsF0[row])->Play(3, 0);
            } else if (DinoUiRvAttackFlag(attack)) {
                ((MMotion *)mViewsF0[row])->Play(1, 0);
            } else {
                ((MMotion *)mViewsF0[row])->Play(2, 0);
            }
        } else {
            mViewsF0[row]->Hide();
            mViews104[row]->Hide();
            mViews118[row]->Hide();
            mNumbers12C[row]->Hide();
            mNumbers140[row]->Hide();
        }
    }
}

void DinoUiRvInformation::SetPage(u8 page) {
    if (page > 3 || mPage == page) {
        return;
    }
    mPage = page;
    gCurrentDisplay = gLcd->mMain;
    mPictureTasks->Clear();
    mUnk16C->Clear();
    if (!mUnk1B0 && mUnk195 && page == 3) {
        mPagePicture = new MTextPicture(*mPictures[mUnk198 + 3], 0, false);
        mPictureTasks->AddTail(mPagePicture);
    } else {
        mPagePicture = new MTextPicture(*mPictures[page], 0, false);
        mPictureTasks->AddTail(mPagePicture);
    }
    mPagePicture->Show();
    mPagePicture->SetPriority(3);
    if (mHasTenPictures && page < 3) {
        GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_021606d4.CreateSubPicture_00));
    }
    if (mPage) {
        mModel->SetUnk84(0);
    }
    GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_02160784.Unk02156800_00));
    if (mUnk1B0) {
        GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_0216069c.Unk021571bc_00));
    } else {
        GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_02160774.Unk02156ee8_00, mInfo));
    }
}

extern "C" struct {
    void (DinoUiRvInformation::*SetPage_00)(u8 page);
} data_ov003_02160794;
// The creature type's mark: the first entry with the type (or -1) gives the anime.
extern "C" struct {
    int type;
    int anime;
} data_ov003_021601e0[6];
extern "C" const char data_ov003_02160c24[];   // the number's format

// Shows a creature: its number, type and name, and the page's details (reloading the page when the picture
// changes).
void DinoUiRvInformation::Unk02156ee8(DinoCreatureInfo info) {
    u32 index = info.state.index;
    if (index == 0 || index > gDinoResource->GetCreatureCount()) {
        return;
    }
    mInfo.state = info.state;
    mUnk1B0 = 0;
    u32 old = mUnk198;
    u32 id = (mInfo.state.index - 1) * 4 + 1;
    {
        const u8 *data = (const u8 *)mUnk19C.Get();
        mUnk195 = (data + ((const u32 *)(data + ((const u32 *)data)[2]))[id])[4];
    }
    {
        const u8 *data = (const u8 *)mUnk19C.Get();
        mUnk198 = *(const u32 *)(data + ((const u32 *)(data + ((const u32 *)data)[2]))[id] + 0x38);
    }
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(index);
    {
        char number[4];
        sprintf(number, data_ov003_02160c24, def->voice);
        ((MIndicator *)mViewsAC[0])->SetText(std::string(number));
    }
    u8 type = def->unk0D;
    for (u32 i = 0; i < 6; i++) {
        int t = data_ov003_021601e0[i].type;
        if (t == type || t == -1) {
            ((MMotion *)mViewsAC[1])->Play(data_ov003_021601e0[i].anime, 0);
            break;
        }
    }
    ((MIndicator *)mViewsAC[2])->SetText(std::string(gDinoResource->GetCreatureText08(index)));
    mViewsAC[0]->Show();
    mViewsAC[1]->Show();
    mViewsAC[2]->Show();
    if (mUnk198 != old && mPage == 3) {
        GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_02160794.SetPage_00, mPage));
        mPage = 0xFF;
        return;
    }
    if (mHasTenPictures && mPage < 3) {
        if (info.state.parts[0].level != 0) {
            mPagePicture->Show();
            mSubPicture->Hide();
        } else {
            mPagePicture->Hide();
            mSubPicture->Show();
        }
    }
    switch (mPage) {
    case 0:
        ShowStatusPage();
        return;
    case 1:
    case 2:
        ShowAttackPage();
        return;
    case 3:
        mUnk16C->Clear();
        mUnk164 = new DinoUiReviveStatus(gLcd->mMain, mInfo, 0);
        mUnk16C->AddTail(mUnk164);
        break;
    }
}

// Hides the page's parts (on the last page, puts its picture back up).
void DinoUiRvInformation::Unk021571bc() {
    mUnk1B0 = 1;
    mViewsAC[0]->Hide();
    mViewsAC[1]->Hide();
    mViewsAC[2]->Hide();
    if (mHasTenPictures && mPage < 3) {
        mPagePicture->Show();
        mSubPicture->Hide();
    }
    switch (mPage) {
    case 0:
        mViewD4->Hide();
        mViewEC->Hide();
        mViewD8->Hide();
        for (int i = 0; i < 4; i++) {
            mViewsDC[i]->Hide();
        }
        for (int i = 0; i < 6; i++) {
            mNumbersBC[i]->Hide();
        }
        mModel->SetUnk84(0);
        break;
    case 1:
    case 2:
        for (int i = 0; i < 3; i++) {
            mViewsF0[i]->Hide();
            mViews104[i]->Hide();
            mViews118[i]->Hide();
            mNumbers12C[i]->Hide();
            mNumbers140[i]->Hide();
        }
        break;
    case 3:
        mUnk16C->Clear();
        if (mUnk195) {
            gCurrentDisplay = gLcd->mMain;
            mPictureTasks->Clear();
            mPagePicture = new MTextPicture(*mPictures[mPage], FALSE, false);
            mPictureTasks->AddTail(mPagePicture);
            mPagePicture->Show();
            mPagePicture->SetPriority(3);
        }
        break;
    }
    mUnk195 = 0;
    mUnk198 = 0;
}

void DinoUiRvInformation::CreateSubPicture() {
    mSubPicture = new MTextPicture(*mPictures[mPage + 7], 1, false);
    mPictureTasks->AddTail(mSubPicture);
    mSubPicture->Hide();
    mSubPicture->SetPriority(2);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiRvInformation, DinoUiRvInformation>;
template MTask *MCreateCallTask(DinoUiRvInformation *obj, void (DinoUiRvInformation::*func)());
template class MCallTask1<DinoUiRvInformation, DinoUiRvInformation, u8>;
template MTask *MCreateCallTask(DinoUiRvInformation *obj, void (DinoUiRvInformation::*func)(u8), u8 a0);
// NONMATCHING (this creator only): we copy the DinoCreatureInfo argument to a stack temporary before constructing;
// the game copies it straight from the argument (a const A1 & constructor does that but breaks the MPoint instances).
template class MCallTask1<DinoUiRvInformation, DinoUiRvInformation, DinoCreatureInfo>;
template MTask *MCreateCallTask(DinoUiRvInformation *obj, void (DinoUiRvInformation::*func)(DinoCreatureInfo), DinoCreatureInfo a0);
template class MBgCallTask0<DinoUiRvInformation3DModel>;
template MTask *MCreateBgCallTask(DinoUiRvInformation3DModel *obj, void (DinoUiRvInformation3DModel::*func)(), int thread);

extern "C" const char data_ov003_02160be0[];   // the model's path
extern "C" const u32 data_02089ecc[];          // by creature
extern "C" const fx32 data_02089ce8[];         // by creature: the height the model turns around

void DinoUiRvInformation3DModel::LoadModel() {
    char path[0x100];
    OS_SPrintf(path, data_ov003_02160be0, mUnk80);
    mAnm = new MAnmModel(mScene);
    mAnm->SetFrames(0, 0x40000000);
    mAnm->Load(path, 0);
    mAnm->Run();
    mAnm->mUnkF4 = 1;
    mAnm->mFollowParent = TRUE;
    mUnk8C = mUnk94 = 0;
    mUnk90 = data_02089ecc[mUnk80];
    mUnk94 = 0xA000;
    mDragging = FALSE;
    mUnkA8 = mUnkAC = 0;
    mRotate = new DinoUiRvInformation3DModelRotate(mAnm, data_02089ce8[mUnk80]);
    MIrqLock lock;
    mTasks->AddTail(mAnm, mRotate);
}

extern "C" const char data_ov003_02160bfc[];    // the name font
extern "C" const char *const data_ov003_02160894[]; // the pictures, by page

// Sets up the information panel: the name and its plate, the pictures (ten with the extra page) and the 3D model.
// (Without common-subexpression elimination: the game loads gCurrentDisplay for the two canvases after copying their
// size.)
#pragma opt_common_subs off
DinoUiRvInformation::DinoUiRvInformation(u8 hasTenPictures)
    : MProcess(gLcd->mMain), mUnk164(NULL), mPage(0), mHasTenPictures(hasTenPictures), mUnk195(0), mUnk198(0),
      mUnk1AC(0), mUnk1B0(1), mUnk1B1(0), mUnk1B4(1) {
    MI_CpuFill8(&mInfo, 0, sizeof(mInfo));
    mUnk19C = gDinoResource->LoadKasekiDefs();
    gCurrentDisplay = gLcd->mMain;
    MFontSet *font = gDinoResource->CreateFont12(0, 1, 0);
    MFontSet *nameFont = new MFontSet(std::string(data_ov003_02160bfc), 0, 0, 0);
    mViewsAC[0] = new MIndicator(new MObjCanvas(gCurrentDisplay, MPoint(0x40, 0x20), gDinoResource->GetUnkTable(), TRUE),
                                 nameFont, 0);
    mParallel.AddTail(mViewsAC[0]);
    mViewsAC[0]->SetPosition(DINO_POOL(MPoint, data_ov003_02160660, 0x34));
    mViewsAC[0]->SetPriority(2);
    mViewsAC[0]->Show();
    mOam = new MSmartOam(1, 0, TRUE);
    mViewsAC[1] = new MMotion(std::string(((const char **)&data_ov003_02160660)[3]), mOam, 2, 1);
    mViewsAC[1]->SetPosition(DINO_POOL(MPoint, data_ov003_02160660, 0x44));
    mViewsAC[1]->SetPriority(2);
    mViewsAC[1]->Show();
    mParallel.AddTail(mViewsAC[1]);
    mParallel.AddTail(mOam);
    mViewsAC[2] = new MIndicator(new MObjCanvas(gCurrentDisplay, MPoint(0xA0, 0x20), gDinoResource->GetUnkTable(), TRUE),
                                 font, 0);
    mParallel.AddTail(mViewsAC[2]);
    mViewsAC[2]->SetPosition(DINO_POOL(MPoint, data_ov003_02160660, 0x1C));
    mViewsAC[2]->SetPriority(2);
    mViewsAC[2]->Show();
    mPictureTasks = new MParallelTaskSet();
    mParallel.AddTail(mPictureTasks);
    mUnk16C = new MParallelTaskSet();
    mParallel.AddTail(mUnk16C);
    mModel = new DinoUiRvInformation3DModel();
    mParallel.AddTail(mModel);
    if (mHasTenPictures) {
        for (int i = 0; i < 10; i++) {
            mPictures[i] = new MPictureFile(std::string(data_ov003_02160894[i]), 7);
        }
    } else {
        for (int i = 0; i < 7; i++) {
            mPictures[i] = new MPictureFile(std::string(data_ov003_02160894[i]), 7);
        }
    }
    mPagePicture = new MTextPicture(*mPictures[0], TRUE, false);
    mPictureTasks->AddTail(mPagePicture);
    mPagePicture->Show();
    mPagePicture->SetPriority(3);
    if (mHasTenPictures) {
        mSubPicture = new MTextPicture(*mPictures[7], TRUE, false);
        mPictureTasks->AddTail(mSubPicture);
        mSubPicture->Hide();
        mSubPicture->SetPriority(2);
    }
    Unk02156800();
    gCurrentDisplay = gLcd->mSub;
}
#pragma opt_common_subs reset

#pragma auto_inline off

// Whether the attack's first value is 8 to 11, or its second 2 or 9 to 15.
BOOL DinoUiRvAttackFlag(u32 id) {
    const DinoAttackDef *def = gDinoResource->GetAttackDef(id);
    if ((1 << *DinoOffsetPtr<u8>(def, def->unk10)) & 0xF00) {
        return TRUE;
    }
    switch (*DinoOffsetPtr<u8>(def, def->unk14)) {
    case 2:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        return TRUE;
    }
    return FALSE;
}

extern "C" void func_ov003_02156eac(DinoUiRvInformation *info, MFx32 angle) {
    BOOL out = angle < MFx32(0) || MFx32(100) <= angle;
    if (!out) {
        info->mUnk1B4 = angle;
    }
}

// The angle's index into the sine table.
static inline int DinoDegToIdx(fx32 deg) {
    return (u16)((deg << 4) / 360);
}

// Turns the model around its pivot by the angles.
void DinoUiRvInformation3DModelRotate::Execute() {
    MTX_Identity44(&mMtx);
    MTX_TransApply44(&mMtx, &mMtx, 0, mPivotY, 0);
    MtxFx44 rot;
    MTX_RotX44(&rot, FX_SinIdx(DinoDegToIdx(mRotX)), FX_CosIdx(DinoDegToIdx(mRotX)));
    MTX_Concat44(&rot, &mMtx, &mMtx);
    MTX_RotY44(&rot, FX_SinIdx(DinoDegToIdx(mRotY)), FX_CosIdx(DinoDegToIdx(mRotY)));
    MTX_Concat44(&rot, &mMtx, &mMtx);
    MTX_RotZ44(&rot, FX_SinIdx(DinoDegToIdx(mRotZ)), FX_CosIdx(DinoDegToIdx(mRotZ)));
    MTX_Concat44(&rot, &mMtx, &mMtx);
    MTX_TransApply44(&mMtx, &mMtx, 0, -mPivotY, 0);
    mOwner->mModel->mUnkC4 = &mMtx;
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov003_02160760[];
// (One pool: the words past its first 0x80 bytes go through the same anchor, so the stores schedule as the game's.)
static struct DinoUiRvInformation3dModelStaticInit {
    DinoUiRvInformation3dModelStaticInit() {
        DINO_POOL_WORD(data_ov003_02160660, 0x9c) = 128;
        DINO_POOL_WORD(data_ov003_02160660, 0xa0) = 176;
        DINO_POOL_WORD(data_ov003_02160660, 0xdc) = 8;
        DINO_POOL_WORD(data_ov003_02160660, 0xe0) = 150;
        DINO_POOL_WORD(data_ov003_02160660, 0x13c) = 64;
        DINO_POOL_WORD(data_ov003_02160660, 0x140) = 112;
        DINO_POOL_WORD(data_ov003_02160660, 0x2c) = 33;
        DINO_POOL_WORD(data_ov003_02160660, 0x30) = 48;
        DINO_POOL_WORD(data_ov003_02160660, 0x94) = 96;
        DINO_POOL_WORD(data_ov003_02160660, 0x98) = 48;
        DINO_POOL_WORD(data_ov003_02160660, 0xb4) = 33;
        DINO_POOL_WORD(data_ov003_02160660, 0xb8) = 64;
        DINO_POOL_WORD(data_ov003_02160660, 0x12c) = 81;
        DINO_POOL_WORD(data_ov003_02160660, 0x130) = 64;
        DINO_POOL_WORD(data_ov003_02160660, 0xe4) = 33;
        DINO_POOL_WORD(data_ov003_02160660, 0xe8) = 82;
        DINO_POOL_WORD(data_ov003_02160660, 0x8c) = 81;
        DINO_POOL_WORD(data_ov003_02160660, 0x90) = 82;
        DINO_POOL_WORD(data_ov003_02160660, 0x25c) = 33;
        DINO_POOL_WORD(data_ov003_02160660, 0x260) = 48;
        DINO_POOL_WORD(data_ov003_02160660, 0x264) = 96;
        DINO_POOL_WORD(data_ov003_02160660, 0x268) = 48;
        DINO_POOL_WORD(data_ov003_02160660, 0x26c) = 33;
        DINO_POOL_WORD(data_ov003_02160660, 0x270) = 64;
        DINO_POOL_WORD(data_ov003_02160660, 0x274) = 81;
        DINO_POOL_WORD(data_ov003_02160660, 0x278) = 64;
        DINO_POOL_WORD(data_ov003_02160660, 0x27c) = 33;
        DINO_POOL_WORD(data_ov003_02160660, 0x280) = 82;
        DINO_POOL_WORD(data_ov003_02160660, 0x284) = 81;
        DINO_POOL_WORD(data_ov003_02160660, 0x288) = 82;
        DINO_POOL_WORD(data_ov003_02160660, 0x11c) = 128;
        DINO_POOL_WORD(data_ov003_02160660, 0x120) = 56;
        DINO_POOL_WORD(data_ov003_02160660, 0xa4) = 128;
        DINO_POOL_WORD(data_ov003_02160660, 0xa8) = 104;
        DINO_POOL_WORD(data_ov003_02160660, 0x24) = 128;
        DINO_POOL_WORD(data_ov003_02160660, 0x28) = 152;
        DINO_POOL_WORD(*(u32 *)&typeid(DinoUiRvInformation3DModel), 0x7c) = 128;
        DINO_POOL_WORD(data_ov003_02160660, 0x200) = 56;
        DINO_POOL_WORD(data_ov003_02160660, 0x204) = 128;
        DINO_POOL_WORD(data_ov003_02160660, 0x208) = 104;
        DINO_POOL_WORD(data_ov003_02160660, 0x20c) = 128;
        DINO_POOL_WORD(data_ov003_02160660, 0x210) = 152;
        DINO_POOL_WORD(data_ov003_02160660, 0x10c) = -119;
        DINO_POOL_WORD(data_ov003_02160660, 0x110) = -16;
        DINO_POOL_WORD(data_ov003_02160660, 0x104) = 3;
        DINO_POOL_WORD(data_ov003_02160660, 0x108) = -15;
        DINO_POOL_WORD(data_ov003_02160660, 0x6c) = -80;
        DINO_POOL_WORD(data_ov003_02160660, 0x70) = 0;
        DINO_POOL_WORD(data_ov003_02160660, 0x64) = -42;
        DINO_POOL_WORD(data_ov003_02160660, 0x68) = 0;
        DINO_POOL_WORD(data_ov003_02160660, 0xfc) = 73;
        DINO_POOL_WORD(data_ov003_02160660, 0x100) = 160;
        DINO_POOL_WORD(data_ov003_02160660, 0x4c) = 123;
        DINO_POOL_WORD(data_ov003_02160660, 0x50) = 160;
        DINO_POOL_WORD(data_ov003_02160660, 0xf4) = 173;
        DINO_POOL_WORD(data_ov003_02160660, 0xf8) = 160;
        DINO_POOL_WORD(data_ov003_02160660, 0x54) = 223;
        DINO_POOL_WORD(data_ov003_02160660, 0x58) = 160;
        DINO_POOL_WORD(data_ov003_02160660, 0x214) = 73;
        DINO_POOL_WORD(data_ov003_02160660, 0x218) = 160;
        DINO_POOL_WORD(data_ov003_02160660, 0x21c) = 123;
        DINO_POOL_WORD(data_ov003_02160660, 0x220) = 160;
        DINO_POOL_WORD(data_ov003_02160660, 0x224) = 173;
        DINO_POOL_WORD(data_ov003_02160660, 0x228) = 160;
        DINO_POOL_WORD(data_ov003_02160660, 0x22c) = 223;
        DINO_POOL_WORD(data_ov003_02160660, 0x230) = 160;
        DINO_POOL_WORD(data_ov003_02160660, 0xc4) = 38;
        DINO_POOL_WORD(data_ov003_02160660, 0xc8) = 48;
        DINO_POOL_WORD(data_ov003_02160660, 0xac) = 98;
        DINO_POOL_WORD(data_ov003_02160660, 0xb0) = 48;
        DINO_POOL_WORD(data_ov003_02160660, 0x5c) = 158;
        DINO_POOL_WORD(data_ov003_02160660, 0x60) = 48;
        DINO_POOL_WORD(data_ov003_02160660, 0xbc) = 218;
        DINO_POOL_WORD(data_ov003_02160660, 0xc0) = 48;
        DINO_POOL_WORD(data_ov003_02160660, 0x34) = 45;
        DINO_POOL_WORD(data_ov003_02160660, 0x38) = 6;
        DINO_POOL_WORD(data_ov003_02160660, 0x44) = 128;
        DINO_POOL_WORD(data_ov003_02160660, 0x48) = 96;
        DINO_POOL_WORD(data_ov003_02160660, 0x1c) = 90;
        DINO_POOL_WORD(data_ov003_02160660, 0x20) = 5;
        DINO_POOL_WORD(data_ov003_02160660, 0xcc) = 8;
        DINO_POOL_WORD(data_ov003_02160660, 0xd0) = 35;
        DINO_POOL_WORD(data_ov003_02160660, 0xd4) = 8;
        DINO_POOL_WORD(data_ov003_02160660, 0xd8) = 102;
        DINO_POOL_WORD(data_ov003_02160660, 0xec) = 60;
        DINO_POOL_WORD(data_ov003_02160660, 0xf0) = 146;
        DINO_POOL_WORD(*(u32 *)&typeid(DinoUiRvInformation3DModel), 0x6c) = 157;
        DINO_POOL_WORD(*(u32 *)&typeid(DinoUiRvInformation3DModel), 0x70) = 31;
        DINO_POOL_WORD(*(u32 *)&typeid(DinoUiRvInformation3DModel), 0x74) = 256;
        DINO_POOL_WORD(*(u32 *)&typeid(DinoUiRvInformation3DModel), 0x78) = 100;
    }
} sDinoUiRvInformation3dModelStaticInit;
