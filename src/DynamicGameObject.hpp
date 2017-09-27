#ifndef DYNAMIC_GAME_OBJECT_H
#define DYNAMIC_GAME_OBJECT_H

#include "GameObject.hpp"

class DynamicGameObject : public GameObject{

public:
    DynamicGameObject(const char* path, GLuint shaderprog, btScalar mass, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll);
    DynamicGameObject(const char* path, GLuint shaderprog, btScalar mass, btVector3 startPos, btQuaternion startRot);
    ~DynamicGameObject();

    void draw(const GLuint model_mat_location);
};
#endif