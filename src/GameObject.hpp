#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Tools.hpp"

class GameObject{

private:
    GLuint vao;
    GLuint texture;
    int vertNumber;
    btRigidBody* rigidBody;
protected:
    bool load_mesh (const char* file_name, GLuint& vao, int& vert_no, btConvexHullShape** col);
    bool load_mesh (const char* file_name, GLuint& vao, int& vert_no);    
    bool load_texture (GLuint shaderprog, const char* texture_path, GLuint& texture, GLuint tex_location);
    
public:
    GLuint tex_location;

    GameObject(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot);
    GameObject(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll);
    
    ~GameObject();

    virtual void draw(const GLuint model_mat_location) = 0;
    
    //getter
    GLuint getVao();
    GLuint getTexture();
    int getVertNumber();  
    btRigidBody* getRigidBody();

    //setter
    void setRigidBody(btRigidBody* rigid);
    void setVertNumber(int vertno);
    void setVao(GLuint vao);
    void setTexture(GLuint tex);
};

#endif