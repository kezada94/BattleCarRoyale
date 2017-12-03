#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <vector>
#include "SceneGameObjectList.hpp"
#include "GameObject.hpp"
#include "Light.hpp"
#include "Car.hpp"
#include "Spotlight.hpp"

typedef std::vector<Car*> CarList;
typedef std::vector<Light*> LightList; //usual O(n^2) check, this could be done with an octree but due to the simplicity of the scene, it can be simplified

//Level class: first one
class Scene{

private:
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    SceneGameObjectLists* objects;

    CarList cars;
    LightList lights;
    Car* player;

    GLuint posLoc;
    GLuint dirLoc;
    GLuint pwrLoc;
    GLuint lightAngle;    
    GLuint numberLoc;
    GLuint depthProj;
    GLuint depthView;


    GLuint shadow_shader;
    GLuint depthTexture;

    GLuint shadowMapID;
    GLuint depthProjID;
    GLuint depthViewID;
    GLuint depthModelID;
    GLuint FramebufferName;


public:
    Scene();
    ~Scene();

    glm::mat4 depthProjectionMatrix;
    glm::mat4 depthViewMatrix;

    void init(GLuint shader);
    void renderShadow();

    void drawAllGameObjects(const GLuint mat_location, const GLuint shader, int mPlayer);
    void updateAllCarsPhysics();
    void addGameObject(GameObject* obj);
    void addLight(Light* lght);
    void stepSimulation(float freq, int skips);

    SceneGameObjectLists* getObjects();
    btDiscreteDynamicsWorld* getDynamicsWorld();

    void setPlayer(Car* play);

    Car* getPlayer();
    CarList getCars();
};
#endif