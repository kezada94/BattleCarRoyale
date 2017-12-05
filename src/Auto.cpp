#include "Auto.hpp"

Auto::Auto(btVector3 startPos, btQuaternion startRot, GLuint shaderprog, btDiscreteDynamicsWorld *world, GLuint specular_loc)
    : Car("res/racing/auto.obj", "res/racing/auto.png", "res/racing/auto_NRM.png", shaderprog, btScalar(10), startPos, startRot, glm::vec3(1, 1, 1), specular_loc)
{
    initialize(world);

    load_mesh("res/racing/rueda.obj", wheel_vao, wheel_vert);
    load_texture2(shaderprog, "res/racing/rueda.png", wheel_tex, wheel_texLocation);

    setHealth(25.f);
    fireRate = 5.f;
}

Auto::~Auto() {}

void Auto::initialize(btDiscreteDynamicsWorld *world)
{
    setWorld(world);
    btRaycastVehicle::btVehicleTuning *tuning = new btRaycastVehicle::btVehicleTuning();
    btVehicleRaycaster *defvehicle = new btDefaultVehicleRaycaster(world);
    getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
    getRigidBody()->setUserPointer(this);
    btRaycastVehicle *vehicle = new btRaycastVehicle(*tuning, getRigidBody(), defvehicle);
    vehicle->setCoordinateSystem(0, 1, 2);

    world->addAction(vehicle);

    btVector3 wheelDirection(0.0f, -1.0f, 0.0f);
    btVector3 wheelAxis(-1.0f, 0.0f, 0.0f);
    btScalar suspensionRestLength(0.2f);                                                                                                                 //TODO: PARAM
    btScalar wheelRadius(2.f);                                                                                                                           //TOCO: PARAM
    vehicle->addWheel(btVector3(-5.6f, 1.69f, 7.3), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, true);   //TODO: PARAM
    vehicle->addWheel(btVector3(3.f, 1.69f, 7.3), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, true);    //TODO: PARAM
    vehicle->addWheel(btVector3(-5.6f, 1.69f, -6.32f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, false);  //TODO: PARAM
    vehicle->addWheel(btVector3(3.f, 1.69f, -6.32f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, *tuning, false); //TODO: PARAM

    this->setCar(vehicle);
    for (int i = 0; i < getCar()->getNumWheels(); i++)
    {
        btWheelInfo &wheel = getCar()->getWheelInfo(i);
        //wheel.m_wheelsDampingRelaxation = 1.7f;    //TODO: PARAM
        //wheel.m_wheelsDampingCompression = -1.7f;   //TODO: PARAM
        wheel.m_frictionSlip = btScalar(10000.); //TODO: PARAM
        wheel.m_rollInfluence = btScalar(0.f);   //TODO: PARAM
        wheel.m_maxSuspensionTravelCm = 100.f;   //TODO: PARAM
    }
    setIsAlive(true);

    frontLight1 = new Spotlight(glm::vec3(0.59f, 2.13f, 2.88f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0, 0, -1.f), 15.f);
    frontLight2 = new Spotlight(glm::vec3(-0.59f, 2.13f, 2.88f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0, 0, -1.f), 15.f);
}

void Auto::updatePhysics()
{
    if (!getTurned())
    {
        if (getCar()->getSteeringValue(0) > 0)
        {
            getCar()->setSteeringValue(getCar()->getSteeringValue(0) - 0.030f, 0); //TODO: PARAM
            getCar()->setSteeringValue(getCar()->getSteeringValue(1) - 0.030f, 1); //TODO: PARAM
            if (getCar()->getSteeringValue(0) < 0)
            {
                getCar()->setSteeringValue(0, 0);
                getCar()->setSteeringValue(0, 1);
            }
        }
        else if (getCar()->getSteeringValue(0) < 0)
        {
            getCar()->setSteeringValue(getCar()->getSteeringValue(0) + 0.03f, 0); //TODO: PARAM
            getCar()->setSteeringValue(getCar()->getSteeringValue(1) + 0.03f, 1); //TODO: PARAM
            if (getCar()->getSteeringValue(0) > 0)
            {
                getCar()->setSteeringValue(0, 0);
                getCar()->setSteeringValue(0, 1);
            }
        }
    }
    else
    {
        setTurned(false);
    }
    getCar()->setBrake(0, 0);
    getCar()->setBrake(0, 1);
    getCar()->setBrake(0, 2);
    getCar()->setBrake(0, 3);
    if (getCar()->getCurrentSpeedKmHour() > 0.f)
    {
        sound->reproducir(2, AL_TRUE, (getCar()->getCurrentSpeedKmHour() * 0.01) + 0.05);
    }
    else if (getCar()->getCurrentSpeedKmHour() < 0.f)
    {
        sound->reproducir(2, AL_TRUE, ((getCar()->getCurrentSpeedKmHour() * 0.01) + 0.05) * -1);
    }
    this->getCar()->applyEngineForce(0, 0); //TODO: Param
    this->getCar()->applyEngineForce(0, 1);
}

void Auto::draw(GLuint model_mat_location)
{
    btTransform trans;
    this->getRigidBody()->getMotionState()->getWorldTransform(trans);

    model = glm::translate(glm::mat4(), glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    model = glm::rotate(model, trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ()));
    btVector3 escala = getRigidBody()->getCollisionShape()->getLocalScaling();
    model = glm::scale(model, glm::vec3(escala.getX(), escala.getY(), escala.getZ()));

    glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &model[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->getTexture());
    glUniform1i(tex_location, 0);

    glBindVertexArray(this->getVao());
    glDrawArrays(GL_TRIANGLES, 0, this->getVertNumber());

    for (int i = 0; i < this->getCar()->getNumWheels(); i++)
    {
        trans = this->getCar()->getWheelInfo(i).m_worldTransform;
        glm::quat hele = glm::angleAxis(trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ()));
        glm::mat4 model2 = glm::toMat4(hele);
        glm::mat4 model3 = glm::translate(glm::mat4(), glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));

        model3 = model3 * model2;
        model3 = glm::scale(model3, glm::vec3(escala.getX()*2, escala.getY()*2, escala.getZ()*2));
        glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &model3[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wheel_tex);
        glUniform1i(wheel_texLocation, 0);

        glBindVertexArray(wheel_vao);
        glDrawArrays(GL_TRIANGLES, 0, wheel_vert);
    }
}
void Auto::accelerate()
{
    if (getCar()->getCurrentSpeedKmHour() < 150.f)
    {
        this->getCar()->applyEngineForce(85, 0); //TODO: Param
        this->getCar()->applyEngineForce(85, 1);
    }
}
void Auto::brake()
{
    getCar()->setBrake(btScalar(1.5), 0); //TODO: PARAM
    getCar()->setBrake(btScalar(1.5), 1); //TODO: PARAM
    getCar()->setBrake(btScalar(1.5), 2); //TODO: PARAM
    getCar()->setBrake(btScalar(1.5), 3); //TODO: PARAM
    if (getCar()->getCurrentSpeedKmHour() > 125.f)
    {
        sound->reproducir(1, AL_FALSE, 1.0);
    }
}
void Auto::reverse()
{
    //TODO: ADD LIMIT
    this->getCar()->applyEngineForce(-50, 0); //TODO: Param
    this->getCar()->applyEngineForce(-50, 1); //TODO: PARAM
}

