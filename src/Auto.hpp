#ifndef AUTO_H
#define AUTO_H

#include "Car.hpp"
#include <cmath>
#include "SoundManager.hpp"
#include "ParticleManager.hpp"
#include "Spotlight.hpp"
#include <glm/gtx/rotate_vector.hpp>

class Auto : public Car
{
  private:
    GLuint wheel_vao, wheel_tex, wheel_texLocation;
    int wheel_vert;

  public:
    Auto(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld *world, GLuint specular_loc);
    ~Auto();

    void initialize(btDiscreteDynamicsWorld *world);
    void updatePhysics();
    void soundInit();
    void accelerate();
    void brake();
    void reverse();
    void turnLeft();
    void turnRight();

    void fire();

    void spawn();
    void despawn(btDiscreteDynamicsWorld *world);

    void draw(GLuint model_mat_location);
};

#endif