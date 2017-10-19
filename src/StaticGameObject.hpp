#ifndef STATIC_GAME_OBJECT_H
#define STATIC_GAME_OBJECT_H

#include "GameObject.hpp"

class StaticGameObject : public GameObject{

public:
    StaticGameObject(const char* path, const char* texture_path, GLuint shaderprog, btVector3 startPos, btQuaternion startRot);
    ~StaticGameObject();

    void draw(const GLuint model_mat_location);
    
    btBvhTriangleMeshShape* load_mesh (const char* file_name, GLuint& vao, int& vert_no);
};
#endif