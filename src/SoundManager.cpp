#include "SoundManager.hpp"
SoundManager::SoundManager(){
    freno = new sound((const char*)"audio/frenar.wav");
    acel = new sound((const char*)"audio/frenar.wav");
    ret = new sound((const char*)"audio/frenar.wav");
    fondo = new sound((const char*)"audio/musica.wav");
}

void SoundManager::reproducir(int valor){
    if(valor==1){
        if(freno->get_source_state() != AL_PLAYING)
        {
            freno->play(AL_FALSE,0.4);
        }
    }
    else if(valor==2){
        acel->play(AL_FALSE,0.4);
    }
    else if(valor==3){
        ret->play(AL_FALSE,0.4);
    }
}
void SoundManager::musicaFondo(){
      fondo->play(AL_TRUE,0.3);
}