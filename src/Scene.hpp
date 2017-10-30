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
#include "Car.hpp"
typedef std::vector<Car*> CarList;

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
    Car* player;


public:
    Scene();
    ~Scene();

    void drawAllGameObjects(const GLuint mat_location, const GLuint shader);
    void updateAllCarsPhysics();
    void addGameObject(GameObject* obj);
    void stepSimulation(float freq, int skips);

    SceneGameObjectLists* getObjects();
    btDiscreteDynamicsWorld* getDynamicsWorld();

    void setPlayer(Car* play);

    Car* getPlayer();
    CarList getCars();
};
#endif