#ifndef DYNAMIC_GAME_OBJECT_H
#define DYNAMIC_GAME_OBJECT_H

#include "GameObject.hpp"

class DynamicGameObject : public GameObject{

public:
    DynamicGameObject(const char* path, const char* texture_path, const char* normal_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, glm::vec3 specular, GLuint specular_loc, btCollisionShape* coll);
    DynamicGameObject(const char* path, const char* texture_path, const char* normal_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, glm::vec3 specular, GLuint specular_loc);
    ~DynamicGameObject();

    void draw(const GLuint model_mat_location);
};
#endif