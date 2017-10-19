#ifndef KOMBI_H
#define KOMBI_H

#include "Car.hpp"
#include <cmath>


class Kombi : public Car{
private:
    GLuint wheel_vao;
    int wheel_vert;
public:
    Kombi(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld* world);
    Kombi(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btCollisionShape* coll, btDiscreteDynamicsWorld* world);
    ~Kombi();

    void initialize(btDiscreteDynamicsWorld* world);
    void updatePhysics();
    
    void accelerate();
    void brake();
    void reverse();
    void turnLeft();
    void turnRight();

    void fire();

    void spawn();
    void despawn();

    void draw(GLuint model_mat_location);
};

#endif