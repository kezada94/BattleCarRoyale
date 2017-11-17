#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include "Light.hpp"

class Spotlight{

private:
    glm::vec3 location;
    glm::vec3 intensity;
    glm::vec3 direction;
    float angle;

public:

    Spotlight(glm::vec3 location, glm::vec3 intensity, glm::vec3 direction, float angle);
    ~Spotlight();

    glm::vec3 getDirection();
    float getAngle();
    glm::vec3 getLocation();
    glm::vec3 getIntensity();

    void setLocation(glm::vec3 location);
    void setIntensity(glm::vec3 intensity);
    void setDirection(glm::vec3 direction);
    void setAngle(float angle);
};

#endif