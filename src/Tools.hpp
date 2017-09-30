#ifndef TOOLS_H
#define TOOLS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btShapeHull.h>

bool load_mesh (const char* file_name, GLuint& vao, int& vert_no, btConvexHullShape** col);
bool load_mesh (const char* file_name, GLuint& vao, int& vert_no);    
bool load_texture (GLuint shaderprog, const char* texture_path, GLuint& texture, GLuint tex_location);
    
#endif