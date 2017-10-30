#include "MonsterTruck.hpp"

MonsterTruck::MonsterTruck(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld* world) 
    : Car("res/meshes/truck/truck.obj", "res/meshes/truck/blank.jpg", shaderprog, btScalar(10), startPos, startRot) {
    initialize(world);

    load_mesh("res/meshes/truck/wheel.obj", wheel_vao, wheel_vert);    
    load_texture (shaderprog, "res/meshes/truck/monster_tire.jpg", wheel_tex, wheel_texLocation);    

    setHealth(100.f);
    fireRate = 5.f;
}

MonsterTruck::MonsterTruck(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btCollisionShape* coll, btDiscreteDynamicsWorld* world)
    : Car("res/meshes/truck/truck.obj", "res/meshes/truck/blank.jpg", shaderprog, btScalar(10), startPos, startRot, coll) {
    initialize(world);     
    
    load_mesh("res/meshes/truck/wheel.obj", wheel_vao, wheel_vert);    
    load_texture (shaderprog, "res/meshes/truck/monster_tire.jpg", wheel_tex, wheel_texLocation);   
    
    setHealth(100.f);
    fireRate = 5.f;
}
    
MonsterTruck::~MonsterTruck(){}

void MonsterTruck::initialize(btDiscreteDynamicsWorld* world){
    setWorld(world);
    btRaycastVehicle::btVehicleTuning* tuning = new btRaycastVehicle::btVehicleTuning();
    btVehicleRaycaster* defvehicle = new btDefaultVehicleRaycaster(world);
    getRigidBody()->setActivationState( DISABLE_DEACTIVATION);
    getRigidBody()->setUserPointer(this);
    btRaycastVehicle* vehicle = new btRaycastVehicle(*tuning, getRigidBody(), defvehicle);
    vehicle->setCoordinateSystem(0, 1, 2);
    
    world->addAction(vehicle);

    btVector3 wheelDirection(0.0f, -1.0f, 0.0f);
    btVector3 wheelAxis(-1.0f, 0.0f, 0.0f);
    btScalar suspensionRestLength(0.2f);    //TODO: PARAM
    btScalar wheelRadius(2.f);              //TOCO: PARAM 
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

    setIsAlive(true);
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
    if(getCar()->getCurrentSpeedKmHour() > 0.f){ 
        sound->reproducir(2,AL_TRUE,(getCar()->getCurrentSpeedKmHour()*0.01)+0.05);
    }else if(getCar()->getCurrentSpeedKmHour() < 0.f){
        sound->reproducir(2,AL_TRUE,((getCar()->getCurrentSpeedKmHour()*0.01)+0.05)*-1);
    }
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
    if(getCar()->getCurrentSpeedKmHour() < 150.f){
        this->getCar()->applyEngineForce(85, 0); //TODO: Param
        this->getCar()->applyEngineForce(85, 1);
    }
}
void MonsterTruck::brake(){
    getCar()->setBrake(btScalar(1.5), 0);   //TODO: PARAM
    getCar()->setBrake(btScalar(1.5), 1);   //TODO: PARAM
    getCar()->setBrake(btScalar(1.5), 2);   //TODO: PARAM
    getCar()->setBrake(btScalar(1.5), 3);   //TODO: PARAM
    sound->reproducir(1, AL_FALSE, 1.0);
}
void MonsterTruck::reverse(){
    //TODO: ADD LIMIT
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

void MonsterTruck::fire(){
    if (lastShot + (1/fireRate) < currentTime){
        btTransform trans;
        this->getRigidBody()->getMotionState()->getWorldTransform(trans);
        btVector3 start = trans.getOrigin() + btVector3(0, 5, 0); //TODO: PARAM
        btQuaternion q = trans.getRotation();
        btVector3 direction = btVector3(2 * (q.x()*q.z() + q.w()*q.y()), 2 * (q.y()*q.z() - q.w()*q.x()), 1 - 2 * (q.x()*q.x() + q.y()*q.y()));
        
        btVector3 end = start + 500*direction;//TODO: PARAM alcance maximo balas

        //getWorld()->getDebugDrawer()->drawLine(start, end, btVector3(0, 0, 0));
        btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
        getWorld()->rayTest(start, end, RayCallback);
        
        if(RayCallback.hasHit()) {
            Car* targ = (Car*)RayCallback.m_collisionObject->getUserPointer();
            if (targ != nullptr)
                targ->setHealth(targ->getHealth()-1.0f);
        }
        btVector3 gunOne = btVector3(3.1f, -0.5f, 3.5f);
        btVector3 gunTwo = btVector3(-3.1f, -0.5f, 3.5f);
        //TODO: OPTIMIZE AND INSTANTIATE
        glm::vec3 gunOne1 = glm::rotate(glm::vec3(gunOne.x(), gunOne.y(), gunOne.z()), trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().x(), trans.getRotation().getAxis().y(), trans.getRotation().getAxis().z()));
        glm::vec3 gunTwo2 = glm::rotate(glm::vec3(gunTwo.x(), gunTwo.y(), gunTwo.z()), trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().x(), trans.getRotation().getAxis().y(), trans.getRotation().getAxis().z()));
        if (RayCallback.hasHit()){
            particleManager->genGunshot(btVector3(start.x() + gunOne1.x, start.y() + gunOne1.y, start.z() + gunOne1.z), RayCallback.m_hitPointWorld);
            particleManager->genGunshot(btVector3(start.x() + gunTwo2.x, start.y() + gunTwo2.y, start.z() + gunTwo2.z), RayCallback.m_hitPointWorld);
        }else{
            particleManager->genGunshot(btVector3(start.x() + gunOne1.x, start.y() + gunOne1.y, start.z() + gunOne1.z), end);
            particleManager->genGunshot(btVector3(start.x() + gunTwo2.x, start.y() + gunTwo2.y, start.z() + gunTwo2.z), end);
        }
        lastShot = currentTime;
    }
}

void MonsterTruck::spawn(){}
void MonsterTruck::despawn(btDiscreteDynamicsWorld* world){

}