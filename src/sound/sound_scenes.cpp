// Sound groups to load for each scene, and the table SetSoundScene() picks from.
// Built as ARM code.

#pragma thumb off

#include "sound.h"

int SoundLoadGroups(int group, ...);

int SoundSceneNone() {
    return -1;
}

int SoundScene01() {
    return SoundLoadGroups(0, 63, 64, 62, 80, -1);
}

int SoundScene02() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 9, -1);
}

int SoundScene03() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 9, -1);
}

int SoundScene04() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 9, -1);
}

int SoundScene05() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 5, -1);
}

int SoundScene06() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 7, -1);
}

int SoundScene07() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 82, -1);
}

int SoundScene08() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 3, -1);
}

int SoundScene09() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 9, -1);
}

int SoundScene10() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 2, -1);
}

int SoundScene11() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 4, -1);
}

int SoundScene12() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 1, -1);
}

int SoundScene13() {
    return SoundLoadGroups(0, 63, 64, 83, 60, -1);
}

int SoundScene14() {
    return SoundLoadGroups(0, 63, 64, 65, 25, -1);
}

int SoundScene15() {
    return SoundLoadGroups(0, 63, 64, 61, 26, -1);
}

int SoundScene16() {
    return SoundLoadGroups(0, 63, 64, 61, 26, -1);
}

int SoundScene17() {
    return SoundLoadGroups(0, 63, 64, 61, 27, -1);
}

int SoundScene18() {
    return SoundLoadGroups(0, 63, 64, 61, 28, -1);
}

int SoundScene19() {
    return SoundLoadGroups(0, 63, 64, 61, 29, -1);
}

int SoundScene20() {
    return SoundLoadGroups(0, 63, 64, 61, 30, -1);
}

int SoundScene21() {
    return SoundLoadGroups(0, 63, 64, 61, 31, -1);
}

int SoundScene22() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 68, 14, -1);
}

int SoundScene23() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 69, 15, -1);
}

int SoundScene24() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 70, 16, -1);
}

int SoundScene25() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 71, 17, -1);
}

int SoundScene26() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 72, 18, -1);
}

int SoundScene27() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 73, 19, -1);
}

int SoundScene28() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 74, 20, -1);
}

int SoundScene29() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 75, 21, -1);
}

int SoundScene30() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 77, 22, -1);
}

int SoundScene31() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 76, 23, -1);
}

int SoundScene32() {
    return SoundLoadGroups(0, 63, 64, 62, 79, 48, -1);
}

int SoundScene33() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 74, 20, -1);
}

int SoundScene34() {
    return SoundLoadGroups(0, 63, 64, 65, 67, 71, 17, -1);
}

int SoundScene35() {
    return SoundLoadGroups(0, 63, 64, 62, 65, 66, 5, -1);
}

int SoundScene36() {
    return SoundLoadGroups(81, 63, 64, 62, 65, 81, 59, -1);
}

int SoundScene37() {
    return SoundLoadGroups(81, 63, 64, 62, 65, 81, 58, -1);
}

int SoundScene38() {
    return SoundLoadGroups(0, 63, 64, 61, 32, -1);
}

int SoundScene39() {
    return SoundLoadGroups(0, 63, 64, 61, 57, -1);
}

int SoundScene40() {
    return SoundLoadGroups(0, 63, 64, 65, 10, -1);
}

int SoundScene41() {
    return SoundLoadGroups(0, 63, 64, 65, 12, -1);
}

int SoundScene42() {
    return SoundLoadGroups(81, 63, 64, 62, 65, 81, 58, -1);
}

static int sCurrentScene = -1;

static int (*sSceneFuncs[43])() = {
    SoundSceneNone,
    SoundScene01,
    SoundScene02,
    SoundScene03,
    SoundScene04,
    SoundScene05,
    SoundScene06,
    SoundScene07,
    SoundScene08,
    SoundScene09,
    SoundScene10,
    SoundScene11,
    SoundScene12,
    SoundScene13,
    SoundScene14,
    SoundScene15,
    SoundScene16,
    SoundScene17,
    SoundScene18,
    SoundScene19,
    SoundScene20,
    SoundScene21,
    SoundScene22,
    SoundScene23,
    SoundScene24,
    SoundScene25,
    SoundScene26,
    SoundScene27,
    SoundScene28,
    SoundScene29,
    SoundScene30,
    SoundScene31,
    SoundScene32,
    SoundScene33,
    SoundScene34,
    SoundScene35,
    SoundScene36,
    SoundScene37,
    SoundScene38,
    SoundScene39,
    SoundScene40,
    SoundScene41,
    SoundScene42,
};

// Loads the sound groups for a scene. Returns the free sound heap size, or -1.
int SetSoundScene(int scene) {
    int result;

    // Off by one in the original: scene 43 reads past the end of the table.
    if (scene < 0 || scene > 43) {
        return -1;
    }
    result = sSceneFuncs[scene]();
    sCurrentScene = scene;
    return result;
}
