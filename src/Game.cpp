#include "Game.hpp"

Game::Game(){
    level = new Scene();
    camera = new Camera();
}
Game::~Game(){
    delete level;
    delete camera;
    if (inputProcessor != nullptr){
        delete inputProcessor;
    }
}    

void Game::init(){

    shader_programme = create_programme_from_files (VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
	glUseProgram (shader_programme);
    model_mat_location  = glGetUniformLocation (shader_programme, "model");

    
    GLDebugDrawer* debug = new GLDebugDrawer();
    debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    camera->deb = debug;
    camera->init(shader_programme, g_gl_width, g_gl_height, fov, CameraModes::THIRD_PERSON);    
    level->getDynamicsWorld()->setDebugDrawer(debug);

    //Creacion de objetos de la escena
    StaticGameObject *piso = new StaticGameObject("res/textures/piso/suelo2.obj", "res/textures/piso/what.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    Kombi* kombi = new Kombi(btVector3(10, 24, 10), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    Patriot* patriot = new Patriot(btVector3(0, 24, 0), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    MonsterTruck* monster = new MonsterTruck(btVector3(20, 30, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    
    enemiesCount = 1;

    piso->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(30, 45, 30));
    monster->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(2.42f, 2, 2));
    
    //Se agregan los objetos la escena
    level->addGameObject(kombi);
    level->addGameObject(monster);
    level->addGameObject(patriot);
    level->addGameObject(piso);
    
    //Se settea el auto que será el jugador.
    camera->setTarget(monster);
    level->setPlayer(monster);

    inputProcessor = new InputProcessor(g_window, camera, monster);
}
void Game::doMainLoop(){
    glLineWidth(7);
    glEnable(GL_LINE_SMOOTH);
    while (!glfwWindowShouldClose(g_window)){

        checkStatus();
        inputProcessor->processInput();

        _update_fps_counter(g_window);
        glClearColor(0.0f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Dibuja todos los objetos de la escena
        level->drawAllGameObjects(model_mat_location);

        // Dibuja todos las figuras colisionadoras de los objetos
        level->getDynamicsWorld()->debugDrawWorld();
        camera->deb->drawLines();

        level->stepSimulation(1.f / 60.f, 10); 

        level->updateAllCarsPhysics();    
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
        