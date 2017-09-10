#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include "gl_utils.h"
#include "tools.h"

#define PI 3.14159265359
bool gameplay(float cam_speed, double elapsed_seconds, float *cam_pos, float *cam_yaw, float cam_yaw_speed){
    bool cam_moved = false;
    float ryaw = *cam_yaw*PI/180.0;
    if (glfwGetKey (g_window, GLFW_KEY_A)) {
		cam_pos[0] -= sin(ryaw + PI/2.0f)* cam_speed * elapsed_seconds;		
        cam_pos[2] -= cos(ryaw + PI/2.0f)* cam_speed * elapsed_seconds;
        cam_moved = true;
    }
    if (glfwGetKey (g_window, GLFW_KEY_D)) {
        cam_pos[0] += sin(ryaw+ PI/2.0f)* cam_speed * elapsed_seconds;
		cam_pos[2] += cos(ryaw+ PI/2.0f)* cam_speed * elapsed_seconds;
		
        cam_moved = true;
    }
    if (glfwGetKey (g_window, GLFW_KEY_Q)) {
        cam_pos[1] += cam_speed * elapsed_seconds;
        cam_moved = true;
    }
    if (glfwGetKey (g_window, GLFW_KEY_E)) {
        cam_pos[1] -= cam_speed * elapsed_seconds;
        cam_moved = true;
    }
    if (glfwGetKey (g_window, GLFW_KEY_W)) {
        cam_pos[0] -= sin(ryaw)* cam_speed * elapsed_seconds;
        cam_pos[2] -= cos(ryaw)* cam_speed * elapsed_seconds;
        cam_moved = true;
    }
    if (glfwGetKey (g_window, GLFW_KEY_S)) {
		cam_pos[0] += sin(ryaw)* cam_speed * elapsed_seconds;
        cam_pos[2] += cos(ryaw)* cam_speed * elapsed_seconds;
        cam_moved = true;
    }
    if (glfwGetKey (g_window, GLFW_KEY_LEFT)) {
        *cam_yaw += cam_yaw_speed * elapsed_seconds;
        cam_moved = true;
    }
    if (glfwGetKey (g_window, GLFW_KEY_RIGHT)) {
        *cam_yaw -= cam_yaw_speed * elapsed_seconds;
        cam_moved = true;
	}
	

    //if(cam_moved){
    //    printf("yaw = %f\n", *cam_yaw);
    //}
    return cam_moved;
}

bool load_mesh (const char* file_name, GLuint* vao, int* point_count) {
	const aiScene* scene = aiImportFile (file_name, aiProcess_Triangulate);
	if (!scene) {
		fprintf (stderr, "ERROR: reading mesh %s\n", file_name);
		return false;
	}
	printf ("  %i animations\n", scene->mNumAnimations);
	printf ("  %i cameras\n", scene->mNumCameras);
	printf ("  %i lights\n", scene->mNumLights);
	printf ("  %i materials\n", scene->mNumMaterials);
	printf ("  %i meshes\n", scene->mNumMeshes);
	printf ("  %i textures\n", scene->mNumTextures);
	
	/* get first mesh in file only */
	const aiMesh* mesh = scene->mMeshes[0];
	printf ("    %i vertices in mesh[0]\n", mesh->mNumVertices);
	
	/* pass back number of vertex points in mesh */
	*point_count = mesh->mNumVertices;
	
	/* generate a VAO, using the pass-by-reference parameter that we give to the
	function */
	glGenVertexArrays (1, vao);
	glBindVertexArray (*vao);
	
	/* we really need to copy out all the data from AssImp's funny little data
	structures into pure contiguous arrays before we copy it into data buffers
	because assimp's texture coordinates are not really contiguous in memory.
	i allocate some dynamic memory to do this. */
	GLfloat* points = NULL; // array of vertex points
	GLfloat* normals = NULL; // array of vertex normals
	GLfloat* texcoords = NULL; // array of texture coordinates
	if (mesh->HasPositions ()) {
		points = (GLfloat*)malloc (*point_count * 3 * sizeof (GLfloat));
		float offset = rand()%5;
		for (int i = 0; i < *point_count; i++) {
			const aiVector3D* vp = &(mesh->mVertices[i]);
			points[i * 3] = (GLfloat)vp->x+offset;
			points[i * 3 + 1] = (GLfloat)vp->y+offset;
			points[i * 3 + 2] = (GLfloat)vp->z+offset;
		}
	}
	if (mesh->HasNormals ()) {
		normals = (GLfloat*)malloc (*point_count * 3 * sizeof (GLfloat));
		for (int i = 0; i < *point_count; i++) {
			const aiVector3D* vn = &(mesh->mNormals[i]);
			normals[i * 3] = (GLfloat)vn->x;
			normals[i * 3 + 1] = (GLfloat)vn->y;
			normals[i * 3 + 2] = (GLfloat)vn->z;
		}
	}
	if (mesh->HasTextureCoords (0)) {
		texcoords = (GLfloat*)malloc (*point_count * 2 * sizeof (GLfloat));
		for (int i = 0; i < *point_count; i++) {
			const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
			texcoords[i * 2] = (GLfloat)vt->x;
			texcoords[i * 2 + 1] = (GLfloat)vt->y;
		}
	}
	
	/* copy mesh data into VBOs */
	if (mesh->HasPositions ()) {
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			3 * *point_count * sizeof (GLfloat),
			points,
			GL_STATIC_DRAW
		);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
		free (points);
	}
	if (mesh->HasNormals ()) {
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			3 * *point_count * sizeof (GLfloat),
			normals,
			GL_STATIC_DRAW
		);
		glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (1);
		free (normals);
	}
	if (mesh->HasTextureCoords (0)) {
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			2 * *point_count * sizeof (GLfloat),
			texcoords,
			GL_STATIC_DRAW
		);
		glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (2);
		free (texcoords);
	}
	if (mesh->HasTangentsAndBitangents ()) {
		// NB: could store/print tangents here
	}
	
	aiReleaseImport (scene);
	printf ("mesh loaded\n");
	
	return true;
}
