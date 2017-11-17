#ifndef LIGHT_H
#define LIGHT_H

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
//Most accurate: a point light
class Light{
private:
    glm::vec3 location;
    glm::vec3 intensity;

public:

    Light();
    ~Light();

    glm::vec3 getLocation();
    glm::vec3 getIntensity();

    void setLocation(glm::vec3 location);
    void setIntensity(glm::vec3 intensity);
};

#endif