#ifndef STATIC_GAME_OBJECT_H
#define STATIC_GAME_OBJECT_H

#include "game_object.hpp"

class StaticGameObject : public GameObject{

public:
    StaticGameObject(const char* path, btVector3 startPos, btQuaternion startRot, btCollisionObject* coll);
    StaticGameObject(const char* path, btVector3 startPos, btQuaternion startRot);
    ~StaticGameObject();
};
#endif