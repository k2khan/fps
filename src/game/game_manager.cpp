#include "game_manager.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

GameManager::GameManager()
    : m_gameState(GameState::MENU)
    , m_currentStage(nullptr)
    , m_camera(nullptr)
    , m_matchTimer(0.0f)
    , m_matchFinished(false)
{
}

GameManager::~GameManager() {
    // Clean up resources
    if (m_currentStage) {
        delete m_currentStage;
    }
    
    for (auto player : m_players) {
        delete player;
    }
    
    if (m_camera) {
        delete m_camera;
    }
}

void GameManager::init() {
    // Initialize camera with side view for 2D game
    m_camera = new Camera(glm::vec3(0.0f, 0.0f, 20.0f));
    // Camera is already looking at -Z direction (yaw = -90)
    
    // Set up orthographic projection for 2D game
    float aspect = 16.0f / 9.0f; // Assuming 16:9 aspect ratio
    m_projection = glm::ortho(-10.0f, 10.0f, -10.0f / aspect, 10.0f / aspect, 0.1f, 100.0f);
    
    // Create default stage
    m_currentStage = new Stage("Default Stage");
    
    // Reset game state
    m_gameState = GameState::MENU;
    m_matchTimer = 0.0f;
    m_matchFinished = false;
}

void GameManager::update(float deltaTime) {
    // Only update game logic if playing
    if (m_gameState != GameState::PLAYING) {
        return;
    }
    
    // Update match timer
    if (m_gameSettings.mode == GameMode::TIME && !m_matchFinished) {
        m_matchTimer += deltaTime;
        if (m_matchTimer >= m_gameSettings.timeLimit) {
            m_matchFinished = true;
            m_gameState = GameState::GAME_OVER;
        }
    }
    
    // Update stage and players
    m_currentStage->update(deltaTime, m_players);
    
    for (auto player : m_players) {
        player->update(deltaTime);
    }
    
    // Check for hitbox collisions between players
    checkHitboxCollisions();
    
    // Check for match end conditions
    checkMatchEnd();
    
    // Update camera to follow players
    updateCamera();
}

void GameManager::checkHitboxCollisions() {
    // Check for collisions between players (hitboxes)
    for (size_t i = 0; i < m_players.size(); i++) {
        Character* attacker = m_players[i];
        
        // Skip dead players
        if (attacker->getState() == CharacterState::DEAD) {
            continue;
        }
        
        // Check hitboxes against other players
        for (size_t j = 0; j < m_players.size(); j++) {
            if (i == j) continue; // Skip self
            
            Character* defender = m_players[j];
            
            // Skip dead players
            if (defender->getState() == CharacterState::DEAD) {
                continue;
            }
            
            // Check each hitbox
            for (const auto& hitbox : attacker->m_activeHitboxes) {
                glm::vec2 hitboxPos = attacker->getPosition() + hitbox.offset;
                glm::vec2 defenderPos = defender->getPosition();
                
                // Simple circle collision
                float distance = glm::length(hitboxPos - defenderPos);
                if (distance < hitbox.radius + defender->getSize().x * 0.5f) {
                    // Apply damage and knockback
                    float knockback = hitbox.knockbackBase + 
                                     hitbox.knockbackScaling * defender->getDamage() * 
                                     m_gameSettings.knockbackMultiplier;
                    
                    int damage = static_cast<int>(hitbox.damage * m_gameSettings.damageMultiplier);
                    
                    defender->takeDamage(damage, knockback, hitbox.knockbackDirection);
                }
            }
        }
    }
}

void GameManager::render(Shader& shader) {
    // Set camera view and projection
    shader.setMat4("view", m_camera->getViewMatrix());
    shader.setMat4("projection", m_projection);
    
    // Render stage
    if (m_currentStage) {
        m_currentStage->render(shader);
    }
    
    // Render players
    for (auto player : m_players) {
        player->render(shader);
    }
}

void GameManager::startGame() {
    // Reset match state
    m_matchTimer = 0.0f;
    m_matchFinished = false;
    
    // Place players at spawn positions
    for (size_t i = 0; i < m_players.size(); i++) {
        m_players[i]->setPosition(m_currentStage->getSpawnPosition(i));
        m_players[i]->setVelocity(glm::vec2(0.0f, 0.0f));
    }
    
    // Start the game
    m_gameState = GameState::PLAYING;
}

void GameManager::pauseGame() {
    if (m_gameState == GameState::PLAYING) {
        m_gameState = GameState::PAUSED;
    }
}

void GameManager::resumeGame() {
    if (m_gameState == GameState::PAUSED) {
        m_gameState = GameState::PLAYING;
    }
}

void GameManager::endGame() {
    m_gameState = GameState::GAME_OVER;
    m_matchFinished = true;
}

void GameManager::addPlayer(FighterType type, int controllerIndex) {
    // Create a new fighter
    std::string name = "Player " + std::to_string(m_players.size() + 1);
    
    // Get spawn position from stage
    glm::vec2 spawnPos = m_currentStage ? 
                         m_currentStage->getSpawnPosition(m_players.size()) : 
                         glm::vec2(0.0f, 5.0f);
    
    Fighter* fighter = new Fighter(name, type, spawnPos);
    
    // Add to players list
    m_players.push_back(fighter);
}

void GameManager::removePlayer(int playerIndex) {
    if (playerIndex >= 0 && playerIndex < m_players.size()) {
        delete m_players[playerIndex];
        m_players.erase(m_players.begin() + playerIndex);
    }
}

void GameManager::processPlayerInput(int playerIndex, const glm::vec2& movement, bool jump, bool attack, AttackType attackType) {
    if (playerIndex < 0 || playerIndex >= m_players.size()) {
        return;
    }
    
    // Cast to Fighter* to access Fighter-specific methods
    Fighter* player = dynamic_cast<Fighter*>(m_players[playerIndex]);
    if (!player) return; // Safety check
    
    // Process movement
    if (movement.x < -0.1f) {
        player->moveLeft(0.016f); // Assuming 60 FPS
    } else if (movement.x > 0.1f) {
        player->moveRight(0.016f);
    }
    
    // Process jump
    if (jump) {
        player->jump();
    }
    
    // Process attack
    if (attack) {
        if (attackType == AttackType::SPECIAL_NEUTRAL ||
            attackType == AttackType::SPECIAL_UP ||
            attackType == AttackType::SPECIAL_DOWN ||
            attackType == AttackType::SPECIAL_SIDE) {
            player->specialAttack(attackType);
        } else {
            player->attack(attackType);
        }
    }
}

void GameManager::updateCamera() {
    if (m_players.empty() || !m_camera) {
        return;
    }
    
    // Calculate the bounding box of all players
    glm::vec2 minPos(FLT_MAX);
    glm::vec2 maxPos(-FLT_MAX);
    
    for (auto player : m_players) {
        // Skip dead players
        if (player->getState() == CharacterState::DEAD) {
            continue;
        }
        
        glm::vec2 pos = player->getPosition();
        minPos = glm::min(minPos, pos);
        maxPos = glm::max(maxPos, pos);
    }
    
    // Add padding
    minPos -= glm::vec2(3.0f);
    maxPos += glm::vec2(3.0f);
    
    // Calculate center and size
    glm::vec2 center = (minPos + maxPos) * 0.5f;
    glm::vec2 size = maxPos - minPos;
    
    // Calculate zoom level
    float targetZoom = std::max(size.x / 16.0f, size.y / 9.0f);
    targetZoom = std::max(targetZoom, 1.0f); // Minimum zoom
    
    // Update camera position directly
    m_camera->Position = glm::vec3(center.x, center.y, 20.0f);
    // We can't call updateCameraVectors() directly since it's private
    // Instead, we'll trigger an update by calling processMouseMovement with zero values
    m_camera->processMouseMovement(0.0f, 0.0f);
    
    // Update projection matrix
    float aspect = 16.0f / 9.0f; // Assuming 16:9 aspect ratio
    float width = 10.0f * targetZoom;
    float height = width / aspect;
    m_projection = glm::ortho(-width, width, -height, height, 0.1f, 100.0f);
}

void GameManager::checkMatchEnd() {
    if (m_matchFinished) {
        return;
    }
    
    // For stock mode, check if only one player has lives left
    if (m_gameSettings.mode == GameMode::STOCK) {
        int playersAlive = 0;
        for (auto player : m_players) {
            if (player->getLives() > 0) {
                playersAlive++;
            }
        }
        
        if (playersAlive <= 1) {
            m_matchFinished = true;
            m_gameState = GameState::GAME_OVER;
        }
    }
}

int GameManager::getWinningPlayerIndex() {
    if (!m_matchFinished) {
        return -1;
    }
    
    // For stock mode, the winner is the last player alive
    if (m_gameSettings.mode == GameMode::STOCK) {
        for (size_t i = 0; i < m_players.size(); i++) {
            if (m_players[i]->getLives() > 0) {
                return i;
            }
        }
    }
    
    return -1; // No winner
}

void GameManager::respawnPlayer(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= m_players.size()) {
        return;
    }
    
    // Cast to Fighter* to access Fighter-specific methods
    Fighter* player = dynamic_cast<Fighter*>(m_players[playerIndex]);
    if (!player) return; // Safety check
    
    // Reset position and velocity
    player->setPosition(m_currentStage->getSpawnPosition(playerIndex));
    player->setVelocity(glm::vec2(0.0f, 0.0f));
}

void GameManager::renderUI() {
    // Placeholder for UI rendering
}

void GameManager::renderDamageDisplay() {
    // Placeholder for damage display rendering
}

void GameManager::renderTimer() {
    // Placeholder for timer rendering
}

void GameManager::renderStockIcons() {
    // Placeholder for stock icons rendering
}
