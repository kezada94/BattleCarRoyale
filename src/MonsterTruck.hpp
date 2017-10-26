#ifndef MONSTERTRUCK_H
#define MONSTERTRUCK_H

#include "Car.hpp"
#include <cmath>
#include "SoundManager.hpp"
#include "ParticleManager.hpp"
#include z<glm/gtx/rotate_vector.hpp>


class MonsterTruck : public Car{
private:
    GLuint wheel_vao, wheel_tex, wheel_texLocation;
    int wheel_vert;
    SoundManager* soundManager;
    

public:
    ParticleManager* particleManager;

    MonsterTruck(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld* world);
    MonsterTruck(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btCollisionShape* coll, btDiscreteDynamicsWorld* world);
    ~MonsterTruck();

    void initialize(btDiscreteDynamicsWorld* world);
    void updatePhysics();
    void soundInit();
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