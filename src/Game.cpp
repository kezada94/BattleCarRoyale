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
    //glfwSwapInterval(1);  
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    
    shader_programme = create_programme_from_files (VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    model_mat_location  = glGetUniformLocation (shader_programme, "model");
    specular_loc = glGetUniformLocation (shader_programme, "ks");
    level->init(shader_programme);
	glUseProgram (shader_programme);
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
    StaticGameObject *piso = new StaticGameObject("res/suelo/suelo.obj", "res/suelo/suelo.jpg", "res/suelo/suelo_NRM.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(0, 0, 0), specular_loc);
    StaticGameObject *tuneles = new StaticGameObject("res/tunel/tunel.obj", "res/tunel/tetete.png", "res/tunel/tetete_NRM.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(.5f, .5f, 0.5f), specular_loc);
    StaticGameObject *tubos = new StaticGameObject("res/tuneles/tubos.obj", "res/tuneles/tex.jpg", nullptr, shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(.5f, .5f, 0.5f), specular_loc);
    StaticGameObject *pared = new StaticGameObject("res/pared/pared.obj", "res/pared/tex.jpg", "res/pared/tex_NRM.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(.5f, .5f, 0.5f), specular_loc);
    StaticGameObject *esfera = new StaticGameObject("res/untitled.obj", "res/tex.png", "res/tex_NRM.png", shader_programme, btVector3(50, 50, 50), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(0, 0, 0), specular_loc);
    StaticGameObject *luz = new StaticGameObject("res/untitled.obj", "res/tex.png", "res/tex_NRM.png", shader_programme, btVector3(30, 100, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(0, 0, 0), specular_loc);
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
    glewInit();

    #ifdef __linux__
        const char *vertex_shader =
            "#version 130\n"
            "in vec3 position;\n"
            "in vec3 color\n;"
            "in vec2 texCoord;\n"
            "\n"
            "out vec3 ourColor;\n"
            "out vec2 TexCoord;\n"
            "\n"
            "void main()\n"
            "{\n"
            "gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
            "ourColor = color;\n"
            // We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
            // TexCoord = texCoord;
            "TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);\n"
            "}\n";
        const char *fragment_shader =
            "#version 130\n"
            "in vec3 ourColor;\n"
            "in vec2 TexCoord;\n"
            "\n"
            "out vec4 color;\n"
            "\n"
            // Texture samplers
            "uniform sampler2D ourTexture1;\n"
            "\n"
            "void main()\n"
            "{\n"
            // Linearly interpolate between both textures (second texture is only slightly combined)
            "color = texture(ourTexture1, TexCoord);\n"
            "}\n";
    #else
         const char *vertex_shader =
            "#version 410\n"
            "layout(location = 0) in vec3 position;\n"
            "layout(location = 1) in vec3 color\n;"
            "layout(location = 2) in vec2 texCoord;\n"
            "\n"
            "out vec3 ourColor;\n"
            "out vec2 TexCoord;\n"
            "\n"
            "void main()\n"
            "{\n"
            "gl_Position = vec4(position.x, position.y, position.z+0.1, 1.0f);\n"
            "ourColor = color;\n"
            // We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
            // TexCoord = texCoord;
            "TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);\n"
            "}\n";
        const char *fragment_shader =
            "#version 410\n"
            "in vec3 ourColor;\n"
            "in vec2 TexCoord;\n"
            "\n"
            "out vec4 color;\n"
            "\n"
            // Texture samplers
            "uniform sampler2D ourTexture1;\n"
            "\n"
            "void main()\n"
            "{\n"
            // Linearly interpolate between both textures (second texture is only slightly combined)
            "color = texture(ourTexture1, TexCoord);\n"
            "}\n";

    #endif
    /* GL shader objects for vertex and fragment shader [components] */
    GLuint vs, fs;
    /* GL shader programme object [combined, to link] */
    GLuint shader_text;

    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    shader_text = glCreateProgram();
    glAttachShader(shader_text, fs);
    glAttachShader(shader_text, vs);
    glLinkProgram(shader_text);
    GLfloat vertices[] =
        {
            // Positions          // Colors           // Texture Coords
            -1.f, 1.f, 0.0f,      1.0f, 0.0f, 0.0f,      0.85f,  0.97f,  // Top Right
            -1.f, -0.8, 0.0f,      0.0f, 1.0f, 0.0f,     0.85f,  0.05f,  // Bottom Right
            1.f, -0.8, 0.0f,     0.0f, 0.0f, 1.0f,     0.12f, 0.05f, // Bottom Left
            1.f, 1.f, 0.0f,      1.0f, 1.0f, 0.0f,     0.12f, 0.97f   // Top Left
        };
    GLuint indices[] =
        {
            // Note that we start from 0!
            0, 1, 3, // First Triangle
            1, 2, 3  // Second Triangle
        };
    // gen and bind VAO
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Texture Coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO

    GLuint texture;

    int width, height;

    // ===================
    // Texture
    // ===================
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    unsigned char *image = stbi_load("res/imagen/fondo.jpg", &width, &height, 0, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);

    gltInit();
    GLTtext *text1 = gltCreateText();
    gltSetText(text1, "Presione Enter para Continuar!!");
    /*glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    //// Draw the triangle
    glUseProgram(shader_text);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(shader_text, 0);

    // Draw container
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);*/
    float t = 0;
    int c = 0;
    while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ENTER) == GLFW_RELEASE)
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        // Render
        //Clear the colorbuffer

        glClearColor(0.f, 0.f, 7.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        //// Draw the triangle
        glUseProgram(shader_text);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(shader_text, 0);

        // Draw container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        gltColor(1.f, 1.f, 1.f, 1.f);
        if (t >= 750)
        {
            c = 1;
        }
        else if (t <= 0)
        {
            c = 0;
        }
        if (c == 0)
        {
            t += 1.5f;
        }
        else
        {
            t -= 1.5f;
        }
        gltDrawText2D(text1, (t), 500.f, 2.f);
        // Swap the screen buffers
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
    gltDeleteText(text1);

    gltTerminate();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //Terminate GLFW, clearing any resources allocated by GLFW.

}

void Game::doMainLoop(){
    //glLineWidth(7);
    glEnable(GL_LINE_SMOOTH);
    //soundManager->musicaFondo();
    camera->setMode(CameraModes::THIRD_PERSON);
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
        glViewport(0,0, 1366, 768/2); // Render on the whole framebuffer, complete from the lower left corner to the upper right

        level->drawAllGameObjects(model_mat_location, shader_programme);
        skybox->draw();
        
        glViewport(0,768/2, 1366, 768); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        level->drawAllGameObjects(model_mat_location, shader_programme);
        skybox->draw();
        // Dibuja todos las figuras colisionadoras de los objetos
        //level->getDynamicsWorld()->debugDrawWorld();
        //camera->debugDrawer->drawLines();

        level->stepSimulation(1 / 30.f, 0); 

        level->updateAllCarsPhysics();    
        glUseProgram(shader_programme);
        camera->update(level->getDynamicsWorld());
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
        