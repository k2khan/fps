#include "application.h"
#include "input.h"
#include "timer.h"
#include <glad/glad.h>
#include <iostream>

Application::Application(const char* title, int width, int height)
    : m_window(nullptr), m_glContext(nullptr), m_running(false), m_width(width), m_height(height),
      m_shader(nullptr), m_gameManager(nullptr) {
    
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // For a 2D game, we don't need relative mouse mode
    SDL_SetRelativeMouseMode(SDL_FALSE);
    
    m_shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    
    // Initialize game manager
    m_gameManager = new GameManager();
    m_gameManager->init();
    
    // Add two players for testing
    m_gameManager->addPlayer(FighterType::BALANCED, 0);
    m_gameManager->addPlayer(FighterType::HEAVY, 1);
    
    // Start the game
    m_gameManager->startGame();
    
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
    
    // Process player 1 input
    glm::vec2 movement1(0.0f);
    if (Input::isKeyDown(SDL_SCANCODE_A)) movement1.x -= 1.0f;
    if (Input::isKeyDown(SDL_SCANCODE_D)) movement1.x += 1.0f;
    bool jump1 = Input::isKeyPressed(SDL_SCANCODE_W);
    bool attack1 = Input::isKeyPressed(SDL_SCANCODE_SPACE);
    
    m_gameManager->processPlayerInput(0, movement1, jump1, attack1, AttackType::NEUTRAL);
    
    // Process player 2 input
    glm::vec2 movement2(0.0f);
    if (Input::isKeyDown(SDL_SCANCODE_LEFT)) movement2.x -= 1.0f;
    if (Input::isKeyDown(SDL_SCANCODE_RIGHT)) movement2.x += 1.0f;
    bool jump2 = Input::isKeyPressed(SDL_SCANCODE_UP);
    bool attack2 = Input::isKeyPressed(SDL_SCANCODE_RCTRL);
    
    m_gameManager->processPlayerInput(1, movement2, jump2, attack2, AttackType::NEUTRAL);
}

void Application::update(float deltaTime) {
    // Update game manager
    m_gameManager->update(deltaTime);
}

void Application::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_shader->use();
    
    // Render game
    m_gameManager->render(*m_shader);
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
    delete m_gameManager;
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
