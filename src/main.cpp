//ASSIMP
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

//OPENGL GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

//GLM
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
class Car;
typedef std::vector<Car*> CarList;

#include "Game.hpp"

int g_gl_width = 1366;
int g_gl_height = 768;
GLFWwindow* g_window = NULL;
float fov = 66.0f;

double lastTime;
double deltaTime;
double currentTime;

int main(){

    init();
    
    Game* juego = new Game();
    juego->init();

    lastTime = glfwGetTime();
    
    juego->doMainLoop();
    glfwTerminate();

    return 0;
}