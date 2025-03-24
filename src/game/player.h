// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include "../rendering/camera.h"
#include "world.h"

class Player {
public:
    Player(glm::vec3 position = glm::vec3(0.0f, 1.75f, 0.0f));
    
    // Update player position and camera
    void update(float deltaTime, World* world);
    
    // Process keyboard input for movement
    void processKeyboard(int direction, float deltaTime);
    
    // Process mouse movement
    void processMouseMovement(float xoffset, float yoffset);
    
    // Jump
    void jump();
    
    // Get camera for rendering
    Camera* getCamera() { return m_camera; }
    
    // Get player position
    glm::vec3 getPosition() const { return m_position; }
    
private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    float m_speed;
    float m_jumpForce;
    bool m_onGround;
    Camera* m_camera;
    
    const float GRAVITY = 9.81f;
    const float PLAYER_RADIUS = 0.5f;
    const float PLAYER_HEIGHT = 1.75f;
};

#endif