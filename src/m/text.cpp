// MText and its transfers.

#include "m/text.h"
#include "m/looper.h"

static const MTextSize sTextSizes[4] = {
    {1, 1, 1},
    {2, 1, 2},
    {1, 2, 2},
    {2, 2, 4},
};

static const MTextBlockLinks sTextBlockLinks[4][4] = {
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{1, 0}, {0, 1}, {1, 0}, {0, 1}},
    {{0, 1}, {1, 0}, {0, 1}, {1, 0}},
    {{1, 2}, {0, 3}, {3, 0}, {2, 1}},
};

// Tile map entries 0, 1, 2, ...: the source for a rectangle of consecutive tiles.
static const u16 sTileSequence[1024] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
    256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271,
    272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
    288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303,
    304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319,
    320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
    336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351,
    352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367,
    368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383,
    384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399,
    400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415,
    416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431,
    432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447,
    448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463,
    464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479,
    480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495,
    496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511,
    512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527,
    528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543,
    544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559,
    560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575,
    576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591,
    592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607,
    608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623,
    624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639,
    640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654, 655,
    656, 657, 658, 659, 660, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 671,
    672, 673, 674, 675, 676, 677, 678, 679, 680, 681, 682, 683, 684, 685, 686, 687,
    688, 689, 690, 691, 692, 693, 694, 695, 696, 697, 698, 699, 700, 701, 702, 703,
    704, 705, 706, 707, 708, 709, 710, 711, 712, 713, 714, 715, 716, 717, 718, 719,
    720, 721, 722, 723, 724, 725, 726, 727, 728, 729, 730, 731, 732, 733, 734, 735,
    736, 737, 738, 739, 740, 741, 742, 743, 744, 745, 746, 747, 748, 749, 750, 751,
    752, 753, 754, 755, 756, 757, 758, 759, 760, 761, 762, 763, 764, 765, 766, 767,
    768, 769, 770, 771, 772, 773, 774, 775, 776, 777, 778, 779, 780, 781, 782, 783,
    784, 785, 786, 787, 788, 789, 790, 791, 792, 793, 794, 795, 796, 797, 798, 799,
    800, 801, 802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815,
    816, 817, 818, 819, 820, 821, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831,
    832, 833, 834, 835, 836, 837, 838, 839, 840, 841, 842, 843, 844, 845, 846, 847,
    848, 849, 850, 851, 852, 853, 854, 855, 856, 857, 858, 859, 860, 861, 862, 863,
    864, 865, 866, 867, 868, 869, 870, 871, 872, 873, 874, 875, 876, 877, 878, 879,
    880, 881, 882, 883, 884, 885, 886, 887, 888, 889, 890, 891, 892, 893, 894, 895,
    896, 897, 898, 899, 900, 901, 902, 903, 904, 905, 906, 907, 908, 909, 910, 911,
    912, 913, 914, 915, 916, 917, 918, 919, 920, 921, 922, 923, 924, 925, 926, 927,
    928, 929, 930, 931, 932, 933, 934, 935, 936, 937, 938, 939, 940, 941, 942, 943,
    944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956, 957, 958, 959,
    960, 961, 962, 963, 964, 965, 966, 967, 968, 969, 970, 971, 972, 973, 974, 975,
    976, 977, 978, 979, 980, 981, 982, 983, 984, 985, 986, 987, 988, 989, 990, 991,
    992, 993, 994, 995, 996, 997, 998, 999, 1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007,
    1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023,
};

u32 MText::GetVramSize(int size) {
    return sTextSizes[size].blocks << 11;
}

MText::MText(MDisplay *display, u32 screenBase, u32 base, int size, int bg)
    : MScreen(display, screenBase, base, sTextSizes[size].blocks << 11, size, bg) {
    mSize = size;
    mWidth = sTextSizes[size].width;
    mHeight = sTextSizes[size].height;
    mBlocks = sTextSizes[size].blocks;
    mUnk30 = 0;
}

void MText::SetUnk30(u32 value, u32 extPaletteSlot) {
    mBgCntBits.extPaletteSlot = extPaletteSlot;
    mUnk30 = value;
}

int MText::GetRightBlock(int size, int block) {
    return sTextBlockLinks[size][block].right;
}

int MText::GetDownBlock(int size, int block) {
    return sTextBlockLinks[size][block].down;
}

void MText::Update() {
    MScreen::Update();
    MPoint offset = mOffset;
    *(volatile u32 *)mDisplay->GetRegs18(mBg) = (u16)offset.x | ((u16)offset.y << 16);
}

