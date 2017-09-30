#include "Car.hpp"

Car::Car(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot)
    : DynamicGameObject(path, texture_path, shaderprog, masa, startPos, startRot) {}

Car::Car(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll) 
    : DynamicGameObject(path, texture_path, shaderprog, masa, startPos, startRot, coll) {}

Car::~Car(){}

