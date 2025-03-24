#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "rendering/shader.h"
#include "rendering/camera.h"
#include "game/world.h"
#include "game/player.h"

class Application {
public:
    Application(const char* title, int width, int height);
    ~Application();

    void run();
private:
    void processInput();
    void update(float deltaTime);  
    void render();
    void initRenderData();

    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    bool m_running;
    int m_width;
    int m_height;
    
    // Game components
    Shader* m_shader;
    Player* m_player;
    World* m_world;
    
    // OpenGL rendering data
    unsigned int VAO, VBO, EBO;
};
#endif