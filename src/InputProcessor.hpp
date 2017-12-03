#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.hpp"
#include "Car.hpp"

class InputProcessor{

private:
    GLFWwindow* window;
    Camera* camera;
    Car* playerOne;
    Car* playerTwo;
    const unsigned char* buttons;
    const unsigned char* buttonsTwo;
    const float *axes;
    const float *axesTwo;
    int count;
    int countTwo;
    int axesCount;    
    int axesCountTwo;    

    int lastX;
    int lastY;

    bool firstMouse;
    bool isWireframe;
    bool isReleased;

public:
    InputProcessor(GLFWwindow* win, Camera* cam, Car* player, Car* player2);
    ~InputProcessor();
    void iniciaSonido();
    void processMouse();
    void processScroll();
    void processInput();
};

#endif