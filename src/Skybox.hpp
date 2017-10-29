#ifndef SKYBOX_H
#define SKYBOX_H

//OPENGL GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string>
#include "gl_utils.h"

#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#define CUBE_VERT_FILE "res/shaders/cube_vs.glsl"
#define CUBE_FRAG_FILE "res/shaders/cube_fs.glsl"

#define FRONT "res/cubemap/nz.png"
#define BACK "res/cubemap/pz.png"
#define TOP "res/cubemap/py.png"
#define BOTTOM "res/cubemap/ny.png"
#define LEFT "res/cubemap/nx.png"
#define RIGHT "res/cubemap/px.png"

class Skybox{
public:
	GLuint vao;
	GLuint tex_cube;
	GLuint shader_programme;
	GLuint view_location;
	GLuint proj_location;

	glm::mat4* proj;
	glm::mat4* view;

	Skybox();
	~Skybox();

	GLuint make_big_cube ();
	bool load_cube_map_side (GLuint texture, GLenum side_target, const char* file_name);
	void create_cube_map (const char* front,const char* back,const char* top,const char* bottom,const char* left,const char* right);
	void draw();
	void setView(glm::mat4* vi);
	void setProj(glm::mat4* pro);

};

#endif


