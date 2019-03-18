#include <cstdio>
#include <cstdlib>
#include "Sound.h"

Sound::Sound() {
    if (!music.openFromFile(WAVE_PATH)) {
        exit(-1);
    }
    music.setLoop(true);
}

void Sound::waves(bool turn) {
    if (turn)
        music.play();
    else
        music.stop();
}
