#include "Car.hpp"

Car::Car(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot)
    : DynamicGameObject(path, texture_path, shaderprog, masa, startPos, startRot) {}

Car::Car(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll) 
    : DynamicGameObject(path, texture_path, shaderprog, masa, startPos, startRot, coll) {}

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
btRaycastVehicle* Car::getCar(){
    return this->car;
}

void Car::setHealth(float vida){
    this->health = vida;
}
void Car::setArmour(float armor){
    this->armour = armor;
}
void Car::setIsAlive(float isAliv){
    this->isAlive = isAliv;
}
void Car::setCar(btRaycastVehicle* car){
    this->car = car;
}

