// Moves a value linearly from one number to another over a number of frames. Built as ARM code.

#pragma thumb off

#include "sound.h"

Fader::Fader() {
    from = 0;
    to = 0;
    value = 0;
    duration = 0;
    frame = 0;
    done = TRUE;
    paused = FALSE;
}

void Fader::Update() {
    if (done || paused) {
        return;
    }
    frame++;
    value = from + frame * (to - from) / duration;
    if (frame >= duration) {
        done = TRUE;
    }
}

void Fader::Start(int start, int end, int frames) {
    from = start;
    to = end;
    frame = 0;
    paused = FALSE;
    if (frames <= 0) {
        value = end;
        duration = 0;
        done = TRUE;
    } else {
        value = from;
        duration = frames;
        done = FALSE;
    }
}

// Jumps to the end of the fade.
void Fader::Reset() {
    if (done) {
        return;
    }
    value = to;
    done = TRUE;
    paused = FALSE;
}
