#include "Game.hpp"
#include "stb_image.h"

#include <iostream>

Game::Game(){
    level = new Scene();
    camera = new Camera();
    camera2 = new Camera();
}
Game::~Game(){
    delete level;
    delete camera;
    if (inputProcessor != nullptr){
        delete inputProcessor;
    }
}    

void Game::init(){
    win = 0;
    srand(time(NULL));
    glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);  
    //glfwSwapInterval(1);  
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_POINT_SMOOTH);
    //glEnable(GL_BLEND);
    Car* p1;
    Car* p2;
    shader_programme = create_programme_from_files (VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    model_mat_location  = glGetUniformLocation (shader_programme, "model");
    specular_loc = glGetUniformLocation (shader_programme, "ks");
    level->init(shader_programme);
	glUseProgram (shader_programme);
    soundManager = new SoundManager();
    soundManager2 = new SoundManager();
    particleManager = new ParticleManager();
    skybox = new Skybox();

    GLDebugDrawer* debug = new GLDebugDrawer();    
    debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe );
    
    camera->debugDrawer = debug;
    camera->particleManager = particleManager;
    camera->skybox = skybox;
    camera->init(shader_programme, g_gl_width/2, g_gl_height, fov, CameraModes::FIRST_PERSON);    

    camera2->debugDrawer = debug;

    camera2->particleManager = particleManager;
    camera2->skybox = skybox;
    camera2->init(shader_programme, g_gl_width/2, g_gl_height, fov, CameraModes::FIRST_PERSON);  

    level->getDynamicsWorld()->setDebugDrawer(debug);

    //Creacion de objetos de la escena
    StaticGameObject *piso = new StaticGameObject("res/suelo/suelo.obj", "res/suelo/suelo.jpg", "res/suelo/suelo_NRM.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(0, 0, 0), specular_loc);
    StaticGameObject *tuneles = new StaticGameObject("res/tunel/tunel.obj", "res/tunel/tex.png", "res/tunel/tex_NRM.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(.5f, .5f, 0.5f), specular_loc);
    StaticGameObject *tubos = new StaticGameObject("res/cuadrados/cuadrados.obj", "res/cuadrados/Metal.jpg", "res/cuadrados/Metal_NRM.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(.5f, .5f, 0.5f), specular_loc);
    StaticGameObject *pared = new StaticGameObject("res/pared/pared.obj", "res/pared/tex.jpg", "res/pared/tex_NRM.png", shader_programme, btVector3(0, -10, 0), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(.5f, .5f, 0.5f), specular_loc);
    
    DynamicGameObject* cono = new DynamicGameObject("res/cono/cono.obj", "res/cono/conotextura.png", "res/cono/conotextura_NRM.png", shader_programme, btScalar(1), btVector3(10, 50, 10), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(1, 1, 1), specular_loc);
    DynamicGameObject* barril = new DynamicGameObject("res/barril/barril.obj", "res/barril/barriltextura.jpg", "res/barril/barriltextura_NRM.png", shader_programme, btScalar(3), btVector3(40, 50, 40), btQuaternion((btVector3(1, 0, 0)), btScalar(0)), glm::vec3(1, 1, 1), specular_loc);
    
<<<<<<< HEAD
    int c = 0;//rand()%3;
=======
    int c = 2;
>>>>>>> 24865bc3f180cb031429b8f15f38d1cf2a9b3023
        switch(c){
            case 0:
                p1 = (Car*) new Kombi(btVector3(20, 5, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                p1->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(1.6, 1.6, 1.6));
                
                break;
            case 1:
                p1 = (Car*) new MonsterTruck(btVector3(20, 5, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                p1->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(2.42f, 2, 2));
                break;
            case 2:
                p1 = (Car*) new Patriot(btVector3(20, 5, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
            case 3:
                p1 = (Car*) new Bus(btVector3(20, 30, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
            case 4:
                p1 = (Car*) new Auto(btVector3(20, 30, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
        }
        c = rand()%4;
        switch(c){
            case 0:
                p2 = (Car*) new Kombi(btVector3(0, 5, 0), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                p2->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(1.6, 1.6, 1.6));
                break;
            case 1:
                p2 = (Car*) new MonsterTruck(btVector3(0, 5, 0), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                p2->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(2.42f, 2, 2));
                break;
            case 2:
                p2 = (Car*) new Patriot(btVector3(0, 5, 0), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
            case 3:
                p2 = (Car*) new Bus(btVector3(20, 30, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
            case 4:
                p2 = (Car*) new Auto(btVector3(20, 30, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
        }

        p1->setSoundManager(soundManager);
        p1->particleManager = particleManager;
        p2->setSoundManager(soundManager2);
        p2->particleManager = particleManager;

        level->addGameObject(p1);
        level->addGameObject(p2);

         //Se settea el auto que será el jugador.
        camera->setTarget(p1);
        camera2->setTarget(p2);

        level->setPlayerOne(p1);
        level->setPlayerTwo(p2);

        inputProcessor = new InputProcessor(g_window, camera, p1, p2);
    
    
   
    piso->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(40, 40, 40));
    tuneles->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(40, 40, 40));
    tubos->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(40, 40,40));
    pared->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(40, 40, 40));

    //Se agregan los objetos la escena
    level->addGameObject(piso);
    level->addGameObject(tuneles);
    level->addGameObject(tubos);
    level->addGameObject(pared);
    level->addGameObject(cono);
    level->addGameObject(barril);
    
    
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
            "gl_Position = vec4(position.x, position.y, position.z+0.1, 1.0f);\n"
            "ourColor = color;\n"
            // We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
            // TexCoord = texCoord;
            "TexCoord = vec2(1.0 - texCoord.x, 1.0 - texCoord.y);\n"
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
            -1.f, 1.f, 0.0f,      1.0f, 0.0f, 0.0f,      1.f,  1.f,  // Top Right
            -1.f, -1.0, 0.0f,      0.0f, 1.0f, 0.0f,     1.f,  0.f,  // Bottom Right
            1.f, -1.0, 0.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f, // Bottom Left
            1.f, 1.f, 0.0f,      1.0f, 1.0f, 0.0f,     0.0f, 1.f   // Top Left
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
    unsigned char *image = stbi_load("res/imagen/fondo.png", &width, &height, 0, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);

    gltInit();
    GLTtext *text1 = gltCreateText();
    gltSetText(text1, "Presione Enter para Continuar!!");

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
    //soundManager->musicaFondo();
    camera->setMode(CameraModes::THIRD_PERSON);
    camera2->setMode(CameraModes::THIRD_PERSON);
    int frameCount = 0;
    gltInit();
    GLTtext *text1 = gltCreateText();
    GLTtext *text2 = gltCreateText();
    char t[50];
    while (!glfwWindowShouldClose(g_window) && win == 0 ){
        
        currentTime = glfwGetTime();
        frameCount++;        
        deltaTime = currentTime - lastTime;
        if (deltaTime >= 1.0 ){ 
            // printf and reset timer
            printf("%.2f fps, %f ms\n", frameCount/deltaTime,1000.0/double(frameCount));
            frameCount = 0;
            lastTime += 1.0;
        }
        inputProcessor->processInput();
        
        glClearColor(0.0f, 0.f, 0.f, 1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        level->renderShadow();

        // Dibuja todos los objetos de la escena
        camera->update(level->getDynamicsWorld());

        skybox->setProj(&camera->projection);
        skybox->setView(&camera->view);
        particleManager->setProj(&camera->projection);
        particleManager->setView(&camera->view);

        level->drawAllGameObjects(model_mat_location, shader_programme, 1);
        skybox->draw();
        particleManager->drawActiveParticles();
        //camera->debugDrawer->drawLines();

        camera2->update(level->getDynamicsWorld());

        skybox->setProj(&camera2->projection);
        skybox->setView(&camera2->view);
        particleManager->setProj(&camera2->projection);
        particleManager->setView(&camera2->view);

        level->drawAllGameObjects(model_mat_location, shader_programme, 2);
        skybox->draw();
        particleManager->drawActiveParticles();

        // Dibuja todos las figuras colisionadoras de los objetos
        //level->getDynamicsWorld()->debugDrawWorld();
        //camera->debugDrawer->drawLines();
        level->stepSimulation(1 / 30.f, 0); 

        level->updateAllCarsPhysics();    
        glUseProgram(shader_programme);

        glViewport(0, 0, g_gl_width, g_gl_height);

        int a = (int)level->getPlayerOne()->getHealth();
        int a2 = (int)level->getPlayerTwo()->getHealth();
        
        std::stringstream ss;
        std::stringstream ss2;

        ss << a;
        ss2 << a2;

        std::string str = ss.str();
        std::string str2 = ss2.str();
        strcpy(t , "Vida Restante: ");
        gltSetText(text1, strcat(t, str.c_str()));

        strcpy(t , "Vida Restante: ");
        gltSetText(text2, strcat(t, str2.c_str()));

        gltColor(1.f, 1.f, 1.f, 1.f);
        gltDrawText2D(text1, 0.f, 0, 2.f);
        gltDrawText2D(text2, g_gl_width/2, 0.f, 2.f);



        glfwSwapBuffers(g_window);
        glfwPollEvents();
        checkStatus();
    }

    gltDeleteText(text1);
    gltDeleteText(text2);
    gltTerminate();

    if (win != 0){
        showWinnerScreen();
    }
}

void Game::checkStatus(){
    if (level->getPlayerOne()->getHealth() <=0){
        win = 2;
    }else if (level->getPlayerTwo()->getHealth() <=0){
        win = 1;
    }
}
void Game::showWinnerScreen(){
    glewInit();
    glViewport(0, 0, g_gl_width, g_gl_height);

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
            "gl_Position = vec4(position.x, position.y, position.z+0.1, 1.0f);\n"
            "ourColor = color;\n"
            // We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
            // TexCoord = texCoord;
            "TexCoord = vec2(1.0 - texCoord.x, 1.0 - texCoord.y);\n"
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
            -1.f, 1.f, 0.0f,      1.0f, 0.0f, 0.0f,      1.f,  1.f,  // Top Right
            -1.f, -1.0, 0.0f,      0.0f, 1.0f, 0.0f,     1.f,  0.0f,  // Bottom Right
            1.f, -1.0, 0.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f, // Bottom Left
            1.f, 1.f, 0.0f,      1.0f, 1.0f, 0.0f,     0.0f, 1.f   // Top Left
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
    unsigned char *image;
    if(win == 1)
        image = stbi_load("res/imagen/p1.png", &width, &height, 0, 4);
    else
        image = stbi_load("res/imagen/p2.png", &width, &height, 0, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);

    gltInit();
    GLTtext *text1 = gltCreateText();

    gltSetText(text1, "Felicitaciones!!\nPulse 'Enter' para otra ronda, 'Escape' para salir...");

    float t = 0;
    int c = 0;
    while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ENTER) == GLFW_RELEASE && glfwGetKey(g_window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
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


    //RESET
    if (glfwGetKey(g_window, GLFW_KEY_ENTER) == GLFW_PRESS){
        Car* p1 = level->getPlayerOne();
        Car* p2 = level->getPlayerTwo();
        p1->despawn(level->getDynamicsWorld());
        p2->despawn(level->getDynamicsWorld());
        level->getCars()->clear();
        delete p1;
        delete p2;

        int c = 1;
        switch(c){
            case 0:
                p1 = (Car*) new Kombi(btVector3(20, 5, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
            case 1:
                p1 = (Car*) new MonsterTruck(btVector3(20, 5, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                p1->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(2.42f, 2, 2));
                break;
            case 2:
                p1 = (Car*) new Patriot(btVector3(20, 5, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
            case 3:
                p1 = (Car*) new Bus(btVector3(20, 30, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
            case 4:
                p1 = (Car*) new Auto(btVector3(20, 30, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
        }
        c = rand()%4;
        switch(c){
            case 0:
                p2 = (Car*) new Kombi(btVector3(0, 5, 0), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
            case 1:
                p2 = (Car*) new MonsterTruck(btVector3(0, 5, 0), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                p2->getRigidBody()->getCollisionShape()->setLocalScaling(btVector3(2.42f, 2, 2));
                break;
            case 2:
                p2 = (Car*) new Patriot(btVector3(0, 5, 0), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
            case 3:
                p2 = (Car*) new Bus(btVector3(20, 30, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
            case 4:
                p2 = (Car*) new Auto(btVector3(20, 30, 20), btQuaternion(btVector3(1, 0, 0), btScalar(0)), shader_programme, level->getDynamicsWorld(), specular_loc);
                break;
        }

        p1->setSoundManager(soundManager);
        p1->particleManager = particleManager;
        p2->setSoundManager(soundManager2);
        p2->particleManager = particleManager;

        level->addGameObject(p1);
        level->addGameObject(p2);

         //Se settea el auto que será el jugador.
        camera->setTarget(p1);
        camera2->setTarget(p2);

        level->setPlayerOne(p1);
        level->setPlayerTwo(p2);

        inputProcessor->playerOne = p1;
        inputProcessor->playerTwo = p2;
        win = 0;
        doMainLoop();
    }
}
    //Terminate GLFW, clearing any resources allocated by GLFW.}
        