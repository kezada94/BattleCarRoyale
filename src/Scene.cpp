#include "Scene.hpp"

Scene::Scene(){
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-9.81f,0));
    
    objects = new SceneGameObjectLists();

}
Scene::~Scene(){
    delete broadphase;
    delete collisionConfiguration;
    delete dispatcher;
    delete solver;
    delete dynamicsWorld;
    delete objects;
}

void Scene::drawAllGameObjects(const GLuint mat_location, const GLuint shader){
    glUseProgram(shader);
    for (int i = 0; i < objects->size(); i++){
        objects->get(i)->draw(mat_location);
    }
    for (int i = 0; i < cars.size(); i++){
        if (cars[i]->getIsAlive())
            cars[i]->draw(mat_location);
    }
}
void Scene::updateAllCarsPhysics(){
    for (int i = 0; i < cars.size(); i++){
        cars[i]->updatePhysics();
    }
}

void Scene::addGameObject(GameObject* obj){
    if(dynamic_cast<Car*>(obj)){
        cars.push_back(dynamic_cast<Car*>(obj));
    }else{
        this->objects->addGameObject(obj);
    }
    this->dynamicsWorld->addRigidBody(obj->getRigidBody());
}
void Scene::stepSimulation(float freq, int skips){
    this->dynamicsWorld->stepSimulation(freq, skips);
}
btDiscreteDynamicsWorld* Scene::getDynamicsWorld(){
    return this->dynamicsWorld;
}

void Scene::setPlayer(Car* play){
    this->player = play;
}

Car* Scene::getPlayer(){
    return this->player;
}
CarList Scene::getCars(){
    return this->cars;
}

