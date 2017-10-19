#include "Kombi.hpp"

Kombi::Kombi(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld* world) 
    : Car("res/KOMBI.obj", "res/kombitextura.png", shaderprog, btScalar(10), startPos, startRot) {
        initialize(world);

        load_mesh("res/textures/rueda/rueda.obj", wheel_vao, wheel_vert);
    }

Kombi::Kombi(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btCollisionShape* coll, btDiscreteDynamicsWorld* world)
    : Car("res/KOMBI.obj", "res/kombitextura.png", shaderprog, btScalar(10), startPos, startRot, coll) {
        initialize(world);     
        load_mesh("res/textures/rueda/rueda.obj", wheel_vao, wheel_vert);        
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
    btScalar suspensionRestLength(0.5f);    //TODO: PARAM
    btScalar wheelRadius(1.f);              //TOCO: PARAM 
    vehicle->addWheel(btVector3(-2.f, -1.36f, 4.18f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, true);//TODO: PARAM
    vehicle->addWheel(btVector3(2.f, -1.36f, 4.18f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, true); //TODO: PARAM
    vehicle->addWheel(btVector3(2.f, -1.36f, -3.6f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, false);  //TODO: PARAM
    vehicle->addWheel(btVector3(-2.f, -1.36f, -3.6f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, false); //TODO: PARAM
    
    this->setCar(vehicle);

    for (int i = 0; i < getCar()->getNumWheels(); i++)
    {        
        btWheelInfo& wheel = getCar()->getWheelInfo(i);
        //wheel.m_wheelsDampingRelaxation = 11.7f;    //TODO: PARAM
        //wheel.m_wheelsDampingCompression = 10.7f;   //TODO: PARAM
        wheel.m_frictionSlip = btScalar(10000.);     //TODO: PARAM
        wheel.m_rollInfluence = btScalar(0.f);    //TODO: PARAM
        //wheel.m_maxSuspensionTravelCm = 15.f;       //TODO: PARAM
        
    }
}

void Kombi::updatePhysics(){
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

void Kombi::accelerate(){
    if(getCar()->getCurrentSpeedKmHour() < 100.f){
        this->getCar()->applyEngineForce(-65, 0); //TODO: Param
        this->getCar()->applyEngineForce(-65, 1);
    }
}

void Kombi::brake(){
    getCar()->setBrake(btScalar(3.5), 0);   //TODO: PARAM
    getCar()->setBrake(btScalar(3.5), 1);   //TODO: PARAM
    getCar()->setBrake(btScalar(3.5), 2);   //TODO: PARAM
    getCar()->setBrake(btScalar(3.5), 3);   //TODO: PARAM
}
void Kombi::reverse(){
    this->getCar()->applyEngineForce(50,0);    //TODO: Param
    this->getCar()->applyEngineForce(50,1);    //TODO: PARAM
}

void Kombi::turnRight(){
    if (getCar()->getSteeringValue(0)>-0.4f && getCar()->getSteeringValue(1)>-0.4f){
        getCar()->setSteeringValue(getCar()->getSteeringValue(0)-0.02f, 0); //TODO: Param
        getCar()->setSteeringValue(getCar()->getSteeringValue(1)-0.02f, 1); //TODO: PARAM
    }
    setTurned(true);
}
void Kombi::turnLeft(){
    if (getCar()->getSteeringValue(0)<0.4f && getCar()->getSteeringValue(1)<0.4f){
        getCar()->setSteeringValue(getCar()->getSteeringValue(0)+0.02f, 0); //TODO: PARAM
        getCar()->setSteeringValue(getCar()->getSteeringValue(1)+0.02f, 1); //TODO: PARAM
    }
    setTurned(true);   
}

void Kombi::fire(){}

void Kombi::spawn(){}
void Kombi::despawn(){}