#include "GameObject.hpp"

GameObject::GameObject(const char* path, btScalar masa, btVector3 startPos, btQuaternion startRot){
    
    btConvexHullShape* colShape;

    if(load_mesh(path, &colShape)==false){
        printf("Error loading %s", path);
    }

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
	this->rigidBody->setRestitution(btScalar(1));
	
}
GameObject::GameObject(const char* path, btScalar masa, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll){
    
    if(load_mesh(path)==false){
        printf("Error loading %s", path);
    }
    
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
    delete this->rigidBody->getMotionState();
    delete this->rigidBody->getCollisionShape();
    delete this->rigidBody;
}

void GameObject::draw(const GLuint model_mat_location){
    btTransform trans;
    glm::mat4 model;
    this->rigidBody->getMotionState()->getWorldTransform(trans);

    model = glm::translate(glm::mat4(), glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    model = glm::rotate(model, trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ() ));
    glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &model[0][0]);

    glBindVertexArray(this->getVao());
    glDrawArrays(GL_TRIANGLES, 0, this->getVertNumber()); 
}
bool GameObject::load_mesh (const char* file_name, btConvexHullShape** col) {
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
	vertNumber = mesh->mNumVertices;
	
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
		points = (GLfloat*)malloc (vertNumber * 3 * sizeof (GLfloat));
		for (int i = 0; i < vertNumber; i++) {
			const aiVector3D* vp = &(mesh->mVertices[i]);
			points[i * 3] = (GLfloat)vp->x;
			points[i * 3 + 1] = (GLfloat)vp->y;
			points[i * 3 + 2] = (GLfloat)vp->z;

			//Se agregan todos los vertices del mesh al collisionObject 1 por 1
			collider->addPoint(btVector3(vp->x, vp->y, vp->z));
		}
	}
	if (mesh->HasNormals ()) {
		normals = (GLfloat*)malloc (vertNumber * 3 * sizeof (GLfloat));
		for (int i = 0; i < vertNumber; i++) {
			const aiVector3D* vn = &(mesh->mNormals[i]);
			normals[i * 3] = (GLfloat)vn->x;
			normals[i * 3 + 1] = (GLfloat)vn->y;
			normals[i * 3 + 2] = (GLfloat)vn->z;
		}
	}
	if (mesh->HasTextureCoords (0)) {
		texcoords = (GLfloat*)malloc (vertNumber * 2 * sizeof (GLfloat));
		for (int i = 0; i < vertNumber; i++) {
			const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
			texcoords[i * 2] = (GLfloat)vt->x;
			texcoords[i * 2 + 1] = (GLfloat)vt->y;
		}
	}
	printf("    %i vertices in collider non optimized\n", collider->getNumPoints());

    //Código optimización basado en http://www.bulletphysics.org/mediawiki-1.5.8/index.php/BtShapeHull_vertex_reduction_utility
    //Ligeramente modificado, ya que lo que sale en la wifi esta malo
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
			3 * vertNumber * sizeof (GLfloat),
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
			3 * vertNumber * sizeof (GLfloat),
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
			2 * vertNumber * sizeof (GLfloat),
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
bool GameObject::load_mesh (const char* file_name) {
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
	printf ("    %i vertices in \n", mesh->mNumVertices, file_name);
	
	/* pass back number of vertex points in mesh */
	vertNumber = mesh->mNumVertices;
	
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
		points = (GLfloat*)malloc (vertNumber * 3 * sizeof (GLfloat));
		for (int i = 0; i < vertNumber; i++) {
			const aiVector3D* vp = &(mesh->mVertices[i]);
			points[i * 3] = (GLfloat)vp->x;
			points[i * 3 + 1] = (GLfloat)vp->y;
			points[i * 3 + 2] = (GLfloat)vp->z;
		}
	}
	if (mesh->HasNormals ()) {
		normals = (GLfloat*)malloc (vertNumber * 3 * sizeof (GLfloat));
		for (int i = 0; i < vertNumber; i++) {
			const aiVector3D* vn = &(mesh->mNormals[i]);
			normals[i * 3] = (GLfloat)vn->x;
			normals[i * 3 + 1] = (GLfloat)vn->y;
			normals[i * 3 + 2] = (GLfloat)vn->z;
		}
	}
	if (mesh->HasTextureCoords (0)) {
		texcoords = (GLfloat*)malloc (vertNumber * 2 * sizeof (GLfloat));
		for (int i = 0; i < vertNumber; i++) {
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
			3 * vertNumber * sizeof (GLfloat),
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
			3 * vertNumber * sizeof (GLfloat),
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
			2 * vertNumber * sizeof (GLfloat),
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

//GETTERS
int GameObject::getVertNumber(){
    return this->vertNumber;
}
GLuint GameObject::getVao(){
    return this->vao;
}
btRigidBody* GameObject::getRigidBody(){
    return this->rigidBody;
}