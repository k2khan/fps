#ifndef PLATFORM_H
#define PLATFORM_H

#include <glm/glm.hpp>
#include "../rendering/mesh.h"
#include "../rendering/shader.h"
#include "../rendering/texture.h"

enum class PlatformType {
    SOLID,          // Cannot pass through
    SEMI_SOLID,     // Can jump up through, but not fall down through
    PASS_THROUGH    // Can pass through in any direction
};

class Platform {
public:
    Platform(const glm::vec2& position, const glm::vec2& size, PlatformType type = PlatformType::SOLID);
    ~Platform();
    
    void render(Shader& shader);
    
    // Collision detection
    bool checkCollision(const glm::vec2& position, const glm::vec2& size) const;
    bool checkCollisionFromAbove(const glm::vec2& position, const glm::vec2& size, const glm::vec2& velocity) const;
    
    // Getters
    glm::vec2 getPosition() const { return m_position; }
    glm::vec2 getSize() const { return m_size; }
    PlatformType getType() const { return m_type; }
    
    // Setters
    void setPosition(const glm::vec2& position) { m_position = position; }
    void setSize(const glm::vec2& size) { m_size = size; }
    void setType(PlatformType type) { m_type = type; }
    void setTexture(Texture* texture) { m_texture = texture; }
    
private:
    glm::vec2 m_position;
    glm::vec2 m_size;
    PlatformType m_type;
    
    Mesh* m_mesh;
    Texture* m_texture;
    
    void createMesh();
};

#endif
