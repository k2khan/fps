// world.cpp
#include "world.h"
#include <glm/gtc/matrix_transform.hpp>

World::World() {
    // Load wall texture
    m_wallTexture = new Texture("assets/textures/wall.jpg");
    
    // Create basic level layout
    // Floor
    addWall(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(20.0f, 1.0f, 20.0f));
    
    // Outer walls
    addWall(glm::vec3(0.0f, 1.5f, -10.0f), glm::vec3(20.0f, 4.0f, 1.0f)); // Back wall
    addWall(glm::vec3(0.0f, 1.5f, 10.0f), glm::vec3(20.0f, 4.0f, 1.0f));  // Front wall
    addWall(glm::vec3(-10.0f, 1.5f, 0.0f), glm::vec3(1.0f, 4.0f, 20.0f)); // Left wall
    addWall(glm::vec3(10.0f, 1.5f, 0.0f), glm::vec3(1.0f, 4.0f, 20.0f));  // Right wall
    
    // Some inner walls for a simple maze
    addWall(glm::vec3(-5.0f, 1.5f, -5.0f), glm::vec3(1.0f, 4.0f, 10.0f));
    addWall(glm::vec3(5.0f, 1.5f, 5.0f), glm::vec3(10.0f, 4.0f, 1.0f));
}

World::~World() {
    delete m_wallTexture;
    for (auto& wall : m_walls) {
        delete wall.mesh;
    }
}

void World::addWall(const glm::vec3& position, const glm::vec3& size) {
    Wall wall;
    wall.position = position;
    wall.size = size;
    wall.mesh = createWallMesh(size);
    m_walls.push_back(wall);
}

Mesh* World::createWallMesh(const glm::vec3& size) {
    // Create a box mesh
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Front face
    vertices.push_back({glm::vec3(-0.5f, -0.5f, 0.5f) * size, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, -0.5f, 0.5f) * size, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, 0.5f) * size, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.5f, 0.5f) * size, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)});
    
    // Back face
    vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f) * size, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, -0.5f, -0.5f) * size, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, -0.5f) * size, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.5f, -0.5f) * size, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)});
    
    // Right face
    vertices.push_back({glm::vec3(0.5f, -0.5f, 0.5f) * size, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, -0.5f, -0.5f) * size, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, -0.5f) * size, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, 0.5f) * size, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
    
    // Left face
    vertices.push_back({glm::vec3(-0.5f, -0.5f, 0.5f) * size, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f) * size, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.5f, -0.5f) * size, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.5f, 0.5f) * size, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
    
    // Top face
    vertices.push_back({glm::vec3(-0.5f, 0.5f, 0.5f) * size, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, 0.5f) * size, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, -0.5f) * size, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.5f, -0.5f) * size, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
    
    // Bottom face
    vertices.push_back({glm::vec3(-0.5f, -0.5f, 0.5f) * size, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, -0.5f, 0.5f) * size, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, -0.5f, -0.5f) * size, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f) * size, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
    
    // Indices for all 6 faces (6 squares, each made of 2 triangles)
    for (int i = 0; i < 6; i++) {
        unsigned int baseIndex = i * 4;
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);
        
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex + 3);
    }
    
    std::vector<Texture*> textures = {m_wallTexture};
    return new Mesh(vertices, indices, textures);
}

void World::draw(Shader& shader) {
    for (auto& wall : m_walls) {
        // Set model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, wall.position);
        shader.setMat4("model", model);
        
        // Draw wall
        wall.mesh->draw(shader);
    }
}

bool World::checkCollision(const glm::vec3& position, float radius) {
    for (auto& wall : m_walls) {
        // Calculate AABB bounds for the wall
        glm::vec3 min = wall.position - wall.size * 0.5f;
        glm::vec3 max = wall.position + wall.size * 0.5f;
        
        // Expand bounds by player radius
        min -= glm::vec3(radius);
        max += glm::vec3(radius);
        
        // Check if position is within expanded bounds
        if (position.x >= min.x && position.x <= max.x &&
            position.y >= min.y && position.y <= max.y &&
            position.z >= min.z && position.z <= max.z) {
            return true;
        }
    }
    return false;
}

glm::vec3 World::resolveCollision(const glm::vec3& oldPosition, const glm::vec3& newPosition, float radius) {
    // If no collision, return the new position
    if (!checkCollision(newPosition, radius)) {
        return newPosition;
    }
    
    // Simple collision resolution: Try moving along individual axes
    glm::vec3 resolved = oldPosition;
    
    // Try X axis movement
    glm::vec3 xPos = glm::vec3(newPosition.x, oldPosition.y, oldPosition.z);
    if (!checkCollision(xPos, radius)) {
        resolved.x = newPosition.x;
    }
    
    // Try Z axis movement
    glm::vec3 zPos = glm::vec3(resolved.x, oldPosition.y, newPosition.z);
    if (!checkCollision(zPos, radius)) {
        resolved.z = newPosition.z;
    }
    
    // Try Y axis movement
    glm::vec3 yPos = glm::vec3(resolved.x, newPosition.y, resolved.z);
    if (!checkCollision(yPos, radius)) {
        resolved.y = newPosition.y;
    }
    
    return resolved;
}