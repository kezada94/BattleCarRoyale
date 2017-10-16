#include "GameObject.hpp"
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

GameObject::GameObject(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot){
    
    btConvexHullShape* colShape;

    if(load_mesh(path, vao, vertNumber, &colShape)==false){
        printf("Error loading %s", path);
    }
	load_texture(shaderprog, texture_path, texture, tex_location);

	btTransform startTransform;
    startTransform.setIdentity();

    bool isDynamic = (masa != 0.f);

    btVector3 localInertia(0, 0, 0);
    
	if (isDynamic)
		colShape->calculateLocalInertia(masa, localInertia);

    startTransform.setOrigin(startPos);
    startTransform.setRotation(startRot);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(masa, myMotionState, colShape, localInertia);
	this->rigidBody = new btRigidBody(rbInfo);
}
GameObject::GameObject(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll){
    
    if(load_mesh(path, vao, vertNumber)==false){
        printf("Error loading %s", path);
    }
    load_texture(shaderprog, texture_path, texture, tex_location);
    btTransform startTransform;
	startTransform.setIdentity();
	
    bool isDynamic = (masa != 0.f);	
    btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		coll->calculateLocalInertia(masa, localInertia);

    startTransform.setOrigin(startPos);
    startTransform.setRotation(startRot);
    
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(masa, myMotionState, coll, localInertia);
	this->rigidBody = new btRigidBody(rbInfo);
}
GameObject::~GameObject(){
	printf("hola se llama al destructor de la siguiente clase GAMEOBKECT");
    delete this->rigidBody->getMotionState();
    delete this->rigidBody->getCollisionShape();
    delete this->rigidBody;
}