void Auto::turnRight()
{
    if (getCar()->getSteeringValue(0) > -0.4f && getCar()->getSteeringValue(1) > -0.4f)
    {
        getCar()->setSteeringValue(getCar()->getSteeringValue(0) - 0.02f, 0); //TODO: Param
        getCar()->setSteeringValue(getCar()->getSteeringValue(1) - 0.02f, 1); //TODO: PARAM
    }
    setTurned(true);
}
void Auto::turnLeft()
{
    if (getCar()->getSteeringValue(0) < 0.4f && getCar()->getSteeringValue(1) < 0.4f)
    {
        getCar()->setSteeringValue(getCar()->getSteeringValue(0) + 0.02f, 0); //TODO: PARAM
        getCar()->setSteeringValue(getCar()->getSteeringValue(1) + 0.02f, 1); //TODO: PARAM
    }
    setTurned(true);
}

void Auto::fire()
{
    if (lastShot + (1 / fireRate) < currentTime)
    {
        btTransform trans;
        this->getRigidBody()->getMotionState()->getWorldTransform(trans);
        btVector3 start = trans.getOrigin() + btVector3(0, 5, 0); //TODO: PARAM
        btQuaternion q = trans.getRotation();
        btVector3 direction = btVector3(2 * (q.x() * q.z() + q.w() * q.y()), 2 * (q.y() * q.z() - q.w() * q.x()), 1 - 2 * (q.x() * q.x() + q.y() * q.y()));

        btVector3 end = start + 500 * direction; //TODO: PARAM alcance maximo balas

        getWorld()->getDebugDrawer()->drawLine(start, end, btVector3(0, 0, 0));
        btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
        getWorld()->rayTest(start, end, RayCallback);

        if (RayCallback.hasHit())
        {
            Car *targ = (Car *)RayCallback.m_collisionObject->getUserPointer();
            if (targ != nullptr)
                targ->setHealth(targ->getHealth() - 1.0f);
        }
        glm::vec4 gunOne = glm::vec4(3.35f, 2.34f, 2.52f, 1.f);
        glm::vec4 gunTwo = glm::vec4(-3.35f, 2.34f, 2.52f, 1.f);

        //TODO: OPTIMIZE AND INSTANTIATE
        glm::vec3 gunOne1 = model * gunOne;
        glm::vec3 gunTwo2 = model * gunTwo;

        if (RayCallback.hasHit())
        {
            particleManager->genGunshot(btVector3(gunOne1.x, gunOne1.y, gunOne1.z), RayCallback.m_hitPointWorld);
            particleManager->genGunshot(btVector3(gunTwo2.x, gunTwo2.y, gunTwo2.z), RayCallback.m_hitPointWorld);
        }
        else
        {
            particleManager->genGunshot(btVector3(gunOne1.x, gunOne1.y, gunOne1.z), end);
            particleManager->genGunshot(btVector3(gunTwo2.x, gunTwo2.y, gunTwo2.z), end);
        }
        lastShot = currentTime;
        sound->reproducir(3, AL_FALSE, 1.0);
    }
}

void Auto::spawn() {}
void Auto::despawn(btDiscreteDynamicsWorld *world)
{
    btCollisionShape *col = getRigidBody()->getCollisionShape();
    btRigidBody *rb = getRigidBody();

    world->removeCollisionObject(rb);
    world->removeAction(car);
}