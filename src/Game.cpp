#include "Game.hpp"
#include <iostream>

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
    glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);    
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    
    shader_programme = create_programme_from_files (VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
	glUseProgram (shader_programme);
    model_mat_location  = glGetUniformLocation (shader_programme, "model");
    
    soundManager = new SoundManager();
    particleManager = new ParticleManager();

    GLDebugDrawer* debug = new GLDebugDrawer();    
    debug->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE );
    
    camera->debugDrawer = debug;
    camera->particleManager = particleManager;
    
    camera->init(shader_programme, g_gl_width, g_gl_height, fov, CameraModes::THIRD_PERSON);    

    level->getDynamicsWorld()->setDebugDrawer(debug);

    //Creacion de objetos de la escena
    StaticGameObject *piso = new StaticGameObject("res/textures/piso/suelo2.obj", "res/textures/piso/what.png", "res/textures/piso/what_NRM.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    Kombi* kombi = new Kombi(btVector3(10, 24, 10), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    Patriot* patriot = new Patriot(btVector3(0, 24, 0), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    MonsterTruck* monster = new MonsterTruck(btVector3(20, 30, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    monster->setSoundManager(soundManager);
    monster->particleManager = particleManager;
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
    //glLineWidth(7);
    glEnable(GL_LINE_SMOOTH);
    soundManager->musicaFondo();
    int frameCount = 0;
    
    while (!glfwWindowShouldClose(g_window)){

        currentTime = glfwGetTime();
        frameCount++;        
        deltaTime = currentTime - lastTime;
        if (deltaTime >= 1.0 ){ 
            // printf and reset timer
            printf("%f ms\n", 1000.0/double(frameCount));
            frameCount = 0;
            lastTime += 1.0;
        }
        checkStatus();
        inputProcessor->processInput();
        
        glClearColor(0.0f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Dibuja todos los objetos de la escena
        level->drawAllGameObjects(model_mat_location, shader_programme);

        // Dibuja todos las figuras colisionadoras de los objetos
        level->getDynamicsWorld()->debugDrawWorld();
        camera->debugDrawer->drawLines();


        level->stepSimulation(1.f / 60.f, 0); 

        level->updateAllCarsPhysics();    
        camera->update();
        particleManager->drawActiveParticles();        
        
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
}

void Game::checkStatus(){
    if (level->getPlayer()->getHealth() <=0){
        loose();
    }
    for (int i=0; i < level->getCars().size(); i++){
        if (level->getCars().at(i)->getHealth() <= 0.0f){
            level->getCars().at(i)->setIsAlive(false);
            level->getCars().at(i)->despawn();
            //level->
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
        