bool GameObject::load_mesh (const char* file_name, GLuint& vao, int& vert_no, btConvexHullShape** col) {
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
	printf ("    %i vertices in %s\n", mesh->mNumVertices, file_name);
	
	/* pass back number of vertex points in mesh */
	vert_no = mesh->mNumVertices;
	
	/* generate a VAO, using the pass-by-reference parameter that we give to the
	function */
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	
	/* we really need to copy out all the data from AssImp's funny little data
	structures into pure contiguous arrays before we copy it into data buffers
	because assimp's texture coordinates are not really contiguous in memory.
	i allocate some dynamic memory to do this. */
	GLfloat* points = NULL; 								// array of vertex points
	GLfloat* normals = NULL; 								// array of vertex normals
	GLfloat* texcoords = NULL;								// array of texture coordinates
	btConvexHullShape* collider = new btConvexHullShape();	// mesh collider shape

	if (mesh->HasPositions ()) {
		points = (GLfloat*)malloc (vert_no * 3 * sizeof (GLfloat));
		for (int i = 0; i < vert_no; i++) {
			const aiVector3D* vp = &(mesh->mVertices[i]);
			points[i * 3] = (GLfloat)vp->x;
			points[i * 3 + 1] = (GLfloat)vp->y;
			points[i * 3 + 2] = (GLfloat)vp->z;

			//Se agregan todos los vertices del mesh al collisionObject 1 por 1
			collider->addPoint(btVector3(vp->x, vp->y, vp->z));
		}
	}
	if (mesh->HasNormals ()) {
		normals = (GLfloat*)malloc (vert_no * 3 * sizeof (GLfloat));
		for (int i = 0; i < vert_no; i++) {
			const aiVector3D* vn = &(mesh->mNormals[i]);
			normals[i * 3] = (GLfloat)vn->x;
			normals[i * 3 + 1] = (GLfloat)vn->y;
			normals[i * 3 + 2] = (GLfloat)vn->z;
		}
	}
	if (mesh->HasTextureCoords (0)) {
		printf("	%i texture coords\n", vert_no*2);
		texcoords = (GLfloat*)malloc (vert_no * 2 * sizeof (GLfloat));
		for (int i = 0; i < vert_no; i++) {
			const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
			texcoords[i * 2] = (GLfloat)vt->x;
			texcoords[i * 2 + 1] = (GLfloat)vt->y;
		}
	}
	printf("    %i vertices in collider non optimized\n", collider->getNumPoints());

    //Código optimización basado en http://www.bulletphysics.org/mediawiki-1.5.8/index.php/BtShapeHull_vertex_reduction_utility
    //Ligeramente modificado, ya que lo que sale en la wiki esta malo
	btShapeHull* hull = new btShapeHull(collider);
	btScalar margin = collider->getMargin();
	hull->buildHull(margin);
	btConvexHullShape* simplifiedConvexShape = new btConvexHullShape((const btScalar*)hull->getVertexPointer(), hull->numVertices(), sizeof(btVector3));
	printf("    %i vertices in collider optimized\n", simplifiedConvexShape->getNumPoints());

	//libera memoria del primer collider no-optimzado
	delete collider;

	*col = simplifiedConvexShape;

	/* copy mesh data into VBOs */
	if (mesh->HasPositions ()) {
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			3 * vert_no * sizeof (GLfloat),
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
			3 * vert_no * sizeof (GLfloat),
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
			2 * vert_no * sizeof (GLfloat),
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
bool GameObject::load_mesh (const char* file_name, GLuint& vao, int& vert_no) {
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
	printf ("    %i vertices in %s\n", mesh->mNumVertices, file_name);
	
	/* pass back number of vertex points in mesh */
	vert_no = mesh->mNumVertices;
	
	/* generate a VAO, using the pass-by-reference parameter that we give to the
	function */
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	
	/* we really need to copy out all the data from AssImp's funny little data
	structures into pure contiguous arrays before we copy it into data buffers
	because assimp's texture coordinates are not really contiguous in memory.
	i allocate some dynamic memory to do this. */
	GLfloat* points = NULL; 								// array of vertex points
	GLfloat* normals = NULL; 								// array of vertex normals
	GLfloat* texcoords = NULL;								// array of texture coordinates

	if (mesh->HasPositions ()) {
		points = (GLfloat*)malloc (vert_no * 3 * sizeof (GLfloat));
		for (int i = 0; i < vert_no; i++) {
			const aiVector3D* vp = &(mesh->mVertices[i]);
			points[i * 3] = (GLfloat)vp->x;
			points[i * 3 + 1] = (GLfloat)vp->y;
			points[i * 3 + 2] = (GLfloat)vp->z;
		}
	}
	if (mesh->HasNormals ()) {
		normals = (GLfloat*)malloc (vert_no * 3 * sizeof (GLfloat));
		for (int i = 0; i < vert_no; i++) {
			const aiVector3D* vn = &(mesh->mNormals[i]);
			normals[i * 3] = (GLfloat)vn->x;
			normals[i * 3 + 1] = (GLfloat)vn->y;
			normals[i * 3 + 2] = (GLfloat)vn->z;
		}
	}
	if (mesh->HasTextureCoords (0)) {
		texcoords = (GLfloat*)malloc (vert_no * 2 * sizeof (GLfloat));
		for (int i = 0; i < vert_no; i++) {
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
			3 * vert_no * sizeof (GLfloat),
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
			3 * vert_no * sizeof (GLfloat),
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
			2 * vert_no * sizeof (GLfloat),
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
bool GameObject::load_texture (GLuint shaderprog, const char* texture_path, GLuint& texture, GLuint tex_location){

	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load (texture_path, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf (stderr, "ERROR: could not load %s\n", texture_path);
	}

	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) 
		fprintf (stderr, "WARNING: texture %s is not power-of-2 dimensions: %i, %i\n", texture_path, x, y);

	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for(int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for(int col = 0; col < width_in_bytes; col++){
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	texture = 0;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	tex_location = glGetUniformLocation (shaderprog, "basic_texture");

	return true;
}

//GETTERS
int GameObject::getVertNumber(){
    return this->vertNumber;
}
GLuint GameObject::getVao(){
    return this->vao;
}
GLuint GameObject::getTexture(){
    return this->texture;
}
btRigidBody* GameObject::getRigidBody(){
    return this->rigidBody;
}