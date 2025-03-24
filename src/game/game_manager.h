#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>
#include <string>
#include "fighter.h"
#include "stage.h"
#include "../rendering/camera.h"
#include "../rendering/shader.h"

enum class GameState {
    MENU,
    CHARACTER_SELECT,
    STAGE_SELECT,
    PLAYING,
    PAUSED,
    GAME_OVER
};

enum class GameMode {
    TIME,       // Time-based match
    STOCK,      // Stock/lives-based match
    STAMINA     // Health-based match
};

struct GameSettings {
    GameMode mode = GameMode::STOCK;
    int timeLimit = 180;  // In seconds (3 minutes)
    int stockCount = 3;   // Number of lives
    int staminaAmount = 100;  // Health points
    bool itemsEnabled = true;
    float damageMultiplier = 1.0f;
    float knockbackMultiplier = 1.0f;
};

class GameManager {
public:
    GameManager();
    ~GameManager();
    
    void init();
    void update(float deltaTime);
    void render(Shader& shader);
    
    // Game state management
    void startGame();
    void pauseGame();
    void resumeGame();
    void endGame();
    
    // Player management
    void addPlayer(FighterType type, int controllerIndex);
    void removePlayer(int playerIndex);
    
    // Input handling
    void processPlayerInput(int playerIndex, const glm::vec2& movement, bool jump, bool attack, AttackType attackType);
    
    // Getters
    GameState getGameState() const { return m_gameState; }
    GameSettings& getGameSettings() { return m_gameSettings; }
    
private:
    GameState m_gameState;
    GameSettings m_gameSettings;
    
    Stage* m_currentStage;
    std::vector<Character*> m_players;
    
    Camera* m_camera;
    glm::mat4 m_projection;
    
    float m_matchTimer;
    bool m_matchFinished;
    
    // Helper methods
    void updateCamera();
    void checkHitboxCollisions();
    void checkMatchEnd();
    int getWinningPlayerIndex();
    void respawnPlayer(int playerIndex);
    
    // UI elements
    void renderUI();
    void renderDamageDisplay();
    void renderTimer();
    void renderStockIcons();
};

#endif
