#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <bullet/btBulletDynamicsCommon.h>

#include "GameObject.hpp"

#define UP_FAR_RATIO 0.75f

class Camera{

private:
    GameObject* target;

    glm::vec3 up;

    float upOffset;
    float farOffset;
    float zoomSpeed;

    int viewLocation;
    int projLocation;
    int width;
    int height;
    float fov;

    bool isProjChanged;
    bool isViewChanged;

public:
    Camera();
    ~Camera();

    void update();
    void init(GLuint shader_prog, int width, int height, float fov);
    void getPitchFromQuat(const btQuaternion q1, float& pitch);     
    void zoomIn();
    void zoomOut();   

    void setTarget(GameObject* tar);
    void setWidth(int width);
    void setHeight(int height);
    void setFOV(float fov);

    GameObject* getTarget();
};

#endif