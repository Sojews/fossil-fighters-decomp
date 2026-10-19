// DinoField.

#pragma ipa file

#include "game/dino_field.h"
#include "game/dino_field_kaseki_rock.h"
#include "m/lcd.h"
#include "game/dino_field_chara_focus_task.h"
#include "game/dino_field_chara_wait_move_task.h"
#include "game/dino_field_chara_move_vector_task.h"
#include "game/dino_field_chara_move_offset_task.h"
#include "game/dino_field_chara_move_to_coord_task.h"
#include "game/dino_field_chara_move_to_region_task.h"
#include "game/dino_field_chara_angle_task.h"
#include "game/dino_field_lower_spot.h"
#include "m/display.h"
#include "m/window.h"
#include "m/bright.h"
#include "game/dino_sound.h"
#include "game/dino_field_point.h"
#include "game/dino_field_spot.h"
#include "game/dino_field_capture_fade_in_task.h"
#include "m/bg.h"
#include "m/blend.h"
#include "m/affine_screen.h"
#include "m/ext_affine.h"
#include "m/bg_new_task.h"
#include "m/picture.h"
#include "m/raster.h"
#include "m/call_task.h"
#include "m/polygon_char.h"
#include "m/anm_model.h"
#include "game/dino_field_hole.h"
#include "game/dino_field_capture_fade_out_task_menu.h"
#include "game/dino_field_capture_fade_out_task.h"
#include "game/dino_field_player_control.h"

void DinoField::CreateEffects(MPolygonChar **effects, u32 count, u32 id) {
    if (effects[0] != NULL) {
        return;
    }
    char path[0x100];
    OS_SPrintf(path, "/model/fieldchar/effect%02d.csv", id);
    for (u32 i = 0; i < count; i++) {
        *effects = new MPolygonChar(mScene, 0x7FFF, NULL, NULL);
        (*effects)->Load(path, NULL);
        MPolygonChar *e = *effects;
        e->mUnkAC = 0;
        for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL;
             anm = (MAnmModelBase *) anm->mNext) {
            anm->mFollowParent = FALSE;
        }
        for (MAnmModelBase *anm = (MAnmModelBase *) (*effects)->mParallel.mHead; anm != NULL;
             anm = (MAnmModelBase *) anm->mNext) {
            anm->mActive = FALSE;
        }
        mEffectTasks->AddTail(*effects);
        effects++;
    }
}

#pragma auto_inline off

DinoField::~DinoField() {
    mParallel.Clear();
    if (!mUnk3C8) {
        delete mUnk2B8;
        delete mUnk2C0;
        delete mUnk2C4;
        gLcd->mMain->ResetHooks();
        if (!*mUnk2C8) {
            gLcd->mSub->ResetHooks();
        }
        gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
        if (!*mUnk2C8) {
            gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
        }
        gLcd->VSyncNow();
    }
}

BOOL DinoField::IsUnk258() {
    if (mUnk258) {
        return TRUE;
    }
    return FALSE;
}

DinoFieldCamera *DinoField::GetCamera() {
    return mCamera;
}

DinoFieldMap *DinoField::GetMap() {
    return mMap;
}

u32 DinoField::GetUnk234() {
    return mUnk234;
}

u32 DinoField::GetUnk26C() {
    return mUnk26C;
}

void DinoField::SetUnk258(BOOL value) {
    if (mUnk258 == value) {
        return;
    }
    mUnk258 = value;
    mUnk25C = 0;
    mUnk260 = 0;
    if (mUnk258) {
        mUnk264 = FALSE;
        Disable();
    } else {
        mUnk264 = TRUE;
    }
    for (u32 i = 0; i < 100; i++) {
        if (mCharas[i]->GetModel()->id != 0) {
            if (mUnk258) {
                mCharas[i]->StopWalking();
            } else {
                mCharas[i]->LeaveUnk258();
            }
        }
    }
}

void DinoField::SetUnk238(u32 value) {
    mUnk238 = value;
}

DinoFieldChara *DinoField::GetChara(u32 id) {
    for (u32 i = 0; i < 100; i++) {
        if (mCharas[i]->GetModel()->id == id) {
            if (!mCharas[i]->GetModel()->unk04 || i == mUnk238) {
                return mCharas[i];
            }
        }
    }
    return NULL;
}

// (The game calls even the inline MFx32 operators here, and keeps their copies after this function.)
// NONMATCHING: the logic is the game's, but not the temporaries' stack slots or the skip test's branches.
#pragma dont_inline on

