// NitroSystem sound: sequence archives (several sequences sharing one data block).

#include "nnsys/snd/sndarc_player.h"

typedef struct {
    NNSSndSeqData header;
    u32 count;
    NNSSndSeqArcSeqInfo info[1];
} NNSSndSeqArc;

const NNSSndSeqArcSeqInfo *NNSi_SndSeqArcGetSeqInfo(const void *seqArc, int index) {
    const NNSSndSeqArc *arc = seqArc;
    const NNSSndSeqArcSeqInfo *info;
    if (index < 0) {
        return NULL;
    }
    if (index >= arc->count) {
        return NULL;
    }
    info = &arc->info[index];
    if (info->offset == 0xffffffff) {
        return NULL;
    }
    return info;
}
