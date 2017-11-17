#include "Light.hpp"

Light::Light(){}

Light::~Light(){}

glm::vec3 Light::getLocation(){
    return location;
}
glm::vec3 Light::getIntensity(){
    return intensity;
}

void Light::setLocation(glm::vec3 location){
    this->location = location;
}


void Light::setIntensity(glm::vec3 intensity){
    this->intensity = intensity;
}
