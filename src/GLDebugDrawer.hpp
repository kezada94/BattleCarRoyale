//http://sio2interactive.forumotion.net/t599-enabling-bullet-debug-draw-code-included

#ifndef GL_DEBUG_DRAWER_H
#define GL_DEBUG_DRAWER_H

#include "LinearMath/btIDebugDraw.h"
//OPENGL GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "gl_utils.h"
#include <vector>

struct Line {
    btVector3 from;
    btVector3 to;

    Line(btVector3 f, btVector3 t) {
        from = f;
        to = t;
    }
};

class GLDebugDrawer : public btIDebugDraw
{
    
    int m_debugMode;
    GLuint vao, vbo, ebo, viewLoc, projLoc;
    glm::mat4* view;
    glm::mat4* proj;
    std::vector<Line> lines;
    
public:

    GLuint shader_programme;
    
    GLDebugDrawer();
    ~GLDebugDrawer();

    void setView(glm::mat4* viewe);
    void setProj(glm::mat4* proje);
    void drawLines();

    virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
    
    virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
    
    virtual void reportErrorWarning(const char* warningString);
    
    virtual void draw3dText(const btVector3& location,const char* textString);
    
    virtual void setDebugMode(int debugMode);
    
    virtual int getDebugMode() const { return m_debugMode;}

};
#endif