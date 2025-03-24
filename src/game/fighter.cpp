#include "fighter.h"
#include <algorithm>
#include "../utils/resource_manager.h"

Fighter::Fighter(const std::string& name, FighterType type, const glm::vec2& position)
    : Character(name, position, glm::vec2(1.0f, 2.0f))
    , m_type(type)
    , m_animationTimer(0.0f)
    , m_currentFrame(0)
{
    initializeStats();
    
    // Set up animation times for different states
    m_animationTimes[CharacterState::IDLE] = 0.5f;
    m_animationTimes[CharacterState::RUNNING] = 0.3f;
    m_animationTimes[CharacterState::JUMPING] = 0.2f;
    m_animationTimes[CharacterState::FALLING] = 0.2f;
    m_animationTimes[CharacterState::ATTACKING] = 0.1f;
    m_animationTimes[CharacterState::SPECIAL] = 0.1f;
    m_animationTimes[CharacterState::DAMAGED] = 0.1f;
    
    // Set up special attack cooldowns
    m_specialCooldowns[AttackType::SPECIAL_NEUTRAL] = 0.0f;
    m_specialCooldowns[AttackType::SPECIAL_UP] = 0.0f;
    m_specialCooldowns[AttackType::SPECIAL_DOWN] = 0.0f;
    m_specialCooldowns[AttackType::SPECIAL_SIDE] = 0.0f;
    
    // Load fighter texture based on type
    std::string texturePath;
    switch (type) {
        case FighterType::BALANCED:
            texturePath = "assets/textures/fighter_balanced.png";
            break;
        case FighterType::HEAVY:
            texturePath = "assets/textures/fighter_heavy.png";
            break;
        case FighterType::SPEEDY:
            texturePath = "assets/textures/fighter_speedy.png";
            break;
        case FighterType::TECHNICAL:
            texturePath = "assets/textures/fighter_technical.png";
            break;
    }
    
    // Load texture (commented out until resource manager is updated)
    // m_texture = ResourceManager::loadTexture(texturePath.c_str());
}

Fighter::~Fighter() {
    // Base class destructor handles cleanup
}

void Fighter::update(float deltaTime) {
    // Update base character
    Character::update(deltaTime);
    
    // Update animation
    updateAnimation(deltaTime);
    
    // Update special attack cooldowns
    for (auto& cooldown : m_specialCooldowns) {
        if (cooldown.second > 0.0f) {
            cooldown.second -= deltaTime;
            if (cooldown.second < 0.0f) {
                cooldown.second = 0.0f;
            }
        }
    }
}

void Fighter::attack(AttackType type) {
    // Check if already attacking
    if (m_state == CharacterState::ATTACKING || m_state == CharacterState::SPECIAL) {
        return;
    }
    
    // Set attack state
    m_state = CharacterState::ATTACKING;
    
    // Set attack duration based on fighter type
    float attackDuration = 0.3f;
    switch (m_type) {
        case FighterType::BALANCED:
            attackDuration = 0.3f;
            break;
        case FighterType::HEAVY:
            attackDuration = 0.5f;
            break;
        case FighterType::SPEEDY:
            attackDuration = 0.2f;
            break;
        case FighterType::TECHNICAL:
            attackDuration = 0.4f;
            break;
    }
    
    m_stateTimer = attackDuration;
    m_animationTimer = 0.0f;
    m_currentFrame = 0;
    
    // Create hitbox for the attack
    createHitboxForAttack(type);
}

