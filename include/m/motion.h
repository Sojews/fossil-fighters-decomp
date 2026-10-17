// Motions: animated OBJ graphics loaded from a motion file, shown as a view.
#ifndef M_MOTION_H
#define M_MOTION_H

#include <string>
#include <vector>
#include "m/view.h"
#include "m/smart_oam.h"
#include "m/archive.h"
#include "m/file.h"

class MSmartOam;
class MPalBlock;

// A motion file's header.
struct MMotionHeader {
    u32 unk00;
    u32 unk04;          // 0x04
    u32 animeCount;     // 0x08
    u32 unk0C;
    u32 cellCount;      // 0x10
    u32 cellOffset;     // 0x14: 16-byte cell definitions
};

// A loaded motion file and the files it uses, shared by every motion showing it.
class MMotionData {
public:
    MMotionData(FSFileID id, int unk);
    ~MMotionData();

    static MMotionData *Get(FSFileID id, int unk);
    void AddRef();
    void Release();

    MArchivePtr mArchive;               // 0x00
    MArchiveFile mFile;                 // 0x04
    std::vector<MArchiveFile> mUnk10;   // 0x10
    std::vector<MArchiveFile> mUnk1C;   // 0x1C
    std::vector<MArchiveFile> mUnk28;   // 0x28
    int mRefCount;                      // 0x34
};

// A shared, reference-counted handle on a loaded motion file.
class MMotionFile {
public:
    MMotionFile(std::string path, int unk);
    MMotionFile(const MMotionFile &other);
    ~MMotionFile();
    MMotionFile &operator=(const MMotionFile &other);

    u32 GetUnk04();
    u32 GetAnimeCount();
    u32 GetCellCount();
    void *GetCell(u32 index);
    void *GetAnime(int anime, MArchiveFile *file);

    MMotionData *mData; // 0x00
};

// One cell (OBJ) of a motion.
struct MMotionCell {
    u8 visible;         // 0x00
    u32 unk04;          // 0x04
    int objMode;        // 0x08
    u32 entry;          // 0x0C: index into the motion's smart OAM entries
    int offsetY;        // 0x10
    MPoint unk14;       // 0x14
};

struct MMotionAnimeState {
    MMotionAnimeState() {}
    MMotionAnimeState(u32 a, u32 b) {
        unk00 = a;
        unk04 = b;
    }

    u32 unk00;
    u32 unk04;
};

class MMotion : public MView {
public:
    MMotion(const std::string &path, MSmartOam *oam, int unk, int priority);
    virtual ~MMotion();
    virtual void Execute();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetSize(MPoint size);
    virtual MPoint GetSize();
    virtual MRect GetFrame();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();
    virtual void Play(int anime, int flags);
    virtual void SetAnchor(int anchor);
    virtual int GetAnchor();
    virtual void Unk44();
    virtual void Unk48();

    MSmartOamUnit *GetUnit();
    MPalBlock *GetPalette(u32 index);
    BOOL IsPlaying();
    int GetAnime();
    MTask *CreateWaitStopTask();
    MTask *CreatePlayTransfer(int anime, int flags);
    void SetFlip(u8 hflip, u8 vflip);
    void *GetAnimeData(int anime);
    void ResetAnime();
    void Advance(u32 frames);
    void Init(int fromEnd);
    void FreeAffine(u32 index);

    MDisplay *mDisplay;         // 0x10
    MMotionFile mFile;          // 0x14
    MSmartOam *mOam;            // 0x18
    MSmartOamUnit *mUnit;       // 0x1C
    MSmartOamEntry *mEntries;   // 0x20
    u32 mCellCount;             // 0x24
    u8 mPlaying;                // 0x28
    u8 mHidden;                 // 0x29
    u8 mUnk2A;                  // 0x2A
    u8 mUnk2B;                  // 0x2B
    u8 mUnk2C;                  // 0x2C
    u32 mFramesLeft;            // 0x30: in the current key
    u32 mFlags;                 // 0x34
    int mUnk38;                 // 0x38
    int mAnime;                 // 0x3C
    u32 mUnk40;                 // 0x40
    u32 mFrame;                 // 0x44: frames played
    u32 mTime;                  // 0x48: 24.8, the fraction of a frame carried over
    u32 mSpeed;                 // 0x4C: 24.8 frames per frame
    u32 mUnk50;                 // 0x50
    MPoint mPos;                // 0x54
    MMotionCell *mCells;        // 0x5C
    u32 mUnk60;                 // 0x60
    MMotionAnimeState *mUnk64;  // 0x64
    MPalBlock **mPalettes;      // 0x68
    void *mUnk6C;               // 0x6C
    MAfnBlock **mAffines;       // 0x70
    void *mUnk74;               // 0x74
    struct Override {
        MArchiveFile *unk00;
        MArchiveFile *unk04;
        u8 file[0xC];       // an MArchiveFile
    } *mUnk78;              // 0x78
};

class MMotionWaitStopTask : public MTask {
public:
    MMotionWaitStopTask(MMotion *motion) { mMotion = motion; }
    virtual ~MMotionWaitStopTask() {}
    virtual void Execute();

    MMotion *mMotion;   // 0x10
};

class MMotionPlayTransfer : public MTransfer {
public:
    MMotionPlayTransfer(MMotion *motion, int anime, int flags) {
        mMotion = motion;
        mAnime = anime;
        mFlags = flags;
    }
    virtual ~MMotionPlayTransfer() {}
    virtual void Transfer();

    MMotion *mMotion;   // 0x10
    int mAnime;         // 0x14
    int mFlags;         // 0x18
};

#endif
