// The sound system's entry points (src/sound/sound_api.cpp).
#ifndef SOUND_API_H
#define SOUND_API_H

#include "types.h"

void func_020031d0();
void func_020031dc(void *heapMem, u32 heapSize, const char *arcPath);
void func_020031e8();
void func_020031f4();
void func_02003200(int id, int fadeFrames, int delay);
void func_0200320c(int id, int fadeFrames);
void func_0200321c(int id, int arg);
void func_02003228(int volume);
void func_02003234(int volume);
void func_02003240();
void func_0200324c();
void func_02003258(int fadeFrames);
void func_02003264(int fadeFrames);
void func_02003270(int type, int id, int a, int b);
void func_02003284(int scene);

#endif
