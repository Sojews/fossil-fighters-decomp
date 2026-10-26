// DinoFieldPlayerControl: moves the player on the field and ends when the player does something that needs an event
// (talking, digging, a menu), filling in the request for the event loop.

#include "game/dino_field_player_control.h"
#include "m/call_task.h"
#include "m/fx32.h"
#include "m/polygon_char.h"
#include "m/anm_model.h"
#include "game/dino_field.h"
#include "game/dino_static_init.h"
#include "game/dino_field_hole.h"
#include "game/dino_field_kaseki_rock.h"
#include "game/dino_field_radar_menu.h"
#include "game/dino_backup.h"
#include "game/dino_sound.h"
#include "m/input.h"
#include "m/bit_vector.h"
#include "m/motion.h"
#include <algorithm>

DinoFieldPlayerControl::DinoFieldPlayerControl(DinoField *field, u32 *arg1, DinoFieldPlayerControlResult *result,
                                               u32 arg3)
    : MProcess(NULL), mField(field), mUnk78(arg1), mResult(result), mExitAreas((MBitVector *) arg3), mTouching(0),
      mUnkAC(0), mExcavateDefs(gDinoResource->GetExcavateDefs()) {
    mInput = (MInput *) mField->GetUnk234();
    mMap = 0;
    mArea = 0;
    if (mResult->unk08) {
        mAngle = mResult->unk00;
        mSpeed = mResult->unk04;
    }
    *mUnk78 = 0;
    mEffect = NULL;
}

DinoFieldPlayerControl::~DinoFieldPlayerControl() {
    StoreResult();
}

void DinoFieldPlayerControl::SetEffect(MFx32Point pos, MFx32 height, u16 angle, u32 effect) {
    if (mEffect == NULL) {
        mEffect = new MPolygonChar(mField->mScene, 0x7FFF, NULL, NULL);
        mParallel.AddTail(mEffect);
    }
    MPolygonChar *e = mEffect;
    e->SetPosition(pos.x.v, height.v, pos.y.v);
    for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
        anm->mPosRef = &e->mPos;
        anm->mUnk11B = TRUE;
    }
    e = mEffect;
    e->mRot.x = 0;
    e->mRot.y = (u16) (0x4000 - angle) * 360 / 16;
    e->mRot.z = 0;
    for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
        anm->mRotRef = &e->mRot;
        anm->mUnk11B = TRUE;
    }
    char path[64];
    OS_SPrintf(path, "/model/fieldchar/effect%02d.csv", effect);
    mEffect->Load(path, NULL);
    e = mEffect;
    e->mUnkAC = 1;
    for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
        anm->mFollowParent = TRUE;
    }
    for (MAnmModelBase *anm = (MAnmModelBase *) mEffect->mParallel.mHead; anm != NULL;
         anm = (MAnmModelBase *) anm->mNext) {
        anm->mActive = TRUE;
    }
}

// The player's movement settings (DinoResource::GetExcavate78).
struct DinoPlayerMoveDefs {
    u8 unk00[0x14];
    MFx32 unk14;        // 0x14: the touch walk's speed, per pixel from the player
    u16 turnSpeed;      // 0x18: angle units a frame
    u8 unk1A[2];
    MFx32 accel;        // 0x1C
    MFx32 decel;        // 0x20
    MFx32 walkSpeed;    // 0x24
    MFx32 runSpeed;     // 0x28
    MFx32 slowSpeed;    // 0x2C: up to it, the player walks without steps
    MFx32 stepSpeed;    // 0x30: up to it, the walk's steps; past it, the run's
};

extern "C" MFx32 data_020b5ac0;
extern "C" MFx32 data_ov001_0216a6b4;   // the dig search's range (2, set by the static initialization)

// The buttons released since the last update.
static inline u32 GetReleased(MInput *input) {
    return input->mPrevButtons & (input->mPrevButtons ^ input->mButtons);
}

// The distance between the points (with the hardware square root).
static inline u32 Distance(const MPoint &a, const MPoint &b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    *(vu32 *) 0x040002B8 = dx * dx + dy * dy;
    while (*(vu16 *) 0x040002B0 & 0x8000) {
    }
    return *(vu32 *) 0x040002B4;
}

