#include "Tools.hpp"

void init(){
	restart_gl_log ();
	start_gl ();
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // set counter-clock-wise vertex order to mean the front
    glClearColor (0.2, 0.2, 0.2, 1.0); // grey background to help spot mistakes
    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
	#ifdef __linux__
	    glViewport (0, 0, g_gl_width, g_gl_height);
	#endif
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	#ifdef __linux__
	glViewport (0, 0, width, height);
	#endif
}