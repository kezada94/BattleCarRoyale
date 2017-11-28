#include "StaticGameObject.hpp"
#include "stb_image.h"

StaticGameObject::StaticGameObject(const char* path, const char* texture_path, const char* normal_path, GLuint shaderprog, btVector3 startPos, btQuaternion startRot, glm::vec3 specular, GLuint specular_loc)
: GameObject() {
    this->specular = specular;
    this->specular_loc = specular_loc;
    btBvhTriangleMeshShape* colShape = load_mesh(path, vao, vertNumber);
    
    load_texture(shaderprog, texture_path, normal_path);

	glUniform1i(this->tex_location, 0); 
	glUniform1i(this->normalMapLocation, 1); 
	

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
    glUniform3fv(specular_loc, 1, &specular[0]);

    glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, this->texture);

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, this->normalMap);
    
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glDrawElements(GL_TRIANGLES, indicesNumber, GL_UNSIGNED_INT, 0);
}

btBvhTriangleMeshShape* StaticGameObject::load_mesh (const char* file_name, GLuint& vao, int& vert_no) {
	const aiScene* scene = aiImportFile (file_name, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
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
	GLfloat *tangents = NULL;
	GLfloat *bitangents = NULL;
	

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
	if ( mesh->HasTangentsAndBitangents() ) {
		printf( "mesh has tangents\n" );
		tangents = (GLfloat*)malloc( this->vertNumber * 3 * sizeof( GLfloat ) );
		bitangents = (GLfloat*)malloc( this->vertNumber * 3 * sizeof( GLfloat ) );
		for (int i = 0; i < this->vertNumber; i++) {
			tangents[3*i] = mesh->mTangents[i].x;
			tangents[3*i+1] = mesh->mTangents[i].y;
			tangents[3*i+2] = mesh->mTangents[i].z;

			bitangents[3*i] = mesh->mBitangents[i].x; //quizas sea necesario
			bitangents[3*i+1] = mesh->mBitangents[i].y; //quizas sea necesario
			bitangents[3*i+2] = mesh->mBitangents[i].z; //quizas sea necesario
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
	if ( mesh->HasTangentsAndBitangents() ) {
		GLuint tangents_vbo;
		glGenBuffers( 1, &tangents_vbo );
		glBindBuffer( GL_ARRAY_BUFFER, tangents_vbo );
		glBufferData( GL_ARRAY_BUFFER, 3 * this->vertNumber * sizeof( GLfloat ), tangents, GL_STATIC_DRAW );
		glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 3 );
		free(tangents);

		GLuint bitangents_vbo;
		glGenBuffers( 1, &bitangents_vbo );
		glBindBuffer( GL_ARRAY_BUFFER, bitangents_vbo );
		glBufferData( GL_ARRAY_BUFFER, 3 * this->vertNumber * sizeof( GLfloat ), bitangents, GL_STATIC_DRAW );
		glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 4 );
		free(bitangents);
	}	
	aiReleaseImport (scene);
	printf ("mesh loaded\n");
	
    return colorado;
}
bool StaticGameObject::load_texture (GLuint shaderprog, const char* texture_path, const char* normal_path){
	
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
		glGenerateMipmap(GL_TEXTURE_2D);
		
	
		tex_location = glGetUniformLocation (shaderprog, "basic_texture");
		
		free(image_data);

		if (normal_path != NULL) {
			x, y, n;
			force_channels = 4;
			unsigned char* image_data2 = stbi_load (normal_path, &x, &y, &n, force_channels);
			if (!image_data2) {
				fprintf (stderr, "ERROR: could not load %s\n", normal_path);
			}
		
			if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) 
				fprintf (stderr, "WARNING: texture %s is not power-of-2 dimensions: %i, %i\n", normal_path, x, y);
		
			width_in_bytes = x * 4;
			top = NULL;
			bottom = NULL;
			temp = 0;
			half_height = y / 2;
		
			for(int row = 0; row < half_height; row++) {
				top = image_data2 + row * width_in_bytes;
				bottom = image_data2 + (y - row - 1) * width_in_bytes;
				for(int col = 0; col < width_in_bytes; col++){
					temp = *top;
					*top = *bottom;
					*bottom = temp;
					top++;
					bottom++;
				}
			}
		
			glActiveTexture(GL_TEXTURE1);			
			normalMap = 0;
			glGenTextures(1, &normalMap);
			glBindTexture(GL_TEXTURE_2D, normalMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data2);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			
			normalMapLocation = glGetUniformLocation (shaderprog, "normal_map");

			free(image_data2);			
		}
		return true;
	}

