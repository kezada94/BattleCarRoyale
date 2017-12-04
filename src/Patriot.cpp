#include "Patriot.hpp"

Patriot::Patriot(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld* world, GLuint specular_loc) 
    : Car("res/patriot.obj", "res/Ambulance.png", nullptr, shaderprog, btScalar(10), startPos, startRot, glm::vec3(1,1,1), specular_loc) {
        initialize(world);

        load_mesh("res/textures/rueda/rueda.obj", wheel_vao, wheel_vert);
        load_texture2 (shaderprog, "res/meshes/truck/monster_tire.jpg", wheel_tex, wheel_texLocation);   

        setHealth(25.f);
        fireRate = 5.f;

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
    btVector3 wheelAxis(-1.0f, 0.0f, 0.0f);
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
    

    frontLight1 = new Spotlight(glm::vec3(0.59f, 2.13f, 2.88f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0, 0, -1.f), 15.f);
    frontLight2 = new Spotlight(glm::vec3(-0.59f, 2.13f, 2.88f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0, 0, -1.f), 15.f);
    
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
    if(getCar()->getCurrentSpeedKmHour() > 0.f){ 
        sound->reproducir(2,AL_TRUE,(getCar()->getCurrentSpeedKmHour()*0.01)+0.05);
    }else if(getCar()->getCurrentSpeedKmHour() < 0.f){
        sound->reproducir(2,AL_TRUE,((getCar()->getCurrentSpeedKmHour()*0.01)+0.05)*-1);
    }
    this->getCar()->applyEngineForce(0, 0); //TODO: Param
    this->getCar()->applyEngineForce(0, 1);

}
void Patriot::draw(GLuint model_mat_location){
    btTransform trans;
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
        glm::mat4 model3 = glm::translate(glm::mat4(), glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));

        model3 = model3 * model2;
        model3 = glm::scale(model3, glm::vec3(escala.getX(), escala.getY(), escala.getZ()));
        glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &model3[0][0]);

        glActiveTexture (GL_TEXTURE0);
        glBindTexture (GL_TEXTURE_2D, wheel_tex);
        glUniform1i (wheel_texLocation, 0);

        glBindVertexArray(wheel_vao);
        glDrawArrays(GL_TRIANGLES, 0, wheel_vert); 
    }
}

void Patriot::accelerate(){
    if(getCar()->getCurrentSpeedKmHour() < 150.f){
        this->getCar()->applyEngineForce(85, 0); //TODO: Param
        this->getCar()->applyEngineForce(85, 1);
    }
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
    if(getCar()->getCurrentSpeedKmHour() > 125.f){
        sound->reproducir(1, AL_FALSE, 1.0);
    }
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

void Patriot::fire(){
    if (lastShot + (1/fireRate) < currentTime){
        btTransform trans;
        this->getRigidBody()->getMotionState()->getWorldTransform(trans);
        btVector3 start = trans.getOrigin() + btVector3(0, 5, 0); //TODO: PARAM
        btQuaternion q = trans.getRotation();
        btVector3 direction = btVector3(2 * (q.x()*q.z() + q.w()*q.y()), 2 * (q.y()*q.z() - q.w()*q.x()), 1 - 2 * (q.x()*q.x() + q.y()*q.y()));
        
        start += 2*(direction+ btVector3(0, -1, 0));
        btVector3 end = start + 500*direction;//TODO: PARAM alcance maximo balas

        getWorld()->getDebugDrawer()->drawLine(start, end, btVector3(0, 0, 0));
        btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
        getWorld()->rayTest(start, end, RayCallback);
        
        if(RayCallback.hasHit()) {
            Car* targ = (Car*)RayCallback.m_collisionObject->getUserPointer();
            if (targ != nullptr)
                targ->setHealth(targ->getHealth()-1.0f);
        }
        glm::vec4 gunOne = glm::vec4(0.f, 2.34f, 2.52f, 1.f);
        
        

        //TODO: OPTIMIZE AND INSTANTIATE
        glm::vec3 gunOne1 = model * gunOne;

        if (RayCallback.hasHit()){
            particleManager->genGunshot(btVector3(gunOne1.x, gunOne1.y, gunOne1.z), RayCallback.m_hitPointWorld);
        }else{
            particleManager->genGunshot(btVector3(gunOne1.x, gunOne1.y, gunOne1.z), end);
        }
        lastShot = currentTime;
        sound->reproducir(3,AL_FALSE,1.0);
    }
}

void Patriot::spawn(){}
void Patriot::despawn(btDiscreteDynamicsWorld* world){
    btCollisionShape* col = getRigidBody()->getCollisionShape();
    btRigidBody* rb = getRigidBody();

    world->removeCollisionObject(rb);
        world->removeAction(car);

}