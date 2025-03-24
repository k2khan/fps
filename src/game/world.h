#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <glm/glm.hpp>
#include "../rendering/mesh.h"
#include "../rendering/shader.h"

struct Wall {
    glm::vec3 position;
    glm::vec3 size;
    Mesh* mesh;
};

class World {
public:
    World();
    ~World();
    
    void addWall(const glm::vec3& position, const glm::vec3& size);
    
    void draw(Shader& shader);
    
    bool checkCollision(const glm::vec3& position, float radius = 0.5f);
    
    glm::vec3 resolveCollision(const glm::vec3& oldPosition, const glm::vec3& newPosition, float radius = 0.5f);
    
private:
    std::vector<Wall> m_walls;
    Texture* m_wallTexture;
    
    Mesh* createWallMesh(const glm::vec3& size);
};

#endif