MFx32Point DinoField::ResolveMove(DinoFieldChara *chara, MFx32Point move) {
    u32 focus = mCamera->GetFocus();
    MFx32Point pos = chara->GetModel()->pos;
    MFx32Point result = pos + move;
    if (chara->GetHeight() == MFx32(0)) {
        return result;
    }
    for (u32 i = 0; i < 100; i++) {
        DinoFieldChara *other = mCharas[i];
        if (other->GetModel()->id == 0 || other == chara || other->GetHeight() == MFx32(0) ||
            (focus == chara->GetModel()->id && other->GetTask()->unk4 == 1) ||
            (focus == other->GetModel()->id && chara->GetTask()->unk4 == 1)) {
            continue;
        }
        if (other->GetUnk2C() == 0) {
            MFx32Point d = result - other->GetModel()->pos;
            MFx32 r = chara->GetHeight() + other->GetHeight();
            if (!(d.x * d.x + d.y * d.y >= r * r)) {
                MFx32 len = d.Length();
                result += d / len * (r - len);
            }
        } else {
            MFx32Circle circle(result.x, result.y, chara->GetHeight());
            MFx32Rect rect = other->GetRect30() + other->GetModel()->pos;
            if (circle.Overlaps(rect)) {
                result += rect.PushOut(circle);
            }
        }
    }
    DinoFieldMapInfo *info = mMap->GetInfo();
    if (info->id == mDefs.Get()->GetRockMap() && focus == chara->GetModel()->id) {
        for (u32 i = 0; i < 20; i++) {
            DinoBackupEntry28 rock;
            gDinoBackup->GetUnk138(i, &rock);
            if (rock.used) {
                MFx32Point d = result - *(MFx32Point *) &rock.unkC;
                MFx32 r = chara->GetHeight() + MFx32(1);
                if (!(d.x * d.x + d.y * d.y >= r * r)) {
                    MFx32 len = d.Length();
                    result += d / len * (r - len);
                }
            }
        }
    }
    if (chara->GetTask()->unk4 == 1) {
        return result;
    }

    // Walls: walk the move in small steps, feeling a radius ahead in each direction.
    MFx32Point diff = result - pos;
    MFx32 len = diff.Length();
    MFx32Point dir;
    int shift = 3;
    if (len.v < 0x400 && len.v > 0x400 / 2) {
        dir = diff / len / MFx32(16);
        shift += 1;
    } else if (len.v <= 0x200) {
        dir = diff / len / MFx32(4);
        shift += 2;
    } else {
        dir = diff / len / MFx32(8);
    }
    fx32 height = 0;
    MFx32Point step(MFx32(0), MFx32(0));
    MFx32Point up(MFx32(0), -chara->GetHeight());
    MFx32Point down(MFx32(0), chara->GetHeight());
    MFx32Point left(-chara->GetHeight(), MFx32(0));
    MFx32Point right(chara->GetHeight(), MFx32(0));
    int hit = 0;
    int hit2 = 0;
    while (step.Length() < len) {
        if (!IsWalkable(pos + step + up, &height) && dir.y.v <= 0) {
            hit |= 8;
        }
        if (!IsWalkable(pos + step + down, &height) && dir.y.v >= 0) {
            hit |= 1;
        }
        if (!IsWalkable(pos + step + left, &height) && dir.x.v <= 0) {
            hit |= 2;
        }
        if (!IsWalkable(pos + step + right, &height) && dir.x.v >= 0) {
            hit |= 4;
        }
        if (hit & 0xF) {
            break;
        }
        hit = 0x10;
        step += dir;
    }
    if (!(hit & 0xF)) {
        return result;
    }
    if (hit & 0x10) {
        step -= dir;
    }

    // Blocked: try sliding along the wall, as long as that gets closer to where the move was going.
    if (hit & 1) {
        dir.y.v -= 0x1000 >> shift;
    }
    if (hit & 2) {
        dir.x.v += 0x1000 >> shift;
    }
    if (hit & 8) {
        dir.y.v += 0x1000 >> shift;
    }
    if (hit & 4) {
        dir.x.v -= 0x1000 >> shift;
    }
    if (dir.x.v != 0 || dir.y.v != 0) {
        u32 best = diff.x.v * diff.x.v + diff.y.v * diff.y.v;
        while (step.Length() < len) {
            if (!IsWalkable(pos + step + up, &height) && dir.y.v <= 0) {
                hit2 |= 8;
            }
            if (!IsWalkable(pos + step + down, &height) && dir.y.v >= 0) {
                hit2 |= 1;
            }
            if (!IsWalkable(pos + step + left, &height) && dir.x.v <= 0) {
                hit2 |= 2;
            }
            if (!IsWalkable(pos + step + right, &height) && dir.x.v >= 0) {
                hit2 |= 4;
            }
            if (hit2 & 0xF) {
                break;
            }
            fx32 dy = step.y.v - diff.y.v;
            fx32 dx = step.x.v - diff.x.v;
            u32 dist = dx * dx + dy * dy;
            if (dist > best) {
                break;
            }
            best = dist;
            hit2 |= 0x20;
            step += dir;
        }
    }
    if (hit2 & 0x20) {
        step -= dir;
    }
    int both = (hit | hit2) & 0xF;
    if (both == 6 || both == 9) {
        // Into a corner: keep only the free direction of the move.
        hit2 = 0;
        if (hit & 9) {
            dir.y.v = 0;
        }
        if (hit & 6) {
            dir.x.v = 0;
        }
        if (dir.x.v != 0 || dir.y.v != 0) {
            while (step.Length() < len) {
                if (!IsWalkable(pos + step + up, &height) && dir.y.v < 0) {
                    hit2 |= 8;
                }
                if (!IsWalkable(pos + step + down, &height) && dir.y.v > 0) {
                    hit2 |= 2;
                }
                if (!IsWalkable(pos + step + left, &height) && dir.x.v < 0) {
                    hit2 |= 4;
                }
                if (!IsWalkable(pos + step + right, &height) && dir.x.v > 0) {
                    hit2 |= 6;
                }
                if (hit2 & 0xF) {
                    break;
                }
                hit2 |= 0x20;
                step += dir;
            }
        }
        if (hit2 & 0x20) {
            step -= dir;
        }
    } else if (step.Length().v < 0x200) {
        int same = hit2 & hit & 0xF;
        if (same == 1) {
            step.y.v = 0;
        } else if (same == 8) {
            step.y.v = 0;
        } else if (same == 2) {
            step.x.v = 0;
        } else if (same == 4) {
            step.x.v = 0;
        }
    }
    result = pos + step;
    return result;
}

