#ifndef APPLICATION_H
#define APPLICATION_H

class SDL_Window;
typedef void* SDL_GLContext;

class Application {
public:
    Application(const char* title, int width, int height);
    ~Application();
    
    void run();

private:
    void processInput();
    void update(float deltaTime);  
    void render();
    
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    bool m_running;
    int m_width;
    int m_height;
};

#endif