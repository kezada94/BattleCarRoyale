#include "StaticGameObject.hpp"

StaticGameObject::StaticGameObject(const char* path, const char* texture_path, GLuint shaderprog, btVector3 startPos, btQuaternion startRot)
: GameObject() {
    
    btBvhTriangleMeshShape* colShape = load_mesh(path, vao, vertNumber);
    
    load_texture(shaderprog, texture_path, texture, tex_location);

    btTransform startTransform;
    startTransform.setIdentity();

    btVector3 localInertia(0, 0, 0);
    
    startTransform.setOrigin(startPos);
    startTransform.setRotation(startRot);
    if(colShape == NULL){
        printf("ESTA MALITO");
    }

    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape, localInertia);
    this->rigidBody = new btRigidBody(rbInfo);
}
        
StaticGameObject::~StaticGameObject(){}
    
void StaticGameObject::draw(const GLuint model_mat_location){
    btTransform trans;
    glm::mat4 model;
    this->getRigidBody()->getMotionState()->getWorldTransform(trans);
        
    model = glm::translate(glm::mat4(), glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    model = glm::rotate(model, trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ() ));
    btVector3 escala = getRigidBody()->getCollisionShape()->getLocalScaling();
    model = glm::scale(model, glm::vec3(escala.getX(), escala.getY(), escala.getZ()));
    glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &model[0][0]);
    
    glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, this->texture);
    glUniform1i (this->tex_location, 0);
    
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glDrawElements(GL_TRIANGLES, 3 * indicesNumber, GL_UNSIGNED_INT, 0);
}

btBvhTriangleMeshShape* StaticGameObject::load_mesh (const char* file_name, GLuint& vao, int& vert_no) {
	const aiScene* scene = aiImportFile (file_name, aiProcess_Triangulate);
	if (!scene) {
		fprintf (stderr, "ERROR: reading mesh %s\n", file_name);
		return NULL;
	}
	printf ("  %i animations\n", scene->mNumAnimations);
	printf ("  %i cameras\n", scene->mNumCameras);
	printf ("  %i lights\n", scene->mNumLights);
	printf ("  %i materials\n", scene->mNumMaterials);
	printf ("  %i meshes\n", scene->mNumMeshes);
	printf ("  %i textures\n", scene->mNumTextures);
	
	/* get first mesh in file only */
	const aiMesh* mesh = scene->mMeshes[0];
	printf ("    %i vertices in %s\n", mesh->mNumVertices, file_name);
	
	/* pass back number of vertex points in mesh */
    this->vertNumber = mesh->mNumVertices;
    this->indicesNumber = mesh->mNumFaces * 3;
	
	/* generate a VAO, using the pass-by-reference parameter that we give to the
	function */
	glGenVertexArrays (1, &this->vao);
	glBindVertexArray (this->vao);
	
	/* we really need to copy out all the data from AssImp's funny little data
	structures into pure contiguous arrays before we copy it into data buffers
	because assimp's texture coordinates are not really contiguous in memory.
	i allocate some dynamic memory to do this. */
	GLfloat* points = NULL; 								// array of vertex points
	GLfloat* normals = NULL; 								// array of vertex normals
	GLfloat* texcoords = NULL;								// array of texture coordinates
	int* indices = NULL;								// array of texture coordinates

	if (mesh->HasPositions ()) {
		points = (GLfloat*)malloc (this->vertNumber * 3 * sizeof (GLfloat));
		for (int i = 0; i < this->vertNumber; i++) {
			const aiVector3D* vp = &(mesh->mVertices[i]);
			points[i * 3] = (GLfloat)vp->x;
			points[i * 3 + 1] = (GLfloat)vp->y;
			points[i * 3 + 2] = (GLfloat)vp->z;
		}
	}
	if (mesh->HasNormals ()) {
		normals = (GLfloat*)malloc (this->vertNumber * 3 * sizeof (GLfloat));
		for (int i = 0; i < this->vertNumber; i++) {
			const aiVector3D* vn = &(mesh->mNormals[i]);
			normals[i * 3] = (GLfloat)vn->x;
			normals[i * 3 + 1] = (GLfloat)vn->y;
			normals[i * 3 + 2] = (GLfloat)vn->z;
		}
	}
	if (mesh->HasTextureCoords (0)) {
		printf("	%i texture coords\n", this->vertNumber*2);
		texcoords = (GLfloat*)malloc (this->vertNumber * 2 * sizeof (GLfloat));
		for (int i = 0; i < this->vertNumber; i++) {
			const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
			texcoords[i * 2] = (GLfloat)vt->x;
			texcoords[i * 2 + 1] = (GLfloat)vt->y;
		}
    }
    if (mesh->HasFaces()) {
        printf("	%i indices\n", mesh->mNumFaces*3);        
        printf("	%i faces\n", mesh->mNumFaces);        
		indices = (int*)malloc (mesh->mNumFaces * 3 * sizeof (int));
		for (int i = 0; i < mesh->mNumFaces; i++) {
			indices[3*i] = mesh->mFaces[i].mIndices[0];
			indices[3*i+1] = mesh->mFaces[i].mIndices[1];
			indices[3*i+2] = mesh->mFaces[i].mIndices[2];
		}
    }
    
    // mesh collider shape
    btTriangleIndexVertexArray* collider = new btTriangleIndexVertexArray(
        mesh->mNumFaces,
        indices,
        3*sizeof(int),
        this->vertNumber,
        points,
        3*sizeof(GLfloat)
    );	
    btBvhTriangleMeshShape* colorado = new btBvhTriangleMeshShape(collider, true, true);

	/* copy mesh data into VBOs */
	if (mesh->HasPositions ()) {
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			3 * this->vertNumber * sizeof (GLfloat),
			points,
			GL_STATIC_DRAW
		);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
		//free (points); //no se liberan ya que la clase btBvhTriangleMeshShape no crea una copia
    }
    if (mesh->HasFaces()){
		glGenBuffers (1, &this->ebo);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glBufferData (
			GL_ELEMENT_ARRAY_BUFFER,
			3 * mesh->mNumFaces * sizeof (int),
			indices,
			GL_STATIC_DRAW
		);
		//free (indices);
    }
	if (mesh->HasNormals ()) {
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			3 * this->vertNumber * sizeof (GLfloat),
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
			2 * this->vertNumber * sizeof (GLfloat),
			texcoords,
			GL_STATIC_DRAW
		);
		glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (2);
		free (texcoords);
	}
	
	aiReleaseImport (scene);
	printf ("mesh loaded\n");
	
    return colorado;
}