#include "Car.hpp"

Car::Car(const char* path, const char* texture_path, const char* normal_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, glm::vec3 specular, GLuint specular_loc)
    : DynamicGameObject(path, texture_path, normal_path, shaderprog, masa, startPos, startRot, specular, specular_loc) {
        lastShot = 0.f;
    }

Car::Car(const char* path, const char* texture_path, const char* normal_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, glm::vec3 specular, GLuint specular_loc, btCollisionShape* coll) 
    : DynamicGameObject(path, texture_path, normal_path, shaderprog, masa, startPos, startRot, specular, specular_loc, coll) {
        lastShot = 0.f;        
    }

Car::~Car(){}

float Car::getHealth(){
    return this->health;
}
float Car::getArmour(){
    return this->armour;
}
bool Car::getIsAlive(){
    return this->isAlive;
}
bool Car::getTurned(){
    return this->turned;
}
btRaycastVehicle* Car::getCar(){
    return this->car;
}
btDiscreteDynamicsWorld* Car::getWorld(){
    return this->world;
}

void Car::setHealth(float vida){
    this->health = vida;
}
void Car::setArmour(float armor){
    this->armour = armor;
}
void Car::setIsAlive(bool isAliv){
    this->isAlive = isAliv;
}
void Car::setTurned(bool turn){
    this->turned = turn;
}

void Car::setCar(btRaycastVehicle* car){
    this->car = car;
}
void Car::setWorld(btDiscreteDynamicsWorld* world){
    this->world = world;
}

void Car::setSoundManager(SoundManager* sound){
    this->sound = sound;
}

