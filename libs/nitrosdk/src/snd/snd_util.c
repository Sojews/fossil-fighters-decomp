// NitroSDK SND: decibels to channel volume and shift.

#include "nitro/snd.h"

#define SND_VOL_DB_MIN -723

extern const u8 VolumeTable[-SND_VOL_DB_MIN + 1];

int SND_CalcChannelVolume(int dB) {
    u8 volume;
    int shift;
    if (dB < SND_VOL_DB_MIN) {
        dB = SND_VOL_DB_MIN;
    } else if (dB > 0) {
        dB = 0;
    }
    volume = VolumeTable[dB - SND_VOL_DB_MIN];
    if (dB < -240) {
        shift = 3;
    } else if (dB < -120) {
        shift = 2;
    } else if (dB < -60) {
        shift = 1;
    } else {
        shift = 0;
    }
    return (u16) ((shift << 8) | volume);
}
