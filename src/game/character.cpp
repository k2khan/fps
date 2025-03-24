#include "character.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

Character::Character(const std::string& name, const glm::vec2& position, const glm::vec2& size)
    : m_name(name)
    , m_position(position)
    , m_velocity(0.0f, 0.0f)
    , m_size(size)
    , m_damagePercent(0.0f)
    , m_lives(3)
    , m_moveSpeed(5.0f)
    , m_jumpForce(10.0f)
    , m_weight(1.0f)
    , m_state(CharacterState::IDLE)
    , m_stateTimer(0.0f)
    , m_facingRight(true)
    , m_onGround(false)
    , m_canJump(false)
    , m_mesh(nullptr)
    , m_texture(nullptr)
{
    // Create a simple quad mesh for the character
    std::vector<Vertex> vertices = {
        // Front face vertices
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) }, // Bottom-left
        { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },  // Bottom-right
        { glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },   // Top-right
        { glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) }   // Top-left
    };
    
    std::vector<unsigned int> indices = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };
    
    std::vector<Texture*> textures;  // Empty for now, will be set by derived classes
    
    m_mesh = new Mesh(vertices, indices, textures);
    
    // Load default texture (should be replaced by derived classes)
    // m_texture = ResourceManager::loadTexture("assets/textures/default_character.png");
}

Character::~Character() {
    if (m_mesh) {
        delete m_mesh;
    }
    // Texture is managed by ResourceManager
}

void Character::update(float deltaTime) {
    updateState(deltaTime);
    applyGravity(deltaTime);
    updateHitboxes();
    
    // Update position based on velocity
    m_position += m_velocity * deltaTime;
    
    // Reset state timer if state changed
    if (m_state == CharacterState::ATTACKING || m_state == CharacterState::SPECIAL || m_state == CharacterState::DAMAGED) {
        m_stateTimer -= deltaTime;
        if (m_stateTimer <= 0.0f) {
            m_state = m_onGround ? CharacterState::IDLE : CharacterState::FALLING;
            m_stateTimer = 0.0f;
        }
    }
}

void Character::render(Shader& shader) {
    if (!m_mesh) return;
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(m_position.x, m_position.y, 0.0f));
    model = glm::scale(model, glm::vec3(m_size.x, m_size.y, 1.0f));
    
    // Flip the model if facing left
    if (!m_facingRight) {
        model = glm::scale(model, glm::vec3(-1.0f, 1.0f, 1.0f));
    }
    
    shader.setMat4("model", model);
    
    // Bind texture if available
    if (m_texture) {
        m_texture->bind(0);
    }
    
    // Draw the mesh
    m_mesh->draw(shader);
}

void Character::moveLeft(float deltaTime) {
    m_velocity.x = -m_moveSpeed;
    m_facingRight = false;
    if (m_onGround && m_state != CharacterState::ATTACKING && m_state != CharacterState::SPECIAL) {
        m_state = CharacterState::RUNNING;
    }
}

void Character::moveRight(float deltaTime) {
    m_velocity.x = m_moveSpeed;
    m_facingRight = true;
    if (m_onGround && m_state != CharacterState::ATTACKING && m_state != CharacterState::SPECIAL) {
        m_state = CharacterState::RUNNING;
    }
}

void Character::jump() {
    if (m_onGround || m_canJump) {
        m_velocity.y = m_jumpForce;
        m_onGround = false;
        m_canJump = false;
        m_state = CharacterState::JUMPING;
    }
}

void Character::attack(AttackType type) {
    // Base implementation - should be overridden by derived classes
    if (m_state == CharacterState::ATTACKING || m_state == CharacterState::SPECIAL) {
        return; // Already attacking
    }
    
    m_state = CharacterState::ATTACKING;
    m_stateTimer = 0.3f; // Default attack duration
    
    // Create a basic hitbox
    Hitbox hitbox;
    hitbox.radius = m_size.x * 0.6f;
    hitbox.damage = 5;
    hitbox.knockbackBase = 5.0f;
    hitbox.knockbackScaling = 0.1f;
    
    // Position and direction based on attack type
    switch (type) {
        case AttackType::NEUTRAL:
            hitbox.offset = glm::vec2(m_facingRight ? m_size.x * 0.5f : -m_size.x * 0.5f, 0.0f);
            hitbox.knockbackDirection = glm::vec2(m_facingRight ? 1.0f : -1.0f, 0.5f);
            break;
        case AttackType::UP:
            hitbox.offset = glm::vec2(0.0f, m_size.y * 0.5f);
            hitbox.knockbackDirection = glm::vec2(0.0f, 1.0f);
            break;
        case AttackType::DOWN:
            hitbox.offset = glm::vec2(0.0f, -m_size.y * 0.5f);
            hitbox.knockbackDirection = glm::vec2(0.0f, -1.0f);
            break;
        case AttackType::SIDE:
            hitbox.offset = glm::vec2(m_facingRight ? m_size.x * 0.7f : -m_size.x * 0.7f, 0.0f);
            hitbox.knockbackDirection = glm::vec2(m_facingRight ? 1.0f : -1.0f, 0.2f);
            hitbox.damage = 8;
            hitbox.knockbackBase = 7.0f;
            break;
        default:
            // Special attacks should be implemented by derived classes
            m_state = CharacterState::SPECIAL;
            m_stateTimer = 0.5f;
            return;
    }
    
    m_activeHitboxes.push_back(hitbox);
}

void Character::takeDamage(int damage, float knockback, const glm::vec2& direction) {
    m_damagePercent += damage;
    
    // Calculate knockback based on damage percentage
    float totalKnockback = knockback * (1.0f + m_damagePercent * 0.01f) / m_weight;
    
    // Apply knockback
    m_velocity = direction * totalKnockback;
    
    // Set damaged state
    m_state = CharacterState::DAMAGED;
    m_stateTimer = 0.5f; // Stun duration
    
    // Check if knocked off stage (would be implemented with stage boundaries)
    // For now, just check if damage is too high
    if (m_damagePercent > 150.0f && totalKnockback > 20.0f) {
        m_lives--;
        if (m_lives <= 0) {
            m_state = CharacterState::DEAD;
        } else {
            // Respawn logic would go here
            m_damagePercent = 0.0f;
            m_position = glm::vec2(0.0f, 5.0f); // Respawn position
        }
    }
}

void Character::updateState(float deltaTime) {
    // Update state based on current conditions
    if (m_state != CharacterState::ATTACKING && 
        m_state != CharacterState::SPECIAL && 
        m_state != CharacterState::DAMAGED) {
        
        if (m_onGround) {
            if (std::abs(m_velocity.x) < 0.1f) {
                m_state = CharacterState::IDLE;
            } else {
                m_state = CharacterState::RUNNING;
            }
        } else {
            if (m_velocity.y > 0) {
                m_state = CharacterState::JUMPING;
            } else {
                m_state = CharacterState::FALLING;
            }
        }
    }
    
    // Apply friction when on ground
    if (m_onGround && m_state != CharacterState::DAMAGED) {
        m_velocity.x *= 0.9f;
        if (std::abs(m_velocity.x) < 0.1f) {
            m_velocity.x = 0.0f;
        }
    }
}

void Character::applyGravity(float deltaTime) {
    // Apply gravity
    m_velocity.y -= GRAVITY * deltaTime;
    
    // Limit fall speed
    m_velocity.y = std::max(m_velocity.y, -MAX_FALL_SPEED);
}

void Character::updateHitboxes() {
    // Remove expired hitboxes
    m_activeHitboxes.clear();
}