// NONMATCHING: these three differ from the game in register and stack slot allocation only.
void MText::Transfer(const u16 *src, MPoint srcSize, MRect srcRect, MPoint dst, int tile, int palette, bool hflip,
                     bool vflip) {
    gLooper->AddTransferTask(new MTextTransfer(this, dst, src, srcSize, srcRect, tile, palette, hflip, vflip));
}

void MText::Transfer(const u16 *src, MPoint srcSize, MPoint dst, int tile, int palette, bool hflip, bool vflip) {
    gLooper->AddTransferTask(new MTextTransfer(this, dst, src, srcSize, tile, palette, hflip, vflip));
}

void MText::TransferSequence(MPoint srcSize, MRect srcRect, MPoint dst, int tile, int palette, bool hflip,
                             bool vflip) {
    gLooper->AddTransferTask(new MTextTransfer(this, dst, sTileSequence, srcSize, srcRect, tile, palette, hflip,
                                               vflip));
}

void MText::Fill(MRect rect, int tile, int palette, bool hflip, bool vflip) {
    gLooper->AddTransferTask(
        new MTextFill(this, rect, tile, palette, hflip, vflip));
}

MTextTransfer::~MTextTransfer() {
}

// Copies h rows of w tile entries from (x, y) of the source into the screen block from (dx, dy), adding attr and
// flipping with flip (the rows go bottom up when vertically flipped, each row right to left when horizontally).
static inline void MTextCopyTiles(const u16 *src, int srcWidth, int x, int y, int w, int h, u32 base, int block, int dx,
                                  int dy, u16 attr, u16 flip) {
    const u16 *s;
    if (!(flip & 0x800)) {
        if (!(flip & 0x400)) {
            u16 *d = (u16 *) base + ((dy + block * 32) * 32 + dx);
            s = src + srcWidth * y + x;
            int n;
            int rows = h;
            while (rows != 0) {
                n = w;
                while (n != 0) {
                    n--;
                    *d = (*s++ + attr) ^ flip;
                    d++;
                }
                rows--;
                d += 32 - w;
                s += srcWidth - w;
            }
        } else {
            u16 *d = (u16 *) base + ((dy + block * 32) * 32 + (dx + w - 1));
            s = src + srcWidth * y + x;
            int n;
            int rows = h;
            while (rows != 0) {
                n = w;
                while (n != 0) {
                    n--;
                    *d-- = (*s++ + attr) ^ flip;
                }
                rows--;
                d += w + 32;
                s += srcWidth - w;
            }
        }
    } else {
        if (!(flip & 0x400)) {
            u16 *d = (u16 *) base + (((dy + h - 1) + block * 32) * 32 + dx);
            s = src + srcWidth * y + x;
            int n;
            int rows = h;
            while (rows != 0) {
                n = w;
                while (n != 0) {
                    n--;
                    *d++ = (*s++ + attr) ^ flip;
                }
                rows--;
                d += -(w + 32);
                s += srcWidth - w;
            }
        } else {
            u16 *d = (u16 *) base + (((dy + h - 1) + block * 32) * 32 + (dx + w - 1));
            s = src + srcWidth * y + x;
            int n;
            int rows = h;
            while (rows != 0) {
                n = w;
                while (n != 0) {
                    n--;
                    *d-- = (*s++ + attr) ^ flip;
                }
                rows--;
                d += -(32 - w);
                s += srcWidth - w;
            }
        }
    }
}

// Copies a w x h block of tile entries from (x, y) of the source to (dx, dy) of a 32x32 screen block, with the flips;
// a copy crossing the block's bottom edge goes on in the block below (the source's last rows first when flipped).
extern "C" void func_02056750(const u16 *src, int srcWidth, int srcHeight, int x, int y, int w, int h, u32 base,
                              int size, int block, int dx, int dy, u16 attr, u16 flip) {
    if (dy + h < 32) {
        MTextCopyTiles(src, srcWidth, x, y, w, h, base, block, dx, dy, attr, flip);
    } else if (!(flip & 0x800)) {
        MTextCopyTiles(src, srcWidth, x, y, w, 32 - dy, base, block, dx, dy, attr, flip);
        y += 32;
        MTextCopyTiles(src, srcWidth, x, y - dy, w, h - (32 - dy), base, MText::GetDownBlock(size, block), dx, 0, attr,
                       flip);
    } else {
        MTextCopyTiles(src, srcWidth, x, y + h - (32 - dy), w, 32 - dy, base, block, dx, dy, attr, flip);
        MTextCopyTiles(src, srcWidth, x, y, w, h - (32 - dy), base, MText::GetDownBlock(size, block), dx, 0, attr, flip);
    }
}

