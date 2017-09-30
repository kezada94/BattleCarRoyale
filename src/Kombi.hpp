#ifndef KOMBI_H
#define KOMBI_H

#include "Car.hpp"

class Kombi : public Car{

public:
    Kombi(btVector3 startPos, btQuaternion startRot);
    Kombi(btVector3 startPos, btQuaternion startRot, btCollisionShape* coll);
    ~Kombi();

    void initialize(btScalar hp, btScalar brake, btScalar sterring);

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