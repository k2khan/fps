#ifndef FIGHTER_H
#define FIGHTER_H

#include "character.h"
#include <string>
#include <map>

enum class FighterType {
    BALANCED,   // Average stats
    HEAVY,      // Slow but powerful
    SPEEDY,     // Fast but weaker
    TECHNICAL   // Complex moves with high skill ceiling
};

class Fighter : public Character {
public:
    Fighter(const std::string& name, FighterType type, const glm::vec2& position);
    virtual ~Fighter();
    
    // Override base character methods
    virtual void update(float deltaTime) override;
    virtual void attack(AttackType type) override;
    
    // Fighter-specific methods
    void specialAttack(AttackType type);
    void taunt();
    
    // Getters
    FighterType getType() const { return m_type; }
    
private:
    FighterType m_type;
    
    // Fighter stats based on type
    void initializeStats();
    
    // Animation state
    std::map<CharacterState, float> m_animationTimes;
    float m_animationTimer;
    int m_currentFrame;
    
    // Special attack cooldowns
    std::map<AttackType, float> m_specialCooldowns;
    
    // Helper methods
    void updateAnimation(float deltaTime);
    void createHitboxForAttack(AttackType type);
};

#endif
