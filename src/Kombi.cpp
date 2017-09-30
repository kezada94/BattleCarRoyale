#include "Kombi.hpp"

Kombi::Kombi(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld* world) 
    : Car("res/meshes/kombi3.obj", "res/textures/kombi3.jpg", shaderprog, btScalar(100), startPos, startRot) {
        initialize(world);

        load_mesh("res/textures/rueda/rueda.obj", wheel_vao, wheel_vert);
    }

Kombi::Kombi(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btCollisionShape* coll, btDiscreteDynamicsWorld* world)
    : Car("res/meshes/kombi3.obj", "res/textures/kombi3.jpg", shaderprog, btScalar(100), startPos, startRot, coll) {
        initialize(world);        
    }
    
Kombi::~Kombi(){}

void Kombi::initialize(btDiscreteDynamicsWorld* world){
    btRaycastVehicle::btVehicleTuning* tuning = new btRaycastVehicle::btVehicleTuning();
    btVehicleRaycaster* defvehicle = new btDefaultVehicleRaycaster(world);
    getRigidBody()->setActivationState( DISABLE_DEACTIVATION);
    btRaycastVehicle* vehicle = new btRaycastVehicle(*tuning, getRigidBody(), defvehicle);
    vehicle->setCoordinateSystem(0, 1, 2);
    
    world->addVehicle(vehicle);

    btVector3 wheelDirection(0.0f, -1.0f, 0.0f);
    btVector3 wheelAxis(1.0f, 0.0f, 0.0f);
    btScalar suspensionRestLength(0.6f);
    btScalar wheelRadius(1.f);
    vehicle->addWheel(btVector3(-2.f, -2.0f, -3.5f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, true);
    vehicle->addWheel(btVector3(2.f, -2.0f, -3.5f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, true);
    vehicle->addWheel(btVector3(-2.f, -2.0f, 3.f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, false);
    vehicle->addWheel(btVector3(2.f, -2.0f, 3.f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, false);
    
    this->setCar(vehicle);
}

void Kombi::accelerate(){
    this->getCar()->applyEngineForce(200,0);
    this->getCar()->applyEngineForce(200,1);
}

void Kombi::draw(GLuint model_mat_location){
    btTransform trans;
    glm::mat4 model;
    this->getRigidBody()->getMotionState()->getWorldTransform(trans);
        
    model = glm::translate(glm::mat4(), glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    model = glm::rotate(model, trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ() ));
    glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &model[0][0]);
        
    glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, this->getTexture());
    glUniform1i (tex_location, 0);
    
    glBindVertexArray(this->getVao());
    glDrawArrays(GL_TRIANGLES, 0, this->getVertNumber()); 

    for (int i=0; i < this->getCar()->getNumWheels(); i++){
        trans = this->getCar()->getWheelInfo(i).m_worldTransform;
        glm::quat hele = glm::angleAxis(trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ()));
        glm::mat4 model2 = glm::toMat4(hele);
        model = glm::translate(glm::mat4(), glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
        
        //glm::mat4 model2 = glm::rotate(model, -trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ() ));
        model = model * model2;
        glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &model[0][0]);
        
        glBindVertexArray(wheel_vao);
        glDrawArrays(GL_TRIANGLES, 0, wheel_vert); 
    }
}

void Kombi::brake(){}
void Kombi::reverse(){}
void Kombi::turnRight(){
    if (getCar()->getSteeringValue(0)>-1 && getCar()->getSteeringValue(1)>-1){
        getCar()->setSteeringValue(getCar()->getSteeringValue(0)-0.03f, 0);
        getCar()->setSteeringValue(getCar()->getSteeringValue(1)-0.03f, 1);
    }
}
void Kombi::turnLeft(){
    if (getCar()->getSteeringValue(0)<1 && getCar()->getSteeringValue(0)<1){
        getCar()->setSteeringValue(getCar()->getSteeringValue(0)+0.03f, 0);
        getCar()->setSteeringValue(getCar()->getSteeringValue(1)+0.03f, 1);
    }
}

void Kombi::fire(){}

void Kombi::spawn(){}
void Kombi::despawn(){}