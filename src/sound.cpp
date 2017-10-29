#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>

#include "gl_utils.h"

#include <AL/al.h>
#include <AL/alc.h>
#include "sound.hpp"
#include "AL/alut.h"

#include <iostream>

using namespace std;

sound::sound(const char* filename)
{
	this->filename = (char*)filename;
	alutInit(0, NULL);
	alGetError();

	alGenBuffers(1, &buffer);
	ALbyte* cha = (ALbyte*)filename;
	alutLoadWAVFile(cha, &format, &data, &size, &freq);
    alBufferData(buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);	
	
	alGenSources(1, &this->source);
	
	alSourcef(source,AL_PITCH,1.0f);
    alSourcef(source,AL_GAIN,1.0f);
    alSourcei(source,AL_BUFFER,buffer);
    alSourcei(source,AL_LOOPING,AL_FALSE);
}

void sound::set_listener(const glm::vec3& o)
{
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	alListener3f(AL_POSITION, o[0], o[1], o[2]);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listenerOri);
}

void sound::definir_fuente()
{
	alGenSources((ALuint)1, &source);

	alSourcef(this->source, AL_GAIN, 1);
	alSource3f(this->source, AL_POSITION, 0, 0, 0);
	alSource3f(this->source, AL_VELOCITY, 5, 0, 0);
	
}

void sound::play(ALboolean l,float n,float pit)
{
	alSourcePlay(this->source);
	alSourcei(this->source, AL_LOOPING, l);
}

void sound::pit(float pit){
	alSourcef(this->source,AL_PITCH,pit);
} 

void sound::stop()
{
	alGenBuffers(1, &buffer);
	alBufferData(this->buffer, this->format, this->data, this->size, this->freq);

	alSourcei(this->source, AL_BUFFER, this->buffer);
	alSourceStop(this->source);
}


ALint sound::get_source_state(){
	alGetSourcei(this->source, AL_SOURCE_STATE, &this->source_state);
	return this->source_state;
}

ALuint sound::get_source(){
	return this->source;
}

ALuint sound::get_num_obj(){
	return this->num_obj;
}

void sound::set_num_obj(ALint n){
	this->num_obj = n;
}

void sound::limpia_contexto_recursos(){
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	this->device = alcGetContextsDevice(this->context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(this->context);
	alcCloseDevice(this->device);
}
