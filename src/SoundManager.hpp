#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "sound.hpp"

class SoundManager {
private:
    sound* freno;
    sound* ret;
    sound* fondo;
    sound* acel;
public:
    SoundManager();
    ~SoundManager();
    void musicaFondo();
    void reproducir(int valor,ALboolean l, float pit);
    void detener();
};
#endif