//ASSIMP
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

//OPENGL GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//GLM
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

//EVERYTHING ELSE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string>
#include "gl_utils.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "Scene.hpp"
#include "Camera.hpp"
#include "StaticGameObject.hpp"
#include "DynamicGameObject.hpp"
#include "Kombi.hpp"
#include "InputProcessor.hpp"

#define GL_LOG_FILE "log/gl.log"
#define VERTEX_SHADER_FILE "res/shaders/vert.glsl"
#define FRAGMENT_SHADER_FILE "res/shaders/frag.glsl"

int g_gl_width = 1366;
int g_gl_height = 768;


GLFWwindow* g_window = NULL;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float fov = 66.0f;

int main(){
	init();

    /*-------------------------------CREATE SHADERS-------------------------------*/
	GLuint shader_programme = create_programme_from_files (
		VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
	);

    Scene* level = new Scene();
    Camera* camera = new Camera();

	glUseProgram (shader_programme);
    
    int model_mat_location  = glGetUniformLocation (shader_programme, "model");
    
    camera->init(shader_programme, g_gl_width, g_gl_height, fov, CameraModes::THIRD_PERSON);

    DynamicGameObject *player = new DynamicGameObject("res/meshes/kombi3.obj", "res/textures/kombi3.jpg", shader_programme, btScalar(1), btVector3(0, 5, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    DynamicGameObject *rueda = new DynamicGameObject("res/textures/rueda/rueda.obj", "res/textures/rueda/wheel.png", shader_programme, btScalar(2), btVector3(0, 10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    StaticGameObject *piso = new StaticGameObject("res/meshes/floor.obj", "res/textures/default.jpg", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    //GameObject *moster = new GameObject("res/meshes/monster_truck4.obj", btScalar(20), btVector3(0, 10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    //DynamicGameObject *player = new DynamicGameObject("res/meshes/car_chanta.obj", shader_programme, btScalar(10), btVector3(0, -6, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    //StaticGameObject *buen = new StaticGameObject("res/meshes/cube.obj", shader_programme, btVector3(10, 0, 10), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    Kombi* kombi = new Kombi(btVector3(0, 5, 0), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    
    level->addGameObject(kombi);
    level->addGameObject(piso);
    //level->addGameObject(player);
    //level->addGameObject(rueda);
    //level->addGameObject(buen);


    camera->setTarget(kombi);

    InputProcessor* processor = new InputProcessor(g_window, camera, kombi);
    // activate shader
            
    // render loop
    // -----------
    while (!glfwWindowShouldClose(g_window)){
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        level->stepSimulation(1.f / 60.f, 10);
        processor->processInput();

        if (glfwGetKey(g_window, GLFW_KEY_UP) == GLFW_PRESS){
            //player->getRigidBody()->activate();
            kombi->accelerate();
        } 
        if (glfwGetKey(g_window, GLFW_KEY_LEFT) == GLFW_PRESS){
            kombi->turnLeft();
        } 
        if (glfwGetKey(g_window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            kombi->turnRight();
        }
        if (glfwGetKey(g_window, GLFW_KEY_U) == GLFW_PRESS){
            camera->zoomIn();
        } 
        if (glfwGetKey(g_window, GLFW_KEY_J) == GLFW_PRESS){
            camera->zoomOut();
        }    
        
        _update_fps_counter(g_window);
        // input
        // -----
        // render
        // ------
        glClearColor(0.0f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        level->drawAllGameObjects(model_mat_location);
        camera->update();
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    return 0;
}