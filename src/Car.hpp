#ifndef CAR_H
#define CAR_H

#include "GameObject.hpp"

class Car : public GameObject{

private:
    float health;
    float horsePower;
    float steering;

    float armour;

    bool isAlive;
    
    btRaycastVehicle* car;
public:
    Car(const char* path, btScalar masa, btVector3 startPos, btQuaternion startRot);
    Car(const char* path, btScalar masa, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll);
    ~Car();

    void accelerate();
    void brake();
    void reverse();
    void turnLeft();
    void turnRight();

    void fire();

    void spawn();
    void despawn();
};

#endif