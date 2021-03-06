#include "SoundManager.hpp"
SoundManager::SoundManager(){

    alutInit(0, NULL);
	alGetError();
    freno = new sound((const char*)"audio/frenar.wav");
    acel = new sound((const char*)"audio/acel.wav");
    fondo = new sound((const char*)"audio/musica.wav");
    disparo = new sound((const char*)"audio/disparo.wav");
}

void SoundManager::reproducir(int valor,ALboolean l,float pit){
    acel->pit(pit);
    if(valor==1){
        //if(freno->get_source_state() != AL_PLAYING){   
            freno->play(l,0.3,1);
        //}
    } 
    else if(valor==2){
        if(acel->get_source_state() != AL_PLAYING && freno->get_source_state() != AL_PLAYING){
            acel->play(l,0.3,1);
        }
    }
    else if(valor==3){  
            disparo->play(l,0.3,1);
    }
}
void SoundManager::musicaFondo(){
    fondo->play(AL_TRUE,0.2,1);
}