#pragma dont_inline reset

BOOL DinoField::IsWalkable(fx32 x, fx32 y, fx32 *height) {
    if (mMap != NULL) {
        return mMap->IsWalkable(x, y, height);
    }
    return FALSE;
}

BOOL DinoField::IsWalkable(MFx32Point pos, fx32 *height) {
    return IsWalkable(pos.x.v, pos.y.v, height);
}

fx32 DinoField::GetGroundHeight(fx32 x, fx32 y) {
    if (mMap != NULL) {
        return mMap->GetGroundHeight(x, y);
    }
    return 0;
}

#pragma opt_common_subs off
fx32 DinoField::GetMaxGroundHeight(MFx32Point pos) {
    fx32 y = pos.y.v;
    fx32 h;
    fx32 height = GetGroundHeight(pos.x.v, y);
    if (height == (fx32)0x80000000) {
        return height;
    }
    h = GetGroundHeight(pos.x.v - FX32_ONE, y);
    if (h == (fx32)0x80000000) {
        return h;
    }
    height = std::max(h, height);
    h = GetGroundHeight(pos.x.v + FX32_ONE, y);
    if (h == (fx32)0x80000000) {
        return h;
    }
    height = std::max(h, height);
    h = GetGroundHeight(pos.x.v, y - FX32_ONE);
    if (h == (fx32)0x80000000) {
        return h;
    }
    height = std::max(h, height);
    h = GetGroundHeight(pos.x.v, y + FX32_ONE);
    if (h == (fx32)0x80000000) {
        return h;
    }
    height = std::max(h, height);
    return height + 0.1 * FX32_ONE;
}
#pragma opt_common_subs reset

void DinoField::Write(void *buf) {
    MSerializer::Write(buf);
}

MTask *DinoField::CreateCharaAngleTask(u32 chara, u32 angle, u32 a, u32 b) {
    return new DinoFieldCharaAngleTask(this, chara, angle, a, b);
}

MTask *DinoField::CreateCharaMoveToRegionTask(u32 chara, u32 region, u32 a, u32 b, u8 c) {
    return new DinoFieldCharaMoveToRegionTask(this, chara, region, a, b, c);
}

MTask *DinoField::CreateCharaMoveToCoordTask(u32 chara, MFx32Point coord, u32 a, u32 b, u8 c) {
    return new DinoFieldCharaMoveToCoordTask(this, chara, coord, a, b, c);
}

MTask *DinoField::CreateCharaMoveOffsetTask(u32 chara, MFx32Point offset, u32 a, u32 b, u8 c) {
    return new DinoFieldCharaMoveOffsetTask(this, chara, offset, a, b, c);
}

// NONMATCHING: the game spills the register arguments instead of keeping a in r7.
MTask *DinoField::CreateCharaMoveVectorTask(u32 chara, u16 direction, MFx32 a, u32 b, u32 c, u8 d) {
    return new DinoFieldCharaMoveVectorTask(this, chara, direction, a, b, c, d);
}

MTask *DinoField::CreateCharaWaitMoveTask(u32 chara) {
    return new DinoFieldCharaWaitMoveTask(this, chara);
}

MTask *DinoField::CreateCharaFocusTask(u32 chara) {
    return new DinoFieldCharaFocusTask(this, chara);
}

// The game calls these constructors rather than inlining them.
#pragma auto_inline off

DinoFieldLowerSpotIn::DinoFieldLowerSpotIn(int duration) : MProcess(NULL) {
    mDuration = duration;
    mFrame = 0;
    mStep = 0;
}

DinoFieldLowerSpotOut::DinoFieldLowerSpotOut(int duration) : MProcess(NULL) {
    mDuration = duration;
    mFrame = 0;
    mStep = 0;
}

#pragma auto_inline on

DinoFieldLowerSpotIn::~DinoFieldLowerSpotIn() {
    gLcd->mMain->mWin->Reset();
}

DinoFieldLowerSpotOut::~DinoFieldLowerSpotOut() {
    gLcd->mMain->mWin->Reset();
    gLcd->mMain->mBright->SetMode(2);
    gLcd->mMain->mBright->SetFactor(16);
}

void DinoFieldLowerSpotIn::Idle() {
    switch (mStep) {
    case 0:
        gLcd->mMain->mBright->Reset();
        mBlack = new MTextPicture(gLcd->mMain, "/image/black", TRUE, false);
        mBlack->Show();
        mParallel.AddTail(mBlack);
        mWindow = new MRasterWindow(-1, gLcd->mMain, TRUE);
        mParallel.AddTail(mWindow);
        mWindow->SetTarget(mBlack->GetPlaneMask() ^ 0x3F);
        mWindow->SetObjTarget(mBlack->GetPlaneMask());
        mWindow->Show();
        mStep++;
    case 1:
        if (++mFrame >= mDuration) {
            mBlack->Hide();
            mWindow->Hide();
            MTask *exit = MTaskExitTransfer::Create(this);
            MTask *timer = new MTimer(1);
            MSerialTaskSet *serial = &mSerial;
            serial->AddTail(timer);
            serial->AddTail(exit);
            return;
        }
        mWindow->SetCircle(MPoint(128, 96), mFrame * 160 / mDuration);
        mWindow->SubtractRect(MRect(255, 0, 256, 192));
        break;
    }
}

