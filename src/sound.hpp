#ifndef SOUND_H
#define SOUND_H

#include <AL/al.h>
#include <AL/alc.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class sound{
    private:
        ALint source_state;
        ALvoid *data;
        ALsizei size, freq;
        ALenum format;
        ALuint buffer, source;
        ALboolean loop;
        char* filename;
        const ALCchar *defaultDeviceName ;
        ALCcontext *context;
        ALCdevice *device;
        ALint num_obj;

    public:
        sound(const char *filename);

        // functions
        bool TEST_ERROR (const char* message);
        void play(ALboolean l,float n,float pit);
        void stop();
        void pit(float pit);
        void limpia_contexto_recursos();
        void set_listener(const glm::vec3& o);
        void definir_fuente();

        // gets
        ALint get_source_state();
        ALuint get_source();
        ALuint get_num_obj();

        //sets
        void set_num_obj(ALint n);


};

#endif
