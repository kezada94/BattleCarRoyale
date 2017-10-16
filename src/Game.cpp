#include "Game.hpp"

Game::Game(){
    level = new Scene();
    camera = new Camera();
}
Game::~Game(){
    delete level;
    //delete camera;
    if (inputProcessor != nullptr){
        delete inputProcessor;
    }
}    

void Game::init(){
    shader_programme = create_programme_from_files (VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
	glUseProgram (shader_programme);
    model_mat_location  = glGetUniformLocation (shader_programme, "model");

    camera->init(shader_programme, g_gl_width, g_gl_height, fov, CameraModes::THIRD_PERSON);

    DynamicGameObject *player = new DynamicGameObject("res/meshes/kombi3.obj", "res/textures/kombi3.jpg", shader_programme, btScalar(1), btVector3(0, 5, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    DynamicGameObject *mono1 = new DynamicGameObject("res/meshes/suzanne.obj", "res/textures/default.jpg", shader_programme, btScalar(2), btVector3(0, 10, 10), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    DynamicGameObject *mono2 = new DynamicGameObject("res/meshes/suzanne.obj", "res/textures/default.jpg", shader_programme, btScalar(2), btVector3(10, 10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    DynamicGameObject *cilindro = new DynamicGameObject("res/meshes/cilindro.obj", "res/textures/rueda/wheel.png", shader_programme, btScalar(2), btVector3(5, 5, 5), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    StaticGameObject *piso = new StaticGameObject("res/textures/piso/suelo2.obj", "res/textures/piso/what.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    Kombi* kombi = new Kombi(btVector3(10, 5, 10), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    Patriot* patriot = new Patriot(btVector3(0, 5, 0), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    enemiesCount = 1;
    
    piso->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(30, 10, 30));
    
    level->addGameObject(kombi);
    level->addGameObject(patriot);

    level->addGameObject(mono1);
    level->addGameObject(mono2);
    level->addGameObject(cilindro);
    level->addGameObject(piso);
    


    camera->setTarget(kombi);
    level->setPlayer(kombi);

    inputProcessor = new InputProcessor(g_window, camera, kombi);

}
void Game::doMainLoop(){
    while (!glfwWindowShouldClose(g_window)){
        checkStatus();
        inputProcessor->processInput();

        _update_fps_counter(g_window);
        glClearColor(0.0f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        level->stepSimulation(1.f / 60.f, 10); 
        level->drawAllGameObjects(model_mat_location);
        camera->update();

               
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
}

void Game::checkStatus(){
    if (level->getPlayer()->getHealth() <=0){
        loose();
    }
    for (int i=0; i < level->getCars().size(); i++){
        if (level->getCars().at(i)->getHealth() <= 0){
            level->getCars().at(i)->setIsAlive(false);
            level->getCars().at(i)->despawn();
            enemiesCount--;
        }
    }
    if(enemiesCount == 0){
        win();
    }
}
void Game::win(){
    //DO WIN
}
void Game::loose(){
    //DO LOOSE
}
        