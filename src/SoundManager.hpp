#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <AL/al.h>
#include <AL/alc.h>
#include "sound.hpp"
#include "AL/alut.h"

class SoundManager {
private:
    sound* freno;
    sound* fondo;
    sound* acel;
public:
    SoundManager();
    ~SoundManager();
    void musicaFondo();
    void reproducir(int valor,ALboolean l, float pit);
};
#endif