void DinoFieldLowerSpotOut::Idle() {
    switch (mStep) {
    case 0:
        mBlack = new MTextPicture(gLcd->mMain, "/image/black", TRUE, false);
        mBlack->Show();
        mParallel.AddTail(mBlack);
        mWindow = new MRasterWindow(-1, gLcd->mMain, TRUE);
        mParallel.AddTail(mWindow);
        mWindow->SetTarget(mBlack->GetPlaneMask() ^ 0x3F);
        mWindow->SetObjTarget(mBlack->GetPlaneMask());
        mWindow->Show();
        mStep++;
    case 1:
        if (++mFrame >= mDuration) {
            Exit();
            return;
        }
        mWindow->SetCircle(MPoint(128, 96), (mDuration - mFrame) * 160 / mDuration);
        mWindow->SubtractRect(MRect(255, 0, 256, 192));
        break;
    }
}

#pragma auto_inline off

DinoFieldCharaAngleTask::~DinoFieldCharaAngleTask() {
}

DinoFieldCharaMoveToRegionTask::~DinoFieldCharaMoveToRegionTask() {
}

DinoFieldCharaMoveToCoordTask::~DinoFieldCharaMoveToCoordTask() {
}

DinoFieldCharaMoveOffsetTask::~DinoFieldCharaMoveOffsetTask() {
}

DinoFieldCharaMoveVectorTask::~DinoFieldCharaMoveVectorTask() {
}

DinoFieldCharaWaitMoveTask::~DinoFieldCharaWaitMoveTask() {
}

DinoFieldCharaFocusTask::~DinoFieldCharaFocusTask() {
}

DinoFieldSpotOut::~DinoFieldSpotOut() {
    gLcd->GetTopDisplay()->mBright->SetFactor(16);
    gLcd->GetBottomDisplay()->mBright->SetFactor(16);
}

DinoFieldSpotIn::~DinoFieldSpotIn() {
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
}

DinoFieldWhiteOut::~DinoFieldWhiteOut() {
    gLcd->GetTopDisplay()->mBright->SetFactor(16);
    gLcd->GetBottomDisplay()->mBright->SetFactor(16);
}

DinoFieldWhiteIn::~DinoFieldWhiteIn() {
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
}

DinoFieldBlackOut::~DinoFieldBlackOut() {
    gLcd->GetTopDisplay()->mBright->SetFactor(16);
    gLcd->GetBottomDisplay()->mBright->SetFactor(16);
}

DinoFieldBlackIn::~DinoFieldBlackIn() {
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
}

