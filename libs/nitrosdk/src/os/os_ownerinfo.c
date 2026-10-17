// NitroSDK OS: the console owner's settings, as the firmware left them in main memory.

#include "nitro/mi.h"
#include "nitro/os.h"

typedef struct {
    u8 month;
    u8 day;
} OSBirthday;

typedef struct {
    u8 language;
    u8 favoriteColor;
    OSBirthday birthday;
    u16 nickName[10 + 1];
    u16 nickNameLength;
    u16 comment[26 + 1];
    u16 commentLength;
} OSOwnerInfo;

typedef struct {
    u8 version;
    u8 padding;
    u8 favoriteColor : 4;
    u8 unk2 : 4;
    OSBirthday birthday;
    u8 padding5;
    u16 nickName[10];
    u8 nickNameLength;
    u8 padding1B;
    u16 comment[26];
    u8 commentLength;
    u8 padding51[0x13];
    u16 language : 3;
    u16 unk64 : 13;
} NVRAMOwnerConfig;

#define NVRAM_OWNER ((NVRAMOwnerConfig *) 0x027ffc80)
#define MAC_ADDRESS ((const u8 *) 0x027ffcf4)

void OS_GetMacAddress(u8 *macAddress) {
    MI_CpuCopy8(MAC_ADDRESS, macAddress, 6);
}

void OS_GetOwnerInfo(OSOwnerInfo *info) {
    NVRAMOwnerConfig *src = NVRAM_OWNER;
    info->language = (u8) src->language;
    info->favoriteColor = (u8) src->favoriteColor;
    info->birthday.month = src->birthday.month;
    info->birthday.day = src->birthday.day;
    info->nickNameLength = (u16) src->nickNameLength;
    info->commentLength = (u16) src->commentLength;
    MIi_CpuCopy16(src->nickName, info->nickName, 10 * sizeof(u16));
    MIi_CpuCopy16(src->comment, info->comment, 26 * sizeof(u16));
    info->nickName[10] = 0;
    info->comment[26] = 0;
}
