#include "Patriot.hpp"

Patriot::Patriot(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld* world) 
    : Car("res/patriot.obj", "res/Ambulance.png", shaderprog, btScalar(80), startPos, startRot) {
        initialize(world);

        load_mesh("res/textures/rueda/rueda.obj", wheel_vao, wheel_vert);

        setHealth(25.f);
        
    }

Patriot::Patriot(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btCollisionShape* coll, btDiscreteDynamicsWorld* world)
    : Car("res/patriot.obj", "res/Ambulance.png", shaderprog, btScalar(80), startPos, startRot, coll) {
        initialize(world);    
        load_mesh("res/textures/rueda/rueda.obj", wheel_vao, wheel_vert);
        setHealth(25.f);
        
    }
    
Patriot::~Patriot(){}

void Patriot::initialize(btDiscreteDynamicsWorld* world){
    setWorld(world);
    
    btRaycastVehicle::btVehicleTuning* tuning = new btRaycastVehicle::btVehicleTuning();
    btVehicleRaycaster* defvehicle = new btDefaultVehicleRaycaster(world);
    getRigidBody()->setActivationState( DISABLE_DEACTIVATION);
    getRigidBody()->setUserPointer(this);
    
    btRaycastVehicle* vehicle = new btRaycastVehicle(*tuning, getRigidBody(), defvehicle);
    vehicle->setCoordinateSystem(0, 1, 2);
    
    world->addAction(vehicle);

    btVector3 wheelDirection(0.0f, -1.0f, 0.0f);
    btVector3 wheelAxis(1.0f, 0.0f, 0.0f);
    btScalar suspensionRestLength(0.3f);    //TODO: PARAM
    btScalar wheelRadius(1.f);              //TOCO: PARAM 
    vehicle->addWheel(btVector3(2.37f, -1.f, 6.1f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, true);  //TODO: PARAM
    vehicle->addWheel(btVector3(-2.37f, -1.f, 6.1f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, true); //TODO: PARAM
    vehicle->addWheel(btVector3(-2.37f, -1.f, -2.82f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, false);//TODO: PARAM
    vehicle->addWheel(btVector3(2.37f, -1.f, -2.82f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, false); //TODO: PARAM
    
    this->setCar(vehicle);

    /*for (int i = 0; i < getCar()->getNumWheels(); i++)
    {        
        btWheelInfo& wheel = getCar()->getWheelInfo(i);
        wheel.m_wheelsDampingRelaxation = 11.7f;    //TODO: PARAM
        wheel.m_wheelsDampingCompression = 10.7f;   //TODO: PARAM
        wheel.m_frictionSlip = btScalar(1000.);     //TODO: PARAM
        wheel.m_rollInfluence = btScalar(0.05);     //TODO: PARAM
        wheel.m_maxSuspensionTravelCm = 30.f;       //TODO: PARAM
    }*/
    setIsAlive(true);
    
}
void Patriot::updatePhysics(){
    if(!getTurned()){
        if(getCar()->getSteeringValue(0) > 0){
            getCar()->setSteeringValue(getCar()->getSteeringValue(0)-0.030f, 0);    //TODO: PARAM
            getCar()->setSteeringValue(getCar()->getSteeringValue(1)-0.030f, 1);    //TODO: PARAM
            if(getCar()->getSteeringValue(0) < 0){
                getCar()->setSteeringValue(0, 0);
                getCar()->setSteeringValue(0, 1);
            }
        }else if(getCar()->getSteeringValue(0) < 0){
            getCar()->setSteeringValue(getCar()->getSteeringValue(0)+0.03f, 0);     //TODO: PARAM
            getCar()->setSteeringValue(getCar()->getSteeringValue(1)+0.03f, 1);     //TODO: PARAM
            if(getCar()->getSteeringValue(0) > 0){
                getCar()->setSteeringValue(0, 0);
                getCar()->setSteeringValue(0, 1);
            }
        }
    }else{
        setTurned(false);                
    }
    getCar()->setBrake(0, 0);
    getCar()->setBrake(0, 1);
    getCar()->setBrake(0, 2);
    getCar()->setBrake(0, 3);

    this->getCar()->applyEngineForce(0, 0); //TODO: Param
    this->getCar()->applyEngineForce(0, 1);

}
void Patriot::draw(GLuint model_mat_location){
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

void Patriot::accelerate(){
    getCar()->applyEngineForce(-80,0); //TODO: Param
    getCar()->applyEngineForce(-80,1);
}

void Patriot::brake(){
    getCar()->setBrake(btScalar(0.5), 0);   //TODO: PARAM
    getCar()->setBrake(btScalar(0.5), 1);   //TODO: PARAM
    getCar()->setBrake(btScalar(0.5), 2);   //TODO: PARAM
    getCar()->setBrake(btScalar(0.5), 3);   //TODO: PARAM
}
void Patriot::reverse(){
    this->getCar()->applyEngineForce(25,0);    //TODO: Param
    this->getCar()->applyEngineForce(25,1);    //TODO: PARAM
}

void Patriot::turnRight(){
    if (getCar()->getSteeringValue(0)>-0.4f && getCar()->getSteeringValue(1)>-0.4f){
        getCar()->setSteeringValue(getCar()->getSteeringValue(0)-0.02f, 0); //TODO: Param
        getCar()->setSteeringValue(getCar()->getSteeringValue(1)-0.02f, 1); //TODO: PARAM
    }
    setTurned(true);
}
void Patriot::turnLeft(){
    if (getCar()->getSteeringValue(0)<0.4f && getCar()->getSteeringValue(1)<0.4f){
        getCar()->setSteeringValue(getCar()->getSteeringValue(0)+0.02f, 0); //TODO: PARAM
        getCar()->setSteeringValue(getCar()->getSteeringValue(1)+0.02f, 1); //TODO: PARAM
    }
    setTurned(true);  
}

void Patriot::fire(){}

void Patriot::spawn(){}
void Patriot::despawn(btDiscreteDynamicsWorld* world){
    btCollisionShape* col = getRigidBody()->getCollisionShape();
    btRigidBody* rb = getRigidBody();

    world->removeCollisionObject(rb);

}