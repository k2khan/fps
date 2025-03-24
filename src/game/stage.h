#ifndef STAGE_H
#define STAGE_H

#include <vector>
#include <glm/glm.hpp>
#include "platform.h"
#include "character.h"
#include "../rendering/shader.h"

// Represents the boundaries of the stage
struct BlastZone {
    float left;
    float right;
    float top;
    float bottom;
};

class Stage {
public:
    Stage(const std::string& name = "Default Stage");
    ~Stage();
    
    void update(float deltaTime, std::vector<Character*>& characters);
    void render(Shader& shader);
    
    // Platform management
    void addPlatform(const glm::vec2& position, const glm::vec2& size, PlatformType type = PlatformType::SOLID);
    
    // Character interaction
    void resolveCharacterCollisions(Character* character, float deltaTime);
    bool isCharacterOnGround(Character* character);
    bool isCharacterOutOfBounds(Character* character);
    glm::vec2 getSpawnPosition(int playerIndex);
    
    // Getters
    std::string getName() const { return m_name; }
    BlastZone getBlastZone() const { return m_blastZone; }
    
    // Setters
    void setName(const std::string& name) { m_name = name; }
    void setBlastZone(const BlastZone& blastZone) { m_blastZone = blastZone; }
    
private:
    std::string m_name;
    std::vector<Platform*> m_platforms;
    BlastZone m_blastZone;
    Texture* m_backgroundTexture;
    
    // Spawn positions for different players
    std::vector<glm::vec2> m_spawnPositions;
    
    void setupDefaultStage();
};

#endif
