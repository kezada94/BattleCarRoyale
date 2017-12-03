#ifndef PATRIOT_H
#define PATRIOT_H

#include "Car.hpp"
#include <cmath>


class Patriot : public Car{
private:
    GLuint wheel_vao, wheel_tex, wheel_texLocation;
    int wheel_vert;

public:
    Patriot(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld* world, GLuint specular_loc);
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
    void despawn(btDiscreteDynamicsWorld* world);

    void draw(GLuint model_mat_location);
};

#endif