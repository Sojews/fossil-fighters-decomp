// The revival information screen (ov003).
#ifndef GAME_DINO_UI_RV_INFORMATION_H
#define GAME_DINO_UI_RV_INFORMATION_H

#include "m/picture.h"
#include "m/resource.h"
#include "m/task.h"

class DinoUiRvInformation3DModel;
class DinoUiFieldMenuNumber3;
class MView;

// A creature's details as the information screen shows them (passed by value).
// (As a creature's state: copied member by member.)
struct DinoCreatureInfoState {
    u32 index : 8;      // 0x00
    u32 unk00_8 : 24;
    struct {
        u32 level : 12;
        u32 rest : 20;
    } parts[4];         // 0x04: head, body, arms, legs
    u32 unk14;          // 0x14
};

struct DinoCreatureInfo {
    union {
        u32 unk00[6];
        DinoCreatureInfoState state;
    };
};

// (The data's layout isn't known yet.)
struct DinoUiRvInformationData;
struct DinoKasekiDefs;
class MSmartOam;

class DinoUiRvInformation : public MProcess {
public:
    DinoUiRvInformation(u8 hasTenPictures);
    virtual ~DinoUiRvInformation();
    virtual void Idle();

    void SetPage(u8 page);
    void SetUnk1B1(u8 value);
    void SetUnk1AC(u32 value);
    MTask *CreateWaitModelTask();   // waits until the 3D model's tasks finish
    // Shows the page's second picture (the last three pictures, hidden at first).
    void CreateSubPicture();
    void Unk02156800();
    void Unk021571bc();
    void Unk02156ee8(DinoCreatureInfo info);
    // (Page 0) The support effects' texts: for the creature's own side and the other side, each of the four
    // values signed with a fixed-width sign.
    void ShowSupportEffects();

    MPictureFile *mPictures[10];        // 0x74
    MTextPicture *mPagePicture;         // 0x9C
    u8 unkA0[4];
    MTextPicture *mSubPicture;          // 0xA4
    MSmartOam *mOam;                    // 0xA8
    MView *mViewsAC[3];                 // 0xAC
    u32 unkB8;
    DinoUiFieldMenuNumber3 *mNumbersBC[6];  // 0xBC: (page 0)
    MView *mViewD4;                     // 0xD4
    MView *mViewD8;                     // 0xD8
    MView *mViewsDC[4];                 // 0xDC
    MView *mViewEC;                     // 0xEC
    MView *mViewsF0[5];                 // 0xF0: (pages 1 and 2, by row)
    MView *mViews104[5];                // 0x104
    MView *mViews118[5];                // 0x118
    DinoUiFieldMenuNumber3 *mNumbers12C[5]; // 0x12C
    DinoUiFieldMenuNumber3 *mNumbers140[5]; // 0x140
    u8 unk154[0x164 - 0x154];
    MTask *mUnk164;                     // 0x164: (page 3) the revive status
    MTaskSet *mPictureTasks;            // 0x168
    MTaskSet *mUnk16C;                  // 0x16C
    u8 unk170[0x174 - 0x170];
    DinoUiRvInformation3DModel *mModel; // 0x174
    u8 mPage;                           // 0x178
    DinoCreatureInfo mInfo;             // 0x17C
    u8 mHasTenPictures;                 // 0x194: else 7
    u8 mUnk195;                         // 0x195
    u32 mUnk198;                        // 0x198
    MResource<DinoKasekiDefs> mUnk19C;  // 0x19C
    u32 mUnk1AC;                        // 0x1AC
    u8 mUnk1B0;                         // 0x1B0: set once the page's parts are hidden
    u8 mUnk1B1;                         // 0x1B1
    MFx32 mUnk1B4;                      // 0x1B4
};

#endif
