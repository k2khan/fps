#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

class Input {
public:
    static void init();
    static void update();
    static void processEvent(const SDL_Event& event);
    
    static bool isKeyDown(SDL_Scancode key) { return m_keys[key]; }
    static bool isKeyPressed(SDL_Scancode key) { return m_keysPressed[key]; }
    static bool isKeyReleased(SDL_Scancode key) { return m_keysReleased[key]; }
    
    static int getMouseX() { return m_mouseX; }
    static int getMouseY() { return m_mouseY; }
    static int getMouseDeltaX() { return m_mouseDeltaX; }
    static int getMouseDeltaY() { return m_mouseDeltaY; }
    
    static bool isMouseButtonDown(int button) { return m_mouseButtons[button]; }
    static bool isMouseButtonPressed(int button) { return m_mouseButtonsPressed[button]; }
    static bool isMouseButtonReleased(int button) { return m_mouseButtonsReleased[button]; }
    
private:
    static bool m_keys[SDL_NUM_SCANCODES];
    static bool m_keysPressed[SDL_NUM_SCANCODES];
    static bool m_keysReleased[SDL_NUM_SCANCODES];
    
    static int m_mouseX;
    static int m_mouseY;
    static int m_mouseDeltaX;
    static int m_mouseDeltaY;
    
    static bool m_mouseButtons[5];
    static bool m_mouseButtonsPressed[5];
    static bool m_mouseButtonsReleased[5];
};

#endif