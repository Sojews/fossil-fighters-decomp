// DinoUi2PartsRockName.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_backup.h"
#include "game/dino_ui_common.h"
#include "game/dino_ui2_parts_rock_name_menu.h"
#include "game/dino_ui2_parts_rock_name.h"
#include "game/dino_ui2_parts_kaseki_button.h"
#include "game/dino_ui2_field_menu_cursor.h"
#include "game/dino_sound.h"
#include "m/obj.h"
#include "m/oam_canvas.h"
#include "m/display.h"

DinoUi2PartsRockName::~DinoUi2PartsRockName() {
}

DinoUi2PartsKasekiButton::~DinoUi2PartsKasekiButton() {
}

void DinoUi2PartsRockName::Idle() {
}

MRect DinoUi2PartsKasekiButton::GetRect() {
    return GetFrame();
}

MDisplay *DinoUi2PartsKasekiButton::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoUi2PartsKasekiButton::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUi2PartsKasekiButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mView->Show();
    } else {
        mView->Hide();
    }
}

MRect DinoUi2PartsKasekiButton::GetFrame() {
    return MRect(mPos.Offset(-24, -24), mPos.Offset(24, 24));
}

extern "C" const char data_ov004_02176b2c[]; // ""

// The fossil's name, in a color by its state.
#pragma opt_common_subs off
DinoUi2PartsRockName::DinoUi2PartsRockName(MPoint pos) : MProcess(NULL) {
    mFont = gDinoResource->CreateFont12(0, 1, 0);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, gCurrentDisplay->mObj->mOam, MPoint(256, 32),
                                        gDinoResource->GetUnkTable(), TRUE);
    mText = new MIndicator(canvas, mFont, 0);
    mParallel.AddTail(mText);
    mText->SetPosition(pos);
    mText->SetPriority(2);
    mText->Show();
}
#pragma opt_common_subs reset

void DinoUi2PartsRockName::SetEntry(const DinoBackupEntry *entry, BOOL selected) {
    DinoKasekiRockState rock;
    gDinoBackup->GetKasekiRock(entry->id, (u32 *)&rock);
    mText->SetText(std::string(data_ov004_02176b2c));
    if (mText->mState == STATE_RUN) {
        mText->Execute();
    }
    if (selected) {
        mFont->SetShadowColor(3);
    } else if (!rock.unk10 && entry->unk10_3) {
        mFont->SetShadowColor(5);
    } else {
        mFont->SetShadowColor(1);
    }
    mText->SetText(gDinoBackup->GetEntryName(entry));
}

void DinoUi2PartsRockName::Clear() {
    mText->SetText(std::string(data_ov004_02176b2c));
}

DinoUi2PartsKasekiButton::DinoUi2PartsKasekiButton(MSmartOam *oam, MPoint pos, DinoUiKasekiIcon icon, u8 marked,
                                                   u32 textId) {
    mPos = pos;
    mTextId = textId;
    Setup(oam, icon);
    SetMarked(marked);
    DinoHelpItem::Disable();
}

void DinoUi2PartsKasekiButton::SetMarked(BOOL marked) {
    if (marked) {
        mMark->Show();
    } else {
        mMark->Hide();
    }
}

void DinoUi2PartsKasekiButton::SetActive(BOOL active) {
}

void DinoUi2PartsKasekiButton::HideMotion() {
    mMotion->Hide();
}

MTask *DinoUi2FieldMenuCursor::CreateMoveTask(MRect from, MRect to, int duration, int ease) {
    DinoSoundPlaySe(0x96, 0);
    return CreateFrameAnime(from, to, duration, ease);
}

void DinoUi2FieldMenuCursor::Execute() {
    if (mUnk10->mState == STATE_RUN) {
        mUnk10->Execute();
    }
    if (mUnk14->mState == STATE_RUN) {
        mUnk14->Execute();
    }
    if (mMode == 2 && mTime++ > 120) {
        mUnk14->Hide();
    }
}

void DinoUi2FieldMenuCursor::SetPosition(MPoint pos) {
    mFrame.MoveTo(pos);
    mUnk10->SetPosition(mFrame.GetTopLeft());
    mUnk14->SetPosition(mFrame.GetTopLeft());
    mTime = 0;
}

