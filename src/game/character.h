#ifndef CHARACTER_H
#define CHARACTER_H

#include <glm/glm.hpp>
#include <string>
#include "../rendering/mesh.h"
#include "../rendering/texture.h"
#include "../rendering/shader.h"

enum class CharacterState {
    IDLE,
    RUNNING,
    JUMPING,
    FALLING,
    ATTACKING,
    SPECIAL,
    DAMAGED,
    DEAD
};

enum class AttackType {
    NEUTRAL,
    UP,
    DOWN,
    SIDE,
    SPECIAL_NEUTRAL,
    SPECIAL_UP,
    SPECIAL_DOWN,
    SPECIAL_SIDE
};

struct Hitbox {
    glm::vec2 offset;
    float radius;
    int damage;
    float knockbackBase;
    float knockbackScaling;
    glm::vec2 knockbackDirection;
};

class Character {
public:
    Character(const std::string& name, const glm::vec2& position, const glm::vec2& size);
    virtual ~Character();
    
    virtual void update(float deltaTime);
    virtual void render(Shader& shader);
    
    // Movement
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void jump();
    
    // Attacks
    virtual void attack(AttackType type);
    
    // Damage and knockback
    void takeDamage(int damage, float knockback, const glm::vec2& direction);
    
    // Getters
    glm::vec2 getPosition() const { return m_position; }
    glm::vec2 getVelocity() const { return m_velocity; }
    glm::vec2 getSize() const { return m_size; }
    float getDamage() const { return m_damagePercent; }
    int getLives() const { return m_lives; }
    CharacterState getState() const { return m_state; }
    bool isOnGround() const { return m_onGround; }
    
    // Setters
    void setPosition(const glm::vec2& position) { m_position = position; }
    void setVelocity(const glm::vec2& velocity) { m_velocity = velocity; }
    void setOnGround(bool onGround) { m_onGround = onGround; }
    
    // Public for collision detection in GameManager
    std::vector<Hitbox> m_activeHitboxes;
    
protected:
    std::string m_name;
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    glm::vec2 m_size;
    
    float m_damagePercent;
    int m_lives;
    
    float m_moveSpeed;
    float m_jumpForce;
    float m_weight;
    
    CharacterState m_state;
    float m_stateTimer;
    
    bool m_facingRight;
    bool m_onGround;
    bool m_canJump;
    
    Mesh* m_mesh;
    Texture* m_texture;
    
    // Constants
    const float GRAVITY = 9.81f;
    const float MAX_FALL_SPEED = 15.0f;
    
    virtual void updateState(float deltaTime);
    virtual void applyGravity(float deltaTime);
public:
    virtual void updateHitboxes();
};

#endif
