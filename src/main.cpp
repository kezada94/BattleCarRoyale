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

#define GL_LOG_FILE "log/gl.log"
#define VERTEX_SHADER_FILE "res/shaders/vert.glsl"
#define FRAGMENT_SHADER_FILE "res/shaders/frag.glsl"

int g_gl_width = 1366;
int g_gl_height = 768;
int isWireframe = false;
bool isReleased = false;

GLFWwindow* g_window = NULL;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float fov = 66.0f;

int main(){
	restart_gl_log ();
	start_gl ();
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // set counter-clock-wise vertex order to mean the front
    glClearColor (0.2, 0.2, 0.2, 1.0); // grey background to help spot mistakes
    
	#ifdef __linux__
	    glViewport (0, 0, g_gl_width, g_gl_height);
	#endif

    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);

    /*-------------------------------CREATE SHADERS-------------------------------*/
	GLuint shader_programme = create_programme_from_files (
		VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
	);

    Scene* level = new Scene();
    Camera* camera = new Camera();

	glUseProgram (shader_programme);
    
    int model_mat_location  = glGetUniformLocation (shader_programme, "model");
    
    camera->init(shader_programme, g_gl_width, g_gl_height, fov);

    DynamicGameObject *truck = new DynamicGameObject("res/meshes/cilindro.obj", shader_programme, btScalar(1), btVector3(0, 5, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    StaticGameObject *piso = new StaticGameObject("res/meshes/floor.obj", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    //GameObject *moster = new GameObject("res/meshes/monster_truck4.obj", btScalar(20), btVector3(0, 10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    DynamicGameObject *player = new DynamicGameObject("res/meshes/car_chanta.obj", shader_programme, btScalar(10), btVector3(0, -6, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    StaticGameObject *buen = new StaticGameObject("res/meshes/cube.obj", shader_programme, btVector3(10, 0, 10), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    
    
    level->addGameObject(truck);
    level->addGameObject(piso);
    level->addGameObject(player);
    level->addGameObject(buen);


    camera->setTarget(player);
    // activate shader
            
    // render loop
    // -----------
    while (!glfwWindowShouldClose(g_window)){
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (glfwGetKey(g_window, GLFW_KEY_UP) == GLFW_PRESS){
            player->getRigidBody()->activate();
            player->getRigidBody()->applyForce(btVector3(0, 0, -60),btVector3(0,0,1));
        } 
        if (glfwGetKey(g_window, GLFW_KEY_LEFT) == GLFW_PRESS){
            player->getRigidBody()->activate();
            player->getRigidBody()->applyTorque(btVector3(0, 30, 0));
        } 
        if (glfwGetKey(g_window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            player->getRigidBody()->activate();
            player->getRigidBody()->applyTorque(btVector3(0, -30, 0));
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
        processInput(g_window);
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        level->drawAllGameObjects(model_mat_location);
        camera->update();
        
		level->stepSimulation(1.f / 60.f, 10);
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (isReleased){
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            if (!isWireframe){
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Draw only lines no fill
                isWireframe = true;
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Draw only lines no fill
                isWireframe = false;
            }   
            isReleased = false;  
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE){
        isReleased = true;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	#ifdef __linux__
	glViewport (0, 0, width, height);
	#endif
}