void Fighter::specialAttack(AttackType type) {
    // Check if already attacking
    if (m_state == CharacterState::ATTACKING || m_state == CharacterState::SPECIAL) {
        return;
    }
    
    // Check cooldown
    if (m_specialCooldowns[type] > 0.0f) {
        return;
    }
    
    // Set special attack state
    m_state = CharacterState::SPECIAL;
    
    // Set attack duration and cooldown based on fighter type and attack type
    float attackDuration = 0.5f;
    float cooldown = 1.0f;
    
    switch (type) {
        case AttackType::SPECIAL_NEUTRAL:
            attackDuration = 0.5f;
            cooldown = 1.0f;
            break;
        case AttackType::SPECIAL_UP:
            attackDuration = 0.4f;
            cooldown = 2.0f;
            // Special up attack often acts as a recovery move
            m_velocity.y = m_jumpForce * 1.5f;
            break;
        case AttackType::SPECIAL_DOWN:
            attackDuration = 0.6f;
            cooldown = 1.5f;
            break;
        case AttackType::SPECIAL_SIDE:
            attackDuration = 0.4f;
            cooldown = 1.2f;
            // Special side attack often has horizontal movement
            m_velocity.x = m_facingRight ? m_moveSpeed * 2.0f : -m_moveSpeed * 2.0f;
            break;
        default:
            return; // Not a special attack
    }
    
    // Adjust based on fighter type
    switch (m_type) {
        case FighterType::BALANCED:
            // No adjustment
            break;
        case FighterType::HEAVY:
            attackDuration *= 1.3f;
            cooldown *= 1.2f;
            break;
        case FighterType::SPEEDY:
            attackDuration *= 0.8f;
            cooldown *= 0.9f;
            break;
        case FighterType::TECHNICAL:
            attackDuration *= 0.9f;
            cooldown *= 0.8f;
            break;
    }
    
    m_stateTimer = attackDuration;
    m_specialCooldowns[type] = cooldown;
    m_animationTimer = 0.0f;
    m_currentFrame = 0;
    
    // Create hitbox for the special attack
    createHitboxForAttack(type);
}

void Fighter::taunt() {
    // Simple taunt animation
    if (m_state != CharacterState::ATTACKING && m_state != CharacterState::SPECIAL && m_onGround) {
        // In a real game, this would trigger a taunt animation
        // For now, just pause the character briefly
        m_velocity = glm::vec2(0.0f, 0.0f);
        m_stateTimer = 1.0f;
    }
}

void Fighter::initializeStats() {
    // Set base stats
    m_lives = 3;
    m_damagePercent = 0.0f;
    
    // Adjust stats based on fighter type
    switch (m_type) {
        case FighterType::BALANCED:
            m_moveSpeed = 5.0f;
            m_jumpForce = 10.0f;
            m_weight = 1.0f;
            break;
        case FighterType::HEAVY:
            m_moveSpeed = 3.5f;
            m_jumpForce = 8.0f;
            m_weight = 1.5f;
            break;
        case FighterType::SPEEDY:
            m_moveSpeed = 7.0f;
            m_jumpForce = 11.0f;
            m_weight = 0.8f;
            break;
        case FighterType::TECHNICAL:
            m_moveSpeed = 4.5f;
            m_jumpForce = 9.5f;
            m_weight = 0.9f;
            break;
    }
}

void Fighter::updateAnimation(float deltaTime) {
    // Get animation time for current state
    float animTime = m_animationTimes[m_state];
    if (animTime <= 0.0f) {
        animTime = 0.5f; // Default animation time
    }
    
    // Update animation timer
    m_animationTimer += deltaTime;
    if (m_animationTimer >= animTime) {
        m_animationTimer = 0.0f;
        m_currentFrame++;
        
        // In a real game, we would check if we've reached the end of the animation
        // and loop or transition to another state
    }
}

