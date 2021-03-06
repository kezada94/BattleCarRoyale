#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "gl_utils.h"

#include <bullet/btBulletDynamicsCommon.h>

extern double lastTime;
extern double deltaTime;
extern double currentTime;

class ParticleManager{

    typedef struct ParticleSystem{
        GLuint vao;
        double endTime;
        double startTime;
    }ParticleSystem;

private:
    GLuint shader_programme;
    GLuint time_location;
    GLuint view_location;
    GLuint proj_location;
    GLuint tex;
    glm::mat4* view;
    glm::mat4* proj;

    const int MAX_PARTICLES = 200;
    
    //ParticleSystem* bullets[MAX_PARTICLES];

    std::vector<ParticleSystem*> activeSystems;
public:

    ParticleManager();
    ~ParticleManager();

    void genGunshot(btVector3 from, btVector3 to);

    void drawActiveParticles();

    void setView(glm::mat4* vi);
    void setProj(glm::mat4* pro);

    bool load_texture( const char *file_name, GLuint *tex );
        
};

#endif