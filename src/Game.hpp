#ifndef GAME_H
#define GAME_H
//EVERYTHING ELSE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string>
#include "gl_utils.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "ParticleManager.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "StaticGameObject.hpp"
#include "DynamicGameObject.hpp"
#include "Kombi.hpp"
#include "Patriot.hpp"
#include "MonsterTruck.hpp"
#include "InputProcessor.hpp"
#include "ILogicProcessor.hpp"
#include "GLDebugDrawer.hpp"
#include "SoundManager.hpp"
#include <algorithm>
#include <ostream>
#include <sstream>
#include <vector>

#define GL_LOG_FILE "log/gl.log"
#define VERTEX_SHADER_FILE "res/shaders/vert.glsl"
#define FRAGMENT_SHADER_FILE "res/shaders/frag.glsl"

extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow* g_window;

extern double lastTime;
extern double deltaTime;
extern double currentTime;

extern float fov;

//typedef std::vector<Car*> CarList;


class Game : public ILogicProcessor{
private:
    Scene* level;
    Camera* camera;
    InputProcessor* inputProcessor;
    SoundManager* soundManager;
    ParticleManager* particleManager;

    GLuint shader_programme;
    int model_mat_location;
    int enemiesCount;    

public:
    Game();
    ~Game();

    void init();
    void doMainLoop();

    void win();
    void loose();

    void checkStatus();
};

#endif