#include "Scene.hpp"

Scene::Scene(){
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-9.81f,0));
    
    objects = new SceneGameObjectLists();

}
Scene::~Scene(){
    delete broadphase;
    delete collisionConfiguration;
    delete dispatcher;
    delete solver;
    delete dynamicsWorld;
    delete objects;
}
void Scene::init(GLuint shader){
    posLoc = glGetUniformLocation(shader, "lightPos");
    dirLoc = glGetUniformLocation(shader, "lightDir");
    pwrLoc = glGetUniformLocation(shader, "lightPwr");
    lightAngle = glGetUniformLocation(shader, "lightAngle");
    numberLoc = glGetUniformLocation(shader, "nLights");
}

void Scene::drawAllGameObjects(const GLuint mat_location, const GLuint shader){
    glUseProgram(shader);
    btTransform trans;
    player->getRigidBody()->getMotionState()->getWorldTransform(trans);

    glm::vec3 lightpos1(player->model * glm::vec4(player->frontLight1->getLocation(), 1));
    glm::vec3 lightpos2(player->model * glm::vec4(player->frontLight2->getLocation(), 1));

    glm::vec3 lightpwr1(player->model * glm::vec4(player->frontLight1->getIntensity(), 1));
    glm::vec3 lightpwr2(player->model * glm::vec4(player->frontLight2->getIntensity(), 1));

    btQuaternion q = trans.getRotation();
    btVector3 direction = btVector3(2 * (q.x()*q.z() + q.w()*q.y()), 2 * (q.y()*q.z() - q.w()*q.x()), 1 - 2 * (q.x()*q.x() + q.y()*q.y()));
    

    glm::vec3 positions[2] = {lightpos1, lightpos2};
    btVector3 directions[2] = {direction, direction};
    glm::vec3 angles[2] = {glm::vec3(30.f, 0.f, 0.f), glm::vec3(30.f, 0.f, 0.f)};
    glm::vec3 power[2] = {lightpwr1, lightpwr2};
    glUniform3fv(posLoc, 2, (const GLfloat*)positions);
    glUniform3fv(dirLoc, 2, (const GLfloat*)directions);
    glUniform3fv(pwrLoc, 2, (const GLfloat*)power);
    glUniform3fv(lightAngle, 2, (const GLfloat*)angles);
    glUniform1i(numberLoc, 2);


    for (int i = 0; i < objects->size(); i++){
        objects->get(i)->draw(mat_location);
    }
    for (int i = 0; i < cars.size(); i++){
        if (cars[i]->getIsAlive()){
            cars[i]->draw(mat_location);
        }
    }
}
void Scene::updateAllCarsPhysics(){
    for (int i = 0; i < cars.size(); i++){
        cars[i]->updatePhysics();
    }
}

void Scene::addLight(Light* lght){
    this->lights.push_back(lght);
}

void Scene::addGameObject(GameObject* obj){
    if(dynamic_cast<Car*>(obj)){
        cars.push_back(dynamic_cast<Car*>(obj));
    }else{
        this->objects->addGameObject(obj);
    }
    this->dynamicsWorld->addRigidBody(obj->getRigidBody());
}
void Scene::stepSimulation(float freq, int skips){
    this->dynamicsWorld->stepSimulation(freq, skips);
}
btDiscreteDynamicsWorld* Scene::getDynamicsWorld(){
    return this->dynamicsWorld;
}

void Scene::setPlayer(Car* play){
    this->player = play;
}

Car* Scene::getPlayer(){
    return this->player;
}
CarList Scene::getCars(){
    return this->cars;
}