void DinoFieldWhiteOut::Idle() {
    MTask *top;
    MTask *bottom;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    bottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToWhite(mDuration, 0);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeOutToWhite(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(bottom);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

void DinoFieldWhiteIn::Idle() {
    MTask *top;
    MTask *bottom;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    bottom = gLcd->GetBottomDisplay()->mBright->CreateFadeInFromWhite(mDuration, 0);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeInFromWhite(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(bottom);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

void DinoFieldBlackOut::Idle() {
    MTask *top;
    MTask *bottom;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    bottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(mDuration, 0);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(bottom);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

void DinoFieldBlackIn::Idle() {
    MTask *top;
    MTask *bottom;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    bottom = gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(mDuration, 0);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeInFromBlack(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(bottom);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

void DinoFieldSpotOut::Idle() {
    MTask *top;
    MTask *lower;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    lower = new DinoFieldLowerSpotOut(mDuration);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(lower);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

void DinoFieldSpotIn::Idle() {
    MTask *top;
    MTask *lower;
    MTask *exit;
    MParallelTaskSet *fades;
    exit = MTaskExitTransfer::Create(this);
    lower = new DinoFieldLowerSpotIn(mDuration);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeInFromBlack(mDuration, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(top);
    fades->AddTail(lower);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<MPolygonChar, MPolygonChar>;
template MTask *MCreateCallTask(MPolygonChar *obj, void (MPolygonChar::*func)());
template class MBgCallTask0<DinoFieldCaptureFadeInTask>;
template MTask *MCreateBgCallTask(DinoFieldCaptureFadeInTask *obj, void (DinoFieldCaptureFadeInTask::*func)(), int thread);
template class MCallTask1<MBg, MBg, bg_mode>;
template MTask *MCreateCallTask(MBg *obj, void (MBg::*func)(bg_mode), bg_mode a0);

void DinoFieldCharaFocusTask::Transfer() {
    mField->GetCamera()->SetFocus(mChara);
}

void DinoFieldCharaAngleTask::Transfer() {
    DinoFieldChara *chara = mField->GetChara(mChara);
    if (chara) {
        chara->SetAngle(mAngle, mA, mB);
    }
}

void DinoFieldCharaWaitMoveTask::Idle() {
    DinoFieldChara *chara = mField->GetChara(mChara);
    if (!chara) {
        Exit();
    } else {
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *wait = chara->CreateWaitMoveTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(exit);
    }
}

// NONMATCHING: stack slots of the point temporaries only.
void DinoFieldCharaMoveOffsetTask::Transfer() {
    DinoFieldChara *chara = mField->GetChara(mChara);
    if (chara) {
        MFx32Point offset = mOffset;
        MFx32Point coord = chara->GetModel()->pos + offset;
        if (!mC) {
            chara->SetDirection(MFx32Atan2(offset.y, offset.x));
        }
        chara->MoveTo(coord, mA, mB, mC);
    }
}

// NONMATCHING: stack slots of the point temporaries only (as MoveToCoord).
void DinoFieldCharaMoveToRegionTask::Transfer() {
    DinoFieldChara *chara = mField->GetChara(mChara);
    if (chara) {
        DinoFieldMap *map = mField->mMap;
        if (gDinoResource->GetRegionMapMatch(mRegion) == map->GetInfo()->id) {
            DinoRegionCenter center = gDinoResource->GetRegionCenter(mRegion);
            MFx32Point coord(MFx32((int)center.unk00), MFx32((int)center.unk04));
            MFx32Point delta = coord - chara->GetModel()->pos;
            if (!mC) {
                chara->SetDirection(MFx32Atan2(delta.y, delta.x));
            }
            chara->MoveTo(coord, mA, mB, mC);
        }
    }
}

// Moves the chara the distance mA in mDirection from where it faces.
void DinoFieldCharaMoveVectorTask::Transfer() {
    DinoFieldChara *chara = mField->GetChara(mChara);
    if (chara) {
        u16 direction = mDirection + chara->GetModel()->angle;
        MFx32Point coord = chara->GetModel()->pos +
                           MFx32Point(MFx32::FromRaw(FX_CosIdx(direction)), MFx32::FromRaw(FX_SinIdx(direction))) * mA;
        if (!mD) {
            chara->SetDirection(direction);
        }
        chara->MoveTo(coord, mB, mC, mD);
    }
}

void DinoFieldCharaMoveToCoordTask::Transfer() {
    DinoFieldChara *chara = mField->GetChara(mChara);
    if (chara) {
        MFx32Point coord(mCoord);
        MFx32Point delta = coord - chara->GetModel()->pos;
        if (!mC) {
            chara->SetDirection(MFx32Atan2(delta.y, delta.x));
        }
        chara->MoveTo(coord, mA, mB, mC);
    }
}

extern "C" struct {
    void (MBg::*SetMode_00)(bg_mode);
} data_ov001_021693d4;

// Cross-fades the picture out, then frees it and resets the background mode.
void DinoField::FadeOutPict(MDisplay *display, MExtAffinePicture **pict, u32 frames, long ease) {
    if (*pict == NULL) {
        return;
    }
    MTask *reset = MCreateCallTask(display->mBg, data_ov001_021693d4.SetMode_00, (bg_mode)0);
    MTask *timer = new MTimer(1);
    MTask *clear = display->mBlend->CreateClearTransfer();
    MTask *exit = MTaskExitTransfer::Create(*pict);
    MBlend *blend = display->mBlend;
    MTask *fade = blend->CreateCrossFadeOutTask((*pict)->GetPlaneMask(), frames, ease);
    mPictTasks->AddTail(fade, exit, clear, timer, reset);
    *pict = NULL;
}

extern "C" struct {
    void (MBg::*SetMode_00)(bg_mode);
} data_ov001_0216938c;

// Slides the picture off the screen (0 right, 1 down, 2 left, 3 up), then frees it and resets the background mode.
// NONMATCHING: sDirs' offset in the TU's data (the game's data before it isn't all emitted here).
void DinoField::SlideOutPict(MDisplay *display, MExtAffinePicture **pict, u32 dir, u32 frames, long ease) {
    if (*pict == NULL) {
        return;
    }
    static const MPoint sDirs[4] = {MPoint(256, 0), MPoint(0, 192), MPoint(-256, 0), MPoint(0, -192)};
    MTask *reset = MCreateCallTask(display->mBg, data_ov001_0216938c.SetMode_00, (bg_mode)0);
    MTask *timer = new MTimer(1);
    MTask *exit = MTaskExitTransfer::Create(*pict);
    MPoint to = (*pict)->GetPosition() + sDirs[dir];
    MTask *slide = (*pict)->CreateCoordAnime((*pict)->GetPosition(), to, frames, ease);
    mPictTasks->AddTail(slide, exit, timer, reset);
    *pict = NULL;
}

extern "C" struct {
    void (MBg::*SetMode_00)(bg_mode);
} data_ov001_02169374;

// Cross-fades the picture out while scaling it from 1 to scale, then frees it and resets the background mode.
// NONMATCHING: the stack layout of the scale points (and a cached ease).
void DinoField::ZoomOutPict(MDisplay *display, MExtAffinePicture **pict, MFx32 scale, u32 frames, long ease) {
    if (*pict == NULL) {
        return;
    }
    MTask *reset = MCreateCallTask(display->mBg, data_ov001_02169374.SetMode_00, (bg_mode)0);
    MTask *timer = new MTimer(1);
    MTask *clear = display->mBlend->CreateClearTransfer();
    MTask *exit = MTaskExitTransfer::Create(*pict);
    MTask *zoom = (*pict)->GetScreen()->CreateScaleAnime(MFx32Point(MFx32(1), MFx32(1)), MFx32Point(scale, scale),
                                                         frames, ease);
    MBlend *blend = display->mBlend;
    MTask *fade = blend->CreateCrossFadeOutTask((*pict)->GetPlaneMask(), frames, ease);
    MParallelTaskSet *both = new MParallelTaskSet(TRUE);
    both->AddTail(fade);
    both->AddTail(zoom);
    mPictTasks->AddTail(both, exit, clear, timer, reset);
    *pict = NULL;
}

void DinoField::FadeInLowerPict(u32 pict, MPoint pos, u32 frames, long unk) {
    mLowerPos = pos;
    FadeInPict(gLcd->GetBottomDisplay(), &mLowerPictId, &mLowerPict, pict, pos, frames, unk);
}

void DinoField::FadeOutLowerPict(u32 pict, long frames) {
    FadeOutPict(gLcd->GetBottomDisplay(), &mLowerPict, pict, frames);
}

void DinoField::SlideInLowerPict(u32 pict, MPoint pos, u32 a, u32 b, long c) {
    mLowerPos = pos;
    SlideInPict(gLcd->GetBottomDisplay(), &mLowerPictId, &mLowerPict, pict, pos, a, b, c);
}

void DinoField::SlideOutLowerPict(u32 pict, u32 a, long b) {
    SlideOutPict(gLcd->GetBottomDisplay(), &mLowerPict, pict, a, b);
}

void DinoField::ZoomInLowerPict(u32 pict, MPoint pos, MFx32 scale, u32 a, long b) {
    mLowerPos = pos;
    ZoomInPict(gLcd->GetBottomDisplay(), &mLowerPictId, &mLowerPict, pict, pos, scale, a, b);
}

void DinoField::ZoomOutLowerPict(MFx32 scale, u32 a, long b) {
    ZoomOutPict(gLcd->GetBottomDisplay(), &mLowerPict, scale, a, b);
}

void DinoField::FadeInUpperPict(u32 pict, MPoint pos, u32 frames, long unk) {
    mUpperPos = pos;
    FadeInPict(gLcd->GetTopDisplay(), &mUpperPictId, &mUpperPict, pict, pos, frames, unk);
}

void DinoField::FadeOutUpperPict(u32 pict, long frames) {
    FadeOutPict(gLcd->GetTopDisplay(), &mUpperPict, pict, frames);
}

void DinoField::SlideInUpperPict(u32 pict, MPoint pos, u32 a, u32 b, long c) {
    mUpperPos = pos;
    SlideInPict(gLcd->GetTopDisplay(), &mUpperPictId, &mUpperPict, pict, pos, a, b, c);
}

void DinoField::SlideOutUpperPict(u32 pict, u32 a, long b) {
    SlideOutPict(gLcd->GetTopDisplay(), &mUpperPict, pict, a, b);
}

void DinoField::ZoomInUpperPict(u32 pict, MPoint pos, MFx32 scale, u32 a, long b) {
    mUpperPos = pos;
    ZoomInPict(gLcd->GetTopDisplay(), &mUpperPictId, &mUpperPict, pict, pos, scale, a, b);
}

void DinoField::ZoomOutUpperPict(MFx32 scale, u32 a, long b) {
    ZoomOutPict(gLcd->GetTopDisplay(), &mUpperPict, scale, a, b);
}

void DinoField::CreateKasekiRock(u32 index) {
    mKasekiRocks[index] = new DinoFieldKasekiRock(this, mScene, index);
    mKasekiRockTasks->AddTail(mKasekiRocks[index]);
}

void DinoField::LoadMap(u32 id) {
    mUnk370->Clear();
    mEffectTasks->Clear();
    mHoles->Clear();
    mRockHoles->Clear();
    MI_CpuFill8(&mSteps[0], 0, sizeof(DinoFieldStep));
    MI_CpuFill8(&mSteps[1], 0, sizeof(DinoFieldStep));
    MI_CpuFill8(&mSteps[2], 0, 8 * sizeof(MPolygonChar *));
    mUnk378 = 0;
    mUnk37C = 1;
    mMap->GetInfo()->id = id;
    mMap->Load();
    if (mMap->GetMapData()[0x68] == 0) {
        CreateEffects(mSteps[0].effects, 3, 1);
        CreateEffects(mSteps[1].effects, 3, 2);
    } else {
        CreateEffects(mSteps[2].effects, 8, 0x1D);
    }
}

extern "C" void func_ov001_021603d8(DinoFieldChara *chara);
extern "C" void func_ov001_02157534(DinoField *field);

// Puts the character where the save has it, if it's on this map (else just resets it).
void DinoField::SyncChara(u32 id) {
    DinoCharaState state;
    gDinoBackup->GetChara(id, &state);
    if (state.unk0 && state.map == mMap->GetInfo()->id) {
        DinoFieldChara *chara = GetChara(id);
        if (chara == NULL) {
            chara = GetChara(0);
            if (chara == NULL) {
                return;
            }
        }
        chara->ResetState();
        chara->GetModel()->id = id;
        MFx32Point &pos = chara->GetModel()->pos;
        pos.x = state.x;
        pos.y = state.y;
        chara->GetModel()->angle = state.angle;
        chara->Refresh();
        DinoCharaTask task;
        gDinoBackup->GetCharaTask(id, &task);
        chara->SetTask(task);
    } else {
        DinoFieldChara *chara = GetChara(id);
        if (chara != NULL) {
            chara->ResetState();
            chara->Refresh();
        }
    }
}

void DinoField::ReloadMap() {
    mFadeTasks->Clear();
    for (u32 i = 0; i < 100; i++) {
        mCharas[i]->ResetState();
        func_ov001_021603d8(mCharas[i]);
    }
    u32 map = mMap->GetInfo()->id;
    LoadMap(map);
    for (u32 i = 1; i < gDinoResource->GetCharaCount(); i++) {
        DinoCharaState state;
        gDinoBackup->GetChara(i, &state);
        if (state.unk0) {
            DinoCharaTask task;
            gDinoBackup->GetCharaTask(i, &task);
            if (task.unk4 == 1) {
                DinoCharaState target;
                gDinoBackup->GetChara(task.unk8.fellow.target, &target);
                if (target.unk0) {
                    state.map = target.map;
                    state.x = target.x;
                    state.y = target.y;
                    state.angle = target.angle;
                    gDinoBackup->SetChara(i, &state);
                }
            }
        }
    }
    for (u32 i = 1; i < gDinoResource->GetCharaCount(); i++) {
        DinoCharaState state;
        gDinoBackup->GetChara(i, &state);
        if (state.unk0 && state.map == map) {
            SyncChara(i);
        }
    }
    func_ov001_02157534(this);
    CreateKasekiRocks();
}

void DinoField::Read(const void *buf) {
    MSerializer::Read(buf);
    if (mUnk258) {
        mUnk264 = 0;
        DinoHelpBase::Disable();
    } else {
        DinoHelpBase::Enable();
    }
    if (mBgmScene) {
        DinoSoundLoadScene(mBgmScene);
    }
    if (mBgm) {
        DinoSoundPlayBgm(mBgm, 0);
    }
    LoadMap(mMap->GetInfo()->id);
    CreateKasekiRocks();
    for (u32 i = 0; i < 100; i++) {
        func_ov001_021603d8(mCharas[i]);
    }
    if (!mUnk258 && gDinoBackup->GetSystemFlag(0x1C)) {
        mUnk2B4->Play(1, 0);
    }
    if (!mUnk258 && gDinoBackup->GetSystemFlag(0x20)) {
        mUnk2BC->Play(1, 0);
    }
    DinoFieldNamePanel *panel = mCamera->mNamePanel;
    panel->SetMode(gDinoBackup->GetUnk180(), 0);
}

void DinoField::CreateKasekiRocks() {
    MResource<DinoExcavateDefs> defs = gDinoResource->GetExcavateDefs();
    mKasekiRockTasks->Clear();
    u32 map = mMap->GetInfo()->id;
    if (map != defs.Get()->kasekiRockMap) {
        return;
    }
    for (u32 i = 0; i < 20; i++) {
        DinoBackupEntry28 entry;
        gDinoBackup->GetUnk138(i, &entry);
        if (entry.used) {
            CreateKasekiRock(i);
        } else {
            mKasekiRocks[i] = NULL;
        }
    }
}

void DinoField::HideUnk2B4() {
    mUnk2B4->Hide();
    mUnk2BC->Hide();
    mCamera->mNamePanel->SetMode(0, 1);
}

// NONMATCHING (and WalkOnLand1): the game keeps the run test even with run 0 (movs r2, #0; bne).
void DinoField::WalkOnLand0(u32 dir) {
    if (mMap->GetMapData()[0x68] == 0) {
        Walk(dir, 0, &mSteps[0], 3, 10, 6, 5, 1);
    }
}

void DinoField::WalkOnLand1(u32 dir) {
    if (mMap->GetMapData()[0x68] == 0) {
        Walk(dir, 0, &mSteps[1], 3, 7, 6, 3, 2);
    }
}

void DinoField::WalkInWater(u32 dir, u32 run) {
    if (mMap->GetMapData()[0x68] != 0) {
        Walk(dir, run, &mSteps[2], 4, 8, 8, 4, 0x1D);
    }
}

MTask *DinoField::CreateMoneyShowTask() {
    if (mMoney) {
        return new MTimer(0);
    }
    mMoney = new DinoFieldMoney();
    mParallel.AddTail(mMoney);
    return mMoney->CreateShowTask();
}

MTask *DinoField::CreateMoneyHideTask() {
    if (!mMoney) {
        return new MTimer(0);
    }
    MTask *task = mMoney->CreateHideTask();
    mMoney = NULL;
    return task;
}

MTask *DinoField::CreateDonateShowTask() {
    if (mDonate) {
        return new MTimer(0);
    }
    mDonate = new DinoFieldDonate();
    mParallel.AddTail(mDonate);
    return mDonate->CreateShowTask();
}

MTask *DinoField::CreateDonateHideTask() {
    if (!mDonate) {
        return new MTimer(0);
    }
    MTask *task = mDonate->CreateHideTask();
    mDonate = NULL;
    return task;
}

MTask *DinoField::CreateFadeWaitTask() {
    return MTaskSetWaitEmptyTask::Create(mFadeTasks);
}

void DinoField::PlayBgm(u32 bgm, u32 arg) {
    if (mBgmChanging == 0) {
        mBgm = bgm;
        mBgmScene = DinoSoundGetScene();
        DinoSoundPlayBgm(bgm, arg);
    }
}

void DinoField::StopBgm(u32 fadeFrames) {
    mBgm = 0;
    mBgmChanging = FALSE;
    DinoSoundStopBgm(-1, fadeFrames);
}

void DinoField::PlayBgmFade(u32 bgm, u32 arg) {
    mBgm = bgm;
    mBgmScene = DinoSoundGetScene();
    mBgmChanging = TRUE;
    DinoSoundPlayBgm(bgm, arg);
}

u32 DinoField::GetBgm() {
    return mBgm;
}

BOOL DinoField::IsBgmChanging() {
    if (mBgmChanging) {
        return TRUE;
    }
    return FALSE;
}

void DinoField::Quake(u32 a, u32 b, u32 c) {
    mQuake = TRUE;
    mQuakeArgs[0] = a;
    mQuakeArgs[1] = b;
    mQuakeArgs[2] = c;
    mQuakeTime = 0;
}

DinoFieldHole *DinoField::CreateHole(MFx32Point pos) {
    DinoFieldHole *hole = new DinoFieldHole(this, (u32)mScene, pos, GetMaxGroundHeight(pos), 1);
    mHoles->AddTail(hole);
    return hole;
}

void DinoField::CreateRockHole(MFx32Point pos, u32 arg) {
    mRockHoles->AddTail(new DinoFieldHole(this, (u32)mScene, pos, arg, 0));
}

// Whether a is within the square root of range of b.
static inline bool IsWithin(MFx32Point a, MFx32Point b, MFx32 range) {
    MFx32Point d = a - b;
    return d.x * d.x + d.y * d.y <= range;
}

static inline bool IsNearHole(DinoFieldHole *hole, MFx32Point pos) {
    bool near = false;
    if (hole->GetUnk110()) {
        MFx32 range;
        range.v = 0x2400;
        if (IsWithin(hole->GetPos(), pos, range)) {
            near = true;
        }
    }
    return near;
}

// NONMATCHING: stack slots of the point temporaries only.
DinoFieldHole *DinoField::FindRockHole(MFx32Point pos) {
    for (DinoFieldHole *hole = (DinoFieldHole *)mRockHoles->mHead; hole != NULL;
         hole = (DinoFieldHole *)hole->mNext) {
        if (IsNearHole(hole, pos)) {
            return hole;
        }
    }
    return NULL;
}

MTask *DinoField::CreatePictWaitTask() {
    return MTaskSetWaitEmptyTask::Create(mPictTasks);
}

#pragma auto_inline on

DinoFieldCaptureFadeOutTask::~DinoFieldCaptureFadeOutTask() {
}

DinoFieldCaptureFadeInTask::~DinoFieldCaptureFadeInTask() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.

// Background loads of the lower screen's pictures (made by functions not decompiled yet).
// NONMATCHING (the MFx32 creator only): we copy the MFx32 argument to a stack temporary like the MPoint one; the
// game copies only the MPoint (maybe its MFx32 had no user-declared constructors).
template class MBgCallTask8<DinoField, MDisplay *, MView *, MExtAffinePicture **, u32, MPoint, MFx32, u32, long>;
template MTask *MCreateBgCallTask(DinoField *obj, void (DinoField::*func)(MDisplay *, MView *, MExtAffinePicture **, u32, MPoint, MFx32, u32, long), MDisplay *a1, MView *a2, MExtAffinePicture **a3, u32 a4, MPoint a5, MFx32 a6, u32 a7, long a8, int thread);
template class MBgCallTask8<DinoField, MDisplay *, MView *, MExtAffinePicture **, u32, MPoint, u32, u32, long>;
template MTask *MCreateBgCallTask(DinoField *obj, void (DinoField::*func)(MDisplay *, MView *, MExtAffinePicture **, u32, MPoint, u32, u32, long), MDisplay *a1, MView *a2, MExtAffinePicture **a3, u32 a4, MPoint a5, u32 a6, u32 a7, long a8, int thread);
template class MBgCallTask7<DinoField, MDisplay *, MView *, MExtAffinePicture **, u32, MPoint, u32, long>;
template MTask *MCreateBgCallTask(DinoField *obj, void (DinoField::*func)(MDisplay *, MView *, MExtAffinePicture **, u32, MPoint, u32, long), MDisplay *a1, MView *a2, MExtAffinePicture **a3, u32 a4, MPoint a5, u32 a6, long a7, int thread);

#pragma auto_inline off

// The field's capture fades.
void MPolygonChar::DetachModels() {
    mUnkAC = 0;
    for (MTask *task = mParallel.mHead; task != NULL; task = task->mNext) {
        ((MAnmModelBase *)task)->mFollowParent = FALSE;
    }
}

void MPolygonChar::DeactivateModels() {
    for (MTask *task = mParallel.mHead; task != NULL; task = task->mNext) {
        ((MAnmModelBase *)task)->mActive = FALSE;
    }
}

extern "C" MProcess *func_ov001_02159cd0(void *field) {
    return new DinoFieldCaptureFadeOutTask(field);
}

extern "C" MProcess *func_ov001_02159d3c(DinoField *field) {
    return new DinoFieldCaptureFadeInTask(field);
}

// The player's control on the field.
extern "C" MProcess *func_ov001_021580a0(DinoField *field, u32 *arg1, DinoFieldPlayerControlResult *result, u32 arg3) {
    return new DinoFieldPlayerControl(field, arg1, result, arg3);
}

// Loads the field back from the saved state (in the background), running its task once.
void DinoFieldCaptureFadeInTask::LoadField() {
    mField->Read(gDinoBackup->GetUnk140());
    MTask *task = (MTask *)mField->mUnk24C[0];
    if (task->mState == MTask::STATE_RUN) {
        task->Execute();
    }
}
