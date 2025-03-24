#include "stage.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

Stage::Stage(const std::string& name)
    : m_name(name)
    , m_backgroundTexture(nullptr)
{
    // Set default blast zone
    m_blastZone.left = -15.0f;
    m_blastZone.right = 15.0f;
    m_blastZone.top = 10.0f;
    m_blastZone.bottom = -10.0f;
    
    // Set default spawn positions
    m_spawnPositions = {
        glm::vec2(-5.0f, 5.0f),  // Player 1
        glm::vec2(5.0f, 5.0f),   // Player 2
        glm::vec2(-2.5f, 5.0f),  // Player 3
        glm::vec2(2.5f, 5.0f)    // Player 4
    };
    
    // Setup default stage layout
    setupDefaultStage();
    
    // Load background texture
    // m_backgroundTexture = ResourceManager::loadTexture("assets/textures/background.png");
}

Stage::~Stage() {
    for (auto platform : m_platforms) {
        delete platform;
    }
    // Texture is managed by ResourceManager
}

void Stage::update(float deltaTime, std::vector<Character*>& characters) {
    // Update all characters based on stage physics
    for (auto character : characters) {
        resolveCharacterCollisions(character, deltaTime);
        
        // Check if character is on ground
        character->setOnGround(isCharacterOnGround(character));
        
        // Check if character is out of bounds
        if (isCharacterOutOfBounds(character)) {
            // Character lost a life
            character->takeDamage(0, 0, glm::vec2(0.0f)); // Just to trigger life loss logic
            
            // Respawn character if they have lives left
            if (character->getLives() > 0) {
                character->setPosition(getSpawnPosition(0)); // Use player index in a real game
                character->setVelocity(glm::vec2(0.0f, 0.0f));
            }
        }
    }
}

void Stage::render(Shader& shader) {
    // Render background
    if (m_backgroundTexture) {
        // Render a full-screen quad with the background texture
        // This would be implemented in a real game
    }
    
    // Render all platforms
    for (auto platform : m_platforms) {
        platform->render(shader);
    }
}

void Stage::addPlatform(const glm::vec2& position, const glm::vec2& size, PlatformType type) {
    Platform* platform = new Platform(position, size, type);
    m_platforms.push_back(platform);
}

void Stage::resolveCharacterCollisions(Character* character, float deltaTime) {
    glm::vec2 position = character->getPosition();
    glm::vec2 velocity = character->getVelocity();
    glm::vec2 size = character->getSize();
    
    // Apply velocity to get new position
    glm::vec2 newPosition = position + velocity * deltaTime;
    
    // Check for platform collisions
    for (auto platform : m_platforms) {
        // For semi-solid platforms, only check collision if character is falling
        if (platform->getType() == PlatformType::SEMI_SOLID && velocity.y >= 0) {
            continue;
        }
        
        // Check if character would collide with platform
        if (platform->checkCollision(newPosition, size)) {
            // Resolve collision
            
            // Try horizontal movement only
            glm::vec2 horizontalPos = position + glm::vec2(velocity.x * deltaTime, 0.0f);
            if (!platform->checkCollision(horizontalPos, size)) {
                newPosition.x = horizontalPos.x;
                velocity.x = velocity.x; // Keep horizontal velocity
                velocity.y = 0.0f;       // Reset vertical velocity
            }
            // Try vertical movement only
            else {
                glm::vec2 verticalPos = position + glm::vec2(0.0f, velocity.y * deltaTime);
                if (!platform->checkCollision(verticalPos, size)) {
                    newPosition.y = verticalPos.y;
                    velocity.y = velocity.y; // Keep vertical velocity
                    velocity.x = 0.0f;       // Reset horizontal velocity
                }
                // If both fail, don't move
                else {
                    newPosition = position;
                    velocity = glm::vec2(0.0f);
                }
            }
        }
    }
    
    // Update character position and velocity
    character->setPosition(newPosition);
    character->setVelocity(velocity);
}

bool Stage::isCharacterOnGround(Character* character) {
    glm::vec2 position = character->getPosition();
    glm::vec2 size = character->getSize();
    
    // Check position slightly below the character
    glm::vec2 groundCheckPos = position - glm::vec2(0.0f, 0.1f);
    
    for (auto platform : m_platforms) {
        // For semi-solid platforms, check if character is above the platform
        if (platform->getType() == PlatformType::SEMI_SOLID) {
            if (platform->checkCollisionFromAbove(position, size, glm::vec2(0.0f, -0.1f))) {
                return true;
            }
        }
        // For solid platforms, just check collision
        else if (platform->getType() == PlatformType::SOLID) {
            if (platform->checkCollision(groundCheckPos, size)) {
                return true;
            }
        }
    }
    
    return false;
}

bool Stage::isCharacterOutOfBounds(Character* character) {
    glm::vec2 position = character->getPosition();
    
    return position.x < m_blastZone.left ||
           position.x > m_blastZone.right ||
           position.y < m_blastZone.bottom ||
           position.y > m_blastZone.top;
}

glm::vec2 Stage::getSpawnPosition(int playerIndex) {
    if (playerIndex >= 0 && playerIndex < m_spawnPositions.size()) {
        return m_spawnPositions[playerIndex];
    }
    return glm::vec2(0.0f, 5.0f); // Default spawn position
}

void Stage::setupDefaultStage() {
    // Main platform
    addPlatform(glm::vec2(0.0f, 0.0f), glm::vec2(10.0f, 1.0f), PlatformType::SOLID);
    
    // Side platforms
    addPlatform(glm::vec2(-5.0f, 2.0f), glm::vec2(3.0f, 0.5f), PlatformType::SEMI_SOLID);
    addPlatform(glm::vec2(5.0f, 2.0f), glm::vec2(3.0f, 0.5f), PlatformType::SEMI_SOLID);
    
    // Top platform
    addPlatform(glm::vec2(0.0f, 4.0f), glm::vec2(4.0f, 0.5f), PlatformType::SEMI_SOLID);
}
