#include"application.h"
#include"input.h"
#include"timer.h"
#include<glad/glad.h>
#include<iostream>

Application::Application(const char* title, int width, int height)
    : m_window(nullptr), m_glContext(nullptr), m_running(false), m_width(width), m_height(height) {
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
    m_running = true;
    }

void Application::run() {
    Input::init();
    Timer timer;

    while (m_running) {
        float deltaTime = timer.getDeltaTime();

        processInput();
        update(deltaTime);
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
    // Test keyboard input
    if (Input::isKeyDown(SDL_SCANCODE_W))
        std::cout << "Moving forward" << std::endl;
    if (Input::isKeyDown(SDL_SCANCODE_S))
        std::cout << "Moving backward" << std::endl;
    if (Input::isKeyDown(SDL_SCANCODE_A))
        std::cout << "Strafing left" << std::endl;
    if (Input::isKeyDown(SDL_SCANCODE_D))
        std::cout << "Strafing right" << std::endl;
    
    // Test mouse movement
    if (Input::getMouseDeltaX() != 0 || Input::getMouseDeltaY() != 0)
        std::cout << "Mouse delta: " << Input::getMouseDeltaX() << ", " 
                  << Input::getMouseDeltaY() << std::endl;
}

void Application::render() {
    // Clear the screen with a simple color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Add actual rendering after
}

Application::~Application() {
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
