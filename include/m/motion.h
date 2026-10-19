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
struct MPalUnit;
struct MChrBlock;

// A motion file's list of files: how many, the offset of their indices in the directory and the offset of the
// directory's path.
struct MMotionFileList {
    u32 count;
    u32 table;
    u32 path;
};

// A motion file's header.
struct MMotionHeader {
    u32 unk00;
    u32 unk04;          // 0x04
    u32 animeCount;     // 0x08
    u32 unk0C;
    u32 cellCount;      // 0x10
    u32 cellOffset;     // 0x14: 16-byte cell definitions
    MMotionFileList cellFiles;  // 0x18: the animes, one per cell
    MMotionFileList pals;       // 0x24
    MMotionFileList chrs;       // 0x30
};

// A cell's graphics, as the motion file lists them.
struct MMotionCellDef {
    u32 pal16;      // 0x00
    u32 chr16;      // 0x04: tiles, 16 colors
    u32 pal256;     // 0x08
    u32 chr256;     // 0x0C: tiles, 256 colors
};

// The header of a chr or palette file a motion uses; the data follows.
struct MMotionGfxHeader {
    s32 extPalette : 8;     // palette files: in the extended palettes
    u32 tiles : 8;          // chr files
    s32 mode : 8;           // chr files: 0: 16 colors, 1: 256 colors
    u32 : 8;
};

// A motion file's archive and the motion file itself (locked).
class MMotionArchive {
public:
    MMotionArchive(FSFileID id) : mArchive(id) {
        mFile = mArchive.GetFile(0);
        mFile.Lock();
        mFile.GetData();
    }

    MArchivePtr mArchive;                   // 0x00
    MArchiveFile mFile;                     // 0x04: the motion file
};

// A loaded motion file and the files it uses, shared by every motion showing it.
class MMotionData : public MMotionArchive {
public:
    MMotionData(FSFileID id, int unk);
    ~MMotionData();

    static MMotionData *Get(FSFileID id, int unk);
    void AddRef();
    void Release();
    void LockFiles(int which);
    MChrBlock **AcquireChrs(MDisplay *display);
    void ReleaseChrs(MDisplay *display);
    MPalUnit **AcquirePalettes(MDisplay *display);
    void ReleasePalettes(MDisplay *display);

    std::vector<MArchiveFile> mAnimeFiles;   // 0x10
    std::vector<MArchiveFile> mChrFiles;    // 0x1C
    std::vector<MArchiveFile> mPalFiles;    // 0x28
    int mRefCount;                          // 0x34
    int mPalRefs[2];                        // 0x38: per display (main, sub)
    int mChrRefs[2];                        // 0x40
    MPalUnit **mPals[2];                   // 0x48
    MChrBlock **mChrs[2];                   // 0x50
    MMutex mMutex;                          // 0x58
    std::vector<u32> mUnk70;                // 0x70
    std::vector<u32> mUnk7C;                // 0x7C
};

// A shared, reference-counted handle on a loaded motion file.
class MMotionFile {
public:
    MMotionFile(std::string path, int unk);
    MMotionFile(const MMotionFile &other);
    ~MMotionFile();
    MMotionFile &operator=(const MMotionFile &other);

    u32 GetUnk04();
    u32 GetUnk18();
    MPalUnit **AcquirePalettes(MDisplay *display);
    void ReleasePalettes(MDisplay *display);
    MChrBlock **AcquireChrs(MDisplay *display);
    void ReleaseChrs(MDisplay *display);
    u32 GetAnimeCount();
    u32 GetCellCount();
    const MMotionCellDef *GetCell(u32 index);
    void *GetAnime(int anime, MArchiveFile *file);
    void *GetPalette(u32 index, MArchiveFile *file);

    MMotionData *mData; // 0x00
};

// One cell (OBJ) of a motion.
struct MMotionCell {
    MMotionCell();

    u8 visible;         // 0x00
    u32 unk04;          // 0x04
    int objMode;        // 0x08
    u32 entry;          // 0x0C: index into the motion's smart OAM entries
    int offsetY;        // 0x10
    MPoint unk14;       // 0x14
};

struct MMotionAnimeState {
    MMotionAnimeState() {}
    void Set(u32 a, u32 b) {
        unk00 = a;
        unk04 = b;
    }

    u32 unk00;
    u32 unk04;
};

class MMotion : public MView {
public:
    MMotion(const MMotionFile &file, MSmartOam *oam, int flags, int fromEnd);
    MMotion(const std::string &path, MSmartOam *oam, int flags, int fromEnd);
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
    virtual void Unk44(int arg);
    virtual void Unk48(int arg);

    MSmartOamUnit *GetUnit();
    MPalUnit *GetPalette(u32 index);
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
    void SetOverride(BOOL off);
    void SetPosition(int x, int y);
    MRect GetCellRect(u32 cell);
    MSmartOamCell *GetCellEntry(int cell) { return &mEntries[mCells[cell].entry]; }
    void SetCellOffset(int cell, int x, int y);
    void SetCellOffsetY(int cell, int y);
    void Command(const char *data, int length);
    void SetAnimeState(int index, int a, int b);
    void ShowCell(int cell);
    void HideCell(int cell);
    void SetCellAttr(int cell, int attr);
    void SetCellRect(int cell, MRect rect);
    void SetCellChr(int cell, u32 chr);
    void SetCellPalette(int cell, u32 pal);

    MDisplay *mDisplay;         // 0x10
    MMotionFile mFile;          // 0x14
    MSmartOam *mOam;            // 0x18
    MSmartOamUnit *mUnit;       // 0x1C
    MSmartOamCell *mEntries;   // 0x20
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
    MPalUnit **mPalettes;      // 0x68
    MChrBlock **mChrs;          // 0x6C
    MAfnBlock **mAffines;       // 0x70
    const u16 *mScript;         // 0x74: the current anime's commands
    struct Override {
        MArchiveFile *animes;   // 0x00: one per cell
        MArchiveFile *palettes; // 0x04: one per cell
        MArchiveFile file;      // 0x08
    } *mUnk78;                  // 0x78
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
