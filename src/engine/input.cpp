#include "input.h"

bool Input::m_keys[SDL_NUM_SCANCODES] = {false};
bool Input::m_keysPressed[SDL_NUM_SCANCODES] = {false};
bool Input::m_keysReleased[SDL_NUM_SCANCODES] = {false};
bool Input::m_mouseButtons[5] = {false};
bool Input::m_mouseButtonsPressed[5] = {false};
bool Input::m_mouseButtonsReleased[5] = {false};
int Input::m_mouseX = 0;
int Input::m_mouseY = 0;
int Input::m_mouseDeltaX = 0;
int Input::m_mouseDeltaY = 0;

void Input::init() {
}

void Input::update() {
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        m_keysPressed[i] = false;
        m_keysReleased[i] = false;
    }

    for (int i = 0; i < 5; i++) {
        m_mouseButtonsPressed[i] = false;
        m_mouseButtonsReleased[i] = false;
    }

    m_mouseDeltaX = 0;
    m_mouseDeltaY = 0;
}

void Input::processEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            if (!m_keys[event.key.keysym.scancode]) {
                m_keys[event.key.keysym.scancode] = true;
                m_keysPressed[event.key.keysym.scancode] = true;
            }
            break;

        case SDL_KEYUP:
            m_keys[event.key.keysym.scancode] = false;
            m_keysReleased[event.key.keysym.scancode] = true;
            break;

        case SDL_MOUSEMOTION:
            m_mouseX = event.motion.x;
            m_mouseY = event.motion.y;
            m_mouseDeltaX += event.motion.xrel;
            m_mouseDeltaY += event.motion.yrel;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button <= 3) {
                int index = event.button.button - 1;
                if (!m_mouseButtons[index]) {
                    m_mouseButtons[index] = true;
                    m_mouseButtonsPressed[index] = true;
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button <= 3) {
                int index = event.button.button - 1;
                m_mouseButtons[index] = false;
                m_mouseButtonsReleased[index] = true;
            }
            break;
    }
}