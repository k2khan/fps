#include "world.h"
#include <glm/gtc/matrix_transform.hpp>

World::World() {
    m_wallTexture = new Texture("assets/textures/wall.jpg");
    
    addWall(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(20.0f, 1.0f, 20.0f));
    
    addWall(glm::vec3(0.0f, 1.5f, -10.0f), glm::vec3(20.0f, 4.0f, 1.0f)); // Back wall
    addWall(glm::vec3(0.0f, 1.5f, 10.0f), glm::vec3(20.0f, 4.0f, 1.0f));  // Front wall
    addWall(glm::vec3(-10.0f, 1.5f, 0.0f), glm::vec3(1.0f, 4.0f, 20.0f)); // Left wall
    addWall(glm::vec3(10.0f, 1.5f, 0.0f), glm::vec3(1.0f, 4.0f, 20.0f));  // Right wall
    
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
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    vertices.push_back({glm::vec3(-0.5f, -0.5f, 0.5f) * size, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, -0.5f, 0.5f) * size, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, 0.5f) * size, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.5f, 0.5f) * size, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)});
    
    vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f) * size, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, -0.5f, -0.5f) * size, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, -0.5f) * size, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.5f, -0.5f) * size, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)});
    
    vertices.push_back({glm::vec3(0.5f, -0.5f, 0.5f) * size, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, -0.5f, -0.5f) * size, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, -0.5f) * size, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, 0.5f) * size, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
    
    vertices.push_back({glm::vec3(-0.5f, -0.5f, 0.5f) * size, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f) * size, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.5f, -0.5f) * size, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.5f, 0.5f) * size, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
    
    vertices.push_back({glm::vec3(-0.5f, 0.5f, 0.5f) * size, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, 0.5f) * size, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.5f, -0.5f) * size, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(-0.5f, 0.5f, -0.5f) * size, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
    
    vertices.push_back({glm::vec3(-0.5f, -0.5f, 0.5f) * size, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, -0.5f, 0.5f) * size, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, -0.5f, -0.5f) * size, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
    vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f) * size, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
    
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
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, wall.position);
        shader.setMat4("model", model);
        
        wall.mesh->draw(shader);
    }
}

bool World::checkCollision(const glm::vec3& position, float radius) {
    for (auto& wall : m_walls) {
        glm::vec3 min = wall.position - wall.size * 0.5f;
        glm::vec3 max = wall.position + wall.size * 0.5f;
        
        min -= glm::vec3(radius);
        max += glm::vec3(radius);
        
        if (position.x >= min.x && position.x <= max.x &&
            position.y >= min.y && position.y <= max.y &&
            position.z >= min.z && position.z <= max.z) {
            return true;
        }
    }
    return false;
}

glm::vec3 World::resolveCollision(const glm::vec3& oldPosition, const glm::vec3& newPosition, float radius) {
    if (!checkCollision(newPosition, radius)) {
        return newPosition;
    }
    
    glm::vec3 resolved = oldPosition;
    
    glm::vec3 xPos = glm::vec3(newPosition.x, oldPosition.y, oldPosition.z);
    if (!checkCollision(xPos, radius)) {
        resolved.x = newPosition.x;
    }
    
    glm::vec3 zPos = glm::vec3(resolved.x, oldPosition.y, newPosition.z);
    if (!checkCollision(zPos, radius)) {
        resolved.z = newPosition.z;
    }
    
    glm::vec3 yPos = glm::vec3(resolved.x, newPosition.y, resolved.z);
    if (!checkCollision(yPos, radius)) {
        resolved.y = newPosition.y;
    }
    
    return resolved;
}