void Fighter::createHitboxForAttack(AttackType type) {
    Hitbox hitbox;
    
    // Set hitbox properties based on attack type and fighter type
    switch (type) {
        case AttackType::NEUTRAL:
            hitbox.radius = m_size.x * 0.6f;
            hitbox.damage = 5;
            hitbox.knockbackBase = 5.0f;
            hitbox.knockbackScaling = 0.1f;
            hitbox.offset = glm::vec2(m_facingRight ? m_size.x * 0.5f : -m_size.x * 0.5f, 0.0f);
            hitbox.knockbackDirection = glm::vec2(m_facingRight ? 1.0f : -1.0f, 0.5f);
            break;
            
        case AttackType::UP:
            hitbox.radius = m_size.x * 0.5f;
            hitbox.damage = 4;
            hitbox.knockbackBase = 4.0f;
            hitbox.knockbackScaling = 0.12f;
            hitbox.offset = glm::vec2(0.0f, m_size.y * 0.5f);
            hitbox.knockbackDirection = glm::vec2(0.0f, 1.0f);
            break;
            
        case AttackType::DOWN:
            hitbox.radius = m_size.x * 0.5f;
            hitbox.damage = 6;
            hitbox.knockbackBase = 3.0f;
            hitbox.knockbackScaling = 0.08f;
            hitbox.offset = glm::vec2(0.0f, -m_size.y * 0.5f);
            hitbox.knockbackDirection = glm::vec2(0.0f, -1.0f);
            break;
            
        case AttackType::SIDE:
            hitbox.radius = m_size.x * 0.7f;
            hitbox.damage = 7;
            hitbox.knockbackBase = 6.0f;
            hitbox.knockbackScaling = 0.15f;
            hitbox.offset = glm::vec2(m_facingRight ? m_size.x * 0.7f : -m_size.x * 0.7f, 0.0f);
            hitbox.knockbackDirection = glm::vec2(m_facingRight ? 1.0f : -1.0f, 0.2f);
            break;
            
        case AttackType::SPECIAL_NEUTRAL:
            hitbox.radius = m_size.x * 0.8f;
            hitbox.damage = 10;
            hitbox.knockbackBase = 7.0f;
            hitbox.knockbackScaling = 0.2f;
            hitbox.offset = glm::vec2(0.0f, 0.0f); // Centered on character
            hitbox.knockbackDirection = glm::vec2(m_facingRight ? 1.0f : -1.0f, 0.5f);
            break;
            
        case AttackType::SPECIAL_UP:
            hitbox.radius = m_size.x * 0.7f;
            hitbox.damage = 8;
            hitbox.knockbackBase = 6.0f;
            hitbox.knockbackScaling = 0.18f;
            hitbox.offset = glm::vec2(0.0f, m_size.y * 0.8f);
            hitbox.knockbackDirection = glm::vec2(0.0f, 1.0f);
            break;
            
        case AttackType::SPECIAL_DOWN:
            hitbox.radius = m_size.x * 1.0f;
            hitbox.damage = 12;
            hitbox.knockbackBase = 5.0f;
            hitbox.knockbackScaling = 0.15f;
            hitbox.offset = glm::vec2(0.0f, -m_size.y * 0.5f);
            hitbox.knockbackDirection = glm::vec2(0.0f, -0.8f);
            break;
            
        case AttackType::SPECIAL_SIDE:
            hitbox.radius = m_size.x * 0.9f;
            hitbox.damage = 9;
            hitbox.knockbackBase = 8.0f;
            hitbox.knockbackScaling = 0.22f;
            hitbox.offset = glm::vec2(m_facingRight ? m_size.x * 1.0f : -m_size.x * 1.0f, 0.0f);
            hitbox.knockbackDirection = glm::vec2(m_facingRight ? 1.0f : -1.0f, 0.1f);
            break;
    }
    
    // Adjust hitbox based on fighter type
    switch (m_type) {
        case FighterType::BALANCED:
            // No adjustment
            break;
        case FighterType::HEAVY:
            hitbox.damage = static_cast<int>(hitbox.damage * 1.3f);
            hitbox.knockbackBase *= 1.2f;
            hitbox.radius *= 1.1f;
            break;
        case FighterType::SPEEDY:
            hitbox.damage = static_cast<int>(hitbox.damage * 0.9f);
            hitbox.knockbackBase *= 0.9f;
            hitbox.radius *= 0.9f;
            break;
        case FighterType::TECHNICAL:
            hitbox.knockbackScaling *= 1.2f;
            break;
    }
    
    // Add hitbox to active hitboxes
    m_activeHitboxes.push_back(hitbox);
}
