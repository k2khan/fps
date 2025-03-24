// player.cpp
#include "player.h"
#include "../engine/input.h"

Player::Player(glm::vec3 position) :
    m_position(position),
    m_velocity(0.0f),
    m_speed(5.0f),
    m_jumpForce(5.0f),
    m_onGround(false) {
    
    m_camera = new Camera(position);
}

void Player::update(float deltaTime, World* world) {
    // Apply gravity
    m_velocity.y -= GRAVITY * deltaTime;
    
    // Calculate new position based on velocity
    glm::vec3 newPosition = m_position + m_velocity * deltaTime;
    
    // Resolve collisions
    newPosition = world->resolveCollision(m_position, newPosition, PLAYER_RADIUS);
    
    // Check if on ground
    m_onGround = (m_position.y > newPosition.y && 
                 abs(m_position.y - newPosition.y) < 0.01f);
    
    // Update position
    m_position = newPosition;
    
    // Update camera position
    m_camera->Position = m_position;
}

void Player::processKeyboard(int direction, float deltaTime) {
    float velocity = m_speed * deltaTime;
    glm::vec3 movement(0.0f);
    
    if (direction == FORWARD)
        movement += glm::vec3(m_camera->Front.x, 0.0f, m_camera->Front.z);
    if (direction == BACKWARD)
        movement -= glm::vec3(m_camera->Front.x, 0.0f, m_camera->Front.z);
    if (direction == LEFT)
        movement -= m_camera->Right;
    if (direction == RIGHT)
        movement += m_camera->Right;
    
    // Normalize movement on X/Z to prevent diagonal speed boost
    if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement) * velocity;
        m_velocity.x = movement.x;
        m_velocity.z = movement.z;
    } else {
        // Apply friction to stop horizontal movement when no keys are pressed
        m_velocity.x *= 0.9f;
        m_velocity.z *= 0.9f;
    }
}

void Player::processMouseMovement(float xoffset, float yoffset) {
    m_camera->processMouseMovement(xoffset, yoffset);
}

void Player::jump() {
    if (m_onGround) {
        m_velocity.y = m_jumpForce;
        m_onGround = false;
    }
}