// The unit vector at the angle.
static inline MFx32Point AngleVector(u16 angle) {
    return MFx32Point(MFx32::FromRaw(FX_CosIdx(angle)), MFx32::FromRaw(FX_SinIdx(angle)));
}

static inline MFx32 SquareSum(const MFx32Point &d) {
    return d.x * d.x + d.y * d.y;
}

static inline bool Contains(const MFx32Rect &r, const MFx32Point &p) {
    return r.left <= p.x && p.x < r.right && r.top <= p.y && p.y < r.bottom;
}

// NONMATCHING: written from the game's code; the stack temporaries are laid out differently.
void DinoFieldPlayerControl::Idle() {
    DinoFieldChara *player;
    u32 focus = mField->GetCamera()->GetFocus();
    if (focus == 0 || (player = mField->GetChara(focus)) == NULL) {
        return;
    }
    // The character following the player (the last walk's run partner).
    DinoFieldChara *follower = NULL;
    for (u32 i = 0; i < 100; i++) {
        DinoFieldChara *chara = mField->mCharas[i];
        if (chara != NULL && chara != player && chara->mTask.unk4 == 1) {
            follower = chara;
            break;
        }
    }
    u32 map = mField->GetMap()->GetInfo()->id;
    if (map != mMap) {
        mArea = *(u32 *) mField->mMap->GetArea(player->GetModel()->pos);
        mMap = map;
        if (!mResult->unk08) {
            mResult->unk08 = 1;
            mAngle = player->GetModel()->angle;
            mSpeed = 0;
        }
    }

    const DinoPlayerMoveDefs *defs = (const DinoPlayerMoveDefs *) gDinoResource->GetExcavate78();
    BOOL decide = FALSE;
    BOOL tapped = FALSE;

    if (mInput->GetPressed() & 0x300) {
        DinoFieldRadar *radar = mField->mRadar->mRadar;
        if (radar) {
            radar->Signal();
        }
    }
    // A tap on the player beeps the radar.
    if (mInput->GetPressed() & 0x1000) {
        int radius = MFx32ToInt(MFx32(96) / mField->GetCamera()->GetUnk148());
        MPoint screen;
        DinoFieldCharaGetScreenPos(&screen, player);
        MPoint touch = mInput->GetTouchPos();
        if (Distance(screen, touch) <= radius) {
            DinoFieldRadar *radar = mField->mRadar->mRadar;
            if (radar) {
                radar->Signal();
                return;
            }
        }
    }
    // A touch near the player starts a turn.
    if (mInput->GetPressed() & 0x1000) {
        int radius = MFx32ToInt(MFx32(96) / mField->GetCamera()->GetUnk148());
        MPoint screen;
        DinoFieldCharaGetScreenPos(&screen, player);
        MPoint touch = mInput->GetTouchPos();
        int dist = Distance(screen, touch);
        if (MFx32(dist) <= MFx32(radius) * (player->GetHeight() + data_020b5ac0)) {
            mTouching = 1;
            mTouchStart = mInput->GetTouchPos();
            mTouchFrames = 0;
            MPoint pos;
            DinoFieldCharaGetScreenPos(&pos, player);
            MPoint d = mTouchStart - pos;
            mTouchAngle = MFx32PointAngle(MFx32Point(MFx32(d.x), MFx32(d.y)));
        }
    }
    if ((mInput->mButtons & 0x1000) && mTouching) {
        mTouchFrames++;
        if (mTouchFrames >= 20) {
            mTouching = 0;
        } else {
            MPoint touch = mInput->GetTouchPos();
            if ((int) Distance(mTouchStart, touch) >= 8) {
                mTouching = 0;
            }
        }
    }
    if ((GetReleased(mInput) & 0x1000) && mTouching) {
        mTouching = 0;
        tapped = TRUE;
        u16 diff = mTouchAngle - mAngle;
        if (diff >= 0x8000) {
            diff = 0x8000 - diff;
        }
        int frames = diff / defs->turnSpeed / 2 + 1;
        GetSerial()->AddTail(player->CreateSetMotionTask(1, 0, MFx32(1)),
                             player->CreateAngleToTask(mTouchAngle, frames, 0), new MTimer(frames));
        mAngle = mTouchAngle;
    }

    {
        void *area = mField->mMap->GetArea(player->GetModel()->pos);
        u32 old = mArea;
        mArea = *(u32 *) area;
        if (old != mArea && mExitAreas->Test(mArea)) {
            mUnk78[0] = 2;
            mUnk78[1] = mArea;
            mUnk78[2] = ((u32 *) area)[3];
            mUnk78[3] = ((u32 *) area)[4];
            GetSerial()->AddTail(MTaskExitTransfer::Create(this));
            goto walk;
        }
    }

    if (mInput->GetPressed() & 1) {
        decide = TRUE;
    }
    if (decide || tapped) {
        MFx32Point front;
        front = player->GetModel()->pos + AngleVector(mAngle) * (player->GetHeight() + MFx32(1));

        // A kaseki rock in front of the player.
        if (mMap == mExcavateDefs.Get()->kasekiRockMap) {
            for (u32 i = 0; i < 20; i++) {
                DinoBackupEntry28 rock;
                gDinoBackup->GetUnk138(i, &rock);
                if (rock.used && (rock.pos - front).Length() < MFx32(2)) {
                    MResource<DinoKasekiDefs> kaseki = gDinoResource->LoadKasekiDefs();
                    if (kaseki.Get()->GetDef(rock.id)->unk04) {
                        mUnk78[0] = 10;
                        mUnk78[1] = (rock.id - 1) / 4 + 1;
                        MI_CpuFill8(&rock, 0, sizeof(rock));
                        gDinoBackup->SetUnk138(i, &rock);
                        mField->mKasekiRocks[i]->Exit();
                        GetSerial()->AddTail(MTaskExitTransfer::Create(this));
                    } else {
                        mUnk78[0] = 9;
                        mUnk78[1] = i;
                        GetSerial()->AddTail(MTaskExitTransfer::Create(this));
                    }
                    return;
                }
            }
        }

        // A character in front of the player.
        for (u32 i = 0; i < 100; i++) {
            DinoFieldChara *chara = mField->mCharas[i];
            if (chara == player || chara->GetModel()->id == 0) {
                continue;
            }
            MFx32Point target = front;
            if (tapped) {
                target.y += chara->GetUnk50();
            }
            BOOL hit = FALSE;
            if (chara->GetUnk2D()) {
                if (Contains(chara->GetRect40() + chara->GetModel()->pos, target)) {
                    hit = TRUE;
                }
            } else {
                MFx32 dist = (chara->GetModel()->pos - target).Length();
                if (dist < chara->GetMoveSpeed()) {
                    hit = TRUE;
                }
            }
            if (hit) {
                if (chara->IsUnk20Two()) {
                    chara->PlayReaction();
                    return;
                }
                if (chara->GetModel()->unk04) {
                    mField->SetUnk238(i);
                }
                mUnk78[0] = 1;
                mUnk78[1] = chara->GetModel()->id;
                GetSerial()->AddTail(MTaskExitTransfer::Create(this));
                return;
            }
        }

        // Digging.
        if (gDinoResource->GetMapExcavateMatch(mMap)) {
            front = player->GetModel()->pos + AngleVector(mAngle) * (player->GetHeight() + MFx32::FromRaw(0x1800));
            DinoFieldHole *hole = mField->FindRockHole(front);
            if (hole) {
                MFx32Point d = hole->GetPos() - player->GetModel()->pos;
                hole->SetUnk114();
                GetSerial()->AddTail(
                    player->CreateChangeMotionTask(4, 0, MFx32(1)), new MTimer(25), DinoSoundCreatePlaySeTask(0xCA, 0),
                    MCreateCallTask(this, &DinoFieldPlayerControl::SetEffect, hole->GetPos(),
                                    MFx32::FromRaw(hole->mUnk90), MFx32PointAngle(d), (u32) 3),
                    player->CreateMoveToTask(player->GetModel()->pos +
                                                 d * (d.Length() - MFx32::FromRaw(0x2800)) / d.Length(),
                                             7, 12, 1),
                    player->CreateAngleToTask(MFx32PointAngle(d), 7, 12), player->CreateWaitStopTask());
                return;
            }
            // A buried kaseki rock near the spot.
            MFx32 best = data_ov001_0216a6b4;
            int found = -1;
            for (u32 i = 0; i < 200; i++) {
                DinoBackupEntry entry;
                gDinoBackup->GetUnk134(i, &entry);
                if (entry.map == mMap) {
                    MFx32 dist = (MFx32Point(MFx32::FromRaw(entry.unk0), MFx32::FromRaw(entry.unk4)) - front).Length();
                    if (dist < best) {
                        best = dist;
                        found = i;
                    }
                }
            }
            if (found != -1) {
                mUnk78[0] = 6;
                mUnk78[1] = found;
                mUnk78[2] = front.x.v;
                mUnk78[3] = front.y.v;
                GetSerial()->AddTail(MTaskExitTransfer::Create(this));
                return;
            }
            fx32 ground = mField->GetMaxGroundHeight(front);
            if (ground == (fx32) 0x80000000) {
                player->ClearUnk2E0();
                GetSerial()->AddTail(player->CreateChangeMotionTask(5, 0, MFx32(1)), new MTimer(30),
                                     MCreateCallTask(this, &DinoFieldPlayerControl::SetEffect, front,
                                                     MFx32::FromRaw(player->mUnk114), player->GetModel()->angle,
                                                     (u32) 4),
                                     DinoSoundCreatePlaySeTask(0xCD, 0), player->CreateWaitStopTask());
                return;
            }
            player->ClearUnk2E0();
            GetSerial()->AddTail(MCreateCallTask(mField, &DinoField::CreateRockHole, front, (long) ground),
                                 player->CreateChangeMotionTask(4, 0, MFx32(1)), new MTimer(30),
                                 MCreateCallTask(this, &DinoFieldPlayerControl::SetEffect, front,
                                                 MFx32::FromRaw(ground), player->GetModel()->angle, (u32) 3),
                                 DinoSoundCreatePlaySeTask(0xCA, 0), player->CreateWaitStopTask());
            return;
        }
    }

    if (tapped) {
        return;
    }
    // A character the player walks into.
    for (u32 i = 0; i < 100; i++) {
        DinoFieldChara *chara = mField->mCharas[i];
        if (chara == player || chara->GetModel()->id == 0) {
            continue;
        }
        if (chara->mTask.unk28.active) {
            MFx32Point d = chara->GetModel()->pos - player->GetModel()->pos;
            MFx32 reach = MFx32::FromRaw(0x19A) + chara->GetHeight() + player->GetHeight();
            if (SquareSum(d) <= reach * reach) {
                if (chara->GetModel()->unk04) {
                    mField->SetUnk238(i);
                }
                mUnk78[0] = 8;
                mUnk78[1] = chara->GetModel()->id;
                GetSerial()->AddTail(MTaskExitTransfer::Create(this));
                goto walk;
            }
        }
    }
    // The lower screen's buttons.
    if (gDinoBackup->GetSystemFlag(0x1C) && (mInput->GetPressed() & 0x1000)) {
        MPoint touch = mInput->GetTouchPos();
        if (mField->mUnk2B4->GetFrame().Contains(touch)) {
            mField->DinoHelpBase::Disable();
            mField->mUnk2B4->Play(2, 0);
            mUnk78[0] = 3;
            mUnk78[1] = 0;
            GetSerial()->AddTail(mField->mUnk2B4->CreateWaitStopTask(), MTaskExitTransfer::Create(this));
            DinoSoundPlaySe(0x8F, 0);
            return;
        }
    }
    if (gDinoBackup->GetSystemFlag(0x1C) && (mInput->GetPressed() & 0x400)) {
        mField->DinoHelpBase::Disable();
        mField->mUnk2B4->Hide();
        mUnk78[0] = 3;
        mUnk78[1] = 0;
        Exit();
        DinoSoundPlaySe(0x8F, 0);
        return;
    }
    if (gDinoBackup->GetSystemFlag(0x20) && (mInput->GetPressed() & 0x1000)) {
        MPoint touch = mInput->GetTouchPos();
        if (mField->mUnk2BC->GetFrame().Contains(touch)) {
            mField->DinoHelpBase::Disable();
            mField->mUnk2BC->Play(2, 0);
            mUnk78[0] = 4;
            GetSerial()->AddTail(mField->mUnk2BC->CreateWaitStopTask(), MTaskExitTransfer::Create(this));
            DinoSoundPlaySe(0x8F, 0);
            return;
        }
    }
    if (gDinoBackup->GetSystemFlag(0x20) && (mInput->GetPressed() & 0x800)) {
        mField->DinoHelpBase::Disable();
        mField->mUnk2B4->Hide();
        mUnk78[0] = 4;
        Exit();
        DinoSoundPlaySe(0x8F, 0);
        return;
    }
    if (gDinoBackup->GetSystemFlag(0x1D) && (mInput->GetPressed() & 8)) {
        DinoSoundPlaySe(0x38D, 0);
        mField->DinoHelpBase::Disable();
        mField->mUnk2B4->Hide();
        mUnk78[0] = 5;
        Exit();
        return;
    }

walk : {
    MFx32 speed = 0;
    MFx32 walkSpeed = mField->GetMap()->GetWalkSpeed();
    mField->WalkInWater((u32) player, (u32) follower);
    MPoint touch = mInput->GetTouchPos();
    MPoint screen;
    DinoFieldCharaGetScreenPos(&screen, player);
    int dist = Distance(screen, touch);
    bool onPlayer = dist <= MFx32ToInt(MFx32(96) / mField->GetCamera()->GetUnk148());
    u16 angle;
    if (!(mTouching && mTouchFrames <= 4) && (mInput->mButtons & 0x1000) && !onPlayer) {
        // Walk toward the touch, faster the farther it is.
        MPoint pos;
        DinoFieldCharaGetScreenPos(&pos, player);
        MPoint d = mInput->GetTouchPos() - pos;
        MFx32Point v(MFx32(d.x), MFx32(d.y));
        angle = MFx32PointAngle(v);
        speed = std::min(defs->runSpeed * walkSpeed, defs->runSpeed * walkSpeed * v.Length() * defs->unk14 / MFx32(96));
    } else {
        int dir = mInput->GetDirection();
        if (dir != -1) {
            angle = dir << 13;
            if (mInput->mButtons & 2) {
                speed = defs->runSpeed * walkSpeed;
            } else {
                speed = defs->walkSpeed * walkSpeed;
            }
        } else {
            angle = player->GetModel()->angle;
        }
    }
    angle -= mField->GetCamera()->GetUnkD8();
    if (mSpeed < speed) {
        mSpeed = std::min(mSpeed + defs->accel * walkSpeed, speed);
    } else if (mSpeed > speed) {
        mSpeed = std::max(mSpeed - defs->decel * walkSpeed, speed);
    }
    u32 diff = (u16) (angle - mAngle);
    if (diff >= 0x7800 && diff <= 0x9800) {
        mAngle = angle;
    } else if (diff < 0x8000) {
        u32 step = MFx32ToInt(MFx32(defs->turnSpeed) * walkSpeed);
        mAngle += std::min(diff, step);
    } else {
        u32 step = MFx32ToInt(MFx32(defs->turnSpeed) * walkSpeed);
        mAngle -= std::min(0x10000 - diff, step);
    }
    if (mSpeed == MFx32(0)) {
        mUnkAC = 0;
        player->SetUnk2D8(MFx32Point(MFx32(0), MFx32(0)), 1, walkSpeed);
        return;
    }
    if (mSpeed <= defs->slowSpeed * walkSpeed) {
        player->SetUnk2D8(AngleVector(mAngle) * mSpeed, 1, walkSpeed);
        return;
    }
    if (mSpeed <= defs->stepSpeed * walkSpeed) {
        player->mUnk310 = player->mMoveSe;
        mField->WalkOnLand0((u32) player, (u32) follower);
        player->SetUnk2D8(AngleVector(mAngle) * mSpeed, 2, walkSpeed);
        return;
    }
    player->mUnk310 = player->mMoveSe2;
    mField->WalkOnLand1((u32) player, (u32) follower);
    player->SetUnk2D8(AngleVector(mAngle) * mSpeed, 3, walkSpeed);
}
}

// The file's static initialization (see dino_static_init.h).
static struct DinoFieldPlayerControlStaticInit {
    DinoFieldPlayerControlStaticInit() {
        DINO_POOL_WORD(data_ov001_0216a6b4, 0x0) = 8192;
    }
} sDinoFieldPlayerControlStaticInit;
