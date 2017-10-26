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
    Car* player;
    const unsigned char* buttons;
    const float *axes;
    int count;
    int axesCount;    

    int lastX;
    int lastY;

    bool firstMouse;
    bool isWireframe;
    bool isReleased;

public:
    InputProcessor(GLFWwindow* win, Camera* cam, Car* player);
    ~InputProcessor();
    void iniciaSonido();
    void processMouse();
    void processScroll();
    void processInput();
};

#endif