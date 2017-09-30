#include "StaticGameObject.hpp"

StaticGameObject::StaticGameObject(const char* path, const char* texture_path, GLuint shaderprog, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll)
    : GameObject(path, texture_path, shaderprog, btScalar(0), startPos, startRot, coll) {}

StaticGameObject::StaticGameObject(const char* path, const char* texture_path, GLuint shaderprog, btVector3 startPos, btQuaternion startRot)
    : GameObject(path, texture_path, shaderprog, btScalar(0), startPos, startRot) {}
        
StaticGameObject::~StaticGameObject(){}
    
void StaticGameObject::draw(const GLuint model_mat_location){
    btTransform trans;
    glm::mat4 model;
    this->getRigidBody()->getMotionState()->getWorldTransform(trans);
        
    model = glm::translate(glm::mat4(), glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    model = glm::rotate(model, trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ() ));
    glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &model[0][0]);
    
    glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, this->getTexture());
    glUniform1i (tex_location, 0);
    
    glBindVertexArray(this->getVao());
    glDrawArrays(GL_TRIANGLES, 0, this->getVertNumber()); 
}