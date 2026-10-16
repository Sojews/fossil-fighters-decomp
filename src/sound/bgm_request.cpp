// BGM player driven by requests: Request() during the frame, Update() starts it (or fades out if none came).
// Built as ARM code.

#pragma thumb off

#include "sound.h"

void Unk2FF4::Request(int no, int index, int fadeFrames) {
    reqNo = no;
    reqIndex = index;
    reqFadeFrames = fadeFrames;
    requested = TRUE;
}

void Unk2FF4::Update() {
    if (requested) {
        PlayIfNew(reqNo, reqIndex, reqFadeFrames);
        requested = FALSE;
    } else {
        Stop(10);
    }
}
