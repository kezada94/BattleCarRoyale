#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Tools.hpp"
#include <glm/glm.hpp>

class GameObject{

protected:
    GLuint vao;
    GLuint ebo;
    GLuint texture;
    int vertNumber;
    int indicesNumber;
    btRigidBody* rigidBody;
    
    bool load_mesh (const char* file_name, GLuint& vao, int& vert_no, btCollisionShape** col);
    bool load_mesh (const char* file_name, GLuint& vao, int& vert_no);    
    bool load_texture (GLuint shaderprog, const char* texture_path, const char* normal_path); 
    bool load_texture2 (GLuint shaderprog, const char* texture_path, GLuint& texture, GLuint tex_location);       

    
public:
    GLuint tex_location;
    unsigned int normalMap;
    unsigned int normalMapLocation;
    glm::vec3 specular;
    GLuint specular_loc;

    GameObject();
    GameObject(const char* path, const char* texture_path, const char* normal_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, glm::vec3 specular, GLuint specular_loc);
    GameObject(const char* path, const char* texture_path, const char* normal_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, glm::vec3 specular, GLuint specular_loc, btCollisionShape* coll);
    
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