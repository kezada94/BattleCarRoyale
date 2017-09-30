#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include "SceneGameObjectList.hpp"
#include "GameObject.hpp"

class Scene{

private:
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    SceneGameObjectLists* objects;

public:
    Scene();
    ~Scene();

    void drawAllGameObjects(const GLuint mat_location);
    void addGameObject(GameObject* obj);
    void stepSimulation(float freq, int algo);

    SceneGameObjectLists* getObjects();
    btDiscreteDynamicsWorld* getDynamicsWorld();
};
#endif