MPoint DinoUi2FieldMenuCursor::GetPosition() {
    return mFrame.GetTopLeft();
}

void DinoUi2FieldMenuCursor::SetSize(MPoint size) {
    mFrame.right = mFrame.left + size.x;
    mFrame.bottom = mFrame.top + size.y;
    mUnk10->SetPosition(mFrame.GetTopLeft());
    mUnk14->SetPosition(mFrame.GetTopLeft());
    mTime = 0;
}

MPoint DinoUi2FieldMenuCursor::GetSize() {
    return mFrame.GetSize();
}

void DinoUi2FieldMenuCursor::SetPriority(int priority) {
    mUnk10->SetPriority(priority);
    mUnk14->SetPriority(priority);
}

int DinoUi2FieldMenuCursor::GetPriority() {
    return mUnk10->GetPriority();
}

void DinoUi2FieldMenuCursor::Show() {
    switch (mMode) {
    case 1:
        mUnk10->Show();
        break;
    case 2:
        mUnk14->Show();
        break;
    }
    mHidden = FALSE;
    mTime = 0;
}

void DinoUi2FieldMenuCursor::Hide() {
    switch (mMode) {
    case 1:
        mUnk10->Hide();
        break;
    case 2:
        mUnk14->Hide();
        break;
    }
    mHidden = TRUE;
    mTime = 0;
}

BOOL DinoUi2FieldMenuCursor::IsHidden() {
    return mHidden;
}

u32 DinoUi2FieldMenuCursor::GetPlaneMask() {
    return 0x10;
}

// What the container entry's button shows: a kaseki rock, a fossil (cleaned or not, and whether the creature is
// revived), or one of the two special kinds of rock.
DinoKasekiButtonInfo DinoUi2GetKasekiButtonInfo(const DinoBackupEntry *entry) {
    DinoKasekiButtonInfo info;
    info.type = 0;
    info.level = 0;
    u32 id = entry->id;
    if (id >= 501 && id <= 530 && entry->active == 1) {
        info.icon = (id - 501) / 3 + 19;
        info.color = 0;
        info.type = 5;
        info.level = entry->score;
    } else if (id >= 601 && id <= 630 && entry->active == 1) {
        info.icon = 29;
        info.color = 0;
    } else {
        MResource<DinoKasekiDefs> defs("/etc/kaseki_defs");
        info.icon = defs.Get()->GetDef(entry->id)->icon;
        info.color = entry->unk10_0;
        DinoKasekiRockState rock;
        gDinoBackup->GetKasekiRock(entry->id, (u32 *)&rock);
        if (rock.unk10) {
            DinoCreatureState creature;
            gDinoBackup->GetCreature((entry->id - 1) / 4 + 1, &creature);
            if (creature.parts[0].level != 0) {
                info.type = 1;
            } else {
                info.type = 2;
            }
            info.level = rock.score;
        } else if (entry->unk10_3) {
            info.type = 3;
        } else {
            info.type = 4;
        }
        if (entry->unk10_0 == 1) {
            info.type = 4;
        }
    }
    return info;
}

DinoKasekiButtonInfo DinoUi2GetEmptyKasekiInfo() {
    DinoKasekiButtonInfo info;
    info.icon = 1;
    info.color = 0;
    info.type = 0;
    info.level = 0;
    return info;
}

// NONMATCHING: the stack slots of the items' begin temporaries (the game has one fewer).
void DinoUi2LinkKasekiGrid(MMenu *menu, std::vector<MMenuItem *> &items, MMenuItem *below) {
    for (int i = 0; i < items.size(); i++) {
        if (i + 4 < items.size()) {
            menu->AddLink(items[i], 2, items[i + 4]);
        } else {
            menu->AddLink(items[i], 2, below);
        }
        int col = i % 4;
        if (col != 0) {
            menu->AddLink(items[i], 3, items[i - 1]);
        } else if (i + 4 < items.size()) {
            menu->AddLink(items[i], 3, items[i + 3]);
        } else {
            menu->AddLink(items[i], 3, *(items.end() - 1));
        }
        if (col == 3 || i == items.size() - 1) {
            menu->AddLink(items[i], 1, items[i / 4 * 4]);
        } else {
            menu->AddLink(items[i], 1, items[i + 1]);
        }
    }
}
