#include "GLDebugDrawer.hpp"

#include <stdio.h> //printf debugging

GLDebugDrawer::GLDebugDrawer(){
    shader_programme = create_programme_from_files ("res/shaders/vert_debug.glsl", "res/shaders/frag_debug.glsl");
    viewLoc = glGetUniformLocation(shader_programme, "view");
    projLoc = glGetUniformLocation(shader_programme, "projection");
}
GLDebugDrawer::~GLDebugDrawer(){}

void GLDebugDrawer::setView(glm::mat4* viewe){
    this->view = viewe;
}
void GLDebugDrawer::setProj(glm::mat4* proje){
    this->proj = proje;
}
void GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color){
    lines.push_back(Line(from, to));
}
void GLDebugDrawer::drawLines(){
    int n = lines.size();
    GLuint indices[n*2];
    GLfloat vertices[3*n*2];
    
    for (int i=0; i < n; i++){
        vertices[6*i]   = lines.at(i).from.getX();
        vertices[6*i+1] = lines.at(i).from.getY();
        vertices[6*i+2] = lines.at(i).from.getZ();
    
        vertices[6*i+3] = lines.at(i).to.getX();
        vertices[6*i+4] = lines.at(i).to.getY();
        vertices[6*i+5] = lines.at(i).to.getZ();

        indices[2*i] = i*2;
        indices[2*i+1] = i*2+1;
    }
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
  
    glBindVertexArray(vao);
  
    //UPLOADING VERTEX
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * 3*n*2, vertices, GL_STATIC_DRAW);
    //UPLOADING INDEXES
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * 2*n,
                 indices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(GLfloat) * 3, (GLvoid*)0);
    glBindVertexArray(0);

    //use program
    glUseProgram(shader_programme);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(*view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(*proj));

    //use geometry
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_LINES, 2*n, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //delete buffers
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    lines.clear();
}

void GLDebugDrawer::setDebugMode(int debugMode){
    m_debugMode = debugMode;
}

void GLDebugDrawer::draw3dText(const btVector3& location,const char* textString){
}

void GLDebugDrawer::reportErrorWarning(const char* warningString){
    printf(warningString);
}

void GLDebugDrawer::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color){}