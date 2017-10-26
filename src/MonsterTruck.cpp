#include "MonsterTruck.hpp"

MonsterTruck::MonsterTruck(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld* world) 
    : Car("res/meshes/truck/truck.obj", "res/meshes/truck/blank.jpg", shaderprog, btScalar(10), startPos, startRot) {
    initialize(world);

    load_mesh("res/meshes/truck/wheel.obj", wheel_vao, wheel_vert);    
    load_texture (shaderprog, "res/meshes/truck/monster_tire.jpg", wheel_tex, wheel_texLocation);    
}

MonsterTruck::MonsterTruck(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btCollisionShape* coll, btDiscreteDynamicsWorld* world)
    : Car("res/meshes/truck/truck.obj", "res/meshes/truck/blank.jpg", shaderprog, btScalar(10), startPos, startRot, coll) {
    initialize(world);     
    
    load_mesh("res/meshes/truck/wheel.obj", wheel_vao, wheel_vert);    
    load_texture (shaderprog, "res/meshes/truck/monster_tire.jpg", wheel_tex, wheel_texLocation);    
}
    
MonsterTruck::~MonsterTruck(){}

void MonsterTruck::initialize(btDiscreteDynamicsWorld* world){
    btRaycastVehicle::btVehicleTuning* tuning = new btRaycastVehicle::btVehicleTuning();
    btVehicleRaycaster* defvehicle = new btDefaultVehicleRaycaster(world);
    getRigidBody()->setActivationState( DISABLE_DEACTIVATION);
    btRaycastVehicle* vehicle = new btRaycastVehicle(*tuning, getRigidBody(), defvehicle);
    vehicle->setCoordinateSystem(0, 1, 2);
    
    world->addVehicle(vehicle);

    btVector3 wheelDirection(0.0f, -1.0f, 0.0f);
    btVector3 wheelAxis(-1.0f, 0.0f, 0.0f);
    btScalar suspensionRestLength(0.2f);    //TODO: PARAM
    btScalar wheelRadius(2.42f);              //TOCO: PARAM 
    vehicle->addWheel(btVector3(-1.f*2.42f, 0.94f*2.f, 1.98f*2.f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, true);//TODO: PARAM
    vehicle->addWheel(btVector3(1.f*2.42f, 0.94f*2.f, 1.98f*2.f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, true); //TODO: PARAM
    vehicle->addWheel(btVector3(1.f*2.42f, 0.94f*2.f, -2.11f*2.f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, false);  //TODO: PARAM
    vehicle->addWheel(btVector3(-1.f*2.42f, 0.94f*2.f, -2.11f*2.f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, false); //TODO: PARAM
    
    this->setCar(vehicle);
    for (int i = 0; i < getCar()->getNumWheels(); i++)
    {        
        btWheelInfo& wheel = getCar()->getWheelInfo(i);
        //wheel.m_wheelsDampingRelaxation = 1.7f;    //TODO: PARAM
        //wheel.m_wheelsDampingCompression = -1.7f;   //TODO: PARAM
        wheel.m_frictionSlip = btScalar(10000.);     //TODO: PARAM
        wheel.m_rollInfluence = btScalar(0.f);    //TODO: PARAM
        wheel.m_maxSuspensionTravelCm = 100.f;       //TODO: PARAM
    }
}

void MonsterTruck::updatePhysics(){
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

void MonsterTruck::draw(GLuint model_mat_location){
    btTransform trans;
    glm::mat4 model;
    this->getRigidBody()->getMotionState()->getWorldTransform(trans);
        
    model = glm::translate(glm::mat4(), glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    model = glm::rotate(model, trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ() ));
    btVector3 escala = getRigidBody()->getCollisionShape()->getLocalScaling();
    model = glm::scale(model, glm::vec3(escala.getX(), escala.getY(), escala.getZ()));

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
        model = glm::scale(model, glm::vec3(escala.getX(), escala.getY(), escala.getZ()));
        glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &model[0][0]);

        glActiveTexture (GL_TEXTURE0);
        glBindTexture (GL_TEXTURE_2D, wheel_tex);
        glUniform1i (wheel_texLocation, 0);

        glBindVertexArray(wheel_vao);
        glDrawArrays(GL_TRIANGLES, 0, wheel_vert); 
    }
}
void MonsterTruck::accelerate(){
    if(getCar()->getCurrentSpeedKmHour() < 100.f){
        this->getCar()->applyEngineForce(85, 0); //TODO: Param
        this->getCar()->applyEngineForce(85, 1);
    }
}
void MonsterTruck::brake(){
    getCar()->setBrake(btScalar(1.5), 0);   //TODO: PARAM
    getCar()->setBrake(btScalar(1.5), 1);   //TODO: PARAM
    getCar()->setBrake(btScalar(1.5), 2);   //TODO: PARAM
    getCar()->setBrake(btScalar(1.5), 3);   //TODO: PARAM
    sound->reproducir(1);
}
void MonsterTruck::reverse(){
    this->getCar()->applyEngineForce(-50,0);    //TODO: Param
    this->getCar()->applyEngineForce(-50,1);    //TODO: PARAM
}

void MonsterTruck::turnRight(){
    if (getCar()->getSteeringValue(0)>-0.4f && getCar()->getSteeringValue(1)>-0.4f){
        getCar()->setSteeringValue(getCar()->getSteeringValue(0)-0.02f, 0); //TODO: Param
        getCar()->setSteeringValue(getCar()->getSteeringValue(1)-0.02f, 1); //TODO: PARAM
    }
    setTurned(true);
}
void MonsterTruck::turnLeft(){
    if (getCar()->getSteeringValue(0)<0.4f && getCar()->getSteeringValue(1)<0.4f){
        getCar()->setSteeringValue(getCar()->getSteeringValue(0)+0.02f, 0); //TODO: PARAM
        getCar()->setSteeringValue(getCar()->getSteeringValue(1)+0.02f, 1); //TODO: PARAM
    }
    setTurned(true);   
}

void MonsterTruck::fire(){}

void MonsterTruck::spawn(){}
void MonsterTruck::despawn(){}