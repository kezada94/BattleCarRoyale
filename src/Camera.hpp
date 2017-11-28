#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <bullet/btBulletDynamicsCommon.h>

#include "GameObject.hpp"
#include "GLDebugDrawer.hpp"
#include "ParticleManager.hpp"
#include "Skybox.hpp"

#define UP_FAR_RATIO 0.5f

enum class CameraModes{
    FIRST_PERSON,
    THIRD_PERSON        
};

class Camera{
private:
    GameObject* target;

    glm::vec3 position; 
    glm::vec3 front;
    glm::vec3 up;

    glm::mat4 projection;
    glm::mat4 view;
    
    float upOffset;
    float farOffset;
    float zoomSpeed;

    int viewLocation;
    int projLocation;
    int width;
    int height;
    float fov;
    float yaw;//angulo de giro en Z
    float pitch;//angulo de giro en Y

    
    bool isViewChanged;

    CameraModes mode;

    GLuint shader_programme;
public:
    ParticleManager* particleManager;    
    GLDebugDrawer* debugDrawer; //LAZY
    Skybox* skybox;
    bool isProjChanged;
    
    Camera();
    ~Camera();

    void update(btDiscreteDynamicsWorld* world);
    void init(GLuint shader_prog, int width, int height, float fov, CameraModes m);
    void getPitchFromQuat(const btQuaternion q1, float& pitch, float& attitude, float& bank);     
    void zoomIn();
    void zoomOut();   

    void setTarget(GameObject* tar);
    void setWidth(int width);
    void setHeight(int height);
    void setFOV(float fov);
    void setMode(CameraModes m);
    void setPosition(glm::vec3 pos);
    void setFront(glm::vec3 fron);
    void setUp(glm::vec3 up);
    void setYaw(float y);
    void setPitch(float p);

    GameObject* getTarget();
    glm::vec3 getPosition();
    glm::vec3 getFront();
    glm::vec3 getUp();
    float getYaw();
    float getPitch();
    CameraModes getMode();
    
};

#endif