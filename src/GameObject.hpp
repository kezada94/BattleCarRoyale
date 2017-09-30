#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btShapeHull.h>

class GameObject{

private:
    GLuint vao;
    GLuint texture;
    int vertNumber;
    btRigidBody* rigidBody = nullptr;
    
protected:
    bool load_mesh (const char* file_name, btConvexHullShape** col);
    bool load_mesh (const char* file_name);
    bool load_texture (GLuint shaderprog, const char* texture_path);
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