#include "GameObject.hpp"

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