// The program's entry point: runs the game's main task on the looper.

#include "m/looper.h"
#include "game/dino_main.h"

extern "C" void main() {
    gLooper->Run(new DinoMain());
}
