#include "timer.h"
#include <SDL2/SDL.h>

Timer::Timer() {
    m_lastTime = SDL_GetPerformanceCounter();
    m_deltaTime = 0.0f;
}

float Timer::getDeltaTime() {
    Uint64 currentTime = SDL_GetPerformanceCounter();
    
    m_deltaTime = (currentTime - m_lastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
    
    if (m_deltaTime > 0.1f) {
        m_deltaTime = 0.1f;
    }
    
    m_lastTime = currentTime;
    return m_deltaTime;
}