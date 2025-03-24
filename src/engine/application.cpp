#include "application.h"
#include "input.h"
#include "timer.h"
#include <glad/glad.h>
#include <iostream>

Application::Application(const char* title, int width, int height)
    : m_window(nullptr), m_glContext(nullptr), m_running(false), m_width(width), m_height(height),
      m_shader(nullptr), m_world(nullptr), m_player(nullptr) {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    m_window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!m_window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    m_shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    m_world = new World();
    m_player = new Player(glm::vec3(0.0f, 1.75f, 0.0f));
    
    initRenderData();
    
    m_running = true;
}

void Application::run() {
    Input::init();
    Timer timer;

    while (m_running) {
        float delta_time = timer.getDeltaTime();

        processInput();
        update(delta_time);
        render();
        SDL_GL_SwapWindow(m_window);
    }
}

void Application::processInput() {
    Input::update();
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_running = false;
        }
        
        Input::processEvent(event);
        
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            m_running = false;
        }
    }
}

void Application::update(float deltaTime) {
    if (Input::isKeyDown(SDL_SCANCODE_W))
        m_player->processKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (Input::isKeyDown(SDL_SCANCODE_S))
        m_player->processKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (Input::isKeyDown(SDL_SCANCODE_A))
        m_player->processKeyboard(Camera_Movement::LEFT, deltaTime);
    if (Input::isKeyDown(SDL_SCANCODE_D))
        m_player->processKeyboard(Camera_Movement::RIGHT, deltaTime);
    if (Input::isKeyPressed(SDL_SCANCODE_SPACE))
        m_player->jump();
        
    if (Input::getMouseDeltaX() != 0 || Input::getMouseDeltaY() != 0)
        m_player->processMouseMovement((float)Input::getMouseDeltaX(), -(float)Input::getMouseDeltaY());
        
    m_player->update(deltaTime, m_world);
}

void Application::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_shader->use();
    
    glm::mat4 view = m_player->getCamera()->getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.1f, 100.0f);
    
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);
    
    // Render the world
    m_world->draw(*m_shader);
}

void Application::initRenderData() {
    float vertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f 
    };
    unsigned int indices[] = {
        0, 1, 3, 
        1, 2, 3  
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Application::~Application() {
    delete m_shader;
    delete m_world;
    delete m_player;
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}