// Picks the screen block the destination is in; a copy crossing the block's right edge is done in two parts (in the
// other order when flipped).
void MTextTransfer::Transfer() {
    int block = 0;
    int size;
    if (mDst.x & 0x20) {
        block = MText::GetRightBlock(mDstSize, block);
    }
    if (mDst.y & 0x20) {
        block = MText::GetDownBlock(mDstSize, block);
    }
    mDst.x &= 0x1F;
    mDst.y &= 0x1F;
    const u16 *src;
    int srcWidth;
    int srcHeight;
    int x;
    int y;
    u32 base;
    int dy;
    u16 attr;
    int h;
    int w;
    dy = mDst.y;
    u16 flip = mFlip;
    attr = mAttr;
    base = mDstBase;
    int dx = mDst.x;
    h = mSrcRect.GetHeight();
    size = mDstSize;
    w = mSrcRect.GetWidth();
    y = mSrcRect.top;
    x = mSrcRect.left;
    srcHeight = mSrcSize.y;
    srcWidth = mSrcSize.x;
    src = mSrc;
    if (dx + w < 32) {
        func_02056750(src, srcWidth, srcHeight, x, y, w, h, base, size, block, dx, dy, attr, flip);
    } else if (!(flip & 0x400)) {
        int first = 32 - dx;
        func_02056750(src, srcWidth, srcHeight, x, y, first, h, base, size, block, dx, dy, attr, flip);
        func_02056750(src, srcWidth, srcHeight, x + 32 - dx, y, w - first, h, base, size,
                      MText::GetRightBlock(size, block), 0, dy, attr, flip);
    } else {
        int first = 32 - dx;
        func_02056750(src, srcWidth, srcHeight, x + w - first, y, first, h, base, size, block, dx, dy, attr, flip);
        func_02056750(src, srcWidth, srcHeight, x, y, w - first, h, base, size, MText::GetRightBlock(size, block), 0,
                      dy, attr, flip);
    }
}

MTextFill::~MTextFill() {
}

// A text screen's entry: 32x32 entries per screen block.
#define MTEXT_ENTRY(block, x, y) ((u16 *)(base + (((block) * 32 + (y)) * 32 + (x)) * 2))

// Fills a w x h block of entries from p.
#define MTEXT_FILL_BLOCK(start, w, h) \
    { \
        int i; \
        u16 *p = start; \
        for (int j = h; j != 0; j--, p += 32 - (w)) { \
            for (i = w; i != 0; i--) { \
                *p++ = entry; \
            } \
        } \
    }

// Picks the screen block the rectangle starts in; a fill crossing a block's right or bottom edge carries on in the
// next block.
void MTextFill::Transfer() {
    s32 block;
    s32 base;
    int size;
    int dx;
    int dy;
    dx = mRect.left;
    dy = mRect.top;
    int w = mRect.GetWidth();
    int h = mRect.GetHeight();
    block = 0;
    if (dx & 0x20) {
        block = MText::GetRightBlock(mDstSize, block);
    }
    if (dy & 0x20) {
        block = MText::GetDownBlock(mDstSize, block);
    }
    dx &= 0x1F;
    dy &= 0x1F;
    u16 entry = mEntry;
    size = mDstSize;
    base = mDstBase;
    if (dx + w < 32) {
        if (dy + h < 32) {
            MTEXT_FILL_BLOCK(MTEXT_ENTRY(block, dx, dy), w, h);
        } else {
            MTEXT_FILL_BLOCK(MTEXT_ENTRY(block, dx, dy), w, 32 - dy);
            MTEXT_FILL_BLOCK(MTEXT_ENTRY(MText::GetDownBlock(size, block), dx, 0), w, h - (32 - dy));
        }
    } else {
        if (dy + h < 32) {
            MTEXT_FILL_BLOCK(MTEXT_ENTRY(block, dx, dy), 32 - dx, h);
        } else {
            MTEXT_FILL_BLOCK(MTEXT_ENTRY(block, dx, dy), 32 - dx, 32 - dy);
            MTEXT_FILL_BLOCK(MTEXT_ENTRY(MText::GetDownBlock(size, block), dx, 0), 32 - dx, h - (32 - dy));
        }
        s32 right;
        right = MText::GetRightBlock(size, block);
        if (h + dy < 32) {
            MTEXT_FILL_BLOCK(MTEXT_ENTRY(right, 0, dy), w - (32 - dx), h);
        } else {
            MTEXT_FILL_BLOCK(MTEXT_ENTRY(right, 0, dy), w - (32 - dx), 32 - dy);
            MTEXT_FILL_BLOCK(MTEXT_ENTRY(MText::GetDownBlock(size, right), 0, 0), w - (32 - dx), h - (32 - dy));
        }
    }
}
