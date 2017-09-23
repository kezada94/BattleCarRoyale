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
    int vertNumber;
    btRigidBody* rigidBody = nullptr;
    
protected:
    bool load_mesh (const char* file_name, btConvexHullShape** col);
    bool load_mesh (const char* file_name);
    
public:
    GameObject(const char* path, btScalar masa, btVector3 startPos, btQuaternion startRot);
    GameObject(const char* path, btScalar masa, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll);
    ~GameObject();

    void draw(const GLuint model_mat_location);
    
    //getter
    GLuint getVao();
    int getVertNumber();  
    btRigidBody* getRigidBody();

    //setter
    void setRigidBody(btRigidBody* rigid);
    void setVertNumber(int vertno);
    void setVao(GLuint vao);
};

#endif