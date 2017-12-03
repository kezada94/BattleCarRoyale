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
    depthProj = glGetUniformLocation(shader, "depthProj");
    depthView = glGetUniformLocation(shader, "depthView");
    shadowMapID = glGetUniformLocation(shader, "shadowMap");

    shadow_shader = create_programme_from_files("res/shaders/shadow.vert","res/shaders/shadow.frag");

    depthProjID = glGetUniformLocation(shadow_shader, "proj");
    depthViewID = glGetUniformLocation(shadow_shader, "view");
    depthModelID = glGetUniformLocation(shadow_shader, "model");

    FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    glDrawBuffer(GL_NONE); // No color buffer is drawn to.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


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

    

		glUseProgram(shadow_shader);        
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0,0,1024,1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// We don't use bias in the shader, but instead we draw back faces, 
		// which are already separated from the front faces by a small distance 
		//glCullFace(GL_FRONT);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
        glm::vec3 lightDirection_world = glm::vec3(0.603472, -0.794415, 0.068758); //Sun light (directional)
        //glm::vec3 lightDirection_world = glm::normalize(glm::vec3(0, -1, 0)); //Sun light (directional)
        glm::vec3 dir = glm::vec3(direction.x()*25, direction.y()*25, direction.z()*25);
        // Compute the MVP matrix from the light's point of view
        glm::mat4 depthProjectionMatrix = glm::ortho<float>(-300,300,-300,300,-100,500);
        glm::mat4 depthViewMatrix = glm::lookAt(lightpos1+dir+glm::vec3(0,10,5), lightpos1+dir+glm::vec3(0,10,5)+lightDirection_world, glm::vec3(0,1,0));

        glUniformMatrix4fv(depthViewID, 1, GL_FALSE, &depthViewMatrix[0][0]);
        glUniformMatrix4fv(depthProjID, 1, GL_FALSE, &depthProjectionMatrix[0][0]);
        for (int i = 0; i < objects->size(); i++){
            objects->get(i)->draw(depthModelID);
        }
        for (int i = 0; i < cars.size(); i++){
            if (cars[i]->getIsAlive()){
                cars[i]->draw(depthModelID);
            }
        }
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform

		glCullFace(GL_BACK);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(shader);

    glViewport(0,0, 1366, 768); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(shadowMapID, 4);
    
    glUniformMatrix4fv(depthView, 1, GL_FALSE, &depthViewMatrix[0][0]);
    glUniformMatrix4fv(depthProj, 1, GL_FALSE, &depthProjectionMatrix[0][0]);
    
    //glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &depthViewMatrix[0][0]);
    //glUniformMatrix4fv(glGetUniformLocation(shader, "proj"), 1, GL_FALSE, &depthProjectionMatrix[0][0]);
    
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

