#include "DynamicGameObject.hpp"

DynamicGameObject::DynamicGameObject(const char* path, const char* texture_path, const char* normal_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, glm::vec3 specular, GLuint specular_loc, btCollisionShape* coll)
    : GameObject(path, texture_path, normal_path, shaderprog, masa, startPos, startRot, specular, specular_loc, coll) {
        //this->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);      
    }

DynamicGameObject::DynamicGameObject(const char* path, const char* texture_path, const char* normal_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, glm::vec3 specular, GLuint specular_loc)
    : GameObject(path, texture_path, normal_path, shaderprog, masa, startPos, startRot, specular, specular_loc) {
        //this->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);      
    }
        
DynamicGameObject::~DynamicGameObject(){}
    
void DynamicGameObject::draw(const GLuint model_mat_location){
    btTransform trans;
    glm::mat4 model;
    this->getRigidBody()->getMotionState()->getWorldTransform(trans);
        
    model = glm::translate(glm::mat4(), glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    model = glm::rotate(model, trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ() ));
    btVector3 escala = getRigidBody()->getCollisionShape()->getLocalScaling();
    model = glm::scale(model, glm::vec3(escala.getX(), escala.getY(), escala.getZ()));
    glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &model[0][0]);
    glUniform3fv(specular_loc, 1, &specular[0]);

    glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, this->texture);

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, this->normalMap);
    
    glBindVertexArray(this->getVao());
    glDrawArrays(GL_TRIANGLES, 0, this->getVertNumber()); 
}