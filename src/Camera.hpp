#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include "game_object.hpp"

class Camera{

private:
    GameObject* target;

    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 front;

    int view_location;
    int proj_location;
    int width;
    int height;
    float fov;

public:
    Camera();
    ~Camera();

    void update();
    void init(GLuint shader_prog, int width, int height, float fov);

    void setTarget();
    GameObject* getTarget();
};

#endif