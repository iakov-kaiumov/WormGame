#ifndef LIQUIDFUN_SOUND_H
#define LIQUIDFUN_SOUND_H

#include <SFML/Audio.hpp>

class Sound {
public:
    Sound();
    void waves(bool turn);
private:
    const char *WAVE_PATH = "sound/waves.wav";
    sf::Music music;
};


#endif //LIQUIDFUN_SOUND_H
