#include "Camera.hpp"

/*
    Thanks to Martin John Baker for this useful piece of code.
        http://www.euclideanspace.com/
*/
void Camera::getPitchFromQuat(const btQuaternion q1, float& pitch) {
    float sqw = q1.w()*q1.w();
    float sqx = q1.x()*q1.x();
    float sqy = q1.y()*q1.y();
    float sqz = q1.z()*q1.z();
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = q1.x()*q1.y() + q1.z()*q1.w();
	if (test > 0.499*unit) { // singularity at north pole
		pitch = 2 * atan2(q1.x(),q1.w());
		return;
	}
	if (test < -0.499*unit) { // singularity at south pole
		pitch = -2 * atan2(q1.x(),q1.w());
		return;
	}
    pitch = atan2(2*q1.y()*q1.w()-2*q1.x()*q1.z() , sqx - sqy - sqz + sqw);
}

Camera::Camera() : isViewChanged(true), isProjChanged(true), target(nullptr), upOffset(15.0f), up(glm::vec3(0, 1, 0)), farOffset(20.0f), zoomSpeed(0.5f){}

void Camera::init(GLuint shaderProg, int width, int height, float fov){

    setWidth(width);
    setHeight(height);
    setFOV(fov);

    this->viewLocation = glGetUniformLocation (shaderProg, "view");    
    this->projLocation = glGetUniformLocation (shaderProg, "proj");

    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 300.0f);
    glUniformMatrix4fv (projLocation, 1, GL_FALSE, &projection[0][0]);    
    glm::mat4 view = glm::lookAt(glm::vec3(), glm::vec3(), up);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}
void Camera::update(){

    if(target == nullptr){
        printf("Error, target hasn't been set");
        return;
    }

    if(isProjChanged){
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
        glUniformMatrix4fv (projLocation, 1, GL_FALSE, &projection[0][0]);
        isProjChanged = false;
    }

    float angle;    
    btTransform trans;

    target->getRigidBody()->getMotionState()->getWorldTransform(trans);
    getPitchFromQuat(trans.getRotation(), angle);
    
    glm::vec3 targetPos = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

    float camX = sin(angle) * farOffset;
    float camZ = cos(angle) * farOffset;
    glm::mat4 view = glm::lookAt(glm::vec3(camX, upOffset ,camZ) + targetPos, targetPos, up);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

}
void Camera::zoomIn(){
    if (farOffset<10){
        return;
    }
    this->farOffset = this->farOffset - zoomSpeed;
    this->upOffset = UP_FAR_RATIO*farOffset;
}
void Camera::zoomOut(){
    if (farOffset>40){
        return;
    }
    this->farOffset = this->farOffset + zoomSpeed;
    this->upOffset = UP_FAR_RATIO*farOffset;
}


void Camera::setTarget(GameObject* tar){
    this->target = tar;
}
void Camera::setWidth(int width){
    this->width = width;
    this->isProjChanged = true;
}
void Camera::setHeight(int height){
    this->height = height;
    this->isProjChanged = true;
}
void Camera::setFOV(float fov){
    this->fov = fov;
    this->isProjChanged = true;    
}
GameObject* Camera::getTarget(){
    return this->target;
}
