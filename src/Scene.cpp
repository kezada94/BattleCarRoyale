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

void Scene::drawAllGameObjects(const GLuint mat_location){
    GameObject* obj;
    for (int i = 0; i < objects->size(); i++){
        obj = this->objects->get(i);
        if (obj != nullptr){
            obj->draw(mat_location);
        }else{
            printf("ES NULLLL");
        }
        
    }
}
void Scene::addGameObject(GameObject* obj){
    if(dynamic_cast<Car*>(obj)){
        cars.push_back(dynamic_cast<Car*>(obj));
    }
    this->objects->addGameObject(obj);
    this->dynamicsWorld->addRigidBody(obj->getRigidBody());
}
void Scene::stepSimulation(float freq, int algo){
    this->dynamicsWorld->stepSimulation(freq, algo);
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

