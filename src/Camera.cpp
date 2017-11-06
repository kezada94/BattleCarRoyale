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
//btbvtrianglemeshshape
Camera::Camera() 
    : isViewChanged(true), 
    isProjChanged(true), 
    target(nullptr), 
    upOffset(15.0f), 
    up(glm::vec3(0, 1, 0)), 
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    position(glm::vec3(0.0f, 0.0f, 3.0f)),
    farOffset(30.0f), 
    zoomSpeed(0.5f){}

Camera::~Camera(){}

void Camera::init(GLuint shaderProg, int width, int height, float fov, CameraModes m){

    setWidth(width);
    setHeight(height);
    setFOV(fov);
    setMode(m);

    this->shader_programme = shaderProg;
    this->viewLocation = glGetUniformLocation (shaderProg, "view");    
    this->projLocation = glGetUniformLocation (shaderProg, "proj");

    glUseProgram(shader_programme);
    projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 10000.0f);
    glUniformMatrix4fv (projLocation, 1, GL_FALSE, &projection[0][0]);    
    debugDrawer->setProj(&projection);    
    particleManager->setProj(&projection);
    skybox->setProj(&projection);

    view = glm::lookAt(glm::vec3(), glm::vec3(), up);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
    debugDrawer->setView(&view);   
    particleManager->setView(&view);
    skybox->setView(&view);
    
}


void Camera::update(){
    glUseProgram(shader_programme);    
    if(isProjChanged){
        projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 10000.0f);
        glUniformMatrix4fv (projLocation, 1, GL_FALSE, &projection[0][0]);   
        isProjChanged = false;
    }
    float angle;    
    float camX;
    float camZ;
    btTransform trans;    
    glm::vec3 targetPos;
    
    switch(mode){
        case CameraModes::THIRD_PERSON:
            if(target == nullptr){
                printf("Error, target hasn't been set");
                return;
            }
            target->getRigidBody()->getMotionState()->getWorldTransform(trans);
            getPitchFromQuat(trans.getRotation(), angle);
            
            targetPos = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
            
            camX = -sin(angle) * farOffset;
            camZ = -cos(angle) * farOffset;

            view = glm::lookAt(glm::vec3(camX, upOffset, camZ) + targetPos, glm::vec3(targetPos.x, targetPos.y+10, targetPos.z), up);
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);    
            break;
        case CameraModes::FIRST_PERSON:
            view = glm::lookAt(position, position + front, up);
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
            break;
    }    
}
void Camera::zoomIn(){
    if (farOffset<15){
        return;
    }
    this->farOffset = this->farOffset - zoomSpeed;
    this->upOffset = UP_FAR_RATIO*farOffset;
}
void Camera::zoomOut(){
    if (farOffset>50){
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
void Camera::setMode(CameraModes m){
    this->mode = m;
}
void Camera::setPosition(glm::vec3 pos){
    this->position = pos;
}
void Camera::setFront(glm::vec3 fron){
    this->front = fron;
}
void Camera::setUp(glm::vec3 up){
    this->up = up;
}
void Camera::setYaw(float y){
    this->yaw = y;
}
void Camera::setPitch(float p){
    this->pitch = p;
}

GameObject* Camera::getTarget(){
    return this->target;
}
glm::vec3 Camera::getPosition(){
    return this->position;
}
glm::vec3 Camera::getFront(){
    return this->front;
}
glm::vec3 Camera::getUp(){
    return this->up;
}
float Camera::getYaw(){
    return this->yaw;
}
float Camera::getPitch(){
    return this->pitch;
}
CameraModes Camera::getMode(){
    return this->mode;
}

