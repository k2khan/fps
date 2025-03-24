#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

class Timer {
public:
    Timer();
    float getDeltaTime();
private:
    Uint64 m_lastTime;
    float m_deltaTime;
};

#endif