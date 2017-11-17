#include "Spotlight.hpp"

Spotlight::Spotlight(glm::vec3 location, glm::vec3 intensity, glm::vec3 direction, float angle) 
    : location(location), intensity(intensity), direction(direction), angle(angle){

}

Spotlight::~Spotlight(){}

glm::vec3 Spotlight::getDirection(){
    return direction;
}
float Spotlight::getAngle(){
    return angle;
}
glm::vec3 Spotlight::getLocation(){
    return location;
}
glm::vec3 Spotlight::getIntensity(){
    return intensity;
}

void Spotlight::setLocation(glm::vec3 location){
    this->location = location;
}
void Spotlight::setIntensity(glm::vec3 intensity){
    this->intensity = intensity;
}
void Spotlight::setDirection(glm::vec3 direction){
    this->direction = direction;
}
void Spotlight::setAngle(float angle){
    this->angle = angle;
}