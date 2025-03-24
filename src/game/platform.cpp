#include "platform.h"
#include <glm/gtc/matrix_transform.hpp>

Platform::Platform(const glm::vec2& position, const glm::vec2& size, PlatformType type)
    : m_position(position)
    , m_size(size)
    , m_type(type)
    , m_mesh(nullptr)
    , m_texture(nullptr)
{
    createMesh();
}

Platform::~Platform() {
    if (m_mesh) {
        delete m_mesh;
    }
    // Texture is managed by ResourceManager
}

void Platform::render(Shader& shader) {
    if (!m_mesh) return;
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(m_position.x, m_position.y, 0.0f));
    model = glm::scale(model, glm::vec3(m_size.x, m_size.y, 1.0f));
    
    shader.setMat4("model", model);
    
    // Bind texture if available
    if (m_texture) {
        m_texture->bind(0);
    }
    
    // Draw the mesh
    m_mesh->draw(shader);
}

bool Platform::checkCollision(const glm::vec2& position, const glm::vec2& size) const {
    // For PASS_THROUGH platforms, no collision
    if (m_type == PlatformType::PASS_THROUGH) {
        return false;
    }
    
    // Simple AABB collision detection
    bool collisionX = m_position.x + m_size.x >= position.x - size.x/2 && 
                     m_position.x <= position.x + size.x/2;
                     
    bool collisionY = m_position.y + m_size.y >= position.y - size.y/2 && 
                     m_position.y <= position.y + size.y/2;
                     
    return collisionX && collisionY;
}

bool Platform::checkCollisionFromAbove(const glm::vec2& position, const glm::vec2& size, const glm::vec2& velocity) const {
    // For PASS_THROUGH platforms, no collision
    if (m_type == PlatformType::PASS_THROUGH) {
        return false;
    }
    
    // For SEMI_SOLID platforms, only collide if coming from above
    if (m_type == PlatformType::SEMI_SOLID) {
        // Check if the character is above the platform and moving downward
        if (position.y - size.y/2 >= m_position.y + m_size.y && velocity.y < 0) {
            // Check if the character would intersect with the platform in the next frame
            bool collisionX = m_position.x + m_size.x >= position.x - size.x/2 && 
                             m_position.x <= position.x + size.x/2;
                             
            bool collisionY = m_position.y + m_size.y >= position.y - size.y/2 + velocity.y && 
                             m_position.y <= position.y + size.y/2 + velocity.y;
                             
            return collisionX && collisionY;
        }
        return false;
    }
    
    // For SOLID platforms, use regular collision detection
    return checkCollision(position, size);
}

void Platform::createMesh() {
    // Create a simple quad mesh for the platform
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
    
    std::vector<Texture*> textures;  // Empty for now, will be set later
    
    m_mesh = new Mesh(vertices, indices, textures);
}
