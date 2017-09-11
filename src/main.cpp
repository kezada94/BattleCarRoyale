//ASSIMP
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

//OPENGL GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//GLM
// Header & Source file
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//EVERYTHING ELSE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string>

#include "gl_utils.h"
#include "tools.h"

#define GL_LOG_FILE "log/gl.log"
#define VERTEX_SHADER_FILE "res/shaders/vert.glsl"
#define FRAGMENT_SHADER_FILE "res/shaders/frag.glsl"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 1024;
int g_gl_height = 720;
GLFWwindow* g_window = NULL;



int main(){

	/**************************** GLFW CONFIG ************************************/

	restart_gl_log ();
	start_gl ();
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // set counter-clock-wise vertex order to mean the front
	glClearColor (0.2, 0.2, 0.2, 1.0); // grey background to help spot mistakes
	#ifdef __linux__
	glViewport (0, 0, g_gl_width, g_gl_height);
	#endif
	/*-------------------------------CREATE SHADERS-------------------------------*/

	GLuint shader_programme = create_programme_from_files (
		VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
	);

	#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
	// input variables
	float near = 0.1f; // clipping plane
	float far = 100.0f; // clipping plane
	float fov = 67.0f * ONE_DEG_IN_RAD; // convert 67 degrees to radians
	float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio

	// matrix components
	float range = tan (fov * 0.5f) * near;
	float Sx = (2.0f * near) / (range * aspect + range * aspect);
	float Sy = near / range;
	float Sz = -(far + near) / (far - near);
	float Pz = -(2.0f * far * near) / (far - near);

	GLfloat proj_mat[] = {
		Sx, 0.0f, 0.0f, 0.0f,
		0.0f, Sy, 0.0f, 0.0f,
		0.0f, 0.0f, Sz, -1.0f,
		0.0f, 0.0f, Pz, 0.0f
	};

	float cam_speed = 3.0f; // 1 unit per second
	float cam_yaw_speed = 0.1f; // 10 degrees per second
	float cam_pos[] = {0.0f, 0.0f, 5.0f}; // don't start at zero, or we will be too close

    glm::vec3 campos(0.0f, 0.0f, 5.0f);
	float cam_yaw = 0.0f; // y-rotation in degrees
	glm::mat4 T = glm::translate (glm::mat4 (), glm::vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	glm::mat4 R = glm::rotate (glm::mat4(), -cam_yaw, glm::vec3(0, 1, 0));
	glm::mat4 view_mat = R * T;


	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, glm::value_ptr(view_mat));

	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);

	GLuint vao;
	int vertex_no;
	load_mesh("res/meshes/monster_truck3.obj", &vao, &vertex_no);


	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		_update_fps_counter (g_window);
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		#ifdef __linux__
		glViewport (0, 0, g_gl_width, g_gl_height);
		#endif

		glUseProgram (shader_programme);

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, vertex_no);



		// update other events like input handling
		glfwPollEvents();

		// control keys
        bool cam_moved = gameplay(cam_speed, elapsed_seconds, cam_pos, &cam_yaw, cam_yaw_speed);
		// update view matrix
		if (cam_moved){
			glm::mat4 T = glm::translate (glm::mat4 (), glm::vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2])); // cam translation
			glm::mat4 R = glm::rotate(glm::mat4(),-cam_yaw, glm::vec3(0, 1, 0)); //
			glm::mat4 view_mat = R * T;
			glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, value_ptr(view_mat));
		}
		if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose (g_window, 1);
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
