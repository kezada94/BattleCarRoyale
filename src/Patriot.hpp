#ifndef PATRIOT_H
#define PATRIOT_H

#include "Car.hpp"
#include <cmath>


class Patriot : public Car{
private:
    GLuint wheel_vao;
    int wheel_vert;
public:
    Patriot(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld* world);
    Patriot(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btCollisionShape* coll, btDiscreteDynamicsWorld* world);
    ~Patriot();

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