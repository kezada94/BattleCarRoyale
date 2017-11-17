#include "Game.hpp"
#include "stb_image.h"

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
    glfwSwapInterval(1);  
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    
    shader_programme = create_programme_from_files (VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
	glUseProgram (shader_programme);
    model_mat_location  = glGetUniformLocation (shader_programme, "model");
    level->init(shader_programme);
    soundManager = new SoundManager();
    particleManager = new ParticleManager();
    skybox = new Skybox();

    GLDebugDrawer* debug = new GLDebugDrawer();    
    debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe );
    
    camera->debugDrawer = debug;
    camera->particleManager = particleManager;
    camera->skybox = skybox;
    
    camera->init(shader_programme, g_gl_width, g_gl_height, fov, CameraModes::FIRST_PERSON);    

    level->getDynamicsWorld()->setDebugDrawer(debug);

    //Creacion de objetos de la escena
    StaticGameObject *piso = new StaticGameObject("res/suelo/suelo.obj", "res/suelo/suelo.jpg", "res/suelo/suelo_NRM.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    StaticGameObject *tuneles = new StaticGameObject("res/tunel/tunel.obj", "res/tunel/tetete.png", "res/tunel/tetete_NRM.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    StaticGameObject *tubos = new StaticGameObject("res/tuneles/tubos.obj", "res/tuneles/tex.jpg", nullptr, shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    StaticGameObject *pared = new StaticGameObject("res/pared/pared.obj", "res/pared/tex.jpg", "res/pared/tex_NRM.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    StaticGameObject *esfera = new StaticGameObject("res/untitled.obj", "res/tex.png", "res/tex_NRM.png", shader_programme, btVector3(50, 50, 50), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    StaticGameObject *luz = new StaticGameObject("res/untitled.obj", "res/tex.png", "res/tex_NRM.png", shader_programme, btVector3(30, 100, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    DynamicGameObject* cono = new DynamicGameObject("res/cono/cono.obj", "res/cono/conotextura.png", shader_programme, btScalar(1), btVector3(10, 50, 10), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    DynamicGameObject* barril = new DynamicGameObject("res/barril/barril.obj", "res/barril/barriltextura.jpg", shader_programme, btScalar(3), btVector3(40, 50, 40), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    Kombi* kombi = new Kombi(btVector3(10, 24, 10), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    Patriot* patriot = new Patriot(btVector3(0, 24, 0), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    MonsterTruck* monster = new MonsterTruck(btVector3(20, 30, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld());
    monster->setSoundManager(soundManager);
    monster->particleManager = particleManager;
    enemiesCount = 2;
    
    piso->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(40, 40, 40));
    tuneles->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(40, 40, 40));
    tubos->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(40, 40,40));
    pared->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(40, 40, 40));
    //esfera->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(30, 30, 30));
    monster->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(2.42f, 2, 2));

    //Se agregan los objetos la escena
    level->addGameObject(kombi);
    level->addGameObject(monster);
    level->addGameObject(patriot);
    level->addGameObject(piso);
    level->addGameObject(esfera);
    level->addGameObject(luz);
    level->addGameObject(tuneles);
    level->addGameObject(tubos);
    level->addGameObject(pared);
    level->addGameObject(cono);
    level->addGameObject(barril);
    
    //Se settea el auto que será el jugador.
    camera->setTarget(monster);
    level->setPlayer(monster);

    inputProcessor = new InputProcessor(g_window, camera, monster);
}

void Game::pantallaInicio(){

    gltInit();
    GLTtext *text1 = gltCreateText();
    gltSetText(text1, "Hello World!");

    StaticGameObject* image = new StaticGameObject("res/imagen/image.obj", "res/imagen/image.jpg", nullptr, shader_programme, btVector3(0, 0, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)));
    while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ENTER) == GLFW_RELEASE){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //inputProcessor->processInput();
        
        glUseProgram(shader_programme);
        image->draw(model_mat_location);
        skybox->draw();

        gltColor(1.f, 1.f, 1.f, 1.f);
        gltDrawText2D(text1, 1.f, 1.f, 1.f);
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
    //gltDeleteText(text1);
    
    //gltTerminate();

}

void Game::doMainLoop(){
    //glLineWidth(7);
    camera->isProjChanged = true;
    glEnable(GL_LINE_SMOOTH);
    //soundManager->musicaFondo();
    int frameCount = 0;
    
    while (!glfwWindowShouldClose(g_window)){

        currentTime = glfwGetTime();
        frameCount++;        
        deltaTime = currentTime - lastTime;
        if (deltaTime >= 1.0 ){ 
            // printf and reset timer
            printf("%.2f fps, %f ms\n", frameCount/deltaTime,1000.0/double(frameCount));
            frameCount = 0;
            lastTime += 1.0;
        }
        checkStatus();
        inputProcessor->processInput();
        
        glClearColor(0.0f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Dibuja todos los objetos de la escena
        level->drawAllGameObjects(model_mat_location, shader_programme);
        skybox->draw();
        
        // Dibuja todos las figuras colisionadoras de los objetos
        //level->getDynamicsWorld()->debugDrawWorld();
        //camera->debugDrawer->drawLines();

        level->stepSimulation(1 / 30.f, 0); 

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
        if (level->getCars().at(i)->getHealth() <= 0.0f && level->getCars().at(i)->getIsAlive() == true){
            level->getCars().at(i)->setIsAlive(false);
            level->getCars().at(i)->despawn(level->getDynamicsWorld());
            //level->getCars().erase(level->getCars().begin()+ i);
            enemiesCount--;
        }
    }
    if(enemiesCount == 0){
        win();
    }
}
void Game::win(){
    glfwSetWindowShouldClose(g_window, true);
}
void Game::loose(){
    printf("PERDISTE!!\n");
}
        