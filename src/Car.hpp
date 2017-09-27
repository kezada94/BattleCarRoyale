#ifndef CAR_H
#define CAR_H

#include "DynamicGameObject.hpp"

class Car : public DynamicGameObject{

private:
    float health;
    float armour;
    bool isAlive;
    
    btRaycastVehicle* car;
public:
    Car(const char* path, btScalar masa, btVector3 startPos, btQuaternion startRot);
    Car(const char* path, btScalar masa, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll);
    ~Car();

    virtual void initialize(btScalar hp, btScalar brake, btScalar sterring) = 0;

    void accelerate();
    void brake();
    void reverse();
    void turnLeft();
    void turnRight();

    virtual void fire() = 0;

    virtual void spawn() = 0;
    virtual void despawn() = 0;

    virtual void draw() = 0;
};

#endif