#include "DynamicGameObject.hpp"

DynamicGameObject::DynamicGameObject(const char* path, GLuint shaderprog, btScalar mass, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll)
    : GameObject(path, shaderprog, mass, startPos, startRot, coll) {
        //this->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);      
    }

DynamicGameObject::DynamicGameObject(const char* path, GLuint shaderprog, btScalar mass, btVector3 startPos, btQuaternion startRot)
    : GameObject(path, shaderprog, mass, startPos, startRot) {
        //this->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);      
    }
        
DynamicGameObject::~DynamicGameObject(){}
    
void DynamicGameObject::draw(const GLuint model_mat_location){
    btTransform trans;
    glm::mat4 model;
    this->getRigidBody()->getMotionState()->getWorldTransform(trans);
        
    model = glm::translate(glm::mat4(), glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    model = glm::rotate(model, trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ() ));
    glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &model[0][0]);
        
    glBindVertexArray(this->getVao());
    glDrawArrays(GL_TRIANGLES, 0, this->getVertNumber()); 
}