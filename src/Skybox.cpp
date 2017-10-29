#include "Skybox.hpp"
#include "stb_image.h"

Skybox::Skybox(){
	this->shader_programme = create_programme_from_files("res/shaders/cube_vs.glsl", "res/shaders/cube_fs.glsl");
	glUseProgram(this->shader_programme);

	view_location = glGetUniformLocation(shader_programme, "V"); //Ubicacion de las matrices V y P en el shader para mas tarde mandarle un valor
	proj_location = glGetUniformLocation(shader_programme, "P");

	this->vao = make_big_cube();
	create_cube_map(FRONT, BACK, TOP, BOTTOM, LEFT, RIGHT);
}

GLuint Skybox::make_big_cube () {
	float points[] = {
		-10.0f,  10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		 10.0f,  10.0f, -10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		 10.0f, -10.0f,  10.0f
	};

	GLuint vbo;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (
		GL_ARRAY_BUFFER, 3 * 36 * sizeof (GLfloat), &points, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	return vao;
}


bool Skybox::load_cube_map_side (GLuint texture, GLenum side_target, const char* file_name){
	glBindTexture (GL_TEXTURE_CUBE_MAP, texture);

	int x, y, n;
	int force_channels = 4;
	unsigned char*  image_data = stbi_load (file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf (stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// non-power-of-2 dimensions check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf (stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name);
	}

	// copy image data into 'target' side of cube map
	glTexImage2D (side_target,0,GL_RGBA,x,y,0,GL_RGBA,GL_UNSIGNED_BYTE,image_data);
	free (image_data);
	return true;
}

/* load all 6 sides of the cube-map from images, then apply formatting to the
final texture */
void Skybox::create_cube_map (const char* front,const char* back,const char* top,const char* bottom,const char* left,const char* right){
	// generate a cube-map texture to hold all the sides
	glActiveTexture (GL_TEXTURE0);
	glGenTextures (1, &tex_cube);

	// load each image and copy into a side of the cube-map texture
	assert (load_cube_map_side (tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front));
	assert (load_cube_map_side (tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back));
	assert (load_cube_map_side (tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top));
	assert (load_cube_map_side (tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom));
	assert (load_cube_map_side (tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left));
	assert (load_cube_map_side (tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right));
	// format cube map texture
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Skybox::draw(){
	glDepthMask (GL_FALSE);
	glUseProgram (this->shader_programme);
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(*view));
	glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(*proj));
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_CUBE_MAP, tex_cube);
	glBindVertexArray (this->vao);
	glDrawArrays (GL_TRIANGLES, 0, 36);
	glDepthMask (GL_TRUE);
}

void Skybox::setView(glm::mat4* vi){
	this->view = vi;
}

void Skybox::setProj(glm::mat4* pro){
	this->proj = pro;
}

Skybox